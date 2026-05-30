#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
If a Solution prompt's official-editorial section contains #include, delete
everything from the first #include to the end of that editorial section, then
regenerate Solution.cpp with DeepSeek.

This is intentionally simple and aggressive.

Usage:

  export DEEPSEEK_API_KEY="sk-..."

  # Check only
  python3 fix_solution_include_and_regenerate.py \
    --root solver_outputs_cf1700_test \
    --only_problem CF2203D \
    --dry_run

  # Rewrite prompt and regenerate code
  python3 fix_solution_include_and_regenerate.py \
    --root solver_outputs_cf1700_test \
    --only_problem CF2203D \
    --model deepseek-v4-pro \
    --overwrite

  # Full batch
  python3 fix_solution_include_and_regenerate.py \
    --root solver_outputs_cf1700_test \
    --model deepseek-v4-pro \
    --parallel_workers 6 \
    --overwrite

It backs up original files once:
  *_Solution.prompt.txt.bak
  *_Solution.raw.txt.bak
  *_Solution.cpp.bak
"""

import argparse
import json
import os
import re
import time
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path
from typing import Any, Dict, List, Tuple


DEFAULT_BASE_URL = "https://api.deepseek.com"
CODE_OMITTED = "[code block omitted]"


def split_solution_prompt(prompt: str) -> Tuple[str, str, str]:
    marker = "=== OFFICIAL EDITORIAL / SOLUTION ==="
    idx = prompt.find(marker)
    if idx < 0:
        raise ValueError("Cannot find marker: === OFFICIAL EDITORIAL / SOLUTION ===")

    editorial_start = idx + len(marker)

    suffix_patterns = [
        r"\n\s*Carefully understand the official editorial/solution and convert it into a complete accepted C\+\+17 solution\.",
        r"\n\s*Write a complete C\+\+17 solution\.",
        r"\n\s*Requirements:\s*",
    ]

    best = None
    for pat in suffix_patterns:
        m = re.search(pat, prompt[editorial_start:], flags=re.IGNORECASE)
        if m:
            pos = editorial_start + m.start()
            if best is None or pos < best:
                best = pos

    if best is None:
        best = len(prompt)

    prefix = prompt[:editorial_start]
    editorial = prompt[editorial_start:best]
    suffix = prompt[best:]
    return prefix, editorial, suffix


def remove_from_first_include(editorial: str) -> Tuple[str, Dict[str, Any]]:
    """Delete from the first #include line to the end of the editorial section.

    This handles both normal code:
      #include <bits/stdc++.h>

    and tokenized code:
      #include
      <
      bits
      /
      stdc
      ++.
      h
      >
    """
    lines = editorial.splitlines()

    include_idx = None
    for i, line in enumerate(lines):
        if "#include" in line.strip().lower():
            include_idx = i
            break

    if include_idx is None:
        return editorial.strip(), {
            "changed": False,
            "include_found": False,
            "include_line_index": None,
            "removed_lines": 0,
            "original_editorial_lines": len(lines),
        }

    kept = lines[:include_idx]

    # Avoid leaving a trailing code heading such as "Solution (author)" immediately
    # before the omitted code marker.
    while kept and re.match(r"^\s*(solution|code|implementation)(\s*\([^)]*\))?\s*:?\s*$", kept[-1], flags=re.IGNORECASE):
        kept.pop()

    cleaned = "\n".join(kept).rstrip()
    if cleaned:
        cleaned += "\n\n" + CODE_OMITTED
    else:
        cleaned = CODE_OMITTED

    return cleaned, {
        "changed": True,
        "include_found": True,
        "include_line_index": include_idx,
        "removed_lines": len(lines) - include_idx,
        "original_editorial_lines": len(lines),
    }


def clean_prompt(prompt: str) -> Tuple[str, Dict[str, Any]]:
    prefix, editorial, suffix = split_solution_prompt(prompt)
    cleaned_editorial, meta = remove_from_first_include(editorial)
    new_prompt = prefix + "\n" + cleaned_editorial.strip() + "\n\n" + suffix.lstrip()
    meta["prompt_changed"] = (new_prompt != prompt)
    meta["remaining_include"] = "#include" in cleaned_editorial.lower()
    return new_prompt, meta


def extract_cpp_code(model_output: str) -> str:
    text = model_output or ""

    for pat in [
        r"```(?:cpp|c\+\+|cc|cxx)\s*(.*?)```",
        r"```\s*(.*?)```",
    ]:
        m = re.search(pat, text, flags=re.DOTALL | re.IGNORECASE)
        if m:
            code = m.group(1).strip()
            if code:
                return code + "\n"

    idx = text.find("#include")
    if idx >= 0:
        text = text[idx:]

    return text.strip() + "\n"


def get_client(args):
    from openai import OpenAI

    key = os.getenv(args.api_key_env)
    if not key:
        raise RuntimeError(f"Missing API key. Please run: export {args.api_key_env}='your_api_key'")

    return OpenAI(
        api_key=key,
        base_url=args.base_url,
        timeout=args.timeout_seconds,
    )


def call_deepseek(prompt: str, args) -> Tuple[str, Any]:
    client = get_client(args)

    kwargs = {
        "model": args.model,
        "messages": [
            {
                "role": "system",
                "content": (
                    "You are an expert competitive programmer. "
                    "Generate correct and efficient C++17 solutions. "
                    "Return only one C++17 code block."
                ),
            },
            {"role": "user", "content": prompt},
        ],
        "temperature": args.temperature,
        "top_p": args.top_p,
        "max_tokens": args.max_tokens,
        "stream": False,
    }

    if args.thinking != "omit":
        kwargs["extra_body"] = {"thinking": {"type": args.thinking}}

    last_error = None
    for attempt in range(1, args.retries + 1):
        try:
            resp = client.chat.completions.create(**kwargs)
            return resp.choices[0].message.content or "", getattr(resp, "usage", None)
        except Exception as e:
            last_error = e
            if attempt < args.retries:
                time.sleep(args.retry_sleep_seconds * attempt)

    raise RuntimeError(f"DeepSeek call failed after {args.retries} attempts: {repr(last_error)}")


def infer_prefix(prompt_path: Path) -> str:
    name = prompt_path.name
    if name.endswith("_Solution.prompt.txt"):
        return name[: -len("_Solution.prompt.txt")]
    return name.split("_Solution")[0]


def backup_once(path: Path) -> None:
    if not path.exists():
        return
    bak = path.with_name(path.name + ".bak")
    if not bak.exists():
        bak.write_text(path.read_text(encoding="utf-8", errors="replace"), encoding="utf-8")


def should_process(path: Path, args) -> bool:
    if not args.only_problem:
        return True
    return path.parent.name in args.only_problem or any(x in path.name for x in args.only_problem)


def process_one(prompt_path: Path, args) -> Dict[str, Any]:
    prefix = infer_prefix(prompt_path)
    raw_path = prompt_path.with_name(f"{prefix}_Solution.raw.txt")
    cpp_path = prompt_path.with_name(f"{prefix}_Solution.cpp")
    failed_path = prompt_path.with_name(f"{prefix}_Solution.failed.txt")

    original_prompt = prompt_path.read_text(encoding="utf-8", errors="replace")
    new_prompt, meta = clean_prompt(original_prompt)

    if not meta["include_found"] and args.skip_if_no_include:
        return {
            "prefix": prefix,
            "prompt_path": str(prompt_path),
            "status": "skipped_no_include",
            **meta,
        }

    if args.backup:
        backup_once(prompt_path)
        backup_once(raw_path)
        backup_once(cpp_path)

    if args.dry_run:
        return {
            "prefix": prefix,
            "prompt_path": str(prompt_path),
            "status": "dry_run",
            **meta,
        }

    if meta["prompt_changed"] or args.force_rewrite_prompt:
        prompt_path.write_text(new_prompt, encoding="utf-8")

    if cpp_path.exists() and raw_path.exists() and not args.overwrite and not args.force_regenerate:
        return {
            "prefix": prefix,
            "prompt_path": str(prompt_path),
            "cpp_path": str(cpp_path),
            "raw_path": str(raw_path),
            "status": "skipped_existing",
            **meta,
        }

    try:
        raw_output, usage = call_deepseek(new_prompt, args)
        code = extract_cpp_code(raw_output)

        raw_path.write_text(raw_output, encoding="utf-8")
        cpp_path.write_text(code, encoding="utf-8")

        usage_obj = None
        if usage is not None:
            try:
                usage_obj = usage.model_dump()
            except Exception:
                usage_obj = str(usage)

        return {
            "prefix": prefix,
            "prompt_path": str(prompt_path),
            "raw_path": str(raw_path),
            "cpp_path": str(cpp_path),
            "status": "ok",
            "model": args.model,
            "thinking": args.thinking,
            "usage": usage_obj,
            **meta,
        }

    except Exception as e:
        failed_path.write_text(repr(e), encoding="utf-8")
        return {
            "prefix": prefix,
            "prompt_path": str(prompt_path),
            "failed_path": str(failed_path),
            "status": "failed",
            "error": repr(e),
            **meta,
        }


def find_solution_prompts(root: Path) -> List[Path]:
    return sorted(p for p in root.rglob("*_Solution.prompt.txt") if p.is_file())


def parse_args():
    p = argparse.ArgumentParser()
    p.add_argument("--root", required=True)
    p.add_argument("--model", default="deepseek-v4-pro")
    p.add_argument("--base_url", default=DEFAULT_BASE_URL)
    p.add_argument("--api_key_env", default="DEEPSEEK_API_KEY")
    p.add_argument("--thinking", choices=["omit", "disabled", "enabled"], default="omit")

    p.add_argument("--temperature", type=float, default=0.2)
    p.add_argument("--top_p", type=float, default=0.95)
    p.add_argument("--max_tokens", type=int, default=8192)
    p.add_argument("--timeout_seconds", type=float, default=240.0)
    p.add_argument("--retries", type=int, default=2)
    p.add_argument("--retry_sleep_seconds", type=float, default=3.0)
    p.add_argument("--parallel_workers", type=int, default=4)

    p.add_argument("--overwrite", action="store_true")
    p.add_argument("--force_regenerate", action="store_true")
    p.add_argument("--force_rewrite_prompt", action="store_true")
    p.add_argument("--backup", action="store_true", default=True)
    p.add_argument("--dry_run", action="store_true")
    p.add_argument("--skip_if_no_include", action="store_true", default=True)
    p.add_argument("--only_problem", nargs="*", default=None)
    return p.parse_args()


def main():
    args = parse_args()
    root = Path(args.root).expanduser().resolve()
    if not root.exists():
        raise FileNotFoundError(f"Root not found: {root}")

    prompts = [p for p in find_solution_prompts(root) if should_process(p, args)]

    print(f"Root: {root}")
    print(f"Found Solution prompts: {len(prompts)}")
    print(f"Model: {args.model}")
    print(f"Dry run: {args.dry_run}")
    print(f"Overwrite: {args.overwrite}")
    print(f"Parallel workers: {args.parallel_workers}")

    records: List[Dict[str, Any]] = []
    workers = max(1, min(args.parallel_workers, len(prompts)))

    if workers == 1:
        for prompt in prompts:
            print(f"\nProcessing {prompt}")
            rec = process_one(prompt, args)
            print(
                f"  -> {rec.get('status')}, "
                f"include_found={rec.get('include_found')}, "
                f"removed_lines={rec.get('removed_lines')}, "
                f"remaining_include={rec.get('remaining_include')}"
            )
            records.append(rec)
    else:
        print(f"Running with {workers} parallel workers...")
        with ThreadPoolExecutor(max_workers=workers) as ex:
            futs = {ex.submit(process_one, prompt, args): prompt for prompt in prompts}
            for fut in as_completed(futs):
                prompt = futs[fut]
                try:
                    rec = fut.result()
                except Exception as e:
                    rec = {
                        "prompt_path": str(prompt),
                        "status": "failed_outer",
                        "error": repr(e),
                    }
                print(
                    f"  -> {prompt.parent.name}: {rec.get('status')}, "
                    f"include_found={rec.get('include_found')}, "
                    f"removed_lines={rec.get('removed_lines')}, "
                    f"remaining_include={rec.get('remaining_include')}"
                )
                records.append(rec)

    summary_path = root / "include_cut_solution_regeneration_summary.json"
    summary_path.write_text(json.dumps(records, ensure_ascii=False, indent=2), encoding="utf-8")

    print("\nDone.")
    print(f"Summary: {summary_path}")
    counts: Dict[str, int] = {}
    for r in records:
        status = r.get("status", "unknown")
        counts[status] = counts.get(status, 0) + 1
    print("Status counts:")
    for k, v in sorted(counts.items()):
        print(f"  {k}: {v}")


if __name__ == "__main__":
    main()

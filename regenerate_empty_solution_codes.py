#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Regenerate empty / invalid *_Solution.cpp files with DeepSeek.

This script does NOT clean prompts. It assumes you already have
*_Solution.prompt.txt files, preferably cleaned Solution prompts.

It scans a root directory such as solver_outputs_cf1700_test and finds:

  CFxxxx/CFxxxx_Solution.cpp
  CFxxxx/CFxxxx_Solution.prompt.txt

If Solution.cpp is empty, too short, or does not look like C++ code, it calls
DeepSeek again using the corresponding Solution.prompt.txt.

Typical usage:

  export DEEPSEEK_API_KEY="sk-..."

  python3 regenerate_empty_solution_codes.py \
    --root solver_outputs_cf1700_test \
    --model deepseek-v4-pro \
    --thinking disabled \
    --parallel_workers 6 \
    --overwrite

Dry run:

  python3 regenerate_empty_solution_codes.py \
    --root solver_outputs_cf1700_test \
    --dry_run

Only selected problems:

  python3 regenerate_empty_solution_codes.py \
    --root solver_outputs_cf1700_test \
    --only_problem CF2203D CF1777C \
    --model deepseek-v4-pro \
    --thinking disabled \
    --overwrite
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


def infer_prefix_from_prompt(prompt_path: Path) -> str:
    name = prompt_path.name
    if name.endswith("_Solution.prompt.txt"):
        return name[:-len("_Solution.prompt.txt")]
    return name.split("_Solution")[0]


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

    return text.strip() + ("\n" if text.strip() else "")


def code_quality(code: str, min_chars: int) -> Tuple[bool, List[str]]:
    reasons: List[str] = []
    s = code or ""
    lower = s.lower()

    if len(s.strip()) < min_chars:
        reasons.append(f"too_short<{min_chars}")

    if "#include" not in lower:
        reasons.append("missing_include")

    if "main" not in lower:
        reasons.append("missing_main")

    if "```" in s:
        reasons.append("contains_code_fence")

    return len(reasons) == 0, reasons


def should_regenerate_cpp(cpp_path: Path, min_chars: int, regenerate_invalid: bool) -> Tuple[bool, List[str]]:
    if not cpp_path.exists():
        return True, ["missing_cpp"]

    code = cpp_path.read_text(encoding="utf-8", errors="replace")

    if not code.strip():
        return True, ["empty_cpp"]

    if regenerate_invalid:
        ok, reasons = code_quality(code, min_chars)
        if not ok:
            return True, reasons

    return False, []


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
                    "Return only one C++17 code block. Do not explain."
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
            content = resp.choices[0].message.content or ""
            usage = getattr(resp, "usage", None)
            return content, usage
        except Exception as e:
            last_error = e
            if attempt < args.retries:
                sleep_s = args.retry_sleep_seconds * attempt
                time.sleep(sleep_s)

    raise RuntimeError(f"DeepSeek call failed after {args.retries} attempts: {repr(last_error)}")


def backup_once(path: Path) -> None:
    if not path.exists():
        return
    bak = path.with_name(path.name + ".bak")
    if not bak.exists():
        bak.write_text(path.read_text(encoding="utf-8", errors="replace"), encoding="utf-8")


def find_solution_prompts(root: Path) -> List[Path]:
    return sorted(p for p in root.rglob("*_Solution.prompt.txt") if p.is_file())


def should_process(prompt_path: Path, args) -> bool:
    if not args.only_problem:
        return True
    return prompt_path.parent.name in args.only_problem or any(x in prompt_path.name for x in args.only_problem)


def process_one(prompt_path: Path, args) -> Dict[str, Any]:
    prefix = infer_prefix_from_prompt(prompt_path)
    cpp_path = prompt_path.with_name(f"{prefix}_Solution.cpp")
    raw_path = prompt_path.with_name(f"{prefix}_Solution.raw.txt")
    failed_path = prompt_path.with_name(f"{prefix}_Solution.regen_failed.txt")

    need_regen, reasons = should_regenerate_cpp(
        cpp_path=cpp_path,
        min_chars=args.min_chars,
        regenerate_invalid=args.regenerate_invalid,
    )

    if not need_regen and not args.force_regenerate:
        return {
            "prefix": prefix,
            "prompt_path": str(prompt_path),
            "cpp_path": str(cpp_path),
            "status": "skipped_valid",
            "reasons": reasons,
        }

    if not prompt_path.exists():
        return {
            "prefix": prefix,
            "prompt_path": str(prompt_path),
            "cpp_path": str(cpp_path),
            "status": "failed_no_prompt",
            "reasons": reasons,
        }

    if args.dry_run:
        return {
            "prefix": prefix,
            "prompt_path": str(prompt_path),
            "cpp_path": str(cpp_path),
            "status": "dry_run",
            "reasons": reasons if reasons else ["force_regenerate"],
        }

    if args.backup:
        backup_once(cpp_path)
        backup_once(raw_path)

    prompt = prompt_path.read_text(encoding="utf-8", errors="replace")

    last_record: Dict[str, Any] = {}
    for attempt in range(1, args.codegen_attempts + 1):
        try:
            raw_output, usage = call_deepseek(prompt, args)
            code = extract_cpp_code(raw_output)
            ok, quality_reasons = code_quality(code, args.min_chars)

            usage_obj = None
            if usage is not None:
                try:
                    usage_obj = usage.model_dump()
                except Exception:
                    usage_obj = str(usage)

            # Always save the latest raw output for debugging.
            raw_path.write_text(raw_output or "", encoding="utf-8")

            last_record = {
                "prefix": prefix,
                "prompt_path": str(prompt_path),
                "raw_path": str(raw_path),
                "cpp_path": str(cpp_path),
                "status": "ok" if ok or args.write_invalid else "invalid_generation",
                "initial_reasons": reasons,
                "quality_reasons": quality_reasons,
                "attempt": attempt,
                "model": args.model,
                "thinking": args.thinking,
                "usage": usage_obj,
                "raw_output_chars": len(raw_output or ""),
                "extracted_code_chars": len(code or ""),
            }

            if ok or args.write_invalid:
                cpp_path.write_text(code, encoding="utf-8")
                return last_record

            # If invalid, retry once/more.
            time.sleep(args.retry_sleep_seconds * attempt)

        except Exception as e:
            last_record = {
                "prefix": prefix,
                "prompt_path": str(prompt_path),
                "cpp_path": str(cpp_path),
                "failed_path": str(failed_path),
                "status": "failed",
                "initial_reasons": reasons,
                "attempt": attempt,
                "error": repr(e),
            }
            failed_path.write_text(repr(e), encoding="utf-8")
            if attempt < args.codegen_attempts:
                time.sleep(args.retry_sleep_seconds * attempt)

    return last_record


def parse_args():
    p = argparse.ArgumentParser()
    p.add_argument("--root", required=True, help="Root directory, e.g. solver_outputs_cf1700_test")
    p.add_argument("--model", default="deepseek-v4-pro")
    p.add_argument("--base_url", default=DEFAULT_BASE_URL)
    p.add_argument("--api_key_env", default="DEEPSEEK_API_KEY")
    p.add_argument("--thinking", choices=["omit", "disabled", "enabled"], default="disabled")

    p.add_argument("--temperature", type=float, default=0.2)
    p.add_argument("--top_p", type=float, default=0.95)
    p.add_argument("--max_tokens", type=int, default=8192)
    p.add_argument("--timeout_seconds", type=float, default=300.0)
    p.add_argument("--retries", type=int, default=2)
    p.add_argument("--retry_sleep_seconds", type=float, default=3.0)
    p.add_argument("--parallel_workers", type=int, default=4)

    p.add_argument("--min_chars", type=int, default=200)
    p.add_argument("--regenerate_invalid", action="store_true", default=True)
    p.add_argument("--force_regenerate", action="store_true")
    p.add_argument("--codegen_attempts", type=int, default=2)
    p.add_argument("--write_invalid", action="store_true")
    p.add_argument("--backup", action="store_true", default=True)
    p.add_argument("--dry_run", action="store_true")
    p.add_argument("--overwrite", action="store_true", help="Kept for compatibility; regeneration already overwrites selected cpp/raw files.")
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
    print(f"Thinking: {args.thinking}")
    print(f"Dry run: {args.dry_run}")
    print(f"Force regenerate: {args.force_regenerate}")
    print(f"Parallel workers: {args.parallel_workers}")

    records: List[Dict[str, Any]] = []
    workers = max(1, min(args.parallel_workers, len(prompts)))

    if workers == 1:
        for prompt in prompts:
            print(f"\nProcessing {prompt}")
            rec = process_one(prompt, args)
            print(
                f"  -> {rec.get('status')}, "
                f"initial={rec.get('initial_reasons') or rec.get('reasons')}, "
                f"quality={rec.get('quality_reasons')}, "
                f"chars={rec.get('extracted_code_chars')}"
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
                    f"initial={rec.get('initial_reasons') or rec.get('reasons')}, "
                    f"quality={rec.get('quality_reasons')}, "
                    f"chars={rec.get('extracted_code_chars')}"
                )
                records.append(rec)

    summary_path = root / "empty_solution_regeneration_summary.json"
    summary_path.write_text(json.dumps(records, ensure_ascii=False, indent=2), encoding="utf-8")

    counts: Dict[str, int] = {}
    for r in records:
        status = r.get("status", "unknown")
        counts[status] = counts.get(status, 0) + 1

    print("\nDone.")
    print(f"Summary: {summary_path}")
    print("Status counts:")
    for k, v in sorted(counts.items()):
        print(f"  {k}: {v}")


if __name__ == "__main__":
    main()

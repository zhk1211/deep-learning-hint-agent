#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Regenerate ALL *_Solution.cpp files from existing *_Solution.prompt.txt files.

This script is designed to match the DeepSeek call parameters in the uploaded
deepseek_cf1700_eval.py as closely as possible.

It uses the same:
- system prompt
- messages format
- OpenAI-compatible client construction
- model argument name and default
- thinking behavior and default
- temperature / top_p / max_tokens defaults
- timeout / retry defaults
- extract_cpp_code logic
- per-thread client logic

It does NOT rebuild prompts. It directly reuses existing:
  CFxxxx_Solution.prompt.txt

and overwrites:
  CFxxxx_Solution.raw.txt
  CFxxxx_Solution.cpp

Typical usage:

  export DEEPSEEK_API_KEY="sk-..."

  python3 regenerate_all_solution_same_params.py \
    --root solver_outputs_cf1700_test \
    --solver_model deepseek-v4-pro \
    --parallel_solver_workers 6 \
    --overwrite

Dry run:

  python3 regenerate_all_solution_same_params.py \
    --root solver_outputs_cf1700_test \
    --dry_run

Only selected problems:

  python3 regenerate_all_solution_same_params.py \
    --root solver_outputs_cf1700_test \
    --only_problem CF2203D CF1777C \
    --solver_model deepseek-v4-pro \
    --parallel_solver_workers 2 \
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


DEFAULT_DEEPSEEK_BASE_URL = "https://api.deepseek.com"


# ============================================================
# These three functions intentionally match deepseek_cf1700_eval.py
# ============================================================

def extract_cpp_code(model_output: str) -> str:
    text = model_output or ""

    patterns = [
        r"```(?:cpp|c\+\+|cc|cxx)\s*(.*?)```",
        r"```\s*(.*?)```",
    ]
    for pat in patterns:
        m = re.search(pat, text, flags=re.DOTALL | re.IGNORECASE)
        if m:
            code = m.group(1).strip()
            if code:
                return code + "\n"

    idx = text.find("#include")
    if idx >= 0:
        text = text[idx:]

    return text.strip() + "\n"


def get_openai_client(args):
    from openai import OpenAI

    api_key = os.getenv(args.solver_api_key_env)
    if not api_key:
        raise RuntimeError(
            f"Missing API key. Please run: export {args.solver_api_key_env}='your_api_key'"
        )

    return OpenAI(
        api_key=api_key,
        base_url=args.solver_base_url,
        timeout=args.solver_timeout_seconds,
    )


def call_solver_api(client, prompt: str, args) -> Tuple[str, Any]:
    messages = [
        {
            "role": "system",
            "content": (
                "You are an expert competitive programmer. "
                "Generate correct and efficient C++17 solutions. "
                "Return only one C++17 code block."
            ),
        },
        {"role": "user", "content": prompt},
    ]

    kwargs = {
        "model": args.solver_model,
        "messages": messages,
        "temperature": args.solver_temperature,
        "top_p": args.solver_top_p,
        "max_tokens": args.solver_max_tokens,
        "stream": False,
    }

    if args.thinking != "omit":
        kwargs["extra_body"] = {"thinking": {"type": args.thinking}}

    last_error = None
    for attempt in range(1, args.solver_retries + 1):
        try:
            resp = client.chat.completions.create(**kwargs)
            content = resp.choices[0].message.content or ""
            usage = getattr(resp, "usage", None)
            return content, usage
        except Exception as e:
            last_error = e
            if attempt < args.solver_retries:
                sleep_s = args.retry_sleep_seconds * attempt
                print(f"⚠️ Solver API failed attempt {attempt}/{args.solver_retries}: {repr(e)}")
                print(f"   Sleeping {sleep_s:.1f}s...")
                time.sleep(sleep_s)

    raise RuntimeError(f"Solver API failed after {args.solver_retries} attempts: {repr(last_error)}")


# ============================================================
# Regeneration logic
# ============================================================

def infer_prefix_from_solution_prompt(prompt_path: Path) -> str:
    name = prompt_path.name
    if name.endswith("_Solution.prompt.txt"):
        return name[:-len("_Solution.prompt.txt")]
    return name.split("_Solution")[0]


def find_solution_prompts(root: Path) -> List[Path]:
    return sorted(p for p in root.rglob("*_Solution.prompt.txt") if p.is_file())


def should_process(prompt_path: Path, args) -> bool:
    if not args.only_problem:
        return True
    return prompt_path.parent.name in args.only_problem or any(x in prompt_path.name for x in args.only_problem)


def backup_once(path: Path) -> None:
    if not path.exists():
        return
    bak = path.with_name(path.name + ".bak")
    if not bak.exists():
        bak.write_text(path.read_text(encoding="utf-8", errors="replace"), encoding="utf-8")


def looks_empty_or_invalid_cpp(code: str) -> List[str]:
    reasons: List[str] = []
    s = code or ""
    lower = s.lower()

    if not s.strip():
        reasons.append("empty")
    if len(s.strip()) < 80:
        reasons.append("very_short")
    if "#include" not in lower:
        reasons.append("missing_include")
    if "main" not in lower:
        reasons.append("missing_main")

    return reasons


def process_one_solution(prompt_path: Path, args) -> Dict[str, Any]:
    prefix = infer_prefix_from_solution_prompt(prompt_path)
    prompt = prompt_path.read_text(encoding="utf-8", errors="replace")

    cpp_path = prompt_path.with_name(f"{prefix}_Solution.cpp")
    raw_path = prompt_path.with_name(f"{prefix}_Solution.raw.txt")
    failed_path = prompt_path.with_name(f"{prefix}_Solution.failed.txt")

    if args.backup:
        backup_once(cpp_path)
        backup_once(raw_path)

    if args.dry_run:
        return {
            "prefix": prefix,
            "label": "Solution",
            "kind": "solution",
            "prompt_path": str(prompt_path),
            "cpp_path": str(cpp_path),
            "raw_path": str(raw_path),
            "status": "dry_run",
        }

    try:
        # Same as uploaded code: use a per-thread client.
        local_client = get_openai_client(args)
        raw_output, usage = call_solver_api(local_client, prompt, args)
        code = extract_cpp_code(raw_output)

        raw_path.write_text(raw_output, encoding="utf-8")
        cpp_path.write_text(code, encoding="utf-8")

        usage_obj = None
        if usage is not None:
            try:
                usage_obj = usage.model_dump()
            except Exception:
                usage_obj = str(usage)

        quality_reasons = looks_empty_or_invalid_cpp(code)

        return {
            "prefix": prefix,
            "label": "Solution",
            "kind": "solution",
            "num_hints_visible": 0,
            "prompt_path": str(prompt_path),
            "cpp_path": str(cpp_path),
            "raw_path": str(raw_path),
            "status": "ok",
            "solver_model": args.solver_model,
            "thinking": args.thinking,
            "usage": usage_obj,
            "raw_output_chars": len(raw_output or ""),
            "extracted_code_chars": len(code or ""),
            "quality_warnings": quality_reasons,
        }

    except Exception as e:
        failed_path.write_text(repr(e), encoding="utf-8")
        return {
            "prefix": prefix,
            "label": "Solution",
            "kind": "solution",
            "num_hints_visible": 0,
            "prompt_path": str(prompt_path),
            "failed_path": str(failed_path),
            "status": "failed",
            "solver_model": args.solver_model,
            "thinking": args.thinking,
            "error": repr(e),
        }


def parse_args():
    parser = argparse.ArgumentParser()

    parser.add_argument("--root", type=str, required=True, help="Root dir containing CFxxxx/*_Solution.prompt.txt files.")
    parser.add_argument("--only_problem", nargs="*", default=None, help="Only process selected problem folders/prefixes.")
    parser.add_argument("--dry_run", action="store_true")
    parser.add_argument("--overwrite", action="store_true", help="Kept for consistency. This script always regenerates selected Solution files unless --dry_run is used.")
    parser.add_argument("--backup", action="store_true", default=True)

    # Keep names/defaults the same as uploaded deepseek_cf1700_eval.py
    parser.add_argument("--solver_base_url", type=str, default=DEFAULT_DEEPSEEK_BASE_URL)
    parser.add_argument("--solver_api_key_env", type=str, default="DEEPSEEK_API_KEY")
    parser.add_argument("--solver_model", type=str, default="deepseek-v4-pro")
    parser.add_argument("--thinking", choices=["omit", "disabled", "enabled"], default="omit")
    parser.add_argument("--solver_temperature", type=float, default=0.2)
    parser.add_argument("--solver_top_p", type=float, default=0.95)
    parser.add_argument("--solver_max_tokens", type=int, default=8192)
    parser.add_argument("--solver_timeout_seconds", type=float, default=240.0)
    parser.add_argument("--solver_retries", type=int, default=2)
    parser.add_argument("--retry_sleep_seconds", type=float, default=3.0)
    parser.add_argument("--sleep_between_calls", type=float, default=0.5)
    parser.add_argument(
        "--parallel_solver_workers",
        type=int,
        default=1,
        help="Number of parallel DeepSeek/API code-generation calls. Use 1 for sequential.",
    )

    return parser.parse_args()


def main():
    args = parse_args()

    root = Path(args.root).expanduser().resolve()
    if not root.exists():
        raise FileNotFoundError(f"Root not found: {root}")

    prompts = [p for p in find_solution_prompts(root) if should_process(p, args)]

    print(f"📦 Root: {root}")
    print(f"📄 Found Solution prompts: {len(prompts)}")
    print(f"🤖 Solver model: {args.solver_model}")
    print(f"🧠 Thinking parameter: {args.thinking}")
    print(f"🌡 Temperature: {args.solver_temperature}")
    print(f"🔝 Top-p: {args.solver_top_p}")
    print(f"📏 Max tokens: {args.solver_max_tokens}")
    print(f"⏱ Timeout: {args.solver_timeout_seconds}")
    print(f"🔁 Retries: {args.solver_retries}")
    print(f"🧵 Parallel solver workers: {args.parallel_solver_workers}")
    print(f"🧪 Dry run: {args.dry_run}")

    if not prompts:
        print("No Solution prompts found.")
        return

    records: List[Dict[str, Any]] = []
    workers = max(1, int(args.parallel_solver_workers))
    workers = min(workers, len(prompts))

    if workers <= 1:
        for prompt_path in prompts:
            print(f"\n🚀 Regenerating {prompt_path.parent.name} Solution...")
            record = process_one_solution(prompt_path, args)
            if record["status"] == "ok":
                print(f"✅ Wrote code: {Path(record['cpp_path']).name}")
                if record.get("quality_warnings"):
                    print(f"⚠️ quality warnings: {record['quality_warnings']}")
                if record.get("usage"):
                    print(f"📊 usage: {record['usage']}")
            elif record["status"] == "dry_run":
                print(f"🧪 Dry run: {prompt_path}")
            else:
                print(f"❌ Failed: {record.get('failed_path')}")
            records.append(record)
            if args.sleep_between_calls > 0 and not args.dry_run:
                time.sleep(args.sleep_between_calls)
    else:
        print(f"\n🚀 Running {len(prompts)} Solution API calls with {workers} parallel workers...")
        with ThreadPoolExecutor(max_workers=workers) as executor:
            future_to_prompt = {executor.submit(process_one_solution, p, args): p for p in prompts}
            for future in as_completed(future_to_prompt):
                prompt_path = future_to_prompt[future]
                try:
                    record = future.result()
                except Exception as e:
                    failed_path = prompt_path.with_name(f"{infer_prefix_from_solution_prompt(prompt_path)}_Solution.failed.txt")
                    failed_path.write_text(repr(e), encoding="utf-8")
                    record = {
                        "prefix": infer_prefix_from_solution_prompt(prompt_path),
                        "label": "Solution",
                        "kind": "solution",
                        "prompt_path": str(prompt_path),
                        "failed_path": str(failed_path),
                        "status": "failed",
                        "solver_model": args.solver_model,
                        "thinking": args.thinking,
                        "error": repr(e),
                    }

                if record["status"] == "ok":
                    msg = f"✅ Done {prompt_path.parent.name}: {Path(record['cpp_path']).name}"
                    if record.get("quality_warnings"):
                        msg += f" warnings={record['quality_warnings']}"
                    print(msg)
                    if record.get("usage"):
                        print(f"📊 usage: {record['usage']}")
                elif record["status"] == "dry_run":
                    print(f"🧪 Dry run: {prompt_path.parent.name}")
                else:
                    print(f"❌ Failed {prompt_path.parent.name}: {record.get('failed_path')}")

                records.append(record)

    summary_path = root / "regenerate_all_solution_same_params_summary.json"
    summary_path.write_text(json.dumps(records, ensure_ascii=False, indent=2), encoding="utf-8")

    print("\n🎉 Done.")
    print(f"📄 Summary: {summary_path}")

    counts: Dict[str, int] = {}
    warning_count = 0
    for r in records:
        status = r.get("status", "unknown")
        counts[status] = counts.get(status, 0) + 1
        if r.get("quality_warnings"):
            warning_count += 1

    print("Status counts:")
    for k, v in sorted(counts.items()):
        print(f"  {k}: {v}")
    print(f"Quality-warning files: {warning_count}")


if __name__ == "__main__":
    main()

# python3 regenerate_all_solution_same_params.py \
#   --root solver_outputs_cf1700_test \
#   --solver_model deepseek-v4-pro \
#   --thinking disabled \
#   --parallel_solver_workers 12 \
#   --overwrite
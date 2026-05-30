#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Remove code blocks from Solution prompts and regenerate Solution code with DeepSeek.

Typical usage:

  export DEEPSEEK_API_KEY="sk-..."

  python3 regenerate_solution_without_code.py \
    --root solver_outputs_cf1700_test \
    --model deepseek-v4-pro \
    --parallel_workers 6 \
    --replace \
    --overwrite

What this script does:
1. Recursively finds files like:
     CF1777C_Solution.prompt.txt
2. Removes code from the "OFFICIAL EDITORIAL / SOLUTION" section.
3. Writes a cleaned prompt.
4. Calls DeepSeek V4-pro again to generate C++17 code for the Solution condition.
5. Saves regenerated raw/code.

By default, it does NOT overwrite original Solution files.
Use --replace to overwrite Solution.prompt/raw/cpp.
Without --replace, it writes:
     CF1777C_SolutionNoCode.prompt.txt
     CF1777C_SolutionNoCode.raw.txt
     CF1777C_SolutionNoCode.cpp
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


# ============================================================
# Code removal
# ============================================================

def is_code_like_line(line: str) -> bool:
    s = line.strip()
    if not s:
        return False

    lower = s.lower()
    strong_markers = [
        "#include",
        "using namespace",
        "int main",
        "void solve",
        "long long",
        "vector<",
        "pair<",
        "cin >>",
        "cout <<",
        "scanf",
        "printf",
        "ios::sync_with_stdio",
        "return 0;",
        "typedef ",
        "template<",
        "template <",
        "struct ",
        "class ",
        "namespace ",
    ]
    if any(x in lower for x in strong_markers):
        return True

    if re.search(r"\b(for|while|if|else if|switch)\s*\(", s):
        return True
    if re.search(r"\b(int|long|ll|bool|char|string|double|auto|size_t)\s+[A-Za-z_][A-Za-z0-9_]*", s):
        return True

    syntax_chars = sum(ch in "{}[]();=<>+-*/%&|!^,:." for ch in s)
    alpha_chars = sum(ch.isalpha() for ch in s)
    if len(s) >= 8 and syntax_chars >= 4 and syntax_chars >= alpha_chars * 0.45:
        if s.endswith(";") or s.endswith("{") or s.endswith("}") or "++" in s or "--" in s:
            return True

    return False


def looks_like_code_fence_content(block: str) -> bool:
    lower = block.lower()
    strong_markers = [
        "#include",
        "using namespace",
        "int main",
        "void solve",
        "long long",
        "vector<",
        "cin >>",
        "cout <<",
        "scanf",
        "printf",
        "ios::sync_with_stdio",
        "return 0",
        "def ",
        "import ",
    ]
    if any(x in lower for x in strong_markers):
        return True

    lines = [ln.strip() for ln in block.splitlines() if ln.strip()]
    if len(lines) >= 3:
        codeish = sum(is_code_like_line(ln) for ln in lines)
        return codeish / max(1, len(lines)) >= 0.45

    return False


def remove_fenced_code_blocks(text: str) -> Tuple[str, int]:
    removed = 0

    def repl(match: re.Match) -> str:
        nonlocal removed
        lang = (match.group(1) or "").strip().lower()
        body = match.group(2) or ""

        code_langs = {
            "cpp", "c++", "cc", "cxx", "c", "java", "python", "py",
            "go", "rust", "rs", "kotlin", "kt", "javascript", "js",
        }

        if lang in code_langs or looks_like_code_fence_content(body):
            removed += 1
            return f"\n{CODE_OMITTED}\n"

        return match.group(0)

    pattern = r"```([A-Za-z0-9_+\-]*)\s*\n(.*?)```"
    new_text = re.sub(pattern, repl, text, flags=re.DOTALL)
    return new_text, removed


def remove_html_code_blocks(text: str) -> Tuple[str, int]:
    removed = 0
    for pat in [r"<pre[^>]*>.*?</pre>", r"<code[^>]*>.*?</code>"]:
        matches = re.findall(pat, text, flags=re.DOTALL | re.IGNORECASE)
        if matches:
            removed += len(matches)
            text = re.sub(pat, f"\n{CODE_OMITTED}\n", text, flags=re.DOTALL | re.IGNORECASE)
    return text, removed


def remove_plain_code_runs(text: str) -> Tuple[str, int]:
    lines = text.splitlines()
    out: List[str] = []
    removed_runs = 0

    i = 0
    n = len(lines)
    while i < n:
        if not is_code_like_line(lines[i]):
            out.append(lines[i])
            i += 1
            continue

        run = []
        while i < n and (is_code_like_line(lines[i]) or lines[i].strip() == ""):
            run.append(lines[i])
            i += 1

        nonempty = [ln for ln in run if ln.strip()]
        joined = "\n".join(nonempty).lower()

        strong = any(x in joined for x in [
            "#include",
            "using namespace",
            "int main",
            "void solve",
            "ios::sync_with_stdio",
            "cin >>",
            "cout <<",
            "return 0;",
        ])

        if strong or len(nonempty) >= 4:
            if not out or out[-1].strip() != CODE_OMITTED:
                out.append(CODE_OMITTED)
            removed_runs += 1
        else:
            out.extend(run)

    cleaned = "\n".join(out)
    cleaned = re.sub(rf"(?:\n\s*{re.escape(CODE_OMITTED)}\s*){{2,}}", f"\n{CODE_OMITTED}\n", cleaned)
    return cleaned, removed_runs


def strip_code_from_editorial(editorial: str) -> Tuple[str, Dict[str, int]]:
    text = editorial
    text, fenced = remove_fenced_code_blocks(text)
    text, html = remove_html_code_blocks(text)
    text, plain = remove_plain_code_runs(text)
    text = re.sub(r"\n{4,}", "\n\n\n", text).strip()

    stats = {
        "removed_fenced_blocks": fenced,
        "removed_html_blocks": html,
        "removed_plain_code_runs": plain,
        "total_removed_blocks": fenced + html + plain,
    }
    return text, stats


def split_solution_prompt(prompt: str) -> Tuple[str, str, str]:
    marker = "=== OFFICIAL EDITORIAL / SOLUTION ==="
    idx = prompt.find(marker)
    if idx < 0:
        raise ValueError("Cannot find OFFICIAL EDITORIAL / SOLUTION marker.")

    editorial_start = idx + len(marker)

    suffix_patterns = [
        r"\n\s*Carefully understand the official editorial/solution and convert it into a complete accepted C\+\+17 solution\.",
        r"\n\s*Write a complete C\+\+17 solution\.",
    ]

    for pat in suffix_patterns:
        m = re.search(pat, prompt[editorial_start:], flags=re.IGNORECASE)
        if m:
            editorial_end = editorial_start + m.start()
            suffix_start = editorial_end
            return prompt[:editorial_start], prompt[editorial_start:editorial_end], prompt[suffix_start:]

    return prompt[:editorial_start], prompt[editorial_start:], ""


def clean_solution_prompt(prompt: str) -> Tuple[str, Dict[str, Any]]:
    prefix, editorial, suffix = split_solution_prompt(prompt)
    clean_editorial, stats = strip_code_from_editorial(editorial)
    new_prompt = prefix + "\n" + clean_editorial.strip() + "\n\n" + suffix.lstrip()
    changed = new_prompt != prompt
    meta = {
        **stats,
        "changed": changed,
        "original_editorial_chars": len(editorial),
        "clean_editorial_chars": len(clean_editorial),
    }
    return new_prompt, meta


# ============================================================
# DeepSeek call
# ============================================================

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

    api_key = os.getenv(args.api_key_env)
    if not api_key:
        raise RuntimeError(f"Missing API key. Please run: export {args.api_key_env}='your_api_key'")

    return OpenAI(
        api_key=api_key,
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
            content = resp.choices[0].message.content or ""
            usage = getattr(resp, "usage", None)
            return content, usage
        except Exception as e:
            last_error = e
            if attempt < args.retries:
                time.sleep(args.retry_sleep_seconds * attempt)

    raise RuntimeError(f"DeepSeek call failed after {args.retries} attempts: {repr(last_error)}")


# ============================================================
# File processing
# ============================================================

def infer_prefix_from_solution_prompt(path: Path) -> str:
    name = path.name
    if name.endswith("_Solution.prompt.txt"):
        return name[: -len("_Solution.prompt.txt")]
    return path.stem


def should_process(prompt_path: Path, args) -> bool:
    if args.only_problem:
        return prompt_path.parent.name in args.only_problem or any(x in prompt_path.name for x in args.only_problem)
    return True


def process_one_prompt(prompt_path: Path, args) -> Dict[str, Any]:
    prefix = infer_prefix_from_solution_prompt(prompt_path)
    original_prompt = prompt_path.read_text(encoding="utf-8", errors="replace")
    cleaned_prompt, meta = clean_solution_prompt(original_prompt)

    if not meta["changed"] and args.skip_if_no_code_removed:
        return {
            "prefix": prefix,
            "prompt_path": str(prompt_path),
            "status": "skipped_no_code_removed",
            **meta,
        }

    if args.replace:
        out_prompt = prompt_path
        out_raw = prompt_path.with_name(f"{prefix}_Solution.raw.txt")
        out_cpp = prompt_path.with_name(f"{prefix}_Solution.cpp")

        backup_prompt = prompt_path.with_name(f"{prefix}_Solution.prompt.txt.bak")
        backup_raw = prompt_path.with_name(f"{prefix}_Solution.raw.txt.bak")
        backup_cpp = prompt_path.with_name(f"{prefix}_Solution.cpp.bak")

        if args.backup:
            if not backup_prompt.exists():
                backup_prompt.write_text(original_prompt, encoding="utf-8")
            if out_raw.exists() and not backup_raw.exists():
                backup_raw.write_text(out_raw.read_text(encoding="utf-8", errors="replace"), encoding="utf-8")
            if out_cpp.exists() and not backup_cpp.exists():
                backup_cpp.write_text(out_cpp.read_text(encoding="utf-8", errors="replace"), encoding="utf-8")
    else:
        out_prompt = prompt_path.with_name(f"{prefix}_SolutionNoCode.prompt.txt")
        out_raw = prompt_path.with_name(f"{prefix}_SolutionNoCode.raw.txt")
        out_cpp = prompt_path.with_name(f"{prefix}_SolutionNoCode.cpp")

    if out_cpp.exists() and out_raw.exists() and not args.overwrite:
        return {
            "prefix": prefix,
            "prompt_path": str(out_prompt),
            "cpp_path": str(out_cpp),
            "raw_path": str(out_raw),
            "status": "skipped_existing",
            **meta,
        }

    out_prompt.write_text(cleaned_prompt, encoding="utf-8")

    if args.dry_run:
        return {
            "prefix": prefix,
            "prompt_path": str(out_prompt),
            "status": "dry_run",
            **meta,
        }

    raw_output, usage = call_deepseek(cleaned_prompt, args)
    code = extract_cpp_code(raw_output)

    out_raw.write_text(raw_output, encoding="utf-8")
    out_cpp.write_text(code, encoding="utf-8")

    usage_obj = None
    if usage is not None:
        try:
            usage_obj = usage.model_dump()
        except Exception:
            usage_obj = str(usage)

    return {
        "prefix": prefix,
        "original_prompt_path": str(prompt_path),
        "prompt_path": str(out_prompt),
        "raw_path": str(out_raw),
        "cpp_path": str(out_cpp),
        "status": "ok",
        "model": args.model,
        "thinking": args.thinking,
        "usage": usage_obj,
        **meta,
    }


def find_solution_prompts(root: Path) -> List[Path]:
    return sorted(p for p in root.rglob("*_Solution.prompt.txt") if p.is_file())


# ============================================================
# CLI
# ============================================================

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=str, required=True, help="Root directory, e.g. solver_outputs_cf1700_test")
    parser.add_argument("--model", type=str, default="deepseek-v4-pro")
    parser.add_argument("--base_url", type=str, default=DEFAULT_BASE_URL)
    parser.add_argument("--api_key_env", type=str, default="DEEPSEEK_API_KEY")
    parser.add_argument("--thinking", choices=["omit", "disabled", "enabled"], default="omit")

    parser.add_argument("--temperature", type=float, default=0.2)
    parser.add_argument("--top_p", type=float, default=0.95)
    parser.add_argument("--max_tokens", type=int, default=8192)
    parser.add_argument("--timeout_seconds", type=float, default=240.0)
    parser.add_argument("--retries", type=int, default=2)
    parser.add_argument("--retry_sleep_seconds", type=float, default=3.0)
    parser.add_argument("--parallel_workers", type=int, default=4)

    parser.add_argument("--replace", action="store_true", help="Overwrite original Solution.prompt/raw/cpp.")
    parser.add_argument("--backup", action="store_true", default=True, help="Backup original prompt/raw/cpp before replacing.")
    parser.add_argument("--overwrite", action="store_true")
    parser.add_argument("--dry_run", action="store_true")
    parser.add_argument("--skip_if_no_code_removed", action="store_true", default=True)
    parser.add_argument("--only_problem", nargs="*", default=None, help="Only process selected problem folders/prefixes.")
    return parser.parse_args()


def main():
    args = parse_args()
    root = Path(args.root).expanduser().resolve()
    if not root.exists():
        raise FileNotFoundError(f"Root not found: {root}")

    prompts = [p for p in find_solution_prompts(root) if should_process(p, args)]

    print(f"Root: {root}")
    print(f"Found Solution prompts: {len(prompts)}")
    print(f"Model: {args.model}")
    print(f"Replace original files: {args.replace}")
    print(f"Dry run: {args.dry_run}")

    if not prompts:
        return

    records: List[Dict[str, Any]] = []
    workers = max(1, min(args.parallel_workers, len(prompts)))

    if workers == 1:
        for p in prompts:
            print(f"\nProcessing {p}")
            try:
                rec = process_one_prompt(p, args)
            except Exception as e:
                rec = {
                    "prompt_path": str(p),
                    "status": "failed",
                    "error": repr(e),
                }
            print(f"  -> {rec.get('status')}, removed={rec.get('total_removed_blocks')}")
            records.append(rec)
    else:
        print(f"Running with {workers} parallel workers...")
        with ThreadPoolExecutor(max_workers=workers) as executor:
            future_to_prompt = {executor.submit(process_one_prompt, p, args): p for p in prompts}
            for future in as_completed(future_to_prompt):
                p = future_to_prompt[future]
                try:
                    rec = future.result()
                except Exception as e:
                    rec = {
                        "prompt_path": str(p),
                        "status": "failed",
                        "error": repr(e),
                    }
                print(f"  -> {p.parent.name}: {rec.get('status')}, removed={rec.get('total_removed_blocks')}")
                records.append(rec)

    summary_path = root / "solution_nocode_regeneration_summary.json"
    summary_path.write_text(json.dumps(records, ensure_ascii=False, indent=2), encoding="utf-8")

    print()
    print(f"Done. Summary written to: {summary_path}")
    print("Status counts:")
    counts: Dict[str, int] = {}
    for r in records:
        status = r.get("status", "unknown")
        counts[status] = counts.get(status, 0) + 1
    for k, v in sorted(counts.items()):
        print(f"  {k}: {v}")


if __name__ == "__main__":
    main()

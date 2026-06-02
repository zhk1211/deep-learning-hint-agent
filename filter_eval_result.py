#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import re
from pathlib import Path
from typing import Optional, Set


PROBLEM_ID_RE = re.compile(r"CF\d+[A-Z](?:\d+)?")


def extract_problem_id(text: str) -> Optional[str]:
    match = PROBLEM_ID_RE.search(text)
    if match:
        return match.group(0)
    return None


def resolve_with_hint_dir(path: Path) -> Path:
    """
    支持两种传法：
    1. 直接传 cf_dataset/with_hint
    2. 传 cf_dataset，然后自动进入 cf_dataset/with_hint
    """
    if path.name == "with_hint":
        return path

    candidate = path / "with_hint"
    if candidate.exists():
        return candidate

    return path


def collect_existing_problem_ids(with_hint_dir: Path) -> Set[str]:
    existing = set()

    for file_path in with_hint_dir.rglob("*.json"):
        pid = extract_problem_id(file_path.name)
        if pid:
            existing.add(pid)

    return existing


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--input", required=True, help="原始 eval_result.txt")
    parser.add_argument("--with_hint_dir", required=True, help="with_hint 文件夹，或者 cf_dataset 文件夹")
    parser.add_argument("--output", default="eval_result_filtered.txt", help="过滤后的输出文件")
    parser.add_argument("--removed", default="removed_existing_with_hint.txt", help="被删除的行")
    parser.add_argument("--inplace", action="store_true", help="直接覆盖原文件，同时生成 .bak 备份")
    args = parser.parse_args()

    input_path = Path(args.input)
    with_hint_dir = resolve_with_hint_dir(Path(args.with_hint_dir))
    output_path = Path(args.output)
    removed_path = Path(args.removed)

    if not input_path.exists():
        raise FileNotFoundError(f"Input file not found: {input_path}")

    if not with_hint_dir.exists():
        raise FileNotFoundError(f"with_hint dir not found: {with_hint_dir}")

    existing_ids = collect_existing_problem_ids(with_hint_dir)

    kept_lines = []
    removed_lines = []

    for raw_line in input_path.read_text(encoding="utf-8").splitlines():
        line = raw_line.strip()

        if not line:
            kept_lines.append(raw_line)
            continue

        pid = extract_problem_id(line)

        if pid and pid in existing_ids:
            removed_lines.append(raw_line)
        else:
            kept_lines.append(raw_line)

    if args.inplace:
        backup_path = input_path.with_suffix(input_path.suffix + ".bak")
        backup_path.write_text(input_path.read_text(encoding="utf-8"), encoding="utf-8")
        input_path.write_text("\n".join(kept_lines) + "\n", encoding="utf-8")
        final_output_path = input_path
    else:
        output_path.write_text("\n".join(kept_lines) + "\n", encoding="utf-8")
        final_output_path = output_path

    removed_path.write_text("\n".join(removed_lines) + ("\n" if removed_lines else ""), encoding="utf-8")

    print("Done.")
    print(f"with_hint dir: {with_hint_dir}")
    print(f"Existing problem ids in with_hint: {len(existing_ids)}")
    print(f"Original lines: {len(kept_lines) + len(removed_lines)}")
    print(f"Kept lines: {len(kept_lines)}")
    print(f"Removed lines: {len(removed_lines)}")
    print(f"Output written to: {final_output_path}")
    print(f"Removed lines written to: {removed_path}")


if __name__ == "__main__":
    main()
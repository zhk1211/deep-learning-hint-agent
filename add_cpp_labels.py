#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import re
import argparse
from pathlib import Path

def infer_label(path: Path) -> str | None:
    name = path.name

    m = re.search(r"_Hint(\d+)(?:_s\d+)?\.cpp$", name)
    if m:
        return f"Hint{m.group(1)}"

    if re.search(r"_Solution(?:_s\d+)?\.cpp$", name):
        return "Solution"

    return None

def add_label_to_file(path: Path, label: str, dry_run: bool = False) -> bool:
    text = path.read_text(encoding="utf-8", errors="replace")
    tag = f"// {label}"

    # 避免重复添加
    if text.startswith(tag + "\n") or text.startswith(tag + "\r\n"):
        return False

    # 如果文件开头已经是别的 Hint/Solution 标记，也不重复堆叠，直接替换第一行
    lines = text.splitlines(keepends=True)
    if lines and re.fullmatch(r"// (Hint\d+|Solution)\s*\r?\n?", lines[0]):
        new_text = tag + "\n" + "".join(lines[1:])
    else:
        new_text = tag + "\n" + text

    if not dry_run:
        path.write_text(new_text, encoding="utf-8")

    return True

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=str, default="solver_outputs_cf1700_test")
    parser.add_argument("--dry_run", action="store_true")
    args = parser.parse_args()

    root = Path(args.root)
    if not root.exists():
        raise FileNotFoundError(f"Root directory not found: {root}")

    changed = 0
    skipped = 0

    for path in sorted(root.rglob("*.cpp")):
        label = infer_label(path)
        if label is None:
            skipped += 1
            continue

        ok = add_label_to_file(path, label, dry_run=args.dry_run)
        if ok:
            changed += 1
            print(f"ADD // {label}: {path}")
        else:
            skipped += 1

    print()
    print(f"changed: {changed}")
    print(f"skipped: {skipped}")

if __name__ == "__main__":
    main()

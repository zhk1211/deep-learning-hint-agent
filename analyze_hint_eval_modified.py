#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Analyze hint-evaluation results and draw statistics figures.

Input format:
    CF1798C 111111
    CF1792D 0011111
    CF1776F

A line without a bit string means the problem is not in the evaluated set and will be skipped.

Bit-string convention:
    first bit    = statement only / 0 hint
    middle bits  = first k hints
    last bit     = official solution/editorial condition

Important:
    percentage_accuracy_curve.png only analyzes the hint process:
        0 Hint -> 1 Hint -> ... -> All Hints

    It does NOT include the final official-solution bit in the normalized hint curve.
    Instead, it plots the official-solution accuracy as a separate point on the right.
"""

import argparse
import csv
import json
import re
from pathlib import Path
from typing import Dict, List, Set, Tuple

import matplotlib.pyplot as plt


SAMPLE_DATA = r"""
CF1799C
CF1798C 111111
CF1792D 0011111
CF1783C 110111111
CF1779D 11010000
CF1777C 0111001
CF1776F
CF1772E 10001
CF1770C 00111
CF1765D 0000001
CF1760G 0010011
CF1743D 0011000001
CF1735D 0100011
CF1731D 0111111
CF1731C 1111111
CF1722F 10001001
CF1715C 0111111
CF1714G 111111
CF1709D 000001
CF1706D1
CF1702F 011011
CF1700C
CF1699C 01111
CF1695C 1111111
CF1693B 111111
CF1692H 00101
CF1690F 1100111
CF1688C 11111
CF1687B 11100011
CF1684D 0001011100001
CF1681D 000000
CF1677B 0011
CF1672D 11101
CF1670D 00001011
CF1661C 11011
CF1660F1 11111
"""


PROBLEM_ID_RE = re.compile(r"\bCF\s*(\d+)\s*([A-Za-z])([0-9]*)\b")


def normalize_problem_id(pid: str) -> str:
    """Normalize problem ids such as cf2219b1 / CF 2219 B1 to CF2219B1."""
    match = PROBLEM_ID_RE.search(pid)
    if not match:
        return pid.strip().upper()
    contest_id, index_letter, index_suffix = match.groups()
    return f"CF{contest_id}{index_letter.upper()}{index_suffix}"


def collect_with_hint_problem_ids(with_hint_dir: Path) -> Set[str]:
    """Collect problem ids that already appear under cf_scraper/cf_dataset/with_hint."""
    problem_ids: Set[str] = set()

    if not with_hint_dir.exists():
        return problem_ids

    for path in with_hint_dir.rglob("*"):
        if not path.is_file():
            continue

        match = PROBLEM_ID_RE.search(path.stem)
        if match:
            problem_ids.add(normalize_problem_id(match.group(0)))

    return problem_ids


def skip_records_existing_in_with_hint(records: List[Dict], existing_problem_ids: Set[str]) -> Tuple[List[Dict], List[str]]:
    """Remove evaluation records whose problem id has already appeared in with_hint."""
    if not existing_problem_ids:
        return records, []

    kept_records: List[Dict] = []
    skipped_existing: List[str] = []

    for rec in records:
        problem_id = normalize_problem_id(rec["problem"])
        if problem_id in existing_problem_ids:
            skipped_existing.append(rec["problem"])
        else:
            kept_records.append(rec)

    return kept_records, skipped_existing


def parse_data(text: str) -> Tuple[List[Dict], List[str], List[str]]:
    records: List[Dict] = []
    skipped: List[str] = []
    bad_lines: List[str] = []

    for raw_line in text.splitlines():
        line = raw_line.strip()
        if not line:
            continue

        parts = line.split()
        pid = parts[0]
        bits = parts[1] if len(parts) >= 2 else ""

        if not bits:
            skipped.append(pid)
            continue

        if not re.fullmatch(r"[01]+", bits):
            bad_lines.append(raw_line)
            continue

        if len(bits) < 2:
            bad_lines.append(raw_line)
            continue

        hint_bits = bits[:-1]
        solution_bit = bits[-1]

        records.append({
            "problem": pid,
            "bits": bits,
            "hint_bits": hint_bits,
            "length": len(bits),
            "hint_length": len(hint_bits),
            "num_hints": max(0, len(bits) - 2),
            "no_hint": int(bits[0]),
            "all_hints": int(hint_bits[-1]),
            "solution": int(solution_bit),
        })

    return records, skipped, bad_lines


def write_csv(path: Path, rows: List[Dict]) -> None:
    if not rows:
        path.write_text("", encoding="utf-8")
        return

    fieldnames = list(rows[0].keys())
    with path.open("w", encoding="utf-8", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(rows)


def percentage_bins(records: List[Dict], bins: int = 20) -> List[Dict]:
    """
    Aggregate only hint_bits = bits[:-1] onto fixed percentage bins.

    For a hint_bits string of length H, bit i occupies [i/H, (i+1)/H).
    The final solution bit is deliberately excluded here.
    """
    rows: List[Dict] = []

    for b in range(bins):
        bin_l = b / bins
        bin_r = (b + 1) / bins

        weighted_success = 0.0
        total_weight = 0.0
        contributing = set()

        for rec in records:
            hint_bits = rec["hint_bits"]
            H = len(hint_bits)
            if H == 0:
                continue

            for i, ch in enumerate(hint_bits):
                seg_l = i / H
                seg_r = (i + 1) / H
                overlap = max(0.0, min(bin_r, seg_r) - max(bin_l, seg_l))
                if overlap <= 0:
                    continue

                weighted_success += int(ch) * overlap
                total_weight += overlap
                contributing.add(rec["problem"])

        rows.append({
            "bin": b,
            "percent_left": bin_l * 100,
            "percent_right": bin_r * 100,
            "percent_mid": (bin_l + bin_r) * 50,
            "accuracy": weighted_success / total_weight if total_weight > 0 else float("nan"),
            "contributing_problems": len(contributing),
            "total_weight": total_weight,
        })

    return rows


def stage_summary(records: List[Dict]) -> List[Dict]:
    total = len(records)

    def avg(key: str) -> float:
        if total == 0:
            return float("nan")
        return sum(int(r[key]) for r in records) / total

    rows = []
    for name, key in [
        ("0 Hint / statement only", "no_hint"),
        ("All generated hints", "all_hints"),
        ("Official solution/editorial", "solution"),
    ]:
        correct = sum(int(r[key]) for r in records)
        rows.append({
            "stage": name,
            "correct": correct,
            "total": total,
            "accuracy": avg(key),
        })

    return rows


def length_summary(records: List[Dict]) -> List[Dict]:
    cnt: Dict[int, int] = {}
    for r in records:
        cnt[r["length"]] = cnt.get(r["length"], 0) + 1

    return [{"length": k, "count": cnt[k]} for k in sorted(cnt)]


def plot_percentage_curve(bin_rows: List[Dict], stage_rows: List[Dict], out_path: Path) -> None:
    xs = [r["percent_mid"] for r in bin_rows]
    ys = [r["accuracy"] * 100 for r in bin_rows]

    solution_acc = None
    for r in stage_rows:
        if r["stage"] == "Official solution/editorial":
            solution_acc = r["accuracy"] * 100

    plt.figure(figsize=(9.5, 5.2))
    plt.plot(xs, ys, marker="o", label="Hint process")

    if solution_acc is not None:
        solution_x = 110
        plt.scatter([solution_x], [solution_acc], marker="*", s=180, label="Official solution")
        plt.axvline(100, linestyle="--", linewidth=1, alpha=0.5)
        plt.text(solution_x, solution_acc + 2, "Solution", ha="center", va="bottom")

    plt.xlabel("Normalized hint progress: 0 Hint → All Hints (%)")
    plt.ylabel("Accuracy (%)")
    plt.title("Accuracy by normalized hint progress")
    plt.ylim(0, 100)
    plt.xlim(0, 115)
    plt.grid(True, alpha=0.3)
    plt.legend()
    plt.tight_layout()
    plt.savefig(out_path, dpi=200)
    plt.close()


def plot_stage_bar(stage_rows: List[Dict], out_path: Path) -> None:
    labels = [r["stage"] for r in stage_rows]
    values = [r["accuracy"] * 100 for r in stage_rows]

    plt.figure(figsize=(8, 5))
    plt.bar(labels, values)
    plt.ylabel("Accuracy (%)")
    plt.title("Accuracy at key conditions")
    plt.ylim(0, 100)
    plt.xticks(rotation=15, ha="right")
    plt.tight_layout()
    plt.savefig(out_path, dpi=200)
    plt.close()


def plot_length_distribution(len_rows: List[Dict], out_path: Path) -> None:
    labels = [str(r["length"]) for r in len_rows]
    values = [r["count"] for r in len_rows]

    plt.figure(figsize=(8, 5))
    plt.bar(labels, values)
    plt.xlabel("Full bit-string length, including solution bit")
    plt.ylabel("Number of problems")
    plt.title("Distribution of evaluation-string lengths")
    plt.tight_layout()
    plt.savefig(out_path, dpi=200)
    plt.close()


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--input", type=str, default=None, help="Input txt file. If omitted, use SAMPLE_DATA in script.")
    parser.add_argument("--output_dir", type=str, default="hint_eval_analysis")
    parser.add_argument("--bins", type=int, default=20, help="Number of percentage bins; 20 means 5% per bin.")
    parser.add_argument(
        "--dataset_dir",
        type=str,
        default="cf_scraper/cf_dataset",
        help="Dataset root directory. Used to find the with_hint folder.",
    )
    parser.add_argument(
        "--with_hint_dir",
        type=str,
        default=None,
        help="Existing with_hint directory. If omitted, use <dataset_dir>/with_hint.",
    )
    args = parser.parse_args()

    if args.input:
        text = Path(args.input).read_text(encoding="utf-8")
    else:
        text = SAMPLE_DATA

    out_dir = Path(args.output_dir)
    out_dir.mkdir(parents=True, exist_ok=True)

    records, skipped, bad_lines = parse_data(text)

    dataset_dir = Path(args.dataset_dir)
    with_hint_dir = Path(args.with_hint_dir) if args.with_hint_dir else dataset_dir / "with_hint"
    existing_with_hint_problem_ids = collect_with_hint_problem_ids(with_hint_dir)
    records, skipped_existing_with_hint = skip_records_existing_in_with_hint(records, existing_with_hint_problem_ids)

    bin_rows = percentage_bins(records, bins=args.bins)
    stage_rows = stage_summary(records)
    len_rows = length_summary(records)

    write_csv(out_dir / "parsed_records.csv", records)
    write_csv(out_dir / "percentage_bins_hint_only.csv", bin_rows)
    write_csv(out_dir / "stage_summary.csv", stage_rows)
    write_csv(out_dir / "length_summary.csv", len_rows)

    (out_dir / "skipped_problems.txt").write_text("\n".join(skipped) + ("\n" if skipped else ""), encoding="utf-8")
    (out_dir / "skipped_existing_with_hint.txt").write_text(
        "\n".join(skipped_existing_with_hint) + ("\n" if skipped_existing_with_hint else ""),
        encoding="utf-8",
    )
    (out_dir / "bad_lines.txt").write_text("\n".join(bad_lines) + ("\n" if bad_lines else ""), encoding="utf-8")

    plot_percentage_curve(bin_rows, stage_rows, out_dir / "percentage_accuracy_curve.png")
    plot_stage_bar(stage_rows, out_dir / "stage_accuracy_bar.png")
    plot_length_distribution(len_rows, out_dir / "length_distribution.png")

    def find_acc(stage_name: str):
        for r in stage_rows:
            if r["stage"] == stage_name:
                return r["accuracy"]
        return None

    summary = {
        "evaluated_problems": len(records),
        "skipped_problems": len(skipped),
        "skipped_existing_with_hint": len(skipped_existing_with_hint),
        "bad_lines": len(bad_lines),
        "bins": args.bins,
        "dataset_dir": str(dataset_dir),
        "with_hint_dir": str(with_hint_dir),
        "existing_with_hint_problems": len(existing_with_hint_problem_ids),
        "percentage_curve": "hint_only; official solution is plotted separately on the right",
        "no_hint_accuracy": find_acc("0 Hint / statement only"),
        "all_hints_accuracy": find_acc("All generated hints"),
        "solution_accuracy": find_acc("Official solution/editorial"),
    }

    (out_dir / "summary.json").write_text(json.dumps(summary, ensure_ascii=False, indent=2), encoding="utf-8")

    print("Done.")
    print(f"Evaluated problems: {len(records)}")
    print(f"Skipped problems: {len(skipped)}")
    print(f"Skipped existing with_hint problems: {len(skipped_existing_with_hint)}")
    print(f"Existing problems found in {with_hint_dir}: {len(existing_with_hint_problem_ids)}")
    print(f"Bad lines: {len(bad_lines)}")
    print("\nStage summary:")
    for r in stage_rows:
        print(f"{r['stage']}: {r['correct']}/{r['total']} = {r['accuracy']:.2%}")
    print(f"\nOutputs written to: {out_dir.resolve()}")


if __name__ == "__main__":
    main()

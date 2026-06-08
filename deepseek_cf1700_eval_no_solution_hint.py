#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Batch evaluate a solver model on Codeforces problems with hints generated
WITHOUT using the official solution/editorial.

This script is a variant of deepseek_cf1700_eval.py for testing the hint
model's ability when it cannot see the official editorial during hint
generation.

Main pipeline:
  1. Load problem statement from the dataset.
  2. Generate Hint1..HintK using the local LoRA hint model with:
       problem statement + previous hints
     but WITHOUT:
       official editorial / solution
  3. Ask DeepSeek V4-Pro to generate code under:
       Hint0, Hint1, Hint2, ..., HintK
  4. Optionally also create a Solution condition as an upper-bound reference.
     The Solution condition is only for solver evaluation; it is NOT used by
     the hint generator.

Typical usage:

  export DEEPSEEK_API_KEY="sk-..."

  python3 deepseek_cf1700_eval_no_solution_hint.py \
    --prefix_file cf_difficulty_1700_prefixes.txt \
    --output_dir solver_outputs_cf1700_no_solution_hint \
    --solver_model deepseek-v4-pro \
    --thinking disabled \
    --mode both \
    --samples_per_condition 1 \
    --hint_source auto \
    --base_model_path ./qwen-coder-7b \
    --lora_path ./cf_hint_lora_model_budgeted/checkpoint-175 \
    --num_candidates 10 \
    --candidate_batch_size 1 \
    --parallel_solver_workers 6 \
    --overwrite

For a single problem:

  python3 deepseek_cf1700_eval_no_solution_hint.py \
    --prefix CF1777C \
    --output_dir solver_outputs_test_no_solution_hint \
    --solver_model deepseek-v4-pro \
    --thinking disabled \
    --mode both \
    --hint_source auto \
    --overwrite
    
    
python3 deepseek_cf1700_eval_no_solution_hint.py \
  --prefix_file selected_no_solution_hint_prefixes.txt \
  --output_dir solver_outputs_selected_no_solution_hint \
  --solver_model deepseek-v4-pro \
  --thinking disabled \
  --mode both \
  --samples_per_condition 1 \
  --hint_source auto \
  --hint_context no_solution \
  --base_model_path ./qwen-coder-7b \
  --lora_path ./cf_hint_lora_model_budgeted/checkpoint-175 \
  --num_candidates 5 \
  --candidate_batch_size 1 \
  --parallel_solver_workers 6 \
  --overwrite
"""

import argparse
import glob
import json
import os
import random
import re
import time
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple


BASE_DIR = Path(__file__).resolve().parent

DEFAULT_BASE_MODEL_PATH = str(BASE_DIR / "qwen-coder-7b")
DEFAULT_LORA_PATH = str(BASE_DIR / "cf_hint_lora_model_budgeted" / "checkpoint-175")
DEFAULT_DEEPSEEK_BASE_URL = "https://api.deepseek.com"

FINAL_STOP_REPLY = "No. The rest you need to think about yourself."

SYSTEM_PROMPT = (
    "You are an elite Codeforces tutor.\n"
    "For every request, independently decide the next hint based on the problem statement, the full editorial, "
    "and all previous hints already given.\n"
    "Your primary goal is to identify the MOST PRELIMINARY and CRITICAL next observation that bridges the gap between the current hint state and the final solution. "
    "The current hint state is defined only by the hints already given to the user, not by the full editorial. "
    "Before generating a new hint, first infer what ideas have already been covered by previous hints. "
    "Then either give the next smallest useful observation that advances beyond those hints, or stop if another hint would only be repetitive, vague, or implementation-level.\n\n"
    "You must output JSON only. Do not blindly continue a memorized sequence; re-evaluate the state every time.\n\n"
    "Decision rules:\n"
    "1. Analyze the problem statement and the full editorial, then read the previous hints to precisely track what has already been covered.\n"
    "2. Extract the NEXT MOST FUNDAMENTAL OBSERVATION. Focus ONLY on the immediate next logical stepping stone. Do not jump ahead to the final algorithm.\n"
    "3. The visible hint must contain exactly ONE new idea. Do not combine multiple future steps into one hint.\n"
    "4. A hint may be a Socratic question, but it must include a concrete object, condition, observation, or subproblem.\n"
    "5. If another useful, non-redundant hint remains, set action='hint'.\n"
    "6. If the remaining gap is small enough that any further hint would be repetitive, overly vague, or just implementation work, set action='stop'. Be willing to stop early for easy problems.\n"
    "7. The next hint must not repeat or paraphrase previous hints.\n\n"
    "Required JSON schema for a hint:\n"
    "{\n"
    "  \"action\": \"hint\",\n"
    "  \"covered_so_far\": [\"short summary of a previous hint\"],\n"
    "  \"next_focus\": \"the most preliminary and critical next observation\",\n"
    "  \"hint\": \"**Hint k**\\n...\"\n"
    "}\n\n"
    "Required JSON schema for stopping:\n"
    "{\n"
    "  \"action\": \"stop\",\n"
    "  \"covered_so_far\": [\"short summary of previous hints\"],\n"
    "  \"next_focus\": \"\",\n"
    f"  \"hint\": \"{FINAL_STOP_REPLY}\"\n"
    "}"
)

SYSTEM_PROMPT_NO_SOLUTION = (
    "You are an elite Codeforces tutor.\n"
    "For every request, independently decide the next hint based only on the problem statement "
    "and all previous hints already given. You do NOT have access to the official editorial or solution.\n"
    "Your primary goal is to identify the MOST PRELIMINARY and CRITICAL next observation that can help the user progress from the current hint state. "
    "The current hint state is defined only by the hints already given to the user.\n"
    "Before generating a new hint, first infer what ideas have already been covered by previous hints. "
    "Then either give the next smallest useful observation that advances beyond those hints, or stop if another hint would only be repetitive, vague, or implementation-level.\n\n"
    "You must output JSON only. Do not blindly continue a memorized sequence; re-evaluate the state every time.\n\n"
    "Decision rules:\n"
    "1. Analyze the problem statement and read the previous hints to precisely track what has already been covered.\n"
    "2. Extract the NEXT MOST FUNDAMENTAL OBSERVATION that can be inferred from the statement and the current hint state. Focus ONLY on the immediate next logical stepping stone. Do not jump ahead to the final algorithm unless it is already strongly implied.\n"
    "3. The visible hint must contain exactly ONE new idea. Do not combine multiple future steps into one hint.\n"
    "4. A hint may be a Socratic question, but it must include a concrete object, condition, observation, or subproblem.\n"
    "5. If another useful, non-redundant hint remains, set action='hint'.\n"
    "6. If the remaining gap is small enough that any further hint would be repetitive, overly vague, or just implementation work, set action='stop'. Be willing to stop early for easy problems.\n"
    "7. The next hint must not repeat or paraphrase previous hints.\n\n"
    "Required JSON schema for a hint:\n"
    "{\n"
    "  \"action\": \"hint\",\n"
    "  \"covered_so_far\": [\"short summary of a previous hint\"],\n"
    "  \"next_focus\": \"the most preliminary and critical next observation\",\n"
    "  \"hint\": \"**Hint k**\\n...\"\n"
    "}\n\n"
    "Required JSON schema for stopping:\n"
    "{\n"
    "  \"action\": \"stop\",\n"
    "  \"covered_so_far\": [\"short summary of previous hints\"],\n"
    "  \"next_focus\": \"\",\n"
    f"  \"hint\": \"{FINAL_STOP_REPLY}\"\n"
    "}"
)

STOP_MARKERS = [
    FINAL_STOP_REPLY.lower(),
    "no. the rest",
    "think about yourself",
    "no further hint",
    "no more hint",
]


# ============================================================
# General utilities
# ============================================================

def normalize_text(x: Any) -> str:
    return str(x or "").strip()


def sanitize_filename(name: str) -> str:
    name = normalize_text(name)
    name = re.sub(r"\s+", "_", name)
    name = re.sub(r"[^A-Za-z0-9_.-]+", "_", name)
    name = re.sub(r"_+", "_", name).strip("_")
    return name or "problem"


def infer_prefix(problem_file: Path, explicit_prefix: Optional[str]) -> str:
    if explicit_prefix:
        return sanitize_filename(explicit_prefix)

    stem = problem_file.stem
    m = re.search(r"(CF\d+[A-Z]\d*)", stem, flags=re.IGNORECASE)
    if m:
        return sanitize_filename(m.group(1).upper())

    return sanitize_filename(stem)


def extract_prefixes_from_text(text: str) -> List[str]:
    """Extract CF prefixes from copied Codeforces problem-list text."""
    seen = set()
    prefixes = []

    # Lines like: 1777C\tQuiz Master...
    for m in re.finditer(r"(?m)^\s*(\d{3,5}[A-Z]\d*)\b", text):
        p = "CF" + m.group(1)
        if p not in seen:
            seen.add(p)
            prefixes.append(p)

    # Also accept already-normalized CF1777C anywhere.
    for m in re.finditer(r"\bCF(\d{3,5}[A-Z]\d*)\b", text, flags=re.IGNORECASE):
        p = "CF" + m.group(1).upper()
        if p not in seen:
            seen.add(p)
            prefixes.append(p)

    return prefixes


def read_prefixes(args) -> List[str]:
    prefixes: List[str] = []
    seen = set()

    def add(p: str):
        p = normalize_text(p).upper()
        if not p:
            return
        if not p.startswith("CF"):
            p = "CF" + p
        if p not in seen:
            seen.add(p)
            prefixes.append(p)

    if args.prefix:
        add(args.prefix)

    for path_str in args.prefix_file or []:
        path = Path(path_str).expanduser()
        text = path.read_text(encoding="utf-8", errors="replace")
        for p in extract_prefixes_from_text(text):
            add(p)

    if args.shuffle:
        random.Random(args.seed).shuffle(prefixes)

    if args.start_index > 0:
        prefixes = prefixes[args.start_index:]

    if args.limit is not None:
        prefixes = prefixes[: args.limit]

    if not prefixes:
        raise ValueError("No prefixes provided. Use --prefix or --prefix_file.")

    return prefixes


# ============================================================
# JSON loading
# ============================================================

def join_sections(obj: Any) -> str:
    """Robustly join statement / solution sections from common scraper schemas."""
    if obj is None:
        return ""

    if isinstance(obj, str):
        return normalize_text(obj)

    if isinstance(obj, dict):
        if isinstance(obj.get("sections"), list):
            return join_sections(obj.get("sections"))

        title = normalize_text(obj.get("title", ""))
        content = normalize_text(
            obj.get("content")
            or obj.get("text")
            or obj.get("description")
            or obj.get("statement")
            or obj.get("solution")
            or obj.get("editorial")
            or ""
        )

        if content:
            if title and title.lower() not in {"statement", "description", "solution", "tutorial", "editorial"}:
                return f"{title}\n{content}".strip()
            return content

        parts = []
        for key in ("name", "legend", "input", "output", "note"):
            value = normalize_text(obj.get(key, ""))
            if value:
                parts.append(f"{key}\n{value}")
        return "\n\n".join(parts).strip()

    if isinstance(obj, list):
        parts = []
        for item in obj:
            text = join_sections(item)
            if text:
                parts.append(text)
        return "\n\n".join(parts).strip()

    return ""


def find_statement_field(data: Dict[str, Any]) -> Any:
    for key in (
        "statement",
        "problem_statement",
        "statements",
        "description",
        "problemDescription",
        "content",
    ):
        value = data.get(key)
        if value:
            return value

    problem = data.get("problem")
    if isinstance(problem, dict):
        for key in ("statement", "description", "content"):
            value = problem.get(key)
            if value:
                return value

    return None


def find_editorial_field(data: Dict[str, Any]) -> Any:
    for key in ("solutions", "solution", "editorial", "tutorial", "tutorials"):
        value = data.get(key)
        if value:
            return value
    return None


def clean_existing_hints(raw_hints: Any) -> List[str]:
    hints: List[str] = []
    if not isinstance(raw_hints, list):
        return hints

    for item in raw_hints:
        if isinstance(item, dict):
            content = normalize_text(
                item.get("content")
                or item.get("text")
                or item.get("hint")
                or ""
            )
        else:
            content = normalize_text(item)

        if not content:
            continue

        content = re.sub(
            r"^\s*(?:\*\*)?\s*hint\s*\d+\s*(?:\*\*)?\s*[:.\-]?\s*",
            "",
            content,
            flags=re.IGNORECASE,
        ).strip()

        if content:
            hints.append(f"**Hint {len(hints) + 1}**\n{content}")

    return hints


def pick_first_nonempty(*values: Any) -> str:
    for value in values:
        text = normalize_text(value)
        if text and text.lower() not in {"none", "null"}:
            return text
    return ""


def extract_limit_from_text(statement: str, kind: str) -> str:
    text = statement or ""

    if kind == "time":
        patterns = [
            r"time\s+limit\s+per\s+test\s*[:：]?\s*([^\n\r]+)",
            r"time\s+limit\s*[:：]?\s*([^\n\r]+)",
        ]
    else:
        patterns = [
            r"memory\s+limit\s+per\s+test\s*[:：]?\s*([^\n\r]+)",
            r"memory\s+limit\s*[:：]?\s*([^\n\r]+)",
        ]

    for pat in patterns:
        m = re.search(pat, text, flags=re.IGNORECASE)
        if m:
            value = normalize_text(m.group(1))
            value = re.split(r"\s{2,}|\t|input|output", value, maxsplit=1, flags=re.IGNORECASE)[0].strip()
            if value:
                return value

    return ""


def extract_limits(data: Dict[str, Any], statement: str, default_time_limit: str, default_memory_limit: str) -> Tuple[str, str]:
    problem = data.get("problem", {})
    if not isinstance(problem, dict):
        problem = {}

    time_limit = pick_first_nonempty(
        data.get("timeLimit"),
        data.get("time_limit"),
        data.get("time_limit_per_test"),
        data.get("timeLimitPerTest"),
        problem.get("timeLimit"),
        problem.get("time_limit"),
        problem.get("timeLimitPerTest"),
        extract_limit_from_text(statement, "time"),
        default_time_limit,
    )

    memory_limit = pick_first_nonempty(
        data.get("memoryLimit"),
        data.get("memory_limit"),
        data.get("memory_limit_per_test"),
        data.get("memoryLimitPerTest"),
        problem.get("memoryLimit"),
        problem.get("memory_limit"),
        problem.get("memoryLimitPerTest"),
        extract_limit_from_text(statement, "memory"),
        default_memory_limit,
    )

    if re.fullmatch(r"\d+", time_limit):
        time_limit = f"{time_limit} seconds"
    if re.fullmatch(r"\d+", memory_limit):
        memory_limit = f"{memory_limit} megabytes"

    return time_limit, memory_limit


def load_problem_json(problem_file: str, default_time_limit: str, default_memory_limit: str) -> Dict[str, Any]:
    path = Path(problem_file).expanduser().resolve()
    with path.open("r", encoding="utf-8") as f:
        data = json.load(f)

    statement = join_sections(find_statement_field(data))
    editorial = join_sections(find_editorial_field(data))
    existing_hints = clean_existing_hints(data.get("hints", []))
    time_limit, memory_limit = extract_limits(data, statement, default_time_limit, default_memory_limit)

    if not statement:
        keys = sorted(list(data.keys()))
        raise ValueError(
            f"No statement found in {path}\n"
            f"Top-level keys: {keys}"
        )

    return {
        "path": str(path),
        "statement": statement,
        "editorial": editorial,
        "existing_hints": existing_hints,
        "time_limit": time_limit,
        "memory_limit": memory_limit,
    }


# ============================================================
# Auto-search
# ============================================================

def quick_score_problem_file(path: Path) -> Tuple[int, str]:
    try:
        with path.open("r", encoding="utf-8") as f:
            data = json.load(f)

        statement = join_sections(find_statement_field(data))
        editorial = join_sections(find_editorial_field(data))
        hints = clean_existing_hints(data.get("hints", []))

        score = 0
        reasons = []

        if statement:
            score += 100
            reasons.append("statement")
        else:
            reasons.append("no_statement")

        if editorial:
            score += 80
            reasons.append("editorial")
        else:
            reasons.append("no_editorial")

        if hints:
            score += min(20, 5 * len(hints))
            reasons.append(f"hints={len(hints)}")
        else:
            reasons.append("hints=0")

        normalized = str(path).replace("\\", "/")
        if "/with_hint/" in normalized:
            score += 3
            reasons.append("with_hint")
        elif "/without_hint/" in normalized:
            score += 1
            reasons.append("without_hint")

        return score, ", ".join(reasons)

    except Exception as e:
        return -1, f"bad_json:{e}"


def find_problem_file_by_prefix(prefix: str, search_roots: List[str], verbose: bool = True) -> Path:
    prefix = normalize_text(prefix).upper()
    if not prefix:
        raise ValueError("Empty prefix is not allowed.")

    candidates: List[Tuple[int, str, Path]] = []

    for root in search_roots:
        root_path = Path(root).expanduser()
        if not root_path.exists():
            continue

        patterns = [
            str(root_path / f"{prefix}*.json"),
            str(root_path / "**" / f"{prefix}*.json"),
        ]

        for pat in patterns:
            for raw in glob.glob(pat, recursive=True):
                path = Path(raw).resolve()
                if path.is_file():
                    score, reason = quick_score_problem_file(path)
                    candidates.append((score, reason, path))

    dedup: Dict[str, Tuple[int, str, Path]] = {}
    for score, reason, path in candidates:
        key = str(path)
        if key not in dedup or score > dedup[key][0]:
            dedup[key] = (score, reason, path)

    candidates = sorted(dedup.values(), key=lambda x: (-x[0], str(x[2])))

    if not candidates:
        searched = "\n".join(f"  - {Path(r).expanduser()}" for r in search_roots)
        raise FileNotFoundError(f"No JSON file found for prefix {prefix!r}.\nSearched roots:\n{searched}")

    if verbose:
        print(f"\n🔎 Auto-search candidates for {prefix}:")
        for i, (score, reason, path) in enumerate(candidates[:5], start=1):
            mark = "SELECTED" if i == 1 else "candidate"
            print(f"  [{i}] {mark} score={score} ({reason}) {path}")

    best_score, best_reason, best_path = candidates[0]
    if best_score < 100:
        raise ValueError(
            f"Found files for prefix {prefix!r}, but none seems to contain a valid statement.\n"
            f"Best candidate: {best_path}, score={best_score}, reason={best_reason}"
        )

    return best_path


# ============================================================
# Token utilities for local hint model
# ============================================================

def token_ids(tokenizer, text: str) -> List[int]:
    return tokenizer(text or "", add_special_tokens=False)["input_ids"]


def token_len(tokenizer, text: str) -> int:
    return len(token_ids(tokenizer, text))


def truncate_by_tokens(tokenizer, text: str, max_tokens: int, keep_tail: bool = False) -> str:
    ids = token_ids(tokenizer, text)
    if len(ids) <= max_tokens:
        return text
    ids = ids[-max_tokens:] if keep_tail else ids[:max_tokens]
    return tokenizer.decode(ids, skip_special_tokens=True)


def head_tail_truncate(tokenizer, text: str, max_tokens: int) -> str:
    ids = token_ids(tokenizer, text)
    if len(ids) <= max_tokens:
        return text

    marker = "\n\n...[middle part omitted due to length]...\n\n"
    marker_len = token_len(tokenizer, marker)
    usable = max(1, max_tokens - marker_len)
    head_n = usable // 2
    tail_n = usable - head_n

    head = tokenizer.decode(ids[:head_n], skip_special_tokens=True)
    tail = tokenizer.decode(ids[-tail_n:], skip_special_tokens=True)
    return head + marker + tail


def chat_text(tokenizer, messages: List[Dict[str, str]], add_generation_prompt: bool) -> str:
    return tokenizer.apply_chat_template(messages, tokenize=False, add_generation_prompt=add_generation_prompt)


def chat_token_len(tokenizer, messages: List[Dict[str, str]], add_generation_prompt: bool = True) -> int:
    return token_len(tokenizer, chat_text(tokenizer, messages, add_generation_prompt))


def build_hint_context(
    tokenizer,
    statement: str,
    editorial: str,
    statement_budget: int,
    editorial_budget: int,
    include_editorial: bool,
) -> str:
    """Build the context shown to the hint model.

    If include_editorial=False, the official editorial/solution is deliberately
    removed from hint generation. This is the main experimental setting of this
    script.
    """
    statement_short = truncate_by_tokens(tokenizer, statement, statement_budget, keep_tail=False)

    if not include_editorial:
        return (
            "=== PROBLEM STATEMENT ===\n"
            f"{statement_short}\n\n"
            "=== OFFICIAL EDITORIAL / SOLUTION ===\n"
            "[not provided in this experiment]\n"
        )

    editorial_short = head_tail_truncate(tokenizer, editorial, editorial_budget)
    return (
        "=== PROBLEM STATEMENT ===\n"
        f"{statement_short}\n\n"
        "=== FULL EDITORIAL EXCERPT ===\n"
        f"{editorial_short}\n"
    )


def format_previous_hints(previous_hints: List[str]) -> str:
    if not previous_hints:
        return "No previous hints have been given."
    return "\n\n".join(f"Hint {i}:\n{h}" for i, h in enumerate(previous_hints, start=1))


def build_hint_user_content(
    tokenizer,
    statement: str,
    editorial: str,
    previous_hints: List[str],
    turn_idx: int,
    statement_budget: int,
    editorial_budget: int,
    include_editorial: bool,
) -> str:
    question = "I am stuck. Can you give me Hint 1?" if turn_idx == 1 else "Can you give me the next hint?"

    no_solution_note = (
        "\n\nImportant: The official editorial/solution is intentionally not provided. "
        "Generate the next hint only from the problem statement and previous hints."
        if not include_editorial
        else ""
    )

    return (
        build_hint_context(
            tokenizer=tokenizer,
            statement=statement,
            editorial=editorial,
            statement_budget=statement_budget,
            editorial_budget=editorial_budget,
            include_editorial=include_editorial,
        )
        + "\n\n=== PREVIOUS HINTS ALREADY GIVEN ===\n"
        + format_previous_hints(previous_hints)
        + "\n\n=== CURRENT REQUEST ===\n"
        + question
        + no_solution_note
        + "\n\nIndependently decide whether to give one new hint or stop. Return JSON only."
    )


def build_hint_messages_under_budget(
    tokenizer,
    statement: str,
    editorial: str,
    previous_hints: List[str],
    turn_idx: int,
    max_seq_length: int,
    statement_budget: int,
    editorial_budget: int,
    include_editorial: bool,
) -> List[Dict[str, str]]:
    if include_editorial:
        profiles = [
            (statement_budget, editorial_budget),
            (int(statement_budget * 0.85), int(editorial_budget * 0.85)),
            (int(statement_budget * 0.70), int(editorial_budget * 0.70)),
            (int(statement_budget * 0.55), int(editorial_budget * 0.55)),
            (256, 768),
            (128, 384),
        ]
        system_prompt = SYSTEM_PROMPT
    else:
        # Without editorial, give the statement a larger relative budget.
        profiles = [
            (max(statement_budget, 1800), 0),
            (max(statement_budget, 1400), 0),
            (max(statement_budget, 1000), 0),
            (768, 0),
            (512, 0),
            (256, 0),
        ]
        system_prompt = SYSTEM_PROMPT_NO_SOLUTION

    for s_budget, e_budget in profiles:
        user_content = build_hint_user_content(
            tokenizer=tokenizer,
            statement=statement,
            editorial=editorial,
            previous_hints=previous_hints,
            turn_idx=turn_idx,
            statement_budget=max(64, s_budget),
            editorial_budget=max(0, e_budget),
            include_editorial=include_editorial,
        )

        messages = [
            {"role": "system", "content": system_prompt},
            {"role": "user", "content": user_content},
        ]

        if chat_token_len(tokenizer, messages, add_generation_prompt=True) <= max_seq_length:
            return messages

    user_content = build_hint_user_content(
        tokenizer=tokenizer,
        statement=statement,
        editorial=editorial,
        previous_hints=previous_hints[-8:],
        turn_idx=turn_idx,
        statement_budget=256,
        editorial_budget=0 if not include_editorial else 384,
        include_editorial=include_editorial,
    )

    return [
        {"role": "system", "content": system_prompt},
        {"role": "user", "content": user_content},
    ]


# ============================================================
# Hint generation parsing / ranking
# ============================================================

def extract_json_object(text: str) -> Optional[Dict[str, Any]]:
    text = (text or "").strip()
    text = re.sub(r"^```(?:json)?", "", text, flags=re.IGNORECASE).strip()
    text = re.sub(r"```$", "", text).strip()

    candidates = [text]
    m = re.search(r"\{.*\}", text, flags=re.DOTALL)
    if m:
        candidates.append(m.group(0))

    for cand in candidates:
        try:
            obj = json.loads(cand)
            if isinstance(obj, dict):
                return obj
        except Exception:
            continue

    return None


def should_stop(response: str) -> bool:
    lower = (response or "").lower().strip()
    return any(marker in lower for marker in STOP_MARKERS)


def strip_hint_title(text: str) -> str:
    text = re.sub(r"^\s*\*\*?\s*hint\s*\d*\s*\*\*?\s*[:.\-]?\s*", "", text or "", flags=re.IGNORECASE)
    text = re.sub(r"^\s*hint\s*\d+\s*[:.\-]?\s*", "", text, flags=re.IGNORECASE)
    return re.sub(r"\s+", " ", text).strip()


def sanitize_visible_hint(hint_text: str, turn_idx: int) -> str:
    text = (hint_text or "").strip()
    if should_stop(text):
        return FINAL_STOP_REPLY

    body = strip_hint_title(text)
    body = re.split(r"(?i)\n\s*(?:\*\*)?\s*hint\s*\d+\s*(?:\*\*)?\s*[:.\-]?\s*", body, maxsplit=1)[0].strip()

    if not body:
        body = "Think about the next useful observation."

    return f"**Hint {turn_idx}**\n{body}"


def normalize_decision(obj: Optional[Dict[str, Any]], raw_text: str, turn_idx: int) -> Dict[str, Any]:
    if not obj:
        if should_stop(raw_text):
            return {"action": "stop", "covered_so_far": [], "next_focus": "", "hint": FINAL_STOP_REPLY}
        return {
            "action": "hint",
            "covered_so_far": [],
            "next_focus": "unparsed model output",
            "hint": sanitize_visible_hint(raw_text, turn_idx),
        }

    action = normalize_text(obj.get("action", "hint")).lower()
    if action not in {"hint", "stop"}:
        action = "stop" if should_stop(str(obj.get("hint", ""))) else "hint"

    covered = obj.get("covered_so_far", [])
    if not isinstance(covered, list):
        covered = []

    next_focus = normalize_text(obj.get("next_focus", ""))
    hint = normalize_text(obj.get("hint", ""))

    if action == "stop":
        return {"action": "stop", "covered_so_far": covered, "next_focus": "", "hint": FINAL_STOP_REPLY}

    return {
        "action": "hint",
        "covered_so_far": covered,
        "next_focus": next_focus,
        "hint": sanitize_visible_hint(hint, turn_idx),
    }


def normalize_for_similarity(text: str) -> str:
    text = (text or "").lower()
    text = re.sub(r"\*\*hint\s*\d*\*\*", " ", text)
    text = re.sub(r"\bhint\s*\d+\s*[:.\-]?", " ", text)
    text = re.sub(r"\$\$\$|\$|\\leq|\\geq|\\lt|\\gt", " ", text)
    text = re.sub(r"\d+", " <num> ", text)
    text = re.sub(r"[^a-z0-9_<>=]+", " ", text)
    return re.sub(r"\s+", " ", text).strip()


def content_words(text: str) -> set:
    stop_words = {
        "the", "a", "an", "to", "of", "and", "or", "in", "on", "for", "with",
        "is", "are", "be", "can", "could", "you", "try", "consider", "think",
        "about", "when", "what", "how", "why", "this", "that", "it", "as",
        "by", "from", "into", "if", "then", "there", "such", "which",
        "calculate", "find", "number", "solve", "use",
    }
    return {w for w in normalize_for_similarity(text).split() if len(w) >= 2 and w not in stop_words}


def jaccard_similarity(a: str, b: str) -> float:
    wa = content_words(a)
    wb = content_words(b)
    if not wa or not wb:
        return 0.0
    return len(wa & wb) / len(wa | wb)


def most_similar_previous(new_hint: str, previous_hints: List[str]) -> Tuple[float, Optional[int], bool]:
    best_score = 0.0
    best_idx = None
    new_norm = normalize_for_similarity(new_hint)

    for i, old in enumerate(previous_hints, start=1):
        old_norm = normalize_for_similarity(old)
        if new_norm and old_norm and new_norm == old_norm:
            return 1.0, i, True

        score = jaccard_similarity(new_hint, old)
        if score > best_score:
            best_score = score
            best_idx = i

    return best_score, best_idx, False


def evaluate_decision(decision: Dict[str, Any], valid_json: bool, previous_hints: List[str], args) -> Dict[str, Any]:
    action = decision.get("action", "hint")
    hint = decision.get("hint", "")

    score = 0.0
    sim = 0.0
    sim_idx = None
    exact_duplicate = False

    if not valid_json:
        score += 5.0

    if action == "stop":
        score += args.stop_candidate_score
    else:
        if not hint:
            score += 5.0

        sim, sim_idx, exact_duplicate = most_similar_previous(hint, previous_hints)

        if exact_duplicate:
            score += 100.0
        elif sim > args.similarity_threshold:
            score += args.semantic_repeat_penalty * sim

        if "unparsed" in str(decision.get("next_focus", "")).lower():
            score += 0.35

    return {
        "decision": decision,
        "valid_json": valid_json,
        "score": score,
        "similarity": sim,
        "similar_to_hint_index": sim_idx,
        "exact_duplicate": exact_duplicate,
    }


def choose_best(records: List[Dict[str, Any]], args) -> Dict[str, Any]:
    if not records:
        return {
            "decision": {"action": "stop", "covered_so_far": [], "next_focus": "", "hint": FINAL_STOP_REPLY},
            "score": 999.0,
            "exact_duplicate": False,
        }

    pool = [r for r in records if not r.get("exact_duplicate", False)]
    if not pool:
        return {
            "decision": {"action": "stop", "covered_so_far": [], "next_focus": "", "hint": FINAL_STOP_REPLY},
            "score": 999.0,
            "exact_duplicate": False,
        }

    pool.sort(key=lambda r: (r["score"], 0 if r["decision"].get("action") == "hint" else 1))
    selected = pool[0]

    if args.print_candidates:
        print("\n=================== Hint candidate ranking ===================")
        for i, r in enumerate(sorted(records, key=lambda x: (x["score"], 0 if x["decision"].get("action") == "hint" else 1)), start=1):
            mark = "FILTER_EXACT" if r.get("exact_duplicate") else "OK"
            d = r["decision"]
            print(
                f"[{i}] {mark} score={r['score']:.3f} action={d.get('action')} "
                f"sim={r.get('similarity', 0):.2f} exact={r.get('exact_duplicate')}"
            )
            print(str(d.get("hint", ""))[:300].replace("\n", " "))
        print(f"SELECTED: action={selected['decision'].get('action')} score={selected['score']:.3f}")
        print("==============================================================\n")

    return selected


# ============================================================
# Local hint model
# ============================================================

def load_hint_model(base_model_path: str, lora_path: str):
    import torch
    from peft import PeftModel
    from transformers import AutoModelForCausalLM, AutoTokenizer

    tokenizer = AutoTokenizer.from_pretrained(base_model_path, trust_remote_code=True)
    if tokenizer.pad_token is None:
        tokenizer.pad_token = tokenizer.eos_token

    base_model = AutoModelForCausalLM.from_pretrained(
        base_model_path,
        torch_dtype=torch.bfloat16,
        device_map="auto",
        trust_remote_code=True,
        attn_implementation="sdpa",
    )
    base_model.config.use_cache = True

    model = PeftModel.from_pretrained(base_model, lora_path).merge_and_unload()
    model.eval()

    return model, tokenizer


def generate_hint_candidates(model, tokenizer, messages: List[Dict[str, str]], args) -> List[str]:
    import torch

    text = chat_text(tokenizer, messages, add_generation_prompt=True)
    device = next(model.parameters()).device
    inputs = tokenizer([text], return_tensors="pt").to(device)
    prompt_len = inputs.input_ids.shape[1]

    total = max(1, args.num_candidates)
    batch_size = max(1, min(args.candidate_batch_size, total))
    remaining = total
    decoded: List[str] = []

    while remaining > 0:
        n = min(batch_size, remaining)

        try:
            with torch.no_grad():
                outputs = model.generate(
                    **inputs,
                    max_new_tokens=args.max_new_tokens,
                    temperature=args.temperature,
                    top_p=args.top_p,
                    do_sample=True,
                    num_return_sequences=n,
                    repetition_penalty=args.repetition_penalty,
                    pad_token_id=tokenizer.eos_token_id,
                    eos_token_id=tokenizer.eos_token_id,
                )
        except torch.cuda.OutOfMemoryError:
            if torch.cuda.is_available():
                torch.cuda.empty_cache()
            if n == 1:
                raise
            batch_size = 1
            continue

        decoded.extend(tokenizer.decode(out[prompt_len:], skip_special_tokens=True).strip() for out in outputs)
        del outputs
        remaining -= n

        if torch.cuda.is_available():
            torch.cuda.empty_cache()

    return decoded


def auto_generate_hints(statement: str, editorial: str, args, cached_hint_model=None) -> List[str]:
    if cached_hint_model is None:
        print("\n🔄 Loading local hint model...")
        print(f"📦 Base model: {args.base_model_path}")
        print(f"🎯 LoRA: {args.lora_path}")
        model, tokenizer = load_hint_model(args.base_model_path, args.lora_path)
    else:
        model, tokenizer = cached_hint_model

    include_editorial = (args.hint_context == "with_solution")
    if include_editorial:
        print("\n👨‍🏫 Hint generator starts WITH official editorial/solution.\n")
    else:
        print("\n👨‍🏫 Hint generator starts WITHOUT official editorial/solution.\n")

    hints: List[str] = []

    for turn_idx in range(1, args.max_turns + 1):
        messages = build_hint_messages_under_budget(
            tokenizer=tokenizer,
            statement=statement,
            editorial=editorial,
            previous_hints=hints,
            turn_idx=turn_idx,
            max_seq_length=args.max_seq_length,
            statement_budget=args.statement_budget,
            editorial_budget=args.editorial_budget,
            include_editorial=include_editorial,
        )

        raw_candidates = generate_hint_candidates(model, tokenizer, messages, args)
        records = []
        for raw in raw_candidates:
            obj = extract_json_object(raw)
            decision = normalize_decision(obj, raw, turn_idx)
            records.append(evaluate_decision(decision, obj is not None, hints, args))

        selected = choose_best(records, args)
        decision = selected["decision"]
        hint_text = decision["hint"]

        if decision["action"] == "stop" or should_stop(hint_text):
            print(f"🏁 Hint model stops at turn {turn_idx}.")
            break

        hint_text = sanitize_visible_hint(hint_text, turn_idx)
        print(f"\n=================== Generated Hint {turn_idx} ===================")
        print(hint_text)
        print("===============================================================\n")
        hints.append(hint_text)

    else:
        print(f"⚠️ Reached max_turns={args.max_turns}. Stop.")

    print(f"✅ Total generated hints: {len(hints)}")
    return hints


# ============================================================
# Solver prompt / API generation
# ============================================================

def build_hint_solver_prompt(statement: str, hints: List[str], time_limit: str, memory_limit: str, args) -> str:
    hints_text = "\n\n".join(hints) if hints else "No hints are provided."

    if hints:
        hint_instruction = (
            "The hints are important. Carefully think through every provided hint and use them to derive the algorithm before writing code. "
            "Do not ignore the hints."
        )
    else:
        hint_instruction = "No hints are provided. Solve the problem directly from the statement."

    if args.solver_prompt_style == "direct":
        return f"""You are solving a Codeforces problem.

=== PROBLEM STATEMENT ===
{statement}

=== LIMITS ===
Time limit per test: {time_limit}
Memory limit per test: {memory_limit}

=== HINTS ===
{hints_text}

{hint_instruction}

Write a complete C++17 solution.

Requirements:
- Return only one C++17 code block.
- Do not explain.
- The solution must read from standard input and write to standard output.
- Use standard C++17.
- The algorithm must fit within the given time and memory limits.
"""

    return f"""You are an expert competitive programmer solving a Codeforces problem.

=== PROBLEM STATEMENT ===
{statement}

=== LIMITS ===
Time limit per test: {time_limit}
Memory limit per test: {memory_limit}

=== HINTS ===
{hints_text}

{hint_instruction}

Write a complete C++17 solution.

Requirements:
- Return only one C++17 code block.
- Do not explain.
- Read from standard input and write to standard output.
- Use standard C++17.
- Choose an algorithm that fits the time and memory limits.
- Be careful with edge cases, overflow, and multiple test cases.
"""


def build_solution_solver_prompt(statement: str, editorial: str, time_limit: str, memory_limit: str, args) -> str:
    return f"""You are solving a Codeforces problem.

=== PROBLEM STATEMENT ===
{statement}

=== LIMITS ===
Time limit per test: {time_limit}
Memory limit per test: {memory_limit}

=== OFFICIAL EDITORIAL / SOLUTION ===
{editorial}

Carefully understand the official editorial/solution and convert it into a complete accepted C++17 solution.

Write a complete C++17 solution.

Requirements:
- Return only one C++17 code block.
- Do not explain.
- The solution must read from standard input and write to standard output.
- Use standard C++17.
- The algorithm must fit within the given time and memory limits.
"""


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


def build_conditions(hints: List[str], editorial: str, args) -> List[Dict[str, Any]]:
    conditions: List[Dict[str, Any]] = []

    if args.only_extremes:
        conditions.append({"label": "Hint0", "kind": "hint", "hints": []})
        conditions.append({"label": "HintAll", "kind": "hint", "hints": hints})
    else:
        for k in range(len(hints) + 1):
            conditions.append({"label": f"Hint{k}", "kind": "hint", "hints": hints[:k]})

    if args.include_solution_condition:
        if editorial:
            conditions.append({"label": "Solution", "kind": "solution", "hints": []})
        else:
            print("⚠️ --include_solution_condition is set, but this problem has no editorial. Skip Solution condition.")

    return conditions


def write_or_generate_conditions(
    *,
    problem_out_dir: Path,
    prefix: str,
    statement: str,
    editorial: str,
    hints: List[str],
    time_limit: str,
    memory_limit: str,
    args,
    client,
) -> List[Dict[str, Any]]:
    """Write prompts and optionally call the solver API.

    When --parallel_solver_workers > 1, only solver API calls are parallelized.
    Prompt writing remains deterministic and sequential. Each API task writes a
    unique .raw.txt / .cpp / .failed.txt file, so there is no filename conflict.
    """
    problem_out_dir.mkdir(parents=True, exist_ok=True)
    manifest: List[Dict[str, Any]] = []

    conditions = build_conditions(hints, editorial, args)
    api_tasks: List[Dict[str, Any]] = []

    # First write all prompt files sequentially and build API tasks.
    for cond in conditions:
        label = cond["label"]

        if cond["kind"] == "solution":
            prompt = build_solution_solver_prompt(statement, editorial, time_limit, memory_limit, args)
        else:
            prompt = build_hint_solver_prompt(statement, cond["hints"], time_limit, memory_limit, args)

        prompt_path = problem_out_dir / f"{prefix}_{label}.prompt.txt"

        if args.overwrite or not prompt_path.exists():
            prompt_path.write_text(prompt, encoding="utf-8")
            print(f"✅ Wrote prompt: {prompt_path.name}")
        else:
            print(f"⏭ Skip existing prompt: {prompt_path.name}")

        record_base = {
            "prefix": prefix,
            "label": label,
            "kind": cond["kind"],
            "num_hints_visible": len(cond["hints"]),
            "prompt_path": str(prompt_path),
            "time_limit": time_limit,
            "memory_limit": memory_limit,
        }

        if args.mode == "prompts":
            manifest.append({**record_base, "status": "prompt_only"})
            continue

        for sample_idx in range(args.samples_per_condition):
            suffix = label if args.samples_per_condition == 1 else f"{label}_s{sample_idx}"
            cpp_path = problem_out_dir / f"{prefix}_{suffix}.cpp"
            raw_path = problem_out_dir / f"{prefix}_{suffix}.raw.txt"
            fail_path = problem_out_dir / f"{prefix}_{suffix}.failed.txt"

            if cpp_path.exists() and raw_path.exists() and not args.overwrite:
                print(f"⏭ Skip existing code: {cpp_path.name}")
                manifest.append({
                    **record_base,
                    "sample_idx": sample_idx,
                    "cpp_path": str(cpp_path),
                    "raw_path": str(raw_path),
                    "status": "skipped_existing",
                })
                continue

            api_tasks.append({
                "record_base": record_base,
                "sample_idx": sample_idx,
                "label": label,
                "prompt": prompt,
                "cpp_path": cpp_path,
                "raw_path": raw_path,
                "fail_path": fail_path,
            })

    if args.mode == "prompts" or not api_tasks:
        return manifest

    def run_one_solver_task(task: Dict[str, Any]) -> Dict[str, Any]:
        sample_idx = task["sample_idx"]
        label = task["label"]
        prompt = task["prompt"]
        cpp_path = task["cpp_path"]
        raw_path = task["raw_path"]
        fail_path = task["fail_path"]
        record_base = task["record_base"]

        try:
            # Use a per-thread client to avoid any possible shared-client issue.
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

            return {
                **record_base,
                "sample_idx": sample_idx,
                "cpp_path": str(cpp_path),
                "raw_path": str(raw_path),
                "status": "ok",
                "solver_model": args.solver_model,
                "thinking": args.thinking,
                "usage": usage_obj,
            }

        except Exception as e:
            fail_path.write_text(repr(e), encoding="utf-8")
            return {
                **record_base,
                "sample_idx": sample_idx,
                "failed_path": str(fail_path),
                "status": "failed",
                "solver_model": args.solver_model,
                "thinking": args.thinking,
                "error": repr(e),
            }

    workers = max(1, int(args.parallel_solver_workers))
    workers = min(workers, len(api_tasks))

    if workers <= 1:
        for task in api_tasks:
            print(f"\n🚀 Calling solver for {prefix} {task['label']}, sample {task['sample_idx']}...")
            record = run_one_solver_task(task)
            if record["status"] == "ok":
                print(f"✅ Wrote code: {Path(record['cpp_path']).name}")
                if record.get("usage"):
                    print(f"📊 usage: {record['usage']}")
            else:
                print(f"❌ Solver failed: {Path(record['failed_path']).name}")
            manifest.append(record)
            if args.sleep_between_calls > 0:
                time.sleep(args.sleep_between_calls)
    else:
        print(f"\n🚀 Running {len(api_tasks)} solver API calls with {workers} parallel workers...")
        with ThreadPoolExecutor(max_workers=workers) as executor:
            future_to_task = {executor.submit(run_one_solver_task, task): task for task in api_tasks}
            for future in as_completed(future_to_task):
                task = future_to_task[future]
                try:
                    record = future.result()
                except Exception as e:
                    # This should rarely happen because run_one_solver_task catches errors.
                    fail_path = task["fail_path"]
                    fail_path.write_text(repr(e), encoding="utf-8")
                    record = {
                        **task["record_base"],
                        "sample_idx": task["sample_idx"],
                        "failed_path": str(fail_path),
                        "status": "failed",
                        "solver_model": args.solver_model,
                        "thinking": args.thinking,
                        "error": repr(e),
                    }

                if record["status"] == "ok":
                    print(f"✅ Done {prefix} {task['label']} sample {task['sample_idx']}: {Path(record['cpp_path']).name}")
                    if record.get("usage"):
                        print(f"📊 usage: {record['usage']}")
                else:
                    print(f"❌ Failed {prefix} {task['label']} sample {task['sample_idx']}: {Path(record['failed_path']).name}")

                manifest.append(record)

    return manifest


# ============================================================
# Main
# ============================================================

def parse_args():
    parser = argparse.ArgumentParser()

    parser.add_argument("--prefix", type=str, default=None, help="Single problem prefix, e.g. CF1777C.")
    parser.add_argument("--prefix_file", nargs="*", default=None, help="Text file(s) containing Codeforces problem IDs.")
    parser.add_argument(
        "--search_roots",
        nargs="*",
        default=[
            "cf_scraper/cf_dataset/with_hint",
            "cf_scraper/cf_dataset/without_hint",
        ],
    )

    parser.add_argument("--output_dir", type=str, default="solver_outputs_cf1700")
    parser.add_argument("--overwrite", action="store_true")
    parser.add_argument("--limit", type=int, default=None)
    parser.add_argument("--start_index", type=int, default=0)
    parser.add_argument("--shuffle", action="store_true")
    parser.add_argument("--seed", type=int, default=42)

    parser.add_argument("--hint_source", choices=["auto", "existing", "none"], default="auto")
    parser.add_argument(
        "--hint_context",
        choices=["no_solution", "with_solution"],
        default="no_solution",
        help=(
            "Context used by the local hint generator. "
            "'no_solution' means generated hints are based only on statement + previous hints. "
            "'with_solution' restores the original behavior and gives the official editorial to the hint model."
        ),
    )
    parser.add_argument("--base_model_path", type=str, default=DEFAULT_BASE_MODEL_PATH)
    parser.add_argument("--lora_path", type=str, default=DEFAULT_LORA_PATH)
    parser.add_argument("--max_turns", type=int, default=12)
    parser.add_argument("--max_seq_length", type=int, default=4096)
    parser.add_argument("--max_new_tokens", type=int, default=512)
    parser.add_argument("--statement_budget", type=int, default=900)
    parser.add_argument("--editorial_budget", type=int, default=2200)
    parser.add_argument("--temperature", type=float, default=0.7)
    parser.add_argument("--top_p", type=float, default=0.9)
    parser.add_argument("--repetition_penalty", type=float, default=1.05)
    parser.add_argument("--similarity_threshold", type=float, default=0.45)
    parser.add_argument("--semantic_repeat_penalty", type=float, default=0.8)
    parser.add_argument("--num_candidates", type=int, default=10)
    parser.add_argument("--candidate_batch_size", type=int, default=2)
    parser.add_argument("--stop_candidate_score", type=float, default=0.85)
    parser.add_argument("--print_candidates", action="store_true")

    parser.add_argument("--default_time_limit", type=str, default="1 second")
    parser.add_argument("--default_memory_limit", type=str, default="256 megabytes")

    parser.add_argument("--mode", choices=["prompts", "code", "both"], default="both")
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
    parser.add_argument("--samples_per_condition", type=int, default=1)
    parser.add_argument(
        "--parallel_solver_workers",
        type=int,
        default=1,
        help="Number of parallel DeepSeek/API code-generation calls. Use 1 for sequential.",
    )
    parser.add_argument("--solver_prompt_style", choices=["direct", "careful"], default="direct")

    parser.add_argument("--only_extremes", action="store_true", help="Only test Hint0 and HintAll.")
    parser.add_argument("--include_solution_condition", action="store_true", help="Also test statement + official solution/editorial.")

    return parser.parse_args()


def main():
    args = parse_args()
    prefixes = read_prefixes(args)

    output_dir = Path(args.output_dir).expanduser().resolve()
    output_dir.mkdir(parents=True, exist_ok=True)

    print(f"📌 Total prefixes to process: {len(prefixes)}")
    print(f"📦 Output dir: {output_dir}")
    print(f"🤖 Solver model: {args.solver_model}")
    print(f"🧠 Thinking parameter: {args.thinking}")
    print(f"🧪 Mode: {args.mode}")
    print(f"🧩 Include Solution condition: {args.include_solution_condition}")
    print(f"🧪 Hint generation context: {args.hint_context}")

    client = None
    if args.mode in {"code", "both"}:
        client = get_openai_client(args)

    cached_hint_model = None
    all_records: List[Dict[str, Any]] = []
    skipped: List[Dict[str, Any]] = []

    for idx, prefix in enumerate(prefixes, start=1):
        print(f"\n\n==================== [{idx}/{len(prefixes)}] {prefix} ====================")

        try:
            problem_file = find_problem_file_by_prefix(prefix, args.search_roots, verbose=True)
            loaded = load_problem_json(
                str(problem_file),
                args.default_time_limit,
                args.default_memory_limit,
            )

            real_prefix = infer_prefix(problem_file, prefix)
            problem_out_dir = output_dir / real_prefix
            problem_out_dir.mkdir(parents=True, exist_ok=True)

            print(f"📄 Problem file: {loaded['path']}")
            print(f"⏱ Time limit: {loaded['time_limit']}")
            print(f"💾 Memory limit: {loaded['memory_limit']}")
            print(f"📚 Existing JSON hints: {len(loaded['existing_hints'])}")
            print(f"📖 Editorial available: {bool(loaded['editorial'])}")

            if args.hint_source == "none":
                hints: List[str] = []
            elif args.hint_source == "existing":
                hints = loaded["existing_hints"]
            else:
                if args.hint_context == "with_solution" and not loaded["editorial"]:
                    print(
                        f"⚠️ --hint_context with_solution requires editorial, but no editorial was found. "
                        f"Falling back to no_solution hint generation."
                    )
                    args.hint_context = "no_solution"

                if cached_hint_model is None:
                    cached_hint_model = load_hint_model(args.base_model_path, args.lora_path)

                hints = auto_generate_hints(
                    loaded["statement"],
                    loaded["editorial"],
                    args,
                    cached_hint_model=cached_hint_model,
                )

            print(f"✅ Final hint count: {len(hints)}")

            hints_path = problem_out_dir / f"{real_prefix}_generated_hints.json"
            hints_path.write_text(json.dumps(hints, ensure_ascii=False, indent=2), encoding="utf-8")

            records = write_or_generate_conditions(
                problem_out_dir=problem_out_dir,
                prefix=real_prefix,
                statement=loaded["statement"],
                editorial=loaded["editorial"],
                hints=hints,
                time_limit=loaded["time_limit"],
                memory_limit=loaded["memory_limit"],
                args=args,
                client=client,
            )

            problem_manifest = problem_out_dir / f"{real_prefix}_manifest.json"
            problem_manifest.write_text(json.dumps(records, ensure_ascii=False, indent=2), encoding="utf-8")

            all_records.extend(records)

        except Exception as e:
            print(f"❌ Skip {prefix}: {repr(e)}")
            skipped.append({"prefix": prefix, "error": repr(e)})

    summary = {
        "num_prefixes_requested": len(prefixes),
        "num_records": len(all_records),
        "num_skipped_prefixes": len(skipped),
        "hint_source": args.hint_source,
        "hint_context": args.hint_context,
        "skipped": skipped,
    }

    (output_dir / "all_records.json").write_text(json.dumps(all_records, ensure_ascii=False, indent=2), encoding="utf-8")
    (output_dir / "summary.json").write_text(json.dumps(summary, ensure_ascii=False, indent=2), encoding="utf-8")

    print("\n\n🎉 Batch done.")
    print(f"📄 all_records: {output_dir / 'all_records.json'}")
    print(f"📄 summary: {output_dir / 'summary.json'}")
    print(f"❌ skipped prefixes: {len(skipped)}")


if __name__ == "__main__":
    main()

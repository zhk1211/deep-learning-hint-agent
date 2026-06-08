#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
python3 hint_demo_app.py   --host 0.0.0.0   --port 7860   --base_model_path ./qwen-coder-7b   --candidate_batch_size 1   --num_candidates 10   --thinking disabled

Batch evaluate a solver model on Codeforces problems with:
  1. Hint0: statement only
  2. Hint1..HintK: statement + first k hints
  3. Solution: statement + official editorial/solution, no hints

The solver never sees editorial in Hint0..HintK conditions.
The Solution condition is an upper-bound test where the solver sees the official editorial.

Typical usage:

  export DEEPSEEK_API_KEY="sk-..."

  python3 deepseek_cf1700_eval.py \
    --prefix_file cf_difficulty_1700_prefixes.txt \
    --output_dir solver_outputs_cf1700 \
    --solver_model deepseek-v4-pro \
    --mode both \
    --samples_per_condition 1 \
    --include_solution_condition \
    --overwrite

For a single problem:

  python3 deepseek_cf1700_eval.py \
    --prefix CF1777C \
    --output_dir solver_outputs_test \
    --solver_model deepseek-v4-pro \
    --mode both \
    --include_solution_condition \
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


def build_hint_context(tokenizer, statement: str, editorial: str, statement_budget: int, editorial_budget: int) -> str:
    statement_short = truncate_by_tokens(tokenizer, statement, statement_budget, keep_tail=False)
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
) -> str:
    question = "I am stuck. Can you give me Hint 1?" if turn_idx == 1 else "Can you give me the next hint?"

    return (
        build_hint_context(tokenizer, statement, editorial, statement_budget, editorial_budget)
        + "\n\n=== PREVIOUS HINTS ALREADY GIVEN ===\n"
        + format_previous_hints(previous_hints)
        + "\n\n=== CURRENT REQUEST ===\n"
        + question
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
) -> List[Dict[str, str]]:
    profiles = [
        (statement_budget, editorial_budget),
        (int(statement_budget * 0.85), int(editorial_budget * 0.85)),
        (int(statement_budget * 0.70), int(editorial_budget * 0.70)),
        (int(statement_budget * 0.55), int(editorial_budget * 0.55)),
        (256, 768),
        (128, 384),
    ]

    for s_budget, e_budget in profiles:
        user_content = build_hint_user_content(
            tokenizer=tokenizer,
            statement=statement,
            editorial=editorial,
            previous_hints=previous_hints,
            turn_idx=turn_idx,
            statement_budget=max(64, s_budget),
            editorial_budget=max(128, e_budget),
        )

        messages = [
            {"role": "system", "content": SYSTEM_PROMPT},
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
        statement_budget=128,
        editorial_budget=384,
    )

    return [
        {"role": "system", "content": SYSTEM_PROMPT},
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

    hints: List[str] = []
    print("\n👨‍🏫 Hint generator starts...\n")

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

# ============================================================
# Local web demo
# ============================================================

import threading
import uuid
from types import SimpleNamespace

from fastapi import FastAPI, HTTPException
from fastapi.responses import HTMLResponse
from pydantic import BaseModel


DEMO_TITLE = "Deep Learning Hint Agent Demo"

MODE_CONFIG = {
    "balanced": {
        "label": "Balanced",
        "description": "checkpoint-175: balanced hint quantity and detail",
        "checkpoint": str(BASE_DIR / "cf_hint_lora_model_budgeted" / "checkpoint-175"),
    },
    "enthusiastic": {
        "label": "Enthusiastic",
        "description": "checkpoint-125: more hints and more detailed guidance",
        "checkpoint": str(BASE_DIR / "cf_hint_lora_model_budgeted" / "checkpoint-125"),
    },
    "silent": {
        "label": "Silent",
        "description": "checkpoint-225: fewer hints and earlier stopping",
        "checkpoint": str(BASE_DIR / "cf_hint_lora_model_budgeted" / "checkpoint-225"),
    },
}


SESSIONS: Dict[str, Dict[str, Any]] = {}
SESSION_LOCK = threading.Lock()
MODEL_LOCK = threading.Lock()
CURRENT_HINT_MODEL: Dict[str, Any] = {
    "mode": None,
    "lora_path": None,
    "model": None,
    "tokenizer": None,
}


def make_default_args() -> SimpleNamespace:
    return SimpleNamespace(
        search_roots=[
            str(BASE_DIR / "cf_scraper" / "cf_dataset" / "with_hint"),
            str(BASE_DIR / "cf_scraper" / "cf_dataset" / "without_hint"),
        ],
        base_model_path=str(BASE_DIR / "qwen-coder-7b"),
        lora_path=str(BASE_DIR / "cf_hint_lora_model_budgeted" / "checkpoint-175"),
        max_turns=12,
        max_seq_length=4096,
        max_new_tokens=512,
        statement_budget=900,
        editorial_budget=2200,
        temperature=0.7,
        top_p=0.9,
        repetition_penalty=1.05,
        similarity_threshold=0.45,
        semantic_repeat_penalty=0.8,
        num_candidates=10,
        candidate_batch_size=2,
        stop_candidate_score=0.85,
        print_candidates=False,
        default_time_limit="1 second",
        default_memory_limit="256 megabytes",
        solver_base_url=DEFAULT_DEEPSEEK_BASE_URL,
        solver_api_key_env="DEEPSEEK_API_KEY",
        solver_model="deepseek-v4-pro",
        thinking="disabled",
        solver_temperature=0.2,
        solver_top_p=0.95,
        solver_max_tokens=8192,
        solver_timeout_seconds=240.0,
        solver_retries=2,
        retry_sleep_seconds=3.0,
        sleep_between_calls=0.5,
        solver_prompt_style="direct",
    )


DEMO_ARGS = make_default_args()


def update_demo_args_from_cli(args) -> None:
    DEMO_ARGS.search_roots = args.search_roots
    DEMO_ARGS.base_model_path = args.base_model_path
    DEMO_ARGS.max_turns = args.max_turns
    DEMO_ARGS.max_seq_length = args.max_seq_length
    DEMO_ARGS.max_new_tokens = args.max_new_tokens
    DEMO_ARGS.statement_budget = args.statement_budget
    DEMO_ARGS.editorial_budget = args.editorial_budget
    DEMO_ARGS.temperature = args.temperature
    DEMO_ARGS.top_p = args.top_p
    DEMO_ARGS.repetition_penalty = args.repetition_penalty
    DEMO_ARGS.similarity_threshold = args.similarity_threshold
    DEMO_ARGS.semantic_repeat_penalty = args.semantic_repeat_penalty
    DEMO_ARGS.num_candidates = args.num_candidates
    DEMO_ARGS.candidate_batch_size = args.candidate_batch_size
    DEMO_ARGS.stop_candidate_score = args.stop_candidate_score
    DEMO_ARGS.default_time_limit = args.default_time_limit
    DEMO_ARGS.default_memory_limit = args.default_memory_limit
    DEMO_ARGS.solver_base_url = args.solver_base_url
    DEMO_ARGS.solver_api_key_env = args.solver_api_key_env
    DEMO_ARGS.solver_model = args.solver_model
    DEMO_ARGS.thinking = args.thinking
    DEMO_ARGS.solver_temperature = args.solver_temperature
    DEMO_ARGS.solver_top_p = args.solver_top_p
    DEMO_ARGS.solver_max_tokens = args.solver_max_tokens
    DEMO_ARGS.solver_timeout_seconds = args.solver_timeout_seconds
    DEMO_ARGS.solver_retries = args.solver_retries
    DEMO_ARGS.retry_sleep_seconds = args.retry_sleep_seconds
    DEMO_ARGS.solver_prompt_style = args.solver_prompt_style


def get_lora_path_for_mode(mode: str) -> str:
    if mode not in MODE_CONFIG:
        raise HTTPException(status_code=400, detail=f"Unknown mode: {mode}")
    return MODE_CONFIG[mode]["checkpoint"]


def get_or_load_hint_model(mode: str):
    lora_path = get_lora_path_for_mode(mode)

    with MODEL_LOCK:
        if CURRENT_HINT_MODEL["model"] is not None and CURRENT_HINT_MODEL["lora_path"] == lora_path:
            return CURRENT_HINT_MODEL["model"], CURRENT_HINT_MODEL["tokenizer"]

        # Release previous model to avoid holding multiple 7B models on GPU.
        if CURRENT_HINT_MODEL["model"] is not None:
            try:
                import torch
                del CURRENT_HINT_MODEL["model"]
                del CURRENT_HINT_MODEL["tokenizer"]
                if torch.cuda.is_available():
                    torch.cuda.empty_cache()
            except Exception:
                pass

        print(f"\n🔄 Loading hint model mode={mode}")
        print(f"📦 Base model: {DEMO_ARGS.base_model_path}")
        print(f"🎯 LoRA: {lora_path}")

        model, tokenizer = load_hint_model(DEMO_ARGS.base_model_path, lora_path)

        CURRENT_HINT_MODEL["mode"] = mode
        CURRENT_HINT_MODEL["lora_path"] = lora_path
        CURRENT_HINT_MODEL["model"] = model
        CURRENT_HINT_MODEL["tokenizer"] = tokenizer
        return model, tokenizer


def create_session(problem: Dict[str, Any], prefix: str = "") -> str:
    sid = str(uuid.uuid4())
    with SESSION_LOCK:
        SESSIONS[sid] = {
            "session_id": sid,
            "prefix": prefix,
            "path": problem.get("path", ""),
            "statement": problem["statement"],
            "editorial": problem.get("editorial", ""),
            "time_limit": problem.get("time_limit", DEMO_ARGS.default_time_limit),
            "memory_limit": problem.get("memory_limit", DEMO_ARGS.default_memory_limit),
            "hints": [],
            "stopped": False,
        }
    return sid


def get_session_or_404(session_id: str) -> Dict[str, Any]:
    with SESSION_LOCK:
        sess = SESSIONS.get(session_id)
    if not sess:
        raise HTTPException(status_code=404, detail="Session not found. Load or create a problem first.")
    return sess


def next_hint_once(sess: Dict[str, Any], mode: str) -> Dict[str, Any]:
    if sess.get("stopped"):
        return {
            "action": "stop",
            "hint": FINAL_STOP_REPLY,
            "hints": sess["hints"],
            "stopped": True,
        }

    model, tokenizer = get_or_load_hint_model(mode)

    previous_hints = list(sess["hints"])
    turn_idx = len(previous_hints) + 1

    if turn_idx > DEMO_ARGS.max_turns:
        sess["stopped"] = True
        return {
            "action": "stop",
            "hint": f"Reached max_turns={DEMO_ARGS.max_turns}.",
            "hints": sess["hints"],
            "stopped": True,
        }

    # If no editorial is provided, the model still runs, but the input explicitly says so.
    editorial = sess.get("editorial", "").strip()
    if not editorial:
        editorial = "No official editorial or solution is provided for this demo input."

    messages = build_hint_messages_under_budget(
        tokenizer=tokenizer,
        statement=sess["statement"],
        editorial=editorial,
        previous_hints=previous_hints,
        turn_idx=turn_idx,
        max_seq_length=DEMO_ARGS.max_seq_length,
        statement_budget=DEMO_ARGS.statement_budget,
        editorial_budget=DEMO_ARGS.editorial_budget,
    )

    raw_candidates = generate_hint_candidates(model, tokenizer, messages, DEMO_ARGS)
    records = []
    for raw in raw_candidates:
        obj = extract_json_object(raw)
        decision = normalize_decision(obj, raw, turn_idx)
        records.append(evaluate_decision(decision, obj is not None, previous_hints, DEMO_ARGS))

    selected = choose_best(records, DEMO_ARGS)
    decision = selected["decision"]
    hint_text = decision.get("hint", "")

    if decision.get("action") == "stop" or should_stop(hint_text):
        sess["stopped"] = True
        return {
            "action": "stop",
            "hint": FINAL_STOP_REPLY,
            "hints": sess["hints"],
            "stopped": True,
            "ranking": selected,
        }

    hint_text = sanitize_visible_hint(hint_text, turn_idx)
    sess["hints"].append(hint_text)

    return {
        "action": "hint",
        "hint": hint_text,
        "hints": sess["hints"],
        "stopped": False,
        "ranking": {
            "score": selected.get("score"),
            "similarity": selected.get("similarity"),
            "exact_duplicate": selected.get("exact_duplicate"),
        },
    }


def generate_code_from_session(sess: Dict[str, Any], source: str) -> Dict[str, Any]:
    if source == "hints":
        prompt = build_hint_solver_prompt(
            statement=sess["statement"],
            hints=sess["hints"],
            time_limit=sess["time_limit"],
            memory_limit=sess["memory_limit"],
            args=DEMO_ARGS,
        )
    elif source == "solution":
        editorial = sess.get("editorial", "").strip()
        if not editorial:
            raise HTTPException(status_code=400, detail="No solution/editorial is available for this session.")
        prompt = build_solution_solver_prompt(
            statement=sess["statement"],
            editorial=editorial,
            time_limit=sess["time_limit"],
            memory_limit=sess["memory_limit"],
            args=DEMO_ARGS,
        )
    else:
        raise HTTPException(status_code=400, detail="source must be 'hints' or 'solution'.")

    client = get_openai_client(DEMO_ARGS)
    raw, usage = call_solver_api(client, prompt, DEMO_ARGS)
    code = extract_cpp_code(raw)

    usage_obj = None
    if usage is not None:
        try:
            usage_obj = usage.model_dump()
        except Exception:
            usage_obj = str(usage)

    return {
        "source": source,
        "prompt": prompt,
        "raw": raw,
        "code": code,
        "usage": usage_obj,
        "num_hints": len(sess["hints"]),
    }


class LoadByPrefixRequest(BaseModel):
    prefix: str


class ManualProblemRequest(BaseModel):
    prefix: str = "MANUAL"
    statement: str
    editorial: str = ""
    time_limit: str = "1 second"
    memory_limit: str = "256 megabytes"


class NextHintRequest(BaseModel):
    session_id: str
    mode: str = "balanced"


class GenerateCodeRequest(BaseModel):
    session_id: str
    source: str = "hints"


class ResetHintsRequest(BaseModel):
    session_id: str


app = FastAPI(title=DEMO_TITLE)


@app.get("/", response_class=HTMLResponse)
def index():
    return HTMLResponse(DEMO_HTML)


@app.get("/api/config")
def api_config():
    return {
        "modes": MODE_CONFIG,
        "defaults": {
            "base_model_path": DEMO_ARGS.base_model_path,
            "search_roots": DEMO_ARGS.search_roots,
            "solver_model": DEMO_ARGS.solver_model,
            "thinking": DEMO_ARGS.thinking,
            "num_candidates": DEMO_ARGS.num_candidates,
            "candidate_batch_size": DEMO_ARGS.candidate_batch_size,
            "max_turns": DEMO_ARGS.max_turns,
        },
    }


@app.post("/api/load_by_prefix")
def api_load_by_prefix(req: LoadByPrefixRequest):
    try:
        path = find_problem_file_by_prefix(req.prefix, DEMO_ARGS.search_roots, verbose=False)
        problem = load_problem_json(
            str(path),
            default_time_limit=DEMO_ARGS.default_time_limit,
            default_memory_limit=DEMO_ARGS.default_memory_limit,
        )
        sid = create_session(problem, prefix=req.prefix.upper())
        return {
            "session_id": sid,
            "prefix": req.prefix.upper(),
            "path": problem.get("path", str(path)),
            "statement": problem["statement"],
            "editorial": problem.get("editorial", ""),
            "editorial_available": bool(problem.get("editorial", "").strip()),
            "time_limit": problem["time_limit"],
            "memory_limit": problem["memory_limit"],
            "hints": [],
        }
    except Exception as e:
        raise HTTPException(status_code=400, detail=repr(e))


@app.post("/api/load_manual")
def api_load_manual(req: ManualProblemRequest):
    if not req.statement.strip():
        raise HTTPException(status_code=400, detail="Problem statement is required.")
    problem = {
        "path": "",
        "statement": req.statement.strip(),
        "editorial": req.editorial.strip(),
        "time_limit": req.time_limit.strip() or DEMO_ARGS.default_time_limit,
        "memory_limit": req.memory_limit.strip() or DEMO_ARGS.default_memory_limit,
    }
    sid = create_session(problem, prefix=req.prefix.strip() or "MANUAL")
    return {
        "session_id": sid,
        "prefix": problem.get("prefix", req.prefix),
        "path": "",
        "statement": problem["statement"],
        "editorial": problem["editorial"],
        "editorial_available": bool(problem["editorial"].strip()),
        "time_limit": problem["time_limit"],
        "memory_limit": problem["memory_limit"],
        "hints": [],
    }


@app.post("/api/next_hint")
def api_next_hint(req: NextHintRequest):
    sess = get_session_or_404(req.session_id)
    try:
        result = next_hint_once(sess, req.mode)
        return result
    except Exception as e:
        raise HTTPException(status_code=500, detail=repr(e))


@app.post("/api/generate_code")
def api_generate_code(req: GenerateCodeRequest):
    sess = get_session_or_404(req.session_id)
    try:
        return generate_code_from_session(sess, req.source)
    except HTTPException:
        raise
    except Exception as e:
        raise HTTPException(status_code=500, detail=repr(e))


@app.post("/api/reset_hints")
def api_reset_hints(req: ResetHintsRequest):
    sess = get_session_or_404(req.session_id)
    sess["hints"] = []
    sess["stopped"] = False
    return {"ok": True, "hints": []}


DEMO_HTML = r"""
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8"/>
  <meta name="viewport" content="width=device-width, initial-scale=1"/>
  <title>Deep Learning Hint Agent Demo</title>
  <style>
    :root {
      --bg: #0f172a;
      --panel: #111827;
      --panel2: #1f2937;
      --text: #e5e7eb;
      --muted: #9ca3af;
      --accent: #60a5fa;
      --accent2: #34d399;
      --danger: #fb7185;
      --border: #374151;
      --code: #020617;
    }
    * { box-sizing: border-box; }
    body {
      margin: 0;
      font-family: ui-sans-serif, system-ui, -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif;
      background: linear-gradient(135deg, #020617, #0f172a 50%, #111827);
      color: var(--text);
    }
    header {
      padding: 24px 32px 12px;
      border-bottom: 1px solid var(--border);
      background: rgba(15, 23, 42, 0.85);
      position: sticky;
      top: 0;
      backdrop-filter: blur(12px);
      z-index: 5;
    }
    h1 { margin: 0; font-size: 28px; }
    .subtitle { margin-top: 6px; color: var(--muted); }
    main {
      display: grid;
      grid-template-columns: 420px 1fr;
      gap: 18px;
      padding: 18px;
    }
    section {
      background: rgba(17, 24, 39, 0.92);
      border: 1px solid var(--border);
      border-radius: 16px;
      padding: 16px;
      box-shadow: 0 8px 24px rgba(0,0,0,0.25);
    }
    h2 { margin: 0 0 12px; font-size: 18px; }
    h3 { margin: 16px 0 8px; font-size: 15px; color: #bfdbfe; }
    label { display: block; margin: 10px 0 5px; color: var(--muted); font-size: 13px; }
    input, textarea, select {
      width: 100%;
      padding: 10px 11px;
      border-radius: 10px;
      border: 1px solid var(--border);
      background: #020617;
      color: var(--text);
      outline: none;
    }
    textarea { min-height: 140px; resize: vertical; font-family: ui-monospace, SFMono-Regular, Menlo, monospace; }
    button {
      border: 0;
      border-radius: 10px;
      padding: 10px 13px;
      color: white;
      background: #2563eb;
      cursor: pointer;
      font-weight: 650;
      margin-top: 10px;
    }
    button:hover { filter: brightness(1.08); }
    button.secondary { background: #374151; }
    button.good { background: #059669; }
    button.warn { background: #be123c; }
    button:disabled { opacity: 0.55; cursor: not-allowed; }
    .row { display: grid; grid-template-columns: 1fr 1fr; gap: 10px; }
    .buttons { display: flex; gap: 10px; flex-wrap: wrap; }
    .status {
      margin-top: 10px;
      color: var(--muted);
      white-space: pre-wrap;
      font-family: ui-monospace, SFMono-Regular, Menlo, monospace;
      font-size: 12px;
    }
    .hint {
      background: #0b1220;
      border: 1px solid var(--border);
      border-left: 4px solid var(--accent2);
      border-radius: 12px;
      padding: 12px;
      margin: 10px 0;
      white-space: pre-wrap;
      line-height: 1.5;
    }
    .code {
      background: var(--code);
      border: 1px solid var(--border);
      border-radius: 12px;
      padding: 14px;
      min-height: 360px;
      white-space: pre;
      overflow: auto;
      font-family: ui-monospace, SFMono-Regular, Menlo, Consolas, monospace;
      font-size: 13px;
    }
    .meta {
      color: var(--muted);
      font-size: 13px;
      line-height: 1.6;
      background: rgba(31, 41, 55, 0.6);
      border-radius: 12px;
      padding: 10px;
      margin-bottom: 12px;
      white-space: pre-wrap;
    }
    .tabs {
      display: flex;
      gap: 8px;
      margin-bottom: 10px;
    }
    .tab {
      padding: 8px 10px;
      border-radius: 999px;
      background: #1f2937;
      color: var(--muted);
      cursor: pointer;
      border: 1px solid var(--border);
      font-size: 13px;
    }
    .tab.active { color: white; background: #2563eb; }
    .hidden { display: none; }
    @media (max-width: 1000px) {
      main { grid-template-columns: 1fr; }
    }
  </style>
</head>
<body>
<header>
  <h1>Deep Learning Hint Agent Demo</h1>
  <div class="subtitle">Progressive Codeforces-style hint generation + DeepSeek V4-Pro code generation</div>
</header>

<main>
  <section>
    <h2>1. Choose Problem</h2>

    <div class="tabs">
      <div class="tab active" id="tab-prefix" onclick="switchTab('prefix')">Search by CF ID</div>
      <div class="tab" id="tab-manual" onclick="switchTab('manual')">Manual input</div>
    </div>

    <div id="prefix-panel">
      <label>Codeforces problem ID</label>
      <input id="prefix" placeholder="e.g. CF1777C"/>
      <button onclick="loadByPrefix()">Load Problem</button>
    </div>

    <div id="manual-panel" class="hidden">
      <label>Problem label</label>
      <input id="manual-prefix" value="MANUAL"/>
      <label>Problem statement *</label>
      <textarea id="manual-statement" placeholder="Paste problem statement here"></textarea>
      <label>Editorial / solution (optional)</label>
      <textarea id="manual-editorial" placeholder="Optional. If empty, the hint model runs without official editorial."></textarea>
      <div class="row">
        <div>
          <label>Time limit</label>
          <input id="manual-time" value="1 second"/>
        </div>
        <div>
          <label>Memory limit</label>
          <input id="manual-memory" value="256 megabytes"/>
        </div>
      </div>
      <button onclick="loadManual()">Use Manual Problem</button>
    </div>

    <h2 style="margin-top: 20px;">2. Choose Hint Model</h2>
    <label>Mode</label>
    <select id="mode">
      <option value="balanced">Balanced: checkpoint-175</option>
      <option value="enthusiastic">Enthusiastic: checkpoint-125</option>
      <option value="silent">Silent: checkpoint-225</option>
    </select>

    <div class="buttons">
      <button class="good" onclick="nextHint()">Generate Next Hint</button>
      <button class="secondary" onclick="resetHints()">Reset Hints</button>
    </div>

    <h2 style="margin-top: 20px;">3. Generate Code</h2>
    <div class="buttons">
      <button onclick="generateCode('hints')">DeepSeek from current hints</button>
      <button onclick="generateCode('solution')">DeepSeek from solution</button>
    </div>

    <div class="status" id="status">No problem loaded.</div>
  </section>

  <section>
    <h2>Current Problem</h2>
    <div class="meta" id="problem-meta">No problem loaded.</div>

    <h2>Generated Hints</h2>
    <div id="hints"></div>

    <h2>Generated Code</h2>
    <pre class="code" id="code">// Generated C++17 code will appear here.</pre>
  </section>
</main>

<script>
let sessionId = null;
let currentProblem = null;

function setStatus(msg) {
  document.getElementById("status").textContent = msg;
}

function switchTab(name) {
  document.getElementById("prefix-panel").classList.toggle("hidden", name !== "prefix");
  document.getElementById("manual-panel").classList.toggle("hidden", name !== "manual");
  document.getElementById("tab-prefix").classList.toggle("active", name === "prefix");
  document.getElementById("tab-manual").classList.toggle("active", name === "manual");
}

async function postJSON(url, body) {
  const resp = await fetch(url, {
    method: "POST",
    headers: {"Content-Type": "application/json"},
    body: JSON.stringify(body)
  });
  const text = await resp.text();
  let data;
  try { data = JSON.parse(text); } catch { data = {detail: text}; }
  if (!resp.ok) {
    throw new Error(data.detail || text || "Request failed");
  }
  return data;
}

function renderProblem(data) {
  currentProblem = data;
  sessionId = data.session_id;
  document.getElementById("problem-meta").textContent =
    `Session: ${sessionId}\n` +
    `Prefix: ${data.prefix || ""}\n` +
    `Path: ${data.path || "(manual)"}\n` +
    `Editorial available: ${data.editorial_available}\n` +
    `Time limit: ${data.time_limit}\n` +
    `Memory limit: ${data.memory_limit}\n\n` +
    `Statement preview:\n${(data.statement || "").slice(0, 900)}${(data.statement || "").length > 900 ? "..." : ""}`;
  renderHints([]);
  document.getElementById("code").textContent = "// Generated C++17 code will appear here.";
}

function renderHints(hints) {
  const box = document.getElementById("hints");
  if (!hints || hints.length === 0) {
    box.innerHTML = '<div class="meta">No hints generated yet.</div>';
    return;
  }
  box.innerHTML = "";
  hints.forEach((h, i) => {
    const div = document.createElement("div");
    div.className = "hint";
    div.textContent = h;
    box.appendChild(div);
  });
}

async function loadByPrefix() {
  const prefix = document.getElementById("prefix").value.trim();
  if (!prefix) return alert("Please input a CF problem ID.");
  setStatus("Loading problem...");
  try {
    const data = await postJSON("/api/load_by_prefix", {prefix});
    renderProblem(data);
    setStatus("Loaded problem.");
  } catch (e) {
    setStatus("Error: " + e.message);
  }
}

async function loadManual() {
  const statement = document.getElementById("manual-statement").value.trim();
  if (!statement) return alert("Problem statement is required.");
  const body = {
    prefix: document.getElementById("manual-prefix").value.trim() || "MANUAL",
    statement,
    editorial: document.getElementById("manual-editorial").value,
    time_limit: document.getElementById("manual-time").value || "1 second",
    memory_limit: document.getElementById("manual-memory").value || "256 megabytes"
  };
  setStatus("Creating manual session...");
  try {
    const data = await postJSON("/api/load_manual", body);
    renderProblem(data);
    setStatus("Manual problem loaded.");
  } catch (e) {
    setStatus("Error: " + e.message);
  }
}

async function nextHint() {
  if (!sessionId) return alert("Load a problem first.");
  const mode = document.getElementById("mode").value;
  setStatus(`Generating next hint with mode=${mode}. First call may take time because the model is loading...`);
  try {
    const data = await postJSON("/api/next_hint", {session_id: sessionId, mode});
    renderHints(data.hints);
    if (data.action === "stop") {
      setStatus("Hint model decided to stop.\n" + data.hint);
    } else {
      setStatus(`Generated Hint ${data.hints.length}.`);
    }
  } catch (e) {
    setStatus("Error: " + e.message);
  }
}

async function resetHints() {
  if (!sessionId) return alert("Load a problem first.");
  try {
    const data = await postJSON("/api/reset_hints", {session_id: sessionId});
    renderHints(data.hints);
    setStatus("Hints reset.");
  } catch (e) {
    setStatus("Error: " + e.message);
  }
}

async function generateCode(source) {
  if (!sessionId) return alert("Load a problem first.");
  setStatus(`Calling DeepSeek V4-Pro from ${source}...`);
  document.getElementById("code").textContent = "// Generating...";
  try {
    const data = await postJSON("/api/generate_code", {session_id: sessionId, source});
    document.getElementById("code").textContent = data.code || "// Empty code generated.";
    setStatus(`Code generated from ${source}. Hints used: ${data.num_hints}\nusage: ${JSON.stringify(data.usage)}`);
  } catch (e) {
    document.getElementById("code").textContent = "// Failed.";
    setStatus("Error: " + e.message);
  }
}

fetch("/api/config")
  .then(r => r.json())
  .then(c => {
    setStatus("Ready.\n" + JSON.stringify(c.defaults, null, 2));
  })
  .catch(() => {});
</script>
</body>
</html>
"""


def parse_demo_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--host", type=str, default="127.0.0.1")
    parser.add_argument("--port", type=int, default=7860)
    parser.add_argument(
        "--search_roots",
        nargs="*",
        default=[
            str(BASE_DIR / "cf_scraper" / "cf_dataset" / "with_hint"),
            str(BASE_DIR / "cf_scraper" / "cf_dataset" / "without_hint"),
        ],
    )

    parser.add_argument("--base_model_path", type=str, default=DEFAULT_BASE_MODEL_PATH)
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
    parser.add_argument("--default_time_limit", type=str, default="1 second")
    parser.add_argument("--default_memory_limit", type=str, default="256 megabytes")

    parser.add_argument("--solver_base_url", type=str, default=DEFAULT_DEEPSEEK_BASE_URL)
    parser.add_argument("--solver_api_key_env", type=str, default="DEEPSEEK_API_KEY")
    parser.add_argument("--solver_model", type=str, default="deepseek-v4-pro")
    parser.add_argument("--thinking", choices=["omit", "disabled", "enabled"], default="disabled")
    parser.add_argument("--solver_temperature", type=float, default=0.2)
    parser.add_argument("--solver_top_p", type=float, default=0.95)
    parser.add_argument("--solver_max_tokens", type=int, default=8192)
    parser.add_argument("--solver_timeout_seconds", type=float, default=240.0)
    parser.add_argument("--solver_retries", type=int, default=2)
    parser.add_argument("--retry_sleep_seconds", type=float, default=3.0)
    parser.add_argument("--solver_prompt_style", choices=["direct", "careful"], default="direct")

    return parser.parse_args()


if __name__ == "__main__":
    import uvicorn

    cli_args = parse_demo_args()
    update_demo_args_from_cli(cli_args)

    print(f"🚀 Starting demo at http://{cli_args.host}:{cli_args.port}")
    print("Mode checkpoints:")
    for k, v in MODE_CONFIG.items():
        print(f"  {k}: {v['checkpoint']}")
    print(f"DeepSeek key env: {DEMO_ARGS.solver_api_key_env}")
    print(f"DeepSeek model: {DEMO_ARGS.solver_model}, thinking={DEMO_ARGS.thinking}")

    uvicorn.run(app, host=cli_args.host, port=cli_args.port)

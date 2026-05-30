#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Auto-generate Codeforces solver prompts from only a problem prefix.

Usage:
  python3 auto_generate_hints_and_solver_prompts.py \
    --prefix CF2178D \
    --output_dir solver_prompts \
    --base_model_path ./qwen-coder-7b \
    --lora_path ./cf_hint_lora_model_budgeted/checkpoint-175 \
    --num_candidates 10 \
    --candidate_batch_size 2 \
    --overwrite

What it does:
1. If --problem_file is not given, search by --prefix in:
   - cf_scraper/cf_dataset/with_hint
   - cf_scraper/cf_dataset/without_hint
2. Prefer files with non-empty statement and non-empty editorial/solution.
3. If editorial/solution exists, use the local hint model to generate hints until it stops.
4. If editorial/solution does not exist, fallback to existing JSON hints.
5. If neither editorial nor hints exist, write only Hint0 prompt.
6. Write solver prompts:
   - CFxxxx_Hint0.prompt.txt
   - CFxxxx_Hint1.prompt.txt
   - ...
The solver prompts contain statement + limits + first k hints, and never contain the official solution/editorial.
"""

import argparse
import glob
import json
import os
import re
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple


BASE_DIR = Path(__file__).resolve().parent
DEFAULT_BASE_MODEL_PATH = str(BASE_DIR / "qwen-coder-7b")
DEFAULT_LORA_PATH = str(BASE_DIR / "cf_hint_lora_model_budgeted" / "checkpoint-175")

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
# Basic text / JSON utilities
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


def join_sections(obj: Any) -> str:
    """Robustly join fields such as statement / solutions from common scraper schemas."""
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

        # Fallback for alternate statement schema.
        parts = []
        for key in ("name", "legend", "input", "output", "note"):
            value = normalize_text(obj.get(key, ""))
            if value:
                parts.append(f"{key}\n{value}")
        return "\n\n".join(parts).strip()

    if isinstance(obj, list):
        parts: List[str] = []
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
    for key in (
        "solutions",
        "solution",
        "editorial",
        "tutorial",
        "tutorials",
    ):
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


def load_problem_json(problem_file: str, default_time_limit: str, default_memory_limit: str) -> Tuple[str, str, List[str], str, str, str]:
    path = Path(problem_file).expanduser().resolve()
    if not path.exists():
        raise FileNotFoundError(f"Problem file not found: {path}")

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
            f"Top-level keys: {keys}\n"
            "The file exists, but its statement field is empty or uses an unsupported schema."
        )

    print(f"📄 Loaded problem file: {path}")
    return statement, editorial, existing_hints, time_limit, memory_limit, str(path)


# ============================================================
# Auto-search utilities
# ============================================================

def quick_score_problem_file(path: Path) -> Tuple[int, str]:
    try:
        with path.open("r", encoding="utf-8") as f:
            data = json.load(f)

        statement = join_sections(find_statement_field(data))
        editorial = join_sections(find_editorial_field(data))
        existing_hints = clean_existing_hints(data.get("hints", []))

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

        if existing_hints:
            score += min(20, len(existing_hints) * 5)
            reasons.append(f"hints={len(existing_hints)}")
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


def find_problem_file_by_prefix(prefix: str, search_roots: List[str]) -> Path:
    prefix = normalize_text(prefix)
    if not prefix:
        raise ValueError("Empty --prefix is not allowed.")

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
                if not path.is_file():
                    continue
                score, reason = quick_score_problem_file(path)
                candidates.append((score, reason, path))

    # Deduplicate by path.
    dedup: Dict[str, Tuple[int, str, Path]] = {}
    for score, reason, path in candidates:
        key = str(path)
        if key not in dedup or score > dedup[key][0]:
            dedup[key] = (score, reason, path)

    candidates = sorted(dedup.values(), key=lambda x: (-x[0], str(x[2])))

    if not candidates:
        searched = "\n".join(f"  - {Path(r).expanduser()}" for r in search_roots)
        raise FileNotFoundError(
            f"No JSON file found for prefix {prefix!r}.\n"
            f"Searched roots:\n{searched}"
        )

    print("\n🔎 Auto-search candidates:")
    for i, (score, reason, path) in enumerate(candidates[:10], start=1):
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
# Token utilities for hint generation
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
    return tokenizer.apply_chat_template(
        messages,
        tokenize=False,
        add_generation_prompt=add_generation_prompt,
    )


def chat_token_len(tokenizer, messages: List[Dict[str, str]], add_generation_prompt: bool = True) -> int:
    return token_len(tokenizer, chat_text(tokenizer, messages, add_generation_prompt))


def build_context(tokenizer, statement: str, editorial: str, statement_budget: int, editorial_budget: int) -> str:
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


def build_user_content(
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
        build_context(tokenizer, statement, editorial, statement_budget, editorial_budget)
        + "\n\n=== PREVIOUS HINTS ALREADY GIVEN ===\n"
        + format_previous_hints(previous_hints)
        + "\n\n=== CURRENT REQUEST ===\n"
        + question
        + "\n\nIndependently decide whether to give one new hint or stop. Return JSON only."
    )


def build_messages_under_budget(
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
        user_content = build_user_content(
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

    user_content = build_user_content(
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
# Hint decision parsing and candidate ranking
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

    # If the model leaks several "Hint k" blocks, keep only the first one.
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

    # Never output exact duplicates.
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
        print("\n=================== Candidate ranking ===================")
        for i, r in enumerate(sorted(records, key=lambda x: (x["score"], 0 if x["decision"].get("action") == "hint" else 1)), start=1):
            mark = "FILTER_EXACT" if r.get("exact_duplicate") else "OK"
            d = r["decision"]
            print(
                f"[{i}] {mark} score={r['score']:.3f} action={d.get('action')} "
                f"sim={r.get('similarity', 0):.2f} exact={r.get('exact_duplicate')}"
            )
            print(str(d.get("hint", ""))[:300].replace("\n", " "))
        print(f"SELECTED: action={selected['decision'].get('action')} score={selected['score']:.3f}")
        print("=========================================================\n")

    return selected


# ============================================================
# Hint model loading / generation
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


def generate_raw_candidates(model, tokenizer, messages: List[Dict[str, str]], args) -> List[str]:
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


def auto_generate_hints(statement: str, editorial: str, args) -> List[str]:
    print("\n🔄 Loading hint model...")
    print(f"📦 Base model: {args.base_model_path}")
    print(f"🎯 LoRA: {args.lora_path}")

    model, tokenizer = load_hint_model(args.base_model_path, args.lora_path)

    previous_hints: List[str] = []
    print("\n👨‍🏫 Hint generator starts...\n")

    for turn_idx in range(1, args.max_turns + 1):
        messages = build_messages_under_budget(
            tokenizer=tokenizer,
            statement=statement,
            editorial=editorial,
            previous_hints=previous_hints,
            turn_idx=turn_idx,
            max_seq_length=args.max_seq_length,
            statement_budget=args.statement_budget,
            editorial_budget=args.editorial_budget,
        )

        raw_candidates = generate_raw_candidates(model, tokenizer, messages, args)
        records = []
        for raw in raw_candidates:
            obj = extract_json_object(raw)
            decision = normalize_decision(obj, raw, turn_idx)
            records.append(evaluate_decision(decision, obj is not None, previous_hints, args))

        selected = choose_best(records, args)
        decision = selected["decision"]
        hint_text = decision["hint"]

        if decision["action"] == "stop" or should_stop(hint_text):
            print(f"🏁 Model stops at turn {turn_idx}.")
            break

        hint_text = sanitize_visible_hint(hint_text, turn_idx)
        print(f"\n=================== Generated Hint {turn_idx} ===================")
        print(hint_text)
        print("===============================================================\n")
        previous_hints.append(hint_text)

    else:
        print(f"⚠️ Reached max_turns={args.max_turns}. Stop.")

    print(f"✅ Total generated hints: {len(previous_hints)}")
    return previous_hints


# ============================================================
# Solver prompt writing
# ============================================================

def build_solver_prompt(statement: str, hints: List[str], time_limit: str, memory_limit: str) -> str:
    hints_text = "\n\n".join(hints) if hints else "No hints are provided."

    return f"""You are solving a Codeforces problem.

=== PROBLEM STATEMENT ===
{statement}

=== LIMITS ===
Time limit per test: {time_limit}
Memory limit per test: {memory_limit}

=== HINTS ===
{hints_text}

Write a complete C++17 solution.

Requirements:
- Return only one C++17 code block.
- Do not explain.
- The solution must read from standard input and write to standard output.
- Use standard C++17.
- The algorithm must fit within the given time and memory limits.
"""


def write_solver_prompts(
    output_dir: Path,
    prefix: str,
    statement: str,
    hints: List[str],
    time_limit: str,
    memory_limit: str,
    overwrite: bool,
) -> None:
    output_dir.mkdir(parents=True, exist_ok=True)

    manifest = []

    for k in range(len(hints) + 1):
        prompt = build_solver_prompt(statement, hints[:k], time_limit, memory_limit)
        prompt_path = output_dir / f"{prefix}_Hint{k}.prompt.txt"

        if prompt_path.exists() and not overwrite:
            print(f"⏭ Skip existing: {prompt_path.name}")
        else:
            prompt_path.write_text(prompt, encoding="utf-8")
            print(f"✅ Wrote: {prompt_path.name}")

        manifest.append({
            "k": k,
            "num_hints_visible": k,
            "prompt_path": str(prompt_path),
            "time_limit": time_limit,
            "memory_limit": memory_limit,
        })

    hints_path = output_dir / f"{prefix}_generated_hints.json"
    hints_path.write_text(json.dumps(hints, ensure_ascii=False, indent=2), encoding="utf-8")

    manifest_path = output_dir / f"{prefix}_prompt_manifest.json"
    manifest_path.write_text(json.dumps(manifest, ensure_ascii=False, indent=2), encoding="utf-8")

    print(f"\n🎉 Done. Generated hints: {hints_path}")
    print(f"🎉 Done. Prompt manifest: {manifest_path}")


# ============================================================
# Main
# ============================================================

def parse_args():
    parser = argparse.ArgumentParser()

    parser.add_argument("--problem_file", type=str, default=None, help="Optional explicit JSON file path. If omitted, search by --prefix.")
    parser.add_argument("--prefix", type=str, required=True, help="Problem prefix, e.g. CF2178D. Used for output prefix and auto-search.")
    parser.add_argument(
        "--search_roots",
        nargs="*",
        default=[
            "cf_scraper/cf_dataset/with_hint",
            "cf_scraper/cf_dataset/without_hint",
        ],
        help="Directories to search when --problem_file is omitted.",
    )

    parser.add_argument("--output_dir", type=str, default="solver_prompts")
    parser.add_argument("--overwrite", action="store_true")

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

    parser.add_argument("--default_time_limit", type=str, default="1 second")
    parser.add_argument("--default_memory_limit", type=str, default="256 megabytes")

    parser.add_argument("--print_candidates", action="store_true")

    return parser.parse_args()


def main():
    args = parse_args()

    if args.problem_file:
        problem_file = Path(args.problem_file).expanduser().resolve()
    else:
        problem_file = find_problem_file_by_prefix(args.prefix, args.search_roots)

    output_dir = Path(args.output_dir).expanduser().resolve()
    prefix = infer_prefix(problem_file, args.prefix)

    statement, editorial, existing_hints, time_limit, memory_limit, _ = load_problem_json(
        str(problem_file),
        args.default_time_limit,
        args.default_memory_limit,
    )

    print(f"📦 Output dir: {output_dir}")
    print(f"🏷 Prefix: {prefix}")
    print(f"⏱ Time limit: {time_limit}")
    print(f"💾 Memory limit: {memory_limit}")
    print(f"📚 Existing JSON hints: {len(existing_hints)}")
    print(f"📖 Editorial available: {bool(editorial)}")

    if editorial:
        hints = auto_generate_hints(statement, editorial, args)
    else:
        print("\n⚠️ No non-empty editorial / solution found.")
        print(f"⚠️ Falling back to existing JSON hints: {len(existing_hints)} hint(s).")
        hints = existing_hints

    write_solver_prompts(
        output_dir=output_dir,
        prefix=prefix,
        statement=statement,
        hints=hints,
        time_limit=time_limit,
        memory_limit=memory_limit,
        overwrite=args.overwrite,
    )


if __name__ == "__main__":
    main()

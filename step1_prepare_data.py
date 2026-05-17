import argparse
import glob
import json
import os
import random
from pathlib import Path
from typing import Any, Dict, List

from transformers import AutoTokenizer


# =========================
# Default Configuration
# =========================

DEFAULT_INPUT_GLOB = "cf_scraper/cf_dataset/with_hint/*.json"
DEFAULT_OUTPUT_FILE = "multi_turn_hints.jsonl"
DEFAULT_MODEL_PATH = "./qwen-coder-7b"

FINAL_STOP_REPLY = "No. The rest you need to think about yourself."

NEXT_HINT_USER_VARIANTS = [
    "I thought about it, but I'm still stuck. Can you give me the next hint?",
    "I tried to use the previous hint, but I am still stuck. Can you give me the next hint?",
    "Can you give me next hint?",
    "I still cannot solve it. Could I get another hint?",
]

SYSTEM_PROMPT = (
    "You are an elite competitive programming coach on Codeforces. "
    "You have access to the Problem Statement and the Full Editorial. "
    "Your task is to act as an interactive tutor who gives progressive, non-spoiling hints one by one.\n\n"

    "CORE OBJECTIVE:\n"
    "Help the user make progress without directly giving away the full solution. "
    "Each hint should move the user one step closer to the key idea.\n\n"

    "CRITICAL RULES:\n"
    "1. Progressive Guidance: Give only one hint at a time. Earlier hints should be more indirect; later hints may become more concrete.\n"
    "2. Socratic Style: Prefer a guiding question or a useful observation, especially in early hints.\n"
    "3. No Full Solution: Do not provide complete algorithms, proofs, implementation details, or code.\n"
    "4. No Code: Never write C++ or Python code. Mathematical notation is allowed.\n"
    "5. Useful, Not Empty: Do not output only a single keyword such as 'BFS', 'DP', 'bitset', or 'topological sort'. "
    "If you mention a technique, also explain what structure or observation suggests it.\n"
    "6. Hint Length: Each hint should usually contain 1-3 short sentences. "
    "Be concise, but the hint must still be informative enough to help the user think.\n"
    "7. Codeforces Style: Use a compact competitive-programming tone. Avoid long textbook explanations.\n"
    "8. Hint Budget: Decide by yourself how many useful progressive hints this problem needs. "
    "Use fewer hints for easy problems and more hints for hard problems.\n"
    "9. Stop Rule: Once all useful progressive hints have been given, if the user asks for another hint, "
    f"answer exactly: \"{FINAL_STOP_REPLY}\""
)


# =========================
# Text utilities
# =========================

def normalize_text(x: str) -> str:
    return (x or "").strip()


def join_sections(sections: List[Dict[str, Any]]) -> str:
    """Join statement / solution sections robustly."""
    parts = []

    for sec in sections or []:
        title = normalize_text(sec.get("title", ""))
        content = normalize_text(sec.get("content", ""))

        if not content:
            continue

        if title and title.lower() not in {"statement", "solution", "tutorial", "editorial"}:
            parts.append(f"{title}\n{content}")
        else:
            parts.append(content)

    return "\n\n".join(parts).strip()


def clean_hints(raw_hints: List[Dict[str, Any]]) -> List[Dict[str, str]]:
    """Drop empty hints and normalize titles."""
    hints = []

    for i, hint in enumerate(raw_hints or [], start=1):
        title = normalize_text(hint.get("title", "")) or f"Hint {i}"
        content = normalize_text(hint.get("content", ""))

        if not content:
            continue

        if not title.lower().startswith("hint"):
            title = f"Hint {len(hints) + 1}"

        hints.append({"title": title, "content": content})

    return hints


# =========================
# Token-budget utilities
# =========================

def token_ids(tokenizer, text: str) -> List[int]:
    return tokenizer(text, add_special_tokens=False)["input_ids"]


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

    if max_tokens <= 64:
        return tokenizer.decode(ids[:max_tokens], skip_special_tokens=True)

    marker = "\n\n...[middle part omitted due to length]...\n\n"
    marker_len = token_len(tokenizer, marker)

    usable = max(1, max_tokens - marker_len)
    head_n = usable // 2
    tail_n = usable - head_n

    head = tokenizer.decode(ids[:head_n], skip_special_tokens=True)
    tail = tokenizer.decode(ids[-tail_n:], skip_special_tokens=True)

    return head + marker + tail


def build_previous_hints(
    tokenizer,
    hints: List[Dict[str, str]],
    upto: int,
    max_tokens: int,
) -> str:
    """Keep recent previous hints under a token budget."""
    if upto <= 0 or max_tokens <= 0:
        return ""

    parts = []
    for j in range(upto):
        parts.append(f"{hints[j]['title']}: {hints[j]['content']}")

    text = "\n\n".join(parts)
    return truncate_by_tokens(tokenizer, text, max_tokens=max_tokens, keep_tail=True)


def chat_token_len(tokenizer, messages: List[Dict[str, str]]) -> int:
    text = tokenizer.apply_chat_template(
        messages,
        tokenize=False,
        add_generation_prompt=False,
    )
    return token_len(tokenizer, text)


# =========================
# Example construction
# =========================

def build_messages_for_turn(
    tokenizer,
    statement_text: str,
    solution_text: str,
    hints: List[Dict[str, str]],
    turn_idx: int,
    statement_budget: int,
    solution_budget: int,
    previous_hints_budget: int,
) -> List[Dict[str, str]]:
    statement_short = truncate_by_tokens(
        tokenizer,
        statement_text,
        max_tokens=statement_budget,
        keep_tail=False,
    )

    # Long editorials often put observations at the beginning and final algorithm / complexity near the end.
    # Head-tail truncation keeps both sides.
    solution_short = head_tail_truncate(
        tokenizer,
        solution_text,
        max_tokens=solution_budget,
    )

    previous_hints = build_previous_hints(
        tokenizer,
        hints,
        upto=turn_idx,
        max_tokens=previous_hints_budget,
    )

    context = (
        "=== PROBLEM STATEMENT ===\n"
        f"{statement_short}\n\n"
        "=== FULL EDITORIAL EXCERPT ===\n"
        f"{solution_short}\n"
    )

    if previous_hints:
        context += "\n=== PREVIOUS HINTS ALREADY GIVEN ===\n"
        context += previous_hints
        context += "\n"

    if turn_idx == 0:
        user_question = "I am stuck. Can you give me Hint 1?"
    else:
        user_question = random.choice(NEXT_HINT_USER_VARIANTS)

    if turn_idx < len(hints):
        assistant_answer = f"**{hints[turn_idx]['title']}**\n{hints[turn_idx]['content']}"
    else:
        assistant_answer = FINAL_STOP_REPLY

    return [
        {"role": "system", "content": SYSTEM_PROMPT},
        {"role": "user", "content": context + "\n" + user_question},
        {"role": "assistant", "content": assistant_answer},
    ]


def build_budget_profiles(args) -> List[Dict[str, int]]:
    """Progressively shrink context if a turn-level sample is still too long."""
    s0 = args.statement_budget
    e0 = args.editorial_budget
    h0 = args.previous_hints_budget

    raw_profiles = [
        (s0, e0, h0),
        (int(s0 * 0.85), int(e0 * 0.85), int(h0 * 0.85)),
        (int(s0 * 0.70), int(e0 * 0.70), int(h0 * 0.70)),
        (int(s0 * 0.55), int(e0 * 0.55), int(h0 * 0.55)),
        (int(s0 * 0.40), int(e0 * 0.40), int(h0 * 0.40)),
        (256, 512, 128),
        (128, 256, 64),
    ]

    profiles = []
    seen = set()

    for s, e, h in raw_profiles:
        s = max(64, s)
        e = max(128, e)
        h = max(0, h)
        key = (s, e, h)

        if key not in seen:
            profiles.append({
                "statement_budget": s,
                "solution_budget": e,
                "previous_hints_budget": h,
            })
            seen.add(key)

    return profiles


def build_turn_record(
    tokenizer,
    filepath: str,
    statement_text: str,
    solution_text: str,
    hints: List[Dict[str, str]],
    turn_idx: int,
    args,
    budget_profiles: List[Dict[str, int]],
):
    """Try several budgets and return a valid record, or None if still too long."""
    for profile in budget_profiles:
        messages = build_messages_for_turn(
            tokenizer=tokenizer,
            statement_text=statement_text,
            solution_text=solution_text,
            hints=hints,
            turn_idx=turn_idx,
            statement_budget=profile["statement_budget"],
            solution_budget=profile["solution_budget"],
            previous_hints_budget=profile["previous_hints_budget"],
        )

        tl = chat_token_len(tokenizer, messages)

        if tl <= args.max_seq_length:
            return {
                "source_file": os.path.basename(filepath),
                "turn_idx": turn_idx,
                "num_hints": len(hints),
                "is_stop_turn": turn_idx == len(hints),
                "token_len": tl,
                "budget": profile,
                "messages": messages,
            }

    return None


# =========================
# Main
# =========================

def parse_args():
    parser = argparse.ArgumentParser()

    parser.add_argument("--input_glob", type=str, default=DEFAULT_INPUT_GLOB)
    parser.add_argument("--output_file", type=str, default=DEFAULT_OUTPUT_FILE)
    parser.add_argument("--model_path", type=str, default=DEFAULT_MODEL_PATH)

    parser.add_argument("--max_seq_length", type=int, default=4096)

    # Context budgets. These are not strict final lengths; they are sub-budgets before chat template.
    parser.add_argument("--statement_budget", type=int, default=900)
    parser.add_argument("--editorial_budget", type=int, default=2200)
    parser.add_argument("--previous_hints_budget", type=int, default=600)

    parser.add_argument("--seed", type=int, default=42)

    return parser.parse_args()


def main():
    args = parse_args()
    random.seed(args.seed)

    print(f"🔄 Loading tokenizer from {args.model_path}")
    tokenizer = AutoTokenizer.from_pretrained(args.model_path, trust_remote_code=True)

    input_files = sorted(glob.glob(args.input_glob))
    print(f"🔍 Found {len(input_files)} candidate files from {args.input_glob}")

    budget_profiles = build_budget_profiles(args)

    records = []
    skipped = {
        "bad_json": 0,
        "missing_statement": 0,
        "missing_solution": 0,
        "missing_hints": 0,
        "too_long_after_budgeting": 0,
    }

    source_problem_count = 0

    for filepath in input_files:
        try:
            with open(filepath, "r", encoding="utf-8") as f:
                data = json.load(f)
        except Exception as e:
            print(f"⚠️ Bad JSON, skipped: {filepath}, error={e}")
            skipped["bad_json"] += 1
            continue

        statement_text = join_sections(data.get("statement", []))
        solution_text = join_sections(data.get("solutions", []))
        hints = clean_hints(data.get("hints", []))

        if not statement_text:
            skipped["missing_statement"] += 1
            continue
        if not solution_text:
            skipped["missing_solution"] += 1
            continue
        if not hints:
            skipped["missing_hints"] += 1
            continue

        source_problem_count += 1

        # One sample per real hint, plus one final stop sample.
        for turn_idx in range(len(hints) + 1):
            record = build_turn_record(
                tokenizer=tokenizer,
                filepath=filepath,
                statement_text=statement_text,
                solution_text=solution_text,
                hints=hints,
                turn_idx=turn_idx,
                args=args,
                budget_profiles=budget_profiles,
            )

            if record is None:
                skipped["too_long_after_budgeting"] += 1
                continue

            records.append(record)

    output_path = Path(args.output_file)
    with output_path.open("w", encoding="utf-8") as out_f:
        for record in records:
            out_f.write(json.dumps(record, ensure_ascii=False) + "\n")

    print("\n========== Dataset Build Summary ==========")
    print(f"✅ Source problems used: {source_problem_count}")
    print(f"✅ Turn-level samples written: {len(records)}")
    print(f"📄 Output file: {output_path.resolve()}")
    print(f"📊 Skipped: {skipped}")

    if records:
        lengths = [r["token_len"] for r in records]
        num_stop = sum(1 for r in records if r["is_stop_turn"])
        print(f"📈 token_len max={max(lengths)}, avg={sum(lengths) / len(lengths):.2f}, min={min(lengths)}")
        print(f"🛑 Stop-turn samples: {num_stop}")


if __name__ == "__main__":
    main()

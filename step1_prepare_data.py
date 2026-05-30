import argparse
import glob
import json
import os
import random
import re
from pathlib import Path
from typing import Any, Dict, Iterable, List, Optional, Tuple

from transformers import AutoTokenizer


DEFAULT_INPUT_GLOB = "cf_scraper/cf_dataset/with_hint/**/*.json"
DEFAULT_SFT_OUTPUT_FILE = "multi_turn_next_hint_decision_sft.jsonl"
DEFAULT_DPO_OUTPUT_FILE = "multi_turn_next_hint_decision_dpo.jsonl"
DEFAULT_CORRECTION_LOG = "hint_correction_log.jsonl"
DEFAULT_MODEL_PATH = "./qwen-coder-7b"

FINAL_STOP_REPLY = "No. The rest you need to think about yourself."

SYSTEM_PROMPT = (
    "You are an elite Codeforces tutor.\n"
    "For every request, independently decide the next hint based on the problem statement, the full editorial, "
    "and all previous hints already given.\n"
    "Your primary goal is to identify the MOST PRELIMINARY and CRITICAL next observation that bridges the gap between the current hint state and the final solution. "
    "The current hint state is defined only by the hints already given to the user, not by the full editorial. "
    "Before generating a new hint, first infer what ideas have already been covered by previous hints. "
    "Then give the next smallest useful observation that advances beyond those hints without repeating or paraphrasing them.\n\n"
    "You must output JSON only. Do not blindly continue a memorized sequence; re-evaluate the state every time.\n\n"
    "Decision rules:\n"
    "1. Analyze the problem statement and the full editorial, then read the previous hints to precisely track what has already been covered.\n"
    "2. Extract the NEXT MOST FUNDAMENTAL OBSERVATION. Focus ONLY on the immediate next logical stepping stone. Do not jump ahead to the final algorithm.\n"
    "3. The visible hint must contain exactly ONE new idea. Do not combine multiple future steps into one hint.\n"
    "4. A hint may be a Socratic question, but it must include a concrete object, condition, observation, or subproblem. Avoid vague questions such as 'How to solve it?', 'How to construct it?', or 'How to answer all queries?'.\n"
    "5. If another useful hint remains, set action='hint'.\n"
    "6. If all core logic and critical steps have already been given, set action='stop'.\n"
    "7. The next hint must not repeat or paraphrase previous hints.\n"
    "8. Early hints should focus on preliminary insights, such as simplifying the problem, trivial bounds, parity, problem reduction, extreme cases, or a small subproblem, rather than heavy data structures.\n"
    "9. Short algorithm-name hints are allowed only when the previous hints have fully motivated the technique. Otherwise, explain the observation that suggests the technique first.\n"
    "10. If a rejected candidate is provided, learn from the rejection: do not repeat that candidate's idea.\n\n"
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

NEXT_HINT_USER_VARIANTS = [
    "I am still stuck. Can you give me the next hint?",
    "I tried the previous hint but still cannot solve it. What should I think about next?",
    "Can you give me another hint?",
    "What is the next useful hint?",
]


# ============================================================
# Text utilities
# ============================================================

def normalize_text(x: str) -> str:
    return (x or "").strip()


def normalize_spaces(x: str) -> str:
    return re.sub(r"\s+", " ", (x or "").strip())


def stable_source_name(path_or_name: str) -> str:
    if not path_or_name:
        return "<unknown>"
    if path_or_name.startswith("correction_log:"):
        return path_or_name
    return os.path.basename(path_or_name)


def join_sections(sections: List[Dict[str, Any]]) -> str:
    parts: List[str] = []

    for sec in sections or []:
        title = normalize_text(sec.get("title", ""))
        content = normalize_text(sec.get("content", ""))

        if not content:
            continue

        if title.lower() == "solution" and re.fullmatch(r"\d{6,12}", content):
            continue

        if "tutorial is loading" in content.lower():
            continue

        if title and title.lower() not in {"statement", "solution", "tutorial", "editorial"}:
            parts.append(f"{title}\n{content}")
        else:
            parts.append(content)

    return "\n\n".join(parts).strip()


def split_multi_hint_content(content: str) -> List[str]:
    """Defensive cleanup for rare dirty files where several hints are stored in one content field.

    It is not assuming CF raw hints are repeated; it only prevents training a target
    where one visible hint contains "Hint 1 ... Hint 2 ...".
    """
    content = normalize_text(content)
    if not content:
        return []

    pattern = re.compile(r"(?im)^\s*(?:\*\*)?\s*hint\s*\d+\s*(?:\*\*)?\s*[:.\-]?\s*$")
    matches = list(pattern.finditer(content))
    if len(matches) <= 1:
        return [content]

    chunks: List[str] = []
    for i, m in enumerate(matches):
        start = m.end()
        end = matches[i + 1].start() if i + 1 < len(matches) else len(content)
        part = normalize_text(content[start:end])
        if part:
            chunks.append(part)

    return chunks or [content]


def clean_hints(raw_hints: List[Dict[str, Any]]) -> List[Dict[str, str]]:
    hints: List[Dict[str, str]] = []

    for i, hint in enumerate(raw_hints or [], start=1):
        title = normalize_text(hint.get("title", "")) or f"Hint {i}"
        content = normalize_text(hint.get("content", ""))

        if not content:
            continue

        if "tutorial is loading" in content.lower():
            continue

        for piece in split_multi_hint_content(content):
            hints.append({"title": f"Hint {len(hints) + 1}", "content": piece})

    return hints


def strip_hint_title(text: str) -> str:
    text = re.sub(r"^\s*\*\*?\s*hint\s*\d*\s*\*\*?\s*[:.\-]?\s*", "", text, flags=re.IGNORECASE)
    text = re.sub(r"^\s*hint\s*\d+\s*[:.\-]?\s*", "", text, flags=re.IGNORECASE)
    return normalize_spaces(text)


def renumber_hint(text: str, hint_id: int) -> str:
    body = strip_hint_title(text)
    return f"**Hint {hint_id}**\n{body}"


def hint_to_focus(hint_text: str, max_chars: int = 180) -> str:
    focus = strip_hint_title(hint_text)
    focus = re.sub(r"\$\$\$", "$", focus)
    focus = normalize_spaces(focus)

    if len(focus) > max_chars:
        focus = focus[: max_chars - 3].rstrip() + "..."

    return focus


def format_previous_hints(previous_hints: List[str]) -> str:
    if not previous_hints:
        return "No previous hints have been given."

    return "\n\n".join(f"Hint {i}:\n{h}" for i, h in enumerate(previous_hints, start=1))


def build_covered(previous_hint_texts: List[str]) -> List[str]:
    return [hint_to_focus(h) for h in previous_hint_texts]


def make_assistant_obj(action: str, covered_so_far: List[str], next_focus: str, hint: str) -> Dict[str, Any]:
    return {
        "action": action,
        "covered_so_far": covered_so_far,
        "next_focus": next_focus,
        "hint": hint,
    }


def make_assistant_json(action: str, covered_so_far: List[str], next_focus: str, hint: str) -> str:
    return json.dumps(make_assistant_obj(action, covered_so_far, next_focus, hint), ensure_ascii=False)


def normalize_decision_obj(obj: Dict[str, Any], fallback_hint_id: int) -> Dict[str, Any]:
    action = str(obj.get("action", "hint")).lower().strip()
    if action not in {"hint", "stop"}:
        action = "hint"

    covered = obj.get("covered_so_far", [])
    if not isinstance(covered, list):
        covered = []

    next_focus = normalize_text(str(obj.get("next_focus", "")))
    hint = normalize_text(str(obj.get("hint", "")))

    if action == "stop":
        next_focus = ""
        hint = FINAL_STOP_REPLY
    elif not hint:
        hint = f"**Hint {fallback_hint_id}**\nThink about the next useful step."

    return make_assistant_obj(action, covered, next_focus, hint)


# ============================================================
# Token utilities
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


def chat_token_len(tokenizer, messages: List[Dict[str, str]], add_generation_prompt: bool = False) -> int:
    text = tokenizer.apply_chat_template(
        messages,
        tokenize=False,
        add_generation_prompt=add_generation_prompt,
    )
    return token_len(tokenizer, text)


def build_context(tokenizer, statement: str, editorial: str, statement_budget: int, editorial_budget: int) -> str:
    statement_short = truncate_by_tokens(tokenizer, statement, statement_budget, keep_tail=False)
    editorial_short = head_tail_truncate(tokenizer, editorial, editorial_budget)

    return (
        "=== PROBLEM STATEMENT ===\n"
        f"{statement_short}\n\n"
        "=== FULL EDITORIAL EXCERPT ===\n"
        f"{editorial_short}\n"
    )


def build_user_content(
    tokenizer,
    statement: str,
    editorial: str,
    previous_hints: List[str],
    turn_idx: int,
    statement_budget: int,
    editorial_budget: int,
    rejected_candidate: Optional[Dict[str, Any]] = None,
    rejection_reason: str = "",
) -> str:
    question = "I am stuck. Can you give me Hint 1?" if turn_idx == 1 else random.choice(NEXT_HINT_USER_VARIANTS)

    user = (
        build_context(tokenizer, statement, editorial, statement_budget, editorial_budget)
        + "\n\n=== PREVIOUS HINTS ALREADY GIVEN ===\n"
        + format_previous_hints(previous_hints)
    )

    if rejected_candidate:
        user += (
            "\n\n=== REJECTED CANDIDATE FROM A PREVIOUS ATTEMPT ===\n"
            + json.dumps(rejected_candidate, ensure_ascii=False, indent=2)
            + "\n\n=== WHY IT WAS REJECTED ===\n"
            + (rejection_reason or "The rejected candidate was not a good next hint.")
            + "\n\nWhen answering, choose a different useful next hint focus, or stop if no useful hint remains."
        )

    user += (
        "\n\n=== CURRENT REQUEST ===\n"
        + question
        + "\n\nIndependently decide whether to give one new hint or stop. Return JSON only."
    )

    return user


def build_prompt_messages(
    tokenizer,
    statement: str,
    editorial: str,
    previous_hints: List[str],
    turn_idx: int,
    statement_budget: int,
    editorial_budget: int,
    rejected_candidate: Optional[Dict[str, Any]] = None,
    rejection_reason: str = "",
) -> List[Dict[str, str]]:
    return [
        {"role": "system", "content": SYSTEM_PROMPT},
        {
            "role": "user",
            "content": build_user_content(
                tokenizer=tokenizer,
                statement=statement,
                editorial=editorial,
                previous_hints=previous_hints,
                turn_idx=turn_idx,
                statement_budget=statement_budget,
                editorial_budget=editorial_budget,
                rejected_candidate=rejected_candidate,
                rejection_reason=rejection_reason,
            ),
        },
    ]


def build_messages_for_turn(
    tokenizer,
    statement: str,
    editorial: str,
    previous_hints: List[str],
    turn_idx: int,
    assistant_answer: str,
    statement_budget: int,
    editorial_budget: int,
    rejected_candidate: Optional[Dict[str, Any]] = None,
    rejection_reason: str = "",
) -> List[Dict[str, str]]:
    return build_prompt_messages(
        tokenizer=tokenizer,
        statement=statement,
        editorial=editorial,
        previous_hints=previous_hints,
        turn_idx=turn_idx,
        statement_budget=statement_budget,
        editorial_budget=editorial_budget,
        rejected_candidate=rejected_candidate,
        rejection_reason=rejection_reason,
    ) + [{"role": "assistant", "content": assistant_answer}]


def build_budget_profiles(args) -> List[Tuple[int, int]]:
    s0 = args.statement_budget
    e0 = args.editorial_budget

    raw = [
        (s0, e0),
        (int(s0 * 0.85), int(e0 * 0.85)),
        (int(s0 * 0.70), int(e0 * 0.70)),
        (int(s0 * 0.55), int(e0 * 0.55)),
        (256, 768),
        (128, 384),
    ]

    profiles = []
    seen = set()

    for s, e in raw:
        key = (max(64, s), max(128, e))
        if key not in seen:
            profiles.append(key)
            seen.add(key)

    return profiles


def try_make_sft_record(
    tokenizer,
    filepath: str,
    statement: str,
    editorial: str,
    previous_hints: List[str],
    turn_idx: int,
    assistant_answer: str,
    args,
    budget_profiles: List[Tuple[int, int]],
    meta: Dict[str, Any],
    rejected_candidate: Optional[Dict[str, Any]] = None,
    rejection_reason: str = "",
):
    for statement_budget, editorial_budget in budget_profiles:
        messages = build_messages_for_turn(
            tokenizer=tokenizer,
            statement=statement,
            editorial=editorial,
            previous_hints=previous_hints,
            turn_idx=turn_idx,
            assistant_answer=assistant_answer,
            statement_budget=statement_budget,
            editorial_budget=editorial_budget,
            rejected_candidate=rejected_candidate,
            rejection_reason=rejection_reason,
        )

        tl = chat_token_len(tokenizer, messages, add_generation_prompt=False)

        if tl <= args.max_seq_length:
            return {
                "source_file": stable_source_name(filepath),
                "token_len": tl,
                "budget": {"statement_budget": statement_budget, "editorial_budget": editorial_budget},
                "task_type": str(meta.get("task_type", "")),
                "turn_idx": int(meta.get("turn_idx", turn_idx)),
                "num_hints": int(meta.get("num_hints", -1)),
                "is_stop_turn": bool(meta.get("is_stop_turn", False)),
                "next_focus": str(meta.get("next_focus", "")),
                "correction_source": str(meta.get("correction_source", "")),
                "messages": messages,
            }

    return None


def try_make_dpo_record(
    tokenizer,
    filepath: str,
    statement: str,
    editorial: str,
    previous_hints: List[str],
    turn_idx: int,
    chosen: str,
    rejected: str,
    args,
    budget_profiles: List[Tuple[int, int]],
    meta: Dict[str, Any],
):
    for statement_budget, editorial_budget in budget_profiles:
        prompt_messages = build_prompt_messages(
            tokenizer=tokenizer,
            statement=statement,
            editorial=editorial,
            previous_hints=previous_hints,
            turn_idx=turn_idx,
            statement_budget=statement_budget,
            editorial_budget=editorial_budget,
        )

        prompt = tokenizer.apply_chat_template(prompt_messages, tokenize=False, add_generation_prompt=True)
        prompt_len = token_len(tokenizer, prompt)
        chosen_len = token_len(tokenizer, chosen)
        rejected_len = token_len(tokenizer, rejected)

        if prompt_len + max(chosen_len, rejected_len) <= args.max_seq_length:
            return {
                "source_file": stable_source_name(filepath),
                "task_type": str(meta.get("task_type", "")),
                "turn_idx": int(meta.get("turn_idx", turn_idx)),
                "num_hints": int(meta.get("num_hints", -1)),
                "chosen_next_focus": str(meta.get("chosen_next_focus", "")),
                "rejected_type": str(meta.get("rejected_type", "")),
                "prompt": prompt,
                "chosen": chosen,
                "rejected": rejected,
                "prompt_messages": prompt_messages,
                "token_len": prompt_len + max(chosen_len, rejected_len),
            }

    return None


# ============================================================
# Data building
# ============================================================

def iter_problem_files(input_glob: str) -> Iterable[str]:
    yield from sorted(glob.glob(input_glob, recursive=True))


def load_problem_record(filepath: str) -> Optional[Tuple[str, str, List[Dict[str, str]], Dict[str, Any]]]:
    try:
        with open(filepath, "r", encoding="utf-8") as f:
            data = json.load(f)
    except Exception as e:
        print(f"⚠️ Bad JSON, skipped: {filepath}, error={e}")
        return None

    statement = join_sections(data.get("statement", []))
    editorial = join_sections(data.get("solutions", []))
    raw_hints = clean_hints(data.get("hints", []))

    return statement, editorial, raw_hints, data


def build_problem_records(tokenizer, filepath: str, statement: str, editorial: str, raw_hints: List[Dict[str, str]], args, budget_profiles):
    sft_records = []
    dpo_records = []
    previous_hints: List[str] = []

    for idx, hint_obj in enumerate(raw_hints, start=1):
        visible_hint = f"**Hint {idx}**\n{hint_obj['content']}"
        covered = build_covered(previous_hints)
        next_focus = hint_to_focus(hint_obj["content"])

        chosen_obj = make_assistant_obj("hint", covered, next_focus, visible_hint)
        chosen_json = json.dumps(chosen_obj, ensure_ascii=False)

        # 1. raw hint SFT
        raw_record = try_make_sft_record(
            tokenizer=tokenizer,
            filepath=filepath,
            statement=statement,
            editorial=editorial,
            previous_hints=previous_hints,
            turn_idx=idx,
            assistant_answer=chosen_json,
            args=args,
            budget_profiles=budget_profiles,
            meta={
                "task_type": "raw_hint_decision",
                "turn_idx": idx,
                "num_hints": len(raw_hints),
                "is_stop_turn": False,
                "next_focus": next_focus,
            },
        )
        if raw_record:
            sft_records.append(raw_record)

        # 2. synthetic early-stop rejection SFT + DPO
        # For every non-final hint turn, stopping here is a bad answer.
        if args.add_synthetic_early_stop:
            rejected_stop_obj = make_assistant_obj(
                action="stop",
                covered_so_far=covered,
                next_focus="",
                hint=FINAL_STOP_REPLY,
            )
            rejected_stop_json = json.dumps(rejected_stop_obj, ensure_ascii=False)
            rejection_reason = "This candidate stops too early. There is still a useful next hint that has not been given."

            sft_stop_record = try_make_sft_record(
                tokenizer=tokenizer,
                filepath=filepath,
                statement=statement,
                editorial=editorial,
                previous_hints=previous_hints,
                turn_idx=idx,
                assistant_answer=chosen_json,
                args=args,
                budget_profiles=budget_profiles,
                meta={
                    "task_type": "synthetic_early_stop_rejection",
                    "turn_idx": idx,
                    "num_hints": len(raw_hints),
                    "is_stop_turn": False,
                    "next_focus": next_focus,
                },
                rejected_candidate=rejected_stop_obj,
                rejection_reason=rejection_reason,
            )
            if sft_stop_record:
                sft_records.append(sft_stop_record)

            dpo_stop_record = try_make_dpo_record(
                tokenizer=tokenizer,
                filepath=filepath,
                statement=statement,
                editorial=editorial,
                previous_hints=previous_hints,
                turn_idx=idx,
                chosen=chosen_json,
                rejected=rejected_stop_json,
                args=args,
                budget_profiles=budget_profiles,
                meta={
                    "task_type": "dpo_early_stop",
                    "turn_idx": idx,
                    "num_hints": len(raw_hints),
                    "chosen_next_focus": next_focus,
                    "rejected_type": "early_stop",
                },
            )
            if dpo_stop_record:
                dpo_records.append(dpo_stop_record)

        # 3. synthetic repeat rejection SFT + DPO
        # Teach the model that repeating any previous hint is worse than the correct next hint.
        if args.add_synthetic_repeat and previous_hints:
            if args.repeat_source == "all":
                repeat_indices = list(range(len(previous_hints)))
            elif args.repeat_source == "random":
                repeat_indices = [random.randrange(len(previous_hints))]
            else:
                repeat_indices = [len(previous_hints) - 1]

            for old_i in repeat_indices[: args.max_repeat_rejections_per_turn]:
                repeated_hint = renumber_hint(previous_hints[old_i], idx)
                rejected_repeat_obj = make_assistant_obj(
                    action="hint",
                    covered_so_far=covered,
                    next_focus=hint_to_focus(previous_hints[old_i]),
                    hint=repeated_hint,
                )
                rejected_repeat_json = json.dumps(rejected_repeat_obj, ensure_ascii=False)
                rejection_reason = (
                    f"This candidate repeats previous Hint {old_i + 1}. "
                    "The next hint must advance to a different idea."
                )

                sft_repeat_record = try_make_sft_record(
                    tokenizer=tokenizer,
                    filepath=filepath,
                    statement=statement,
                    editorial=editorial,
                    previous_hints=previous_hints,
                    turn_idx=idx,
                    assistant_answer=chosen_json,
                    args=args,
                    budget_profiles=budget_profiles,
                    meta={
                        "task_type": "synthetic_repeat_rejection",
                        "turn_idx": idx,
                        "num_hints": len(raw_hints),
                        "is_stop_turn": False,
                        "next_focus": next_focus,
                    },
                    rejected_candidate=rejected_repeat_obj,
                    rejection_reason=rejection_reason,
                )
                if sft_repeat_record:
                    sft_records.append(sft_repeat_record)

                dpo_repeat_record = try_make_dpo_record(
                    tokenizer=tokenizer,
                    filepath=filepath,
                    statement=statement,
                    editorial=editorial,
                    previous_hints=previous_hints,
                    turn_idx=idx,
                    chosen=chosen_json,
                    rejected=rejected_repeat_json,
                    args=args,
                    budget_profiles=budget_profiles,
                    meta={
                        "task_type": "dpo_repeat",
                        "turn_idx": idx,
                        "num_hints": len(raw_hints),
                        "chosen_next_focus": next_focus,
                        "rejected_type": "repeat",
                    },
                )
                if dpo_repeat_record:
                    dpo_records.append(dpo_repeat_record)

        previous_hints.append(visible_hint)

    # Final stop sample: raw SFT only.
    stop_idx = len(raw_hints) + 1
    stop_json = make_assistant_json(
        action="stop",
        covered_so_far=build_covered(previous_hints),
        next_focus="",
        hint=FINAL_STOP_REPLY,
    )

    stop_record = try_make_sft_record(
        tokenizer=tokenizer,
        filepath=filepath,
        statement=statement,
        editorial=editorial,
        previous_hints=previous_hints,
        turn_idx=stop_idx,
        assistant_answer=stop_json,
        args=args,
        budget_profiles=budget_profiles,
        meta={
            "task_type": "raw_hint_decision",
            "turn_idx": stop_idx,
            "num_hints": len(raw_hints),
            "is_stop_turn": True,
            "next_focus": "",
        },
    )
    if stop_record:
        sft_records.append(stop_record)

    return sft_records, dpo_records


def read_jsonl(path: str) -> Iterable[Dict[str, Any]]:
    p = Path(path)
    if not p.exists():
        return

    with p.open("r", encoding="utf-8") as f:
        for line_no, line in enumerate(f, start=1):
            line = line.strip()
            if not line:
                continue

            try:
                obj = json.loads(line)
            except Exception as e:
                print(f"⚠️ Bad correction JSON line {line_no}: {e}")
                continue

            if isinstance(obj, dict):
                yield obj


def build_correction_records(tokenizer, correction_log: str, args, budget_profiles):
    sft_records = []
    dpo_records = []
    skipped = 0

    for i, event in enumerate(read_jsonl(correction_log) or [], start=1):
        statement = normalize_text(event.get("statement", ""))
        editorial = normalize_text(event.get("editorial", ""))
        previous_hints = event.get("previous_hints", [])
        if not isinstance(previous_hints, list):
            previous_hints = []

        turn_idx = int(event.get("turn_idx", len(previous_hints) + 1))

        rejected_candidate = event.get("bad_decision") or {
            "action": "hint",
            "next_focus": event.get("bad_next_focus", ""),
            "hint": event.get("bad_hint", ""),
        }

        corrected_decision = event.get("corrected_decision")
        if not isinstance(corrected_decision, dict):
            skipped += 1
            continue

        corrected = normalize_decision_obj(corrected_decision, fallback_hint_id=turn_idx)
        chosen_json = json.dumps(corrected, ensure_ascii=False)
        rejected_json = json.dumps(normalize_decision_obj(rejected_candidate, fallback_hint_id=turn_idx), ensure_ascii=False)

        if not statement or not editorial:
            skipped += 1
            continue

        source = str(event.get("source_file") or f"correction_log:{i}")

        sft_record = try_make_sft_record(
            tokenizer=tokenizer,
            filepath=source,
            statement=statement,
            editorial=editorial,
            previous_hints=previous_hints,
            turn_idx=turn_idx,
            assistant_answer=chosen_json,
            args=args,
            budget_profiles=budget_profiles,
            meta={
                "task_type": "semantic_repeat_correction",
                "turn_idx": turn_idx,
                "num_hints": int(event.get("num_hints", -1)),
                "is_stop_turn": corrected["action"] == "stop",
                "next_focus": corrected.get("next_focus", ""),
                "correction_source": source,
            },
            rejected_candidate=rejected_candidate,
            rejection_reason=event.get("rejection_reason", "The rejected candidate was not a good next hint."),
        )
        if sft_record:
            sft_records.append(sft_record)
        else:
            skipped += 1

        dpo_record = try_make_dpo_record(
            tokenizer=tokenizer,
            filepath=source,
            statement=statement,
            editorial=editorial,
            previous_hints=previous_hints,
            turn_idx=turn_idx,
            chosen=chosen_json,
            rejected=rejected_json,
            args=args,
            budget_profiles=budget_profiles,
            meta={
                "task_type": "dpo_logged_correction",
                "turn_idx": turn_idx,
                "num_hints": int(event.get("num_hints", -1)),
                "chosen_next_focus": corrected.get("next_focus", ""),
                "rejected_type": "logged_bad_candidate",
            },
        )
        if dpo_record:
            dpo_records.append(dpo_record)

    return sft_records, dpo_records, skipped


# ============================================================
# Main
# ============================================================

def parse_args():
    parser = argparse.ArgumentParser()

    parser.add_argument("--input_glob", type=str, default=DEFAULT_INPUT_GLOB)
    parser.add_argument("--sft_output_file", type=str, default=DEFAULT_SFT_OUTPUT_FILE)
    parser.add_argument("--dpo_output_file", type=str, default=DEFAULT_DPO_OUTPUT_FILE)
    parser.add_argument("--model_path", type=str, default=DEFAULT_MODEL_PATH)

    parser.add_argument("--correction_log", type=str, default=DEFAULT_CORRECTION_LOG)
    parser.add_argument("--no_correction_log", action="store_true")

    parser.add_argument("--max_seq_length", type=int, default=4096)
    parser.add_argument("--statement_budget", type=int, default=900)
    parser.add_argument("--editorial_budget", type=int, default=2200)

    parser.add_argument("--min_raw_hints", type=int, default=2)

    parser.add_argument("--add_synthetic_early_stop", action=argparse.BooleanOptionalAction, default=True)
    parser.add_argument("--add_synthetic_repeat", action=argparse.BooleanOptionalAction, default=True)
    parser.add_argument(
        "--repeat_source",
        choices=["last", "random", "all"],
        default="last",
        help="Which previous hints are used to synthesize repeated rejected candidates.",
    )
    parser.add_argument("--max_repeat_rejections_per_turn", type=int, default=1)

    parser.add_argument("--seed", type=int, default=42)

    return parser.parse_args()


def main():
    args = parse_args()
    random.seed(args.seed)

    print(f"🔄 Loading tokenizer from {args.model_path}")
    tokenizer = AutoTokenizer.from_pretrained(args.model_path, trust_remote_code=True)

    input_files = list(iter_problem_files(args.input_glob))
    print(f"🔍 Found {len(input_files)} candidate files from {args.input_glob}")
    print(f"🧹 Minimum raw hints per problem: {args.min_raw_hints}")
    print(f"🛑 Synthetic early-stop rejection: {args.add_synthetic_early_stop}")
    print(f"🔁 Synthetic repeat rejection: {args.add_synthetic_repeat}, repeat_source={args.repeat_source}")

    budget_profiles = build_budget_profiles(args)

    sft_records = []
    dpo_records = []
    skipped = {
        "bad_or_unreadable_json": 0,
        "missing_statement": 0,
        "missing_solution": 0,
        "missing_raw_hints": 0,
        "too_few_raw_hints": 0,
        "correction_skipped": 0,
    }

    stats = {
        "source_problem_count": 0,
        "hint_counts": [],
    }

    for filepath in input_files:
        loaded = load_problem_record(filepath)
        if loaded is None:
            skipped["bad_or_unreadable_json"] += 1
            continue

        statement, editorial, raw_hints, _ = loaded

        if not statement:
            skipped["missing_statement"] += 1
            continue

        if not editorial:
            skipped["missing_solution"] += 1
            continue

        if not raw_hints:
            skipped["missing_raw_hints"] += 1
            continue

        if len(raw_hints) < args.min_raw_hints:
            skipped["too_few_raw_hints"] += 1
            continue

        problem_sft, problem_dpo = build_problem_records(
            tokenizer=tokenizer,
            filepath=filepath,
            statement=statement,
            editorial=editorial,
            raw_hints=raw_hints,
            args=args,
            budget_profiles=budget_profiles,
        )

        sft_records.extend(problem_sft)
        dpo_records.extend(problem_dpo)

        stats["source_problem_count"] += 1
        stats["hint_counts"].append(len(raw_hints))

    if not args.no_correction_log:
        corr_sft, corr_dpo, correction_skipped = build_correction_records(
            tokenizer=tokenizer,
            correction_log=args.correction_log,
            args=args,
            budget_profiles=budget_profiles,
        )
        sft_records.extend(corr_sft)
        dpo_records.extend(corr_dpo)
        skipped["correction_skipped"] = correction_skipped

    with Path(args.sft_output_file).open("w", encoding="utf-8") as out_f:
        for record in sft_records:
            out_f.write(json.dumps(record, ensure_ascii=False) + "\n")

    with Path(args.dpo_output_file).open("w", encoding="utf-8") as out_f:
        for record in dpo_records:
            out_f.write(json.dumps(record, ensure_ascii=False) + "\n")

    from collections import Counter

    sft_task_counts = Counter(r["task_type"] for r in sft_records)
    dpo_task_counts = Counter(r["task_type"] for r in dpo_records)

    print("\n========== Dataset Build Summary ==========")
    print(f"✅ Source problems used: {stats['source_problem_count']}")
    print(f"📄 SFT output file: {Path(args.sft_output_file).resolve()}")
    print(f"📄 DPO output file: {Path(args.dpo_output_file).resolve()}")
    print(f"✅ SFT samples written: {len(sft_records)}")
    print(f"✅ DPO preference pairs written: {len(dpo_records)}")
    print(f"📊 SFT task distribution: {dict(sft_task_counts)}")
    print(f"📊 DPO task distribution: {dict(dpo_task_counts)}")
    print(f"📊 Skipped: {skipped}")

    if sft_records:
        lengths = [r["token_len"] for r in sft_records]
        print(f"📈 SFT token_len max={max(lengths)}, avg={sum(lengths) / len(lengths):.2f}, min={min(lengths)}")

    if dpo_records:
        lengths = [r["token_len"] for r in dpo_records]
        print(f"📈 DPO token_len max={max(lengths)}, avg={sum(lengths) / len(lengths):.2f}, min={min(lengths)}")

    if stats["hint_counts"]:
        hc = stats["hint_counts"]
        print(f"📌 Raw hints per problem: max={max(hc)}, avg={sum(hc) / len(hc):.2f}, min={min(hc)}")


if __name__ == "__main__":
    main()

import argparse
import json
import os
import re
import time
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple

# 引入 OpenAI 兼容库支持 API 调用
from openai import OpenAI
from transformers import AutoTokenizer

# ============================================================
# Default configuration
# ============================================================

BASE_DIR = Path(__file__).resolve().parent

DEFAULT_BASE_MODEL_PATH = str(BASE_DIR / "qwen-coder-7b")
DEFAULT_API_KEY = "AIzaSy你的真实Key"
DEFAULT_BASE_URL = "https://generativelanguage.googleapis.com/v1beta/openai/"
DEFAULT_MODEL_NAME = "gemini-1.5-flash"

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
    "4. If another useful hint remains, set action='hint'.\n"
    "5. If all core logic and critical steps have already been given, set action='stop'.\n"
    "6. The next hint must not repeat or paraphrase previous hints.\n"
    "7. Early hints MUST focus on preliminary insights (e.g., simplifying the problem, trivial bounds, parity, problem reduction, extreme cases) rather than heavy data structures.\n"
    "8. Short algorithm-name hints are allowed ONLY when the previous hints have fully motivated the technique. Otherwise, explain the 'why' first.\n"
    "9. If a rejected candidate is provided, learn from the rejection: do not repeat that candidate's idea.\n\n"
    "Required JSON schema for a hint:\n"
    "{}\n"
    "  \"action\": \"hint\",\n"
    "  \"covered_so_far\": [\"short summary of a previous hint\"],\n"
    "  \"next_focus\": \"the MOST PRELIMINARY and CRITICAL next observation\",\n"
    "  \"hint\": \"**Hint k**\\n...\"\n"
    "}}\n\n"
    "Required JSON schema for stopping:\n"
    "{}\n"
    "  \"action\": \"stop\",\n"
    "  \"covered_so_far\": [\"short summary of previous hints\"],\n"
    "  \"next_focus\": \"\",\n"
    f"  \"hint\": \"{FINAL_STOP_REPLY}\"\n"
    "}}"
).replace("{}", "{{\n").replace("}}", "\n}}") # 维持原 Prompt 结构并避免 f-string 冲突

STOP_MARKERS = [
    FINAL_STOP_REPLY.lower(),
    "no. the rest",
    "think about yourself",
    "no further hint",
    "no more hint",
]


# ============================================================
# Problem loading
# ============================================================

def normalize_text(x: str) -> str:
    return (x or "").strip()

def join_sections(sections) -> str:
    parts: List[str] = []
    for sec in sections or []:
        title = normalize_text(sec.get("title", ""))
        content = normalize_text(sec.get("content", ""))
        if not content:
            continue
        if title.lower() == "solution" and re.fullmatch(r"\d{6,12}", content):
            continue
        if title and title.lower() not in {"statement", "solution", "tutorial", "editorial"}:
            parts.append(f"{title}\n{content}")
        else:
            parts.append(content)
    return "\n\n".join(parts).strip()

def load_problem_json(problem_file: str):
    path = Path(problem_file).expanduser().resolve()
    if not path.exists():
        raise FileNotFoundError(f"Problem file not found: {path}")

    with path.open("r", encoding="utf-8") as f:
        data = json.load(f)

    statement = join_sections(data.get("statement", []))
    editorial = join_sections(data.get("solutions", []))

    if not statement:
        raise ValueError(f"No statement found in: {path}")
    if not editorial:
        raise ValueError(f"No solutions/editorial found in: {path}")

    print(f"📄 Loaded problem file: {path}")
    return statement, editorial, str(path)


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
    rejected_candidate: Optional[Dict[str, Any]] = None,
    rejection_reason: str = "",
) -> str:
    question = "I am stuck. Can you give me Hint 1?" if turn_idx == 1 else "Can you give me the next hint?"

    content = (
        build_context(tokenizer, statement, editorial, statement_budget, editorial_budget)
        + "\n\n=== PREVIOUS HINTS ALREADY GIVEN ===\n"
        + format_previous_hints(previous_hints)
    )

    if rejected_candidate:
        content += (
            "\n\n=== REJECTED CANDIDATE FROM A PREVIOUS ATTEMPT ===\n"
            + json.dumps(rejected_candidate, ensure_ascii=False, indent=2)
            + "\n\n=== WHY IT WAS REJECTED ===\n"
            + (rejection_reason or "It was too similar to a previous hint.")
            + "\n\nWhen answering, choose a different useful next hint focus, or stop if no useful hint remains."
        )

    content += (
        "\n\n=== CURRENT REQUEST ===\n"
        + question
        + "\n\nIndependently decide whether to give one new hint or stop. Return JSON only."
    )
    return content

def build_messages_under_budget(
    tokenizer,
    statement: str,
    editorial: str,
    previous_hints: List[str],
    turn_idx: int,
    max_seq_length: int,
    statement_budget: int,
    editorial_budget: int,
    rejected_candidate: Optional[Dict[str, Any]] = None,
    rejection_reason: str = "",
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
            rejected_candidate=rejected_candidate,
            rejection_reason=rejection_reason,
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
        rejected_candidate=rejected_candidate,
        rejection_reason=rejection_reason,
    )

    return [
        {"role": "system", "content": SYSTEM_PROMPT},
        {"role": "user", "content": user_content},
    ]


# ============================================================
# JSON parsing
# ============================================================

def extract_json_object(text: str) -> Optional[Dict[str, Any]]:
    text = (text or "").strip()
    text = re.sub(r"^
http://googleusercontent.com/immersive_entry_chip/0
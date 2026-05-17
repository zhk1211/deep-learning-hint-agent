import argparse
import json
from pathlib import Path
from typing import Dict, List

import torch
from peft import PeftModel
from transformers import AutoModelForCausalLM, AutoTokenizer


# =========================
# Default Configuration
# =========================

BASE_DIR = Path(__file__).resolve().parent

DEFAULT_BASE_MODEL_PATH = str(BASE_DIR / "qwen-coder-7b")
DEFAULT_LORA_PATH = str(BASE_DIR / "cf_hint_lora_model_budgeted" / "final_best")

FINAL_STOP_REPLY = "No. The rest you need to think about yourself."

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

STOP_MARKERS = [
    FINAL_STOP_REPLY.lower(),
    "no. the rest",
    "think about yourself",
    "no further hint",
    "no more hint",
]


# =========================
# Problem file loading
# =========================

def normalize_text(x: str) -> str:
    return (x or "").strip()


def join_sections(sections) -> str:
    """Join sections like statement / solutions from one Codeforces JSON file."""
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


def load_problem_json(problem_file: str):
    """Load statement and editorial from one JSON file in cf_dataset/with_hint."""
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
    print(f"📝 Statement sections: {len(data.get('statement', []))}")
    print(f"📚 Solution sections: {len(data.get('solutions', []))}")
    print(f"💡 Existing human hints in file: {len(data.get('hints', []))}")

    return statement, editorial



# =========================
# Token budget utilities
# =========================

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


def chat_token_len(tokenizer, messages: List[Dict[str, str]]) -> int:
    text = tokenizer.apply_chat_template(
        messages,
        tokenize=False,
        add_generation_prompt=True,
    )
    return token_len(tokenizer, text)


def build_previous_hints(tokenizer, given_hints: List[str], max_tokens: int) -> str:
    if not given_hints or max_tokens <= 0:
        return ""

    parts = []
    for i, h in enumerate(given_hints, start=1):
        parts.append(f"Hint {i} already given:\n{h}")

    text = "\n\n".join(parts)
    return truncate_by_tokens(tokenizer, text, max_tokens=max_tokens, keep_tail=True)


def build_user_content(
    tokenizer,
    statement: str,
    editorial: str,
    given_hints: List[str],
    turn_idx: int,
    statement_budget: int,
    editorial_budget: int,
    previous_hints_budget: int,
) -> str:
    statement_short = truncate_by_tokens(
        tokenizer,
        statement,
        max_tokens=statement_budget,
        keep_tail=False,
    )

    editorial_short = head_tail_truncate(
        tokenizer,
        editorial,
        max_tokens=editorial_budget,
    )

    previous_hints = build_previous_hints(
        tokenizer,
        given_hints,
        max_tokens=previous_hints_budget,
    )

    content = (
        "=== PROBLEM STATEMENT ===\n"
        f"{statement_short}\n\n"
        "=== FULL EDITORIAL EXCERPT ===\n"
        f"{editorial_short}\n"
    )

    if previous_hints:
        content += "\n=== PREVIOUS HINTS ALREADY GIVEN ===\n"
        content += previous_hints
        content += "\n"

    if turn_idx == 1:
        question = "I am stuck. Can you give me Hint 1?"
    else:
        question = "Can you give me next hint?"

    return content + "\n" + question


def build_messages_under_budget(
    tokenizer,
    statement: str,
    editorial: str,
    given_hints: List[str],
    turn_idx: int,
    max_seq_length: int,
    statement_budget: int,
    editorial_budget: int,
    previous_hints_budget: int,
) -> List[Dict[str, str]]:
    profiles = [
        (statement_budget, editorial_budget, previous_hints_budget),
        (int(statement_budget * 0.85), int(editorial_budget * 0.85), int(previous_hints_budget * 0.85)),
        (int(statement_budget * 0.70), int(editorial_budget * 0.70), int(previous_hints_budget * 0.70)),
        (int(statement_budget * 0.55), int(editorial_budget * 0.55), int(previous_hints_budget * 0.55)),
        (256, 512, 128),
        (128, 256, 64),
    ]

    for s_budget, e_budget, h_budget in profiles:
        user_content = build_user_content(
            tokenizer=tokenizer,
            statement=statement,
            editorial=editorial,
            given_hints=given_hints,
            turn_idx=turn_idx,
            statement_budget=max(64, s_budget),
            editorial_budget=max(128, e_budget),
            previous_hints_budget=max(0, h_budget),
        )

        messages = [
            {"role": "system", "content": SYSTEM_PROMPT},
            {"role": "user", "content": user_content},
        ]

        if chat_token_len(tokenizer, messages) <= max_seq_length:
            return messages

    # Last-resort minimal prompt.
    user_content = (
        "=== PROBLEM STATEMENT ===\n"
        f"{truncate_by_tokens(tokenizer, statement, 128)}\n\n"
        "=== FULL EDITORIAL EXCERPT ===\n"
        f"{head_tail_truncate(tokenizer, editorial, 256)}\n\n"
        + ("I am stuck. Can you give me Hint 1?" if turn_idx == 1 else "Can you give me next hint?")
    )

    return [
        {"role": "system", "content": SYSTEM_PROMPT},
        {"role": "user", "content": user_content},
    ]


# =========================
# IO / generation
# =========================

def read_multiline(prompt: str) -> str:
    print(prompt)
    print("输入结束后，单独一行输入 EOF：")
    lines = []

    while True:
        line = input()
        if line.strip() == "EOF":
            break
        lines.append(line)

    return "\n".join(lines).strip()


def should_stop(response: str) -> bool:
    lower = response.lower().strip()
    return any(marker in lower for marker in STOP_MARKERS)


def generate_response(model, tokenizer, messages: List[Dict[str, str]], args) -> str:
    text = tokenizer.apply_chat_template(
        messages,
        tokenize=False,
        add_generation_prompt=True,
    )

    device = next(model.parameters()).device
    inputs = tokenizer([text], return_tensors="pt").to(device)

    with torch.no_grad():
        outputs = model.generate(
            **inputs,
            max_new_tokens=args.max_new_tokens,
            temperature=args.temperature,
            top_p=args.top_p,
            do_sample=True,
            repetition_penalty=args.repetition_penalty,
            pad_token_id=tokenizer.eos_token_id,
            eos_token_id=tokenizer.eos_token_id,
        )

    response = tokenizer.decode(
        outputs[0][inputs.input_ids.shape[1]:],
        skip_special_tokens=True,
    ).strip()

    return response


def parse_args():
    parser = argparse.ArgumentParser()

    parser.add_argument("--base_model_path", type=str, default=DEFAULT_BASE_MODEL_PATH)
    parser.add_argument("--lora_path", type=str, default=DEFAULT_LORA_PATH)

    parser.add_argument("--max_turns", type=int, default=10)
    parser.add_argument("--max_seq_length", type=int, default=4096)
    parser.add_argument("--max_new_tokens", type=int, default=256)

    parser.add_argument("--statement_budget", type=int, default=900)
    parser.add_argument("--editorial_budget", type=int, default=2200)
    parser.add_argument("--previous_hints_budget", type=int, default=600)

    parser.add_argument("--temperature", type=float, default=0.5)
    parser.add_argument("--top_p", type=float, default=0.9)
    parser.add_argument("--repetition_penalty", type=float, default=1.05)

    parser.add_argument(
        "--problem_file",
        type=str,
        default=None,
        help="Load one problem JSON file from cf_dataset/with_hint instead of manual input.",
    )

    return parser.parse_args()


def main():
    args = parse_args()

    print("🔄 正在加载基础模型与 LoRA 权重...")
    print("📦 Base model:", args.base_model_path)
    print("🎯 LoRA:", args.lora_path)

    tokenizer = AutoTokenizer.from_pretrained(args.base_model_path, trust_remote_code=True)

    if tokenizer.pad_token is None:
        tokenizer.pad_token = tokenizer.eos_token

    base_model = AutoModelForCausalLM.from_pretrained(
        args.base_model_path,
        torch_dtype=torch.bfloat16,
        device_map="auto",
        trust_remote_code=True,
    )

    model = PeftModel.from_pretrained(base_model, args.lora_path)
    model.eval()

    if args.problem_file:
        statement, editorial = load_problem_json(args.problem_file)
    else:
        use_manual = input("是否手动输入题面和题解？(y/N): ").strip().lower() == "y"

        if use_manual:
            statement = read_multiline("请粘贴 PROBLEM STATEMENT")
            editorial = read_multiline("请粘贴 FULL EDITORIAL / SOLUTION")
        else:
            statement = "Given an array A of N integers, find the maximum subarray sum."
            editorial = (
                "This is a classic Kadane's algorithm problem. "
                "Maintain the best subarray sum ending at the current position, "
                "and update the global maximum."
            )

    given_hints: List[str] = []

    print("\nHint generation starts\n")

    for turn_idx in range(1, args.max_turns + 1):
        messages = build_messages_under_budget(
            tokenizer=tokenizer,
            statement=statement,
            editorial=editorial,
            given_hints=given_hints,
            turn_idx=turn_idx,
            max_seq_length=args.max_seq_length,
            statement_budget=args.statement_budget,
            editorial_budget=args.editorial_budget,
            previous_hints_budget=args.previous_hints_budget,
        )

        response = generate_response(model, tokenizer, messages, args)

        print(f"\n=================== answer {turn_idx} ===================")
        print(response)
        print("========================================================\n")

        if should_stop(response):
            print("✅ 模型判断已经没有更多有意义的 Hint，交互结束。")
            break

        given_hints.append(response)

        user_input = input("你要继续索要下一个 Hint 吗？(按回车继续，输入 q 退出): ").strip().lower()
        if user_input == "q":
            break
    else:
        print(f"⚠️ 已达到 max_turns={args.max_turns}，为避免无限生成，自动停止。")


if __name__ == "__main__":
    main()

import argparse
import json
import os
import re
import time
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple

import torch
from peft import PeftModel
from transformers import AutoModelForCausalLM, AutoTokenizer


# ============================================================
# Default configuration
# ============================================================

BASE_DIR = Path(__file__).resolve().parent

DEFAULT_BASE_MODEL_PATH = str(BASE_DIR / "qwen-coder-7b")
DEFAULT_NEXT_DECISION_LORA_PATH = str(BASE_DIR / "cf_hint_lora_model_budgeted" / "checkpoint-175")

FINAL_STOP_REPLY = "No. The rest you need to think about yourself."

# 💥【修复】：100% 还原 Step 1 训练时的 SYSTEM_PROMPT，绝不乱加戏
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
    "{\n"
    "  \"action\": \"hint\",\n"
    "  \"covered_so_far\": [\"short summary of a previous hint\"],\n"
    "  \"next_focus\": \"the MOST PRELIMINARY and CRITICAL next observation\",\n"
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

    # 💥【修复】：严格移除没在训练集中出现过的 Granularity Preference！
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

def normalize_decision(obj: Optional[Dict[str, Any]], raw_text: str, turn_idx: int) -> Dict[str, Any]:
    if not obj:
        if should_stop(raw_text):
            return {
                "action": "stop",
                "covered_so_far": [],
                "next_focus": "",
                "hint": FINAL_STOP_REPLY,
            }
        return {
            "action": "hint",
            "covered_so_far": [],
            "next_focus": "unparsed model output",
            "hint": raw_text.strip() or f"**Hint {turn_idx}**\nThink about the next useful step.",
        }

    action = str(obj.get("action", "hint")).lower().strip()
    hint = str(obj.get("hint", "") or "").strip()
    next_focus = str(obj.get("next_focus", "") or "").strip()
    covered = obj.get("covered_so_far", [])

    if not isinstance(covered, list):
        covered = []

    if action not in {"hint", "stop"}:
        action = "stop" if should_stop(hint) else "hint"

    if action == "stop":
        hint = FINAL_STOP_REPLY
        next_focus = ""

    if action == "hint" and not hint:
        hint = f"**Hint {turn_idx}**\nFocus on the next useful step from the editorial."

    return {
        "action": action,
        "covered_so_far": covered,
        "next_focus": next_focus,
        "hint": hint,
    }


def is_generic_hint_text(hint: str) -> bool:
    h = normalize_for_similarity(hint)
    generic_patterns = [
        "think about the next useful step",
        "focus on the next useful step",
        "unparsed model output",
        "try to solve the problem",
    ]
    return any(p in h for p in generic_patterns)


def count_hint_markers(text: str) -> int:
    """Count explicit Hint-k markers inside a visible hint."""
    return len(re.findall(r"(?i)(?:^|\n)\s*(?:\*\*)?\s*hint\s*\d+\s*(?:\*\*)?\s*[:.\-]?", text or ""))


def sanitize_visible_hint(hint_text: str, turn_idx: int) -> str:
    """Keep exactly one visible hint.

    This is a safety net against future-hint leakage, e.g. a model output like
    "**Hint 1**\nHint 1 ...\nHint 2 ...\nHint 3 ...".
    The training data should avoid this, but inference should still not print it.
    """
    text = (hint_text or "").strip()
    if should_stop(text):
        return FINAL_STOP_REPLY

    # Remove an outer title if present, then normalize to the requested turn number.
    body = re.sub(r"(?is)^\s*(?:\*\*)?\s*hint\s*\d+\s*(?:\*\*)?\s*[:.\-]?\s*", "", text).strip()

    # Some bad generations start the body with another bare "Hint 1" line.
    body = re.sub(r"(?is)^\s*(?:\*\*)?\s*hint\s*\d+\s*(?:\*\*)?\s*[:.\-]?\s*", "", body).strip()

    # Cut before the next explicit Hint-k marker, if any.
    parts = re.split(r"(?i)\n\s*(?:\*\*)?\s*hint\s*\d+\s*(?:\*\*)?\s*[:.\-]?\s*", body, maxsplit=1)
    body = parts[0].strip()

    if not body:
        body = "Think about the next useful observation."

    return f"**Hint {turn_idx}**\n{body}"

# ============================================================
# Semantic Similarity Helpers
# ============================================================

def normalize_for_similarity(text: str) -> str:
    text = (text or "").lower()
    text = re.sub(r"\*\*hint\s*\d*\*\*", " ", text)
    text = re.sub(r"\bhint\s*\d+\s*[:.\-]?", " ", text)
    text = re.sub(r"\$\$\$|\$|\\leq|\\geq", " ", text)
    text = re.sub(r"\d+", " <num> ", text)
    text = re.sub(r"[^a-z0-9_<>=]+", " ", text)
    return re.sub(r"\s+", " ", text).strip()

def content_words(text: str) -> set:
    stop_words = {
        "the", "a", "an", "to", "of", "and", "or", "in", "on", "for", "with",
        "is", "are", "be", "can", "could", "you", "try", "consider", "think",
        "about", "when", "what", "how", "why", "this", "that", "it", "as",
        "by", "from", "into", "if", "then", "there", "such", "which",
        "calculate", "find", "number",
    }
    words = normalize_for_similarity(text).split()
    return {w for w in words if len(w) >= 2 and w not in stop_words}

def jaccard_similarity(a: str, b: str) -> float:
    wa = content_words(a)
    wb = content_words(b)
    if not wa or not wb:
        return 0.0
    return len(wa & wb) / len(wa | wb)

def most_similar_previous(new_hint: str, previous_hints: List[str]) -> Tuple[float, Optional[int], Optional[str]]:
    best_score = 0.0
    best_idx = None
    best_hint = None

    for i, old in enumerate(previous_hints, start=1):
        new_norm = normalize_for_similarity(new_hint)
        old_norm = normalize_for_similarity(old)

        if new_norm and old_norm and (new_norm in old_norm or old_norm in new_norm):
            score = 1.0
        else:
            score = jaccard_similarity(new_hint, old)

        if score > best_score:
            best_score = score
            best_idx = i
            best_hint = old

    return best_score, best_idx, best_hint

def should_stop(response: str) -> bool:
    lower = response.lower().strip()
    return any(marker in lower for marker in STOP_MARKERS)


# ============================================================
# Core Evaluator (WITH SEMANTIC RERANKING!)
# ============================================================
def evaluate_candidate(
    *,
    raw_output: str,
    decision: Dict[str, Any],
    valid_json: bool,
    previous_hints: List[str],
    args,
) -> Dict[str, Any]:
    
    action = decision.get("action", "hint")
    hint = str(decision.get("hint", "") or "")
    next_focus = str(decision.get("next_focus", "") or "")

    similarity = 0.0
    similar_idx = None
    similar_hint = None
    is_repeat = False

    if action == "hint" and hint and previous_hints:
        similarity, similar_idx, similar_hint = most_similar_previous(hint, previous_hints)
        
        # 只要相似度超过阈值，无情判定为重复
        if similarity > args.similarity_threshold:
            is_repeat = True

    if action == "stop":
        rank_score = float(args.stop_candidate_score)
        # 【去除了强制最小 Hint 的惩罚】，相信模型基于数据的自我判断
        if not valid_json:
            rank_score += 5.0
    else:
        rank_score = 0.0
        if not valid_json:
            rank_score += 5.0
        if not hint.strip():
            rank_score += 5.0
        if not next_focus.strip() or "unparsed" in next_focus.lower():
            rank_score += 0.35
        if is_generic_hint_text(hint):
            rank_score += 0.35

        # One candidate should contain exactly one visible hint.
        # If it tries to dump Hint 1/2/3 together, heavily penalize it.
        if count_hint_markers(hint) >= 2:
            rank_score += 8.0
            
        # 如果重复了，立刻判死刑（+100分）
        if is_repeat:
            rank_score += 100.0  

    return {
        "raw_output": raw_output,
        "valid_json": valid_json,
        "decision": decision,
        "action": action,
        "hint": hint,
        "next_focus": next_focus,
        "similarity": similarity,
        "similar_to_hint_index": similar_idx,
        "similar_to_hint": similar_hint,
        "is_repeat": is_repeat,
        "rank_score": rank_score,
    }

def choose_best_candidate(candidate_records: List[Dict[str, Any]]) -> Dict[str, Any]:
    if not candidate_records:
        return {
            "raw_output": "",
            "valid_json": False,
            "decision": {
                "action": "stop",
                "covered_so_far": [],
                "next_focus": "",
                "hint": FINAL_STOP_REPLY,
            },
            "action": "stop",
            "hint": FINAL_STOP_REPLY,
            "next_focus": "",
            "similarity": 0.0,
            "similar_to_hint_index": None,
            "similar_to_hint": None,
            "is_repeat": False,
            "rank_score": 999.0,
        }

    def sort_key(c: Dict[str, Any]):
        return (
            c["rank_score"],
            0 if c["valid_json"] else 1,
            0 if c["action"] == "hint" else 1,
        )

    return sorted(candidate_records, key=sort_key)[0]


# ============================================================
# Generation
# ============================================================

def generate_decisions(model, tokenizer, messages: List[Dict[str, str]], args) -> List[str]:
    text = chat_text(tokenizer, messages, add_generation_prompt=True)
    device = next(model.parameters()).device
    inputs = tokenizer([text], return_tensors="pt").to(device)
    prompt_len = inputs.input_ids.shape[1]

    total_candidates = max(1, int(args.num_candidates))
    chunk_size = max(1, min(int(args.candidate_batch_size), total_candidates))

    decoded: List[str] = []
    remaining = total_candidates

    while remaining > 0:
        batch_n = min(chunk_size, remaining)
        try:
            with torch.no_grad():
                outputs = model.generate(
                    **inputs,
                    max_new_tokens=args.max_new_tokens,
                    temperature=args.temperature,
                    top_p=args.top_p,
                    do_sample=True,
                    num_return_sequences=batch_n,
                    repetition_penalty=args.repetition_penalty,
                    pad_token_id=tokenizer.eos_token_id,
                    eos_token_id=tokenizer.eos_token_id,
                )
        except torch.cuda.OutOfMemoryError:
            if torch.cuda.is_available():
                torch.cuda.empty_cache()
            if batch_n == 1:
                raise
            chunk_size = 1
            continue

        decoded.extend(
            tokenizer.decode(out[prompt_len:], skip_special_tokens=True).strip()
            for out in outputs
        )
        del outputs
        remaining -= batch_n
        if torch.cuda.is_available():
            torch.cuda.empty_cache()

    return decoded

# ============================================================
# Main
# ============================================================

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--base_model_path", type=str, default=DEFAULT_BASE_MODEL_PATH)
    parser.add_argument("--lora_path", type=str, default=DEFAULT_NEXT_DECISION_LORA_PATH)
    parser.add_argument("--max_turns", type=int, default=12)
    parser.add_argument("--max_seq_length", type=int, default=4096)
    parser.add_argument("--max_new_tokens", type=int, default=512)
    parser.add_argument("--statement_budget", type=int, default=900)
    parser.add_argument("--editorial_budget", type=int, default=2200)
    
    # 恢复正常的稳定温度
    parser.add_argument("--temperature", type=float, default=0.7)
    parser.add_argument("--top_p", type=float, default=0.9)
    parser.add_argument("--repetition_penalty", type=float, default=1.05)
    
    parser.add_argument("--similarity_threshold", type=float, default=0.45) 
    parser.add_argument("--num_candidates", type=int, default=10) 
    parser.add_argument("--candidate_batch_size", type=int, default=10) 
    parser.add_argument("--stop_candidate_score", type=float, default=0.85)
    
    parser.add_argument("--problem_file", type=str, default=None)
    return parser.parse_args()


def main():
    args = parse_args()

    print("🔄 正在加载基础模型与 LoRA 权重...")
    print(f"📦 Base model: {args.base_model_path}")
    print(f"🎯 LoRA: {args.lora_path}")
    print(f"🎲 生成候选数量: {args.num_candidates}")
    print(f"🧠 语义查重(Semantic Rerank): ENABLED (阈值={args.similarity_threshold})")

    tokenizer = AutoTokenizer.from_pretrained(args.base_model_path, trust_remote_code=True)
    if tokenizer.pad_token is None:
        tokenizer.pad_token = tokenizer.eos_token

    base_model = AutoModelForCausalLM.from_pretrained(
        args.base_model_path,
        torch_dtype=torch.bfloat16,
        device_map="auto",
        trust_remote_code=True,
        attn_implementation="sdpa" 
    )

    base_model.config.use_cache = True  

    print("🔌 正在将 LoRA 权重直接合并入基础模型底座 (融合计算)...")
    model = PeftModel.from_pretrained(base_model, args.lora_path).merge_and_unload()
    model.eval()


    if args.problem_file:
        statement, editorial, problem_file = load_problem_json(args.problem_file)
    else:
        # 测试模式，随机读取
        import random
        import glob
        dataset_dir = "cf_scraper/cf_dataset/without_hint" 
        json_files = glob.glob(os.path.join(dataset_dir, "*.json"))
        if not json_files:
            print(f"❌ 找不到 JSON 文件: {dataset_dir}")
            return
        problem_file = random.choice(json_files)
        statement, editorial, _ = load_problem_json(problem_file)

    previous_hints: List[str] = []

    print("\n👨‍🏫 OI 导师已上线，开始生成 Hint...\n")

    for turn_idx in range(1, args.max_turns + 1):
        attempt = 1
        rejected_candidate = None
        rejection_reason = ""
        selected = None

        # 💥 重试自我反思循环
        while attempt <= 4:  
            messages = build_messages_under_budget(
                tokenizer=tokenizer,
                statement=statement,
                editorial=editorial,
                previous_hints=previous_hints,
                turn_idx=turn_idx,
                max_seq_length=args.max_seq_length,
                statement_budget=args.statement_budget,
                editorial_budget=args.editorial_budget,
                rejected_candidate=rejected_candidate,
                rejection_reason=rejection_reason,
            )

            raw_outputs = generate_decisions(model, tokenizer, messages, args)
            
            candidate_records: List[Dict[str, Any]] = []
            for raw in raw_outputs:
                obj = extract_json_object(raw)
                candidate_decision = normalize_decision(obj, raw, turn_idx)
                record = evaluate_candidate(
                    raw_output=raw,
                    decision=candidate_decision,
                    valid_json=obj is not None,
                    previous_hints=previous_hints,
                    args=args,
                )
                candidate_records.append(record)

            selected = choose_best_candidate(candidate_records)
            
            if not selected["is_repeat"] or attempt > 3:
                break
                
            print(f"⚠️ [尝试 {attempt}] 检测到输出与历史重复 (相似度={selected['similarity']:.2f})，启动自我反思机制...")
            rejected_candidate = selected["decision"]
            rejection_reason = f"The hint is too similar to previous Hint {selected['similar_to_hint_index']}. You MUST generate a completely DIFFERENT next_focus and hint."
            attempt += 1

        decision = selected["decision"]
        hint_text = sanitize_visible_hint(decision["hint"], turn_idx)
        decision["hint"] = hint_text

        if selected.get("is_repeat", False):
            print("\n🚨 警告：模型已陷入重复死循环，系统自动判定该题所有逻辑已讲完，触发停止！")
            break

        print(f"\n=================== 导师的第 {turn_idx} 个 Hint ===================")
        print(hint_text)
        print("========================================================\n")

        if decision["action"] == "stop" or should_stop(hint_text):
            print("🏁 导师判断核心逻辑已讲完，鼓励你自己去实现，交互结束。")
            break

        previous_hints.append(hint_text)

        user_input = input("你要继续索要下一个 Hint 吗？(按回车继续，输入 q 退出): ").strip().lower()
        if user_input == "q":
            break
    else:
        print(f"⚠️ 已达到最大回合数 {args.max_turns}，自动停止。")

if __name__ == "__main__":
    main()

import inspect
import os
import random
from collections import Counter
from datetime import datetime
from pathlib import Path

import torch
import wandb
from datasets import DatasetDict, load_dataset
from peft import PeftModel
from transformers import AutoModelForCausalLM, AutoTokenizer, TrainingArguments

try:
    from trl import DPOTrainer
except Exception as e:
    raise RuntimeError("TRL DPOTrainer is required. Please install/upgrade trl.") from e


BASE_DIR = Path(__file__).resolve().parent

MODEL_PATH = str(BASE_DIR / "qwen-coder-7b")
SFT_LORA_PATH = str(BASE_DIR / "cf_hint_lora_model_next_decision" / "final_best")
DPO_DATA_FILE = str(BASE_DIR / "multi_turn_next_hint_decision_dpo.jsonl")
OUTPUT_DIR = str(BASE_DIR / "cf_hint_lora_model_next_decision_dpo")

RUN_NAME = "Qwen-7B-CF-Hint-NextDecision-DPO-" + datetime.now().strftime("%m%d-%H%M")

MAX_LENGTH = int(os.environ.get("DPO_MAX_LENGTH", "4096"))
MAX_PROMPT_LENGTH = int(os.environ.get("DPO_MAX_PROMPT_LENGTH", "3200"))

TRAIN_BATCH_SIZE = int(os.environ.get("DPO_TRAIN_BATCH_SIZE", "1"))
EVAL_BATCH_SIZE = int(os.environ.get("DPO_EVAL_BATCH_SIZE", "1"))
GRAD_ACCUM_STEPS = int(os.environ.get("DPO_GRAD_ACCUM_STEPS", "8"))

LEARNING_RATE = float(os.environ.get("DPO_LEARNING_RATE", "5e-6"))
NUM_EPOCHS = float(os.environ.get("DPO_NUM_EPOCHS", "1"))
MAX_STEPS = int(os.environ.get("DPO_MAX_STEPS", "-1"))
BETA = float(os.environ.get("DPO_BETA", "0.1"))

EVAL_RATIO = float(os.environ.get("EVAL_RATIO", "0.1"))
SEED = int(os.environ.get("SEED", "42"))


def make_training_args(**kwargs):
    params = inspect.signature(TrainingArguments.__init__).parameters
    if "eval_strategy" in params and "evaluation_strategy" in kwargs:
        kwargs["eval_strategy"] = kwargs.pop("evaluation_strategy")
    return TrainingArguments(**kwargs)


def split_by_source_file(raw_dataset, eval_ratio: float = 0.1, seed: int = 42):
    if "source_file" not in raw_dataset.column_names:
        return raw_dataset.train_test_split(test_size=eval_ratio, seed=seed)

    all_sources = sorted(set(raw_dataset["source_file"]))
    random.Random(seed).shuffle(all_sources)

    eval_size = max(1, int(len(all_sources) * eval_ratio))
    eval_sources = set(all_sources[:eval_size])

    train_raw = raw_dataset.filter(lambda x: x["source_file"] not in eval_sources)
    eval_raw = raw_dataset.filter(lambda x: x["source_file"] in eval_sources)

    print(f"📚 DPO source split: train sources={len(all_sources) - len(eval_sources)}, eval sources={len(eval_sources)}")
    print(f"📚 DPO sample split: train samples={len(train_raw)}, eval samples={len(eval_raw)}")

    return DatasetDict({"train": train_raw, "test": eval_raw})


def count_column(dataset, col: str):
    if col not in dataset.column_names:
        return {}
    return dict(Counter(dataset[col]))


def build_dpo_trainer(model, tokenizer, train_dataset, eval_dataset, training_args):
    """Handle several TRL DPOTrainer signatures."""
    sig = inspect.signature(DPOTrainer.__init__).parameters

    kwargs = {
        "model": model,
        "args": training_args,
        "train_dataset": train_dataset,
        "eval_dataset": eval_dataset,
        "tokenizer": tokenizer,
    }

    if "ref_model" in sig:
        # Important for 32GB GPUs: avoid loading a second 7B reference model.
        # Newer TRL versions can use the same base model with PEFT adapter disabled.
        kwargs["ref_model"] = None

    if "beta" in sig:
        kwargs["beta"] = BETA
    if "max_length" in sig:
        kwargs["max_length"] = MAX_LENGTH
    if "max_prompt_length" in sig:
        kwargs["max_prompt_length"] = MAX_PROMPT_LENGTH
    if "generate_during_eval" in sig:
        kwargs["generate_during_eval"] = False

    return DPOTrainer(**kwargs)


def main():
    os.environ.setdefault("PYTORCH_CUDA_ALLOC_CONF", "expandable_segments:True")

    print("🚀 Run name:", RUN_NAME)
    print("📦 Base model:", MODEL_PATH)
    print("🎯 SFT LoRA:", SFT_LORA_PATH)
    print("📄 DPO data:", DPO_DATA_FILE)
    print("💾 Output dir:", OUTPUT_DIR)
    print("📏 MAX_LENGTH:", MAX_LENGTH)
    print("📏 MAX_PROMPT_LENGTH:", MAX_PROMPT_LENGTH)
    print("🧪 DPO_MAX_STEPS:", MAX_STEPS)
    print("β DPO_BETA:", BETA)

    wandb.init(project="Tsinghua-DL-Hint-Generator", name=RUN_NAME)

    tokenizer = AutoTokenizer.from_pretrained(MODEL_PATH, trust_remote_code=True)
    if tokenizer.pad_token is None:
        tokenizer.pad_token = tokenizer.eos_token
    tokenizer.padding_side = "right"

    base_model = AutoModelForCausalLM.from_pretrained(
        MODEL_PATH,
        torch_dtype=torch.bfloat16,
        device_map="auto",
        trust_remote_code=True,
    )
    base_model.config.use_cache = False

    model = PeftModel.from_pretrained(base_model, SFT_LORA_PATH, is_trainable=True)
    try:
        model.enable_input_require_grads()
    except Exception:
        pass

    raw_dataset = load_dataset("json", data_files=DPO_DATA_FILE, split="train")
    print(f"📊 Raw DPO pairs: {len(raw_dataset)}")
    if "task_type" in raw_dataset.column_names:
        print("📊 DPO task_type distribution:", count_column(raw_dataset, "task_type"))

    if "token_len" in raw_dataset.column_names:
        before = len(raw_dataset)
        raw_dataset = raw_dataset.filter(lambda x: x["token_len"] <= MAX_LENGTH)
        print(f"🧹 Length filter: {before} -> {len(raw_dataset)}")

    dataset = split_by_source_file(raw_dataset, eval_ratio=EVAL_RATIO, seed=SEED)

    keep_cols = {"prompt", "chosen", "rejected"}
    train_dataset = dataset["train"].remove_columns([c for c in dataset["train"].column_names if c not in keep_cols])
    eval_dataset = dataset["test"].remove_columns([c for c in dataset["test"].column_names if c not in keep_cols])

    training_args = make_training_args(
        output_dir=OUTPUT_DIR,
        run_name=RUN_NAME,
        per_device_train_batch_size=TRAIN_BATCH_SIZE,
        per_device_eval_batch_size=EVAL_BATCH_SIZE,
        gradient_accumulation_steps=GRAD_ACCUM_STEPS,
        learning_rate=LEARNING_RATE,
        num_train_epochs=NUM_EPOCHS,
        max_steps=MAX_STEPS,
        weight_decay=0.0,
        warmup_ratio=0.03,
        lr_scheduler_type="cosine",
        logging_steps=5,
        logging_first_step=True,
        evaluation_strategy="steps",
        eval_steps=50,
        save_strategy="steps",
        save_steps=50,
        save_total_limit=3,
        load_best_model_at_end=False,
        bf16=True,
        fp16=False,
        optim="paged_adamw_8bit",
        gradient_checkpointing=True,
        gradient_checkpointing_kwargs={"use_reentrant": False},
        report_to=["wandb"],
    )

    trainer = build_dpo_trainer(
        model=model,
        tokenizer=tokenizer,
        train_dataset=train_dataset,
        eval_dataset=eval_dataset,
        training_args=training_args,
    )

    print("🚀 开始 DPO 训练！")
    trainer.train()

    final_dir = Path(OUTPUT_DIR) / "final_best"
    trainer.model.save_pretrained(final_dir)
    tokenizer.save_pretrained(final_dir)

    print(f"✅ 已保存 DPO 后的 LoRA 权重到: {final_dir}")
    wandb.finish()


if __name__ == "__main__":
    main()

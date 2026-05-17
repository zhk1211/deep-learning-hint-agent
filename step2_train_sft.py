import os
from datetime import datetime
from pathlib import Path

import torch
import wandb
from datasets import load_dataset
from peft import LoraConfig, get_peft_model
from transformers import (
    AutoModelForCausalLM,
    AutoTokenizer,
    EarlyStoppingCallback,
    TrainingArguments,
)
from trl import DataCollatorForCompletionOnlyLM, SFTTrainer


# =========================
# Configuration
# =========================

BASE_DIR = Path(__file__).resolve().parent

MODEL_PATH = str(BASE_DIR / "qwen-coder-7b")
DATA_FILE = str(BASE_DIR / "multi_turn_hints.jsonl")

OUTPUT_DIR = str(BASE_DIR / "cf_hint_lora_model_budgeted")

RUN_NAME = "Qwen-7B-CF-Style-Budgeted-" + datetime.now().strftime("%m%d-%H%M")

MAX_SEQ_LENGTH = int(os.environ.get("MAX_SEQ_LENGTH", "4096"))

TRAIN_BATCH_SIZE = int(os.environ.get("TRAIN_BATCH_SIZE", "1"))
EVAL_BATCH_SIZE = int(os.environ.get("EVAL_BATCH_SIZE", "1"))
GRAD_ACCUM_STEPS = int(os.environ.get("GRAD_ACCUM_STEPS", "8"))

LEARNING_RATE = float(os.environ.get("LEARNING_RATE", "1e-4"))
NUM_EPOCHS = float(os.environ.get("NUM_EPOCHS", "3"))

LORA_R = int(os.environ.get("LORA_R", "8"))
LORA_ALPHA = int(os.environ.get("LORA_ALPHA", "16"))
LORA_DROPOUT = float(os.environ.get("LORA_DROPOUT", "0.10"))

TARGET_MODULES = ["q_proj", "k_proj", "v_proj", "o_proj"]


def choose_optimizer() -> str:
    try:
        import bitsandbytes  # noqa: F401
        print("✅ bitsandbytes detected; using paged_adamw_8bit.")
        return "paged_adamw_8bit"
    except Exception:
        print("⚠️ bitsandbytes not found; fallback to adamw_torch. This may use more GPU memory.")
        return "adamw_torch"


def main():
    os.environ.setdefault("PYTORCH_CUDA_ALLOC_CONF", "expandable_segments:True")

    print("🚀 Run name:", RUN_NAME)
    print("📦 Model path:", MODEL_PATH)
    print("📄 Data file:", DATA_FILE)
    print("💾 Output dir:", OUTPUT_DIR)
    print("📏 MAX_SEQ_LENGTH:", MAX_SEQ_LENGTH)

    wandb.init(
        project="Tsinghua-DL-Hint-Generator",
        name=RUN_NAME,
    )

    # =========================
    # Tokenizer and model
    # =========================

    print("Loading tokenizer...")
    tokenizer = AutoTokenizer.from_pretrained(MODEL_PATH, trust_remote_code=True)

    if tokenizer.pad_token is None:
        tokenizer.pad_token = tokenizer.eos_token

    tokenizer.padding_side = "right"

    print("Loading model...")
    model = AutoModelForCausalLM.from_pretrained(
        MODEL_PATH,
        torch_dtype=torch.bfloat16,
        device_map="auto",
        trust_remote_code=True,
    )

    # Important for training with gradient checkpointing.
    model.config.use_cache = False

    # =========================
    # LoRA
    # =========================

    lora_config = LoraConfig(
        r=LORA_R,
        lora_alpha=LORA_ALPHA,
        target_modules=TARGET_MODULES,
        lora_dropout=LORA_DROPOUT,
        bias="none",
        task_type="CAUSAL_LM",
    )

    model = get_peft_model(model, lora_config)

    # Helps PEFT + gradient checkpointing in some environments.
    try:
        model.enable_input_require_grads()
    except Exception:
        pass

    model.print_trainable_parameters()

    # =========================
    # Dataset
    # =========================

    print("Loading dataset...")
    raw_dataset = load_dataset("json", data_files=DATA_FILE, split="train")

    # Safety filter: step1 already enforces token_len <= MAX_SEQ_LENGTH, but keep this guard.
    if "token_len" in raw_dataset.column_names:
        before = len(raw_dataset)
        raw_dataset = raw_dataset.filter(lambda x: x["token_len"] <= MAX_SEQ_LENGTH)
        print(f"🧹 Length filter: {before} -> {len(raw_dataset)}")

    dataset = raw_dataset.train_test_split(test_size=0.1, seed=42)

    def format_chat_template(example):
        text = tokenizer.apply_chat_template(
            example["messages"],
            tokenize=False,
            add_generation_prompt=False,
        )
        example["text"] = text
        return example

    train_dataset = dataset["train"].map(format_chat_template)
    eval_dataset = dataset["test"].map(format_chat_template)

    # Keep only text to avoid Trainer column confusion.
    train_dataset = train_dataset.remove_columns([c for c in train_dataset.column_names if c != "text"])
    eval_dataset = eval_dataset.remove_columns([c for c in eval_dataset.column_names if c != "text"])

    print(f"✅ Train samples: {len(train_dataset)}")
    print(f"✅ Eval samples: {len(eval_dataset)}")

    # =========================
    # Data collator
    # =========================

    instruction_template = "<|im_start|>user\n"
    response_template = "<|im_start|>assistant\n"

    collator = DataCollatorForCompletionOnlyLM(
        instruction_template=instruction_template,
        response_template=response_template,
        tokenizer=tokenizer,
    )

    # =========================
    # Training args
    # =========================

    training_args = TrainingArguments(
        output_dir=OUTPUT_DIR,
        run_name=RUN_NAME,

        per_device_train_batch_size=TRAIN_BATCH_SIZE,
        per_device_eval_batch_size=EVAL_BATCH_SIZE,
        gradient_accumulation_steps=GRAD_ACCUM_STEPS,

        learning_rate=LEARNING_RATE,
        num_train_epochs=NUM_EPOCHS,
        weight_decay=0.01,
        warmup_ratio=0.03,
        lr_scheduler_type="cosine",

        logging_steps=5,
        logging_first_step=True,

        evaluation_strategy="steps",
        eval_steps=50,

        save_strategy="steps",
        save_steps=50,
        save_total_limit=3,

        load_best_model_at_end=True,
        metric_for_best_model="eval_loss",
        greater_is_better=False,

        bf16=True,
        fp16=False,

        optim=choose_optimizer(),

        gradient_checkpointing=True,
        gradient_checkpointing_kwargs={"use_reentrant": False},

        group_by_length=True,

        report_to=["wandb"],
    )

    # =========================
    # Trainer
    # =========================

    trainer = SFTTrainer(
        model=model,
        tokenizer=tokenizer,
        train_dataset=train_dataset,
        eval_dataset=eval_dataset,
        dataset_text_field="text",
        data_collator=collator,
        max_seq_length=MAX_SEQ_LENGTH,
        args=training_args,
        callbacks=[EarlyStoppingCallback(early_stopping_patience=3)],
    )

    print("🚀 开始 SFT 训练！")
    trainer.train()

    final_dir = Path(OUTPUT_DIR) / "final_best"
    trainer.model.save_pretrained(final_dir)
    tokenizer.save_pretrained(final_dir)

    print(f"✅ 已保存最佳 LoRA 权重到: {final_dir}")
    wandb.finish()


if __name__ == "__main__":
    main()

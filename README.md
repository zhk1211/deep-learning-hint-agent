# Deep Learning Hint Agent

A Qwen-Coder-7B SFT agent for progressive Codeforces-style hint generation.

This project focuses on learning how to generate competitive-programming hints progressively. Instead of directly producing a full solution or code, the model learns to decide the next useful Codeforces-style hint given the problem statement, the official editorial, and the hints that have already been shown.

## Overview

The goal of this project is to build a learned agent for progressive Codeforces-style hint generation.

We originally tried to model the whole hint conversation as a complete sequence. However, this made the model weak at deciding how many hints a problem actually needs. The model tended to learn a statistical average hint length rather than adaptively stopping based on the complexity of the current problem.

To address this, we reformulate hint generation as a **next-hint decision task**.

At each turn, the model receives:

```text
problem statement + official editorial + existing k hints
```

and independently decides one of two actions:

```text
1. generate the next hint;
2. stop the hint-generation process.
```

This means every hint is treated as a relatively independent decision conditioned on the current hint state. The current hint state is defined by the hints already given to the user, not by the full editorial alone.

The expected behavior is:

```text
Hint 1: reveal the most preliminary useful observation.
Hint 2: advance one small step beyond Hint 1.
Hint 3: advance one small step beyond the previous hints.
...
Stop: stop when the remaining gap is mostly implementation work or when further hints would be repetitive.
```

## Data Collection

The data used in this project is collected from Codeforces.

For each Codeforces problem, we try to obtain:

```text
problem statement
official tutorial / editorial
Codeforces-style hints
```

During data collection, we encountered several practical issues.

### Cloudflare and Access Control

Directly scraping Codeforces pages often triggers Cloudflare checks. After testing different types of Codeforces pages, we found that **contest pages** and **editorial blog pages** are relatively less strictly protected, while **problem statement pages** are much more aggressively protected.

Therefore, our scraper uses different sources for different parts of the data:

```text
Codeforces contest page:
  extract problem links and problem names

Codeforces editorial / blog page:
  extract official tutorial sections

Luogu mirror page:
  extract Codeforces problem statements
```

This design avoids repeatedly accessing the most strictly protected Codeforces problem-statement pages. For contest pages and blog pages, the scraper still uses random suitable waiting intervals between requests to reduce the chance of triggering Cloudflare checks.

In short, the data collection strategy is:

```text
use Codeforces for contest metadata and editorials
use Luogu mirror pages for problem statements
use conservative request intervals throughout the scraping process
```

### Tutorial Loading Problem

Another common issue is that some tutorial pages may temporarily return:

```text
[Tutorial is loading...]
```

In this case, the fetched content is not the real editorial. The scraper therefore needs to retry until the tutorial content is actually loaded.

This retry mechanism is important because otherwise the dataset would contain empty or invalid editorials.

### Dataset Organization

The collected files are organized into folders such as:

```text
cf_scraper/cf_dataset/with_hint/
cf_scraper/cf_dataset/without_hint/
```

A typical JSON file contains:

```json
{
  "statement": [
    {
      "title": "Statement",
      "content": "..."
    }
  ],
  "hints": [
    {
      "content": "..."
    }
  ],
  "solutions": [
    {
      "title": "Solution",
      "content": "..."
    }
  ]
}
```

The hint model uses:

```text
problem statement + official editorial + previous hints
```

The evaluation solver uses:

```text
problem statement + first k generated hints
```

The solver does not see the official editorial except in the separate `Solution` upper-bound condition.

## Core Formulation

For a problem with a full hint sequence:

```text
H1, H2, H3, ..., Hm
```

we construct multi-turn next-hint training examples:

```text
Input:  problem statement + editorial + no previous hints
Output: H1
```

```text
Input:  problem statement + editorial + H1
Output: H2
```

```text
Input:  problem statement + editorial + H1 + H2
Output: H3
```

and so on.

The final turn is a stop decision:

```text
Input:  problem statement + editorial + H1 + H2 + ... + Hm
Output: stop
```

Thus, the model learns a conditional next-action policy:

```text
next action = f(problem statement, editorial, previous hints)
```

where the next action can be either a new hint or a stopping decision.

## Main Challenges

### Deciding When to Stop

When the model is trained on complete hint conversations, it may learn the average number of hints in the training set instead of learning when the problem has already been sufficiently explained.

This causes two failure modes:

- stopping too early before the core idea is explained;
- continuing too long and asking low-quality or unnecessary questions.

We therefore train the model to make a local next-action decision at every turn.

### Avoiding Semantic Repetition

Another major issue is **semantic repetition**, also called **semantic near-duplication**. Even when the model is explicitly told what hints have already been given, it may generate a new hint that is semantically very close to a previous one.

For example, after already saying:

```text
Try to solve a single query first.
```

the model may later say:

```text
Can you think about how to answer one query?
```

Although the wording is different, the idea is almost the same. This is a semantic near-duplicate and should not be selected as the next hint.

## Improvements

We use three main improvements to make the hint-generation process more robust.

### 1. Expanded Training Data

The original SFT data contained only raw next-hint decision examples.

We extend the training data with targeted synthetic cases:

```text
raw_hint_decision
synthetic_early_stop_rejection
synthetic_repeat_rejection
```

The purpose is:

| Data type | Purpose |
|---|---|
| `raw_hint_decision` | Learn the normal Codeforces-style next-hint decision process. |
| `synthetic_early_stop_rejection` | Reduce premature stopping when useful hints still remain. |
| `synthetic_repeat_rejection` | Reduce semantic repetition and near-duplicate hints. |

This makes the model more aware of two important negative cases:

```text
1. stopping too early;
2. repeating an already covered idea.
```

### 2. Candidate Generation and Reranking

At inference time, each hint step does not rely on a single generated output.

Instead, we generate multiple candidates, usually:

```text
10 candidate decisions
```

Then we score them and select the best one.

The reranking logic includes:

| Case | Treatment |
|---|---|
| `stop` decision | Apply a moderate penalty, so the model can still stop, but not too aggressively. |
| Exact repetition | Apply a veto-level penalty. |
| Semantic similarity | Apply a partial penalty based on similarity to previous hints. |
| Length | Apply a small length-related penalty to avoid overly verbose hints. |

The semantic similarity penalty uses a Jaccard-style similarity function over normalized hint content. This helps filter out hints that are not exactly identical but are still semantically too close to previous hints.

The selected candidate is the one with the best final score.

### 3. Shorter SFT Training

We also found that training until full evaluation-loss convergence was not always beneficial.

At first, we trained the model until the training/evaluation loss had almost converged. In practice, this usually required roughly $800–1200$ epochs.

However, we later found that a lower loss did not necessarily lead to better hint-generation behavior. After long training, the model became more deterministic, the generated language became less diverse, and semantic repetition became more common.

We therefore changed the training schedule to a shorter range: $150–250$ epochs.

Empirically, this keeps more linguistic diversity and makes the model less likely to repeat semantically similar hints.

<!-- ## Main Features

- Fine-tunes Qwen-Coder-7B with LoRA/SFT for next-hint generation.
- Treats each hint turn as an independent next-action decision.
- Conditions each decision on the problem statement, official editorial, and previous hints.
- Supports both hint generation and stop decisions.
- Uses synthetic rejection data for early-stop and repeat-hint failure cases.
- Uses multi-candidate generation and reranking at inference time.
- Reduces repeated hints through exact-duplicate and semantic-similarity penalties.
- Supports interactive hint inference.
- Provides DeepSeek V4-Pro based evaluation scripts to test whether generated hints help an external solver.
- Provides analysis scripts for plotting hint-progress accuracy curves.
- Includes utilities for cleaning official-solution prompts when editorials contain copied code. -->

## Repository Structure

```text
deep-learning-hint-agent/
├── step1_prepare_data.py
├── step2_train_sft.py
├── step3_inference.py
├── deepseek_cf1700_eval.py
├── analyze_hint_eval.py
├── fix_solution_include_and_regenerate.py
├── cf_difficulty_1700_prefixes.txt
├── cf_scraper/
│   └── cf_dataset/
│       ├── with_hint/
│       └── without_hint/
├── cf_hint_lora_model_next_decision/
│   └── final_best/
└── README.md
```

Important scripts:

| File | Purpose |
|---|---|
| `step1_prepare_data.py` | Prepare multi-turn next-hint SFT data from Codeforces problem files. |
| `step2_train_sft.py` | Fine-tune Qwen-Coder-7B with LoRA/SFT. |
| `step3_inference.py` | Interactively generate progressive hints using the trained model. |
| `deepseek_cf1700_eval.py` | Evaluate generated hints with an external DeepSeek solver. |
| `analyze_hint_eval.py` | Analyze manual AC results and generate plots. |
| `fix_solution_include_and_regenerate.py` | Remove copied code from solution prompts and regenerate solution-condition code. |

## Model

The hint generator is based on:

```text
Qwen-Coder-7B + LoRA SFT
```

The project does not train a language model from scratch. It fine-tunes a pretrained code LLM to learn a next-hint decision policy.

A typical LoRA adapter directory is:

```text
cf_hint_lora_model_next_decision/final_best/
```

The base model directory is usually not included in the repository because it is large:

```text
qwen-coder-7b/
```

## Training Objective

The SFT task is to imitate Codeforces-style next-hint decisions.

Each training sample asks the model to output JSON with either:

```json
{
  "action": "hint",
  "covered_so_far": ["short summary of previous hints"],
  "next_focus": "the next preliminary and critical observation",
  "hint": "**Hint k**\n..."
}
```

or:

```json
{
  "action": "stop",
  "covered_so_far": ["short summary of previous hints"],
  "next_focus": "",
  "hint": "No. The rest you need to think about yourself."
}
```

The model is expected to avoid repeating previous hints and to generate only the next smallest useful observation.

If using an AutoDL or CUDA environment:

```bash
source /root/autodl-tmp/zhk1211/envs/torch-cu128/bin/activate
```

## Interactive Hint Generation

Run:

```bash
python3 step3_inference.py \
  --base_model_path ./qwen-coder-7b \
  --lora_path ./cf_hint_lora_model_next_decision/final_best \
  --problem_file "./cf_scraper/cf_dataset/with_hint/CF1777C Quiz Master.json" \
  --num_candidates 10
```

Example output:

```text
=================== Mentor Hint 1 ===================
**Hint 1**
Try to identify which property must hold for every number in the chosen range.
=====================================================

Do you want the next hint? Press Enter to continue, or input q to quit:
```

The script keeps track of all previous hints and asks the model to decide the next action at each turn.

## Evaluation

The main project task is hint generation. Evaluation is used to test whether the generated hints are actually useful.

Due to limited resources, we use a controlled evaluation strategy: **fixed Codeforces difficulty + fixed external solver model**, then test how the solver's correctness changes as the number of provided hints increases.

### Choice of Solver Model and Difficulty

We tested several models with free token quotas, but their competitive-programming ability was not strong enough for this evaluation setting.

We finally selected **DeepSeek V4-Pro** as the external solver model. Empirically, we found that DeepSeek V4-Pro can usually solve Codeforces difficulty **1700** problems when given the full official editorial, while its correctness is much lower when given only the problem statement.

This setting gives a useful evaluation range:

```text
statement only: not too easy
full editorial: usually solvable
generated hints: measurable intermediate guidance
```

### Evaluation Data Construction

For a problem with `k` generated hints:

```text
H1, H2, ..., Hk
```

we construct `k + 2` evaluation segments:

```text
Hint0      : statement only
Hint1      : statement + H1
Hint2      : statement + H1 + H2
...
Hintk      : statement + H1 + H2 + ... + Hk
Solution   : statement + cleaned official editorial
```

The last segment is the `Solution` segment. Before using it, we clean the official editorial to remove copied code, as described in the previous section.

Thus, the evaluation measures whether the solver's accepted-code rate increases as it receives more generated hints.

### Evaluation Setup

Run a full evaluation:

```bash
python3 deepseek_cf1700_eval.py \
  --prefix_file cf_difficulty_1700_prefixes.txt \
  --output_dir solver_outputs_cf1700 \
  --solver_model deepseek-v4-pro \
  --mode both \
  --samples_per_condition 1 \
  --include_solution_condition \
  --hint_source auto \
  --base_model_path ./qwen-coder-7b \
  --lora_path ./cf_hint_lora_model_next_decision/final_best \
  --num_candidates 5 \
  --candidate_batch_size 1 \
  --parallel_solver_workers 6 \
  --overwrite
```

Important options:

| Option | Meaning |
|---|---|
| `--hint_source auto` | Use the trained local hint model to generate hints. |
| `--hint_source existing` | Use existing hints from the JSON files as a baseline. |
| `--hint_source none` | Do not provide hints. |
| `--include_solution_condition` | Add a `Solution` condition using the official editorial. |
| `--only_extremes` | Only test `Hint0`, `HintAll`, and optionally `Solution`. |
| `--parallel_solver_workers` | Number of parallel DeepSeek API calls. |

### Evaluation Output

For each problem, the evaluation script creates a folder:

```text
solver_outputs_cf1700/CF1777C/
```

Example files:

```text
CF1777C_Hint0.prompt.txt
CF1777C_Hint0.cpp
CF1777C_Hint1.prompt.txt
CF1777C_Hint1.cpp
CF1777C_Hint2.prompt.txt
CF1777C_Hint2.cpp
CF1777C_Solution.prompt.txt
CF1777C_Solution.cpp
CF1777C_generated_hints.json
CF1777C_manifest.json
```

The generated `.cpp` files can be manually submitted to Codeforces to record whether they are accepted.


## Cleaning Solution Prompts

Some Codeforces editorials contain full source code. If we directly give such editorials to the solver in the `Solution` condition, the solver may simply copy the code instead of solving from the explanation. This makes the `Solution` condition less meaningful as an upper-bound reasoning baseline.

Therefore, we clean the `Solution` prompts before using them in evaluation.

Use:

```bash
python3 fix_solution_include_and_regenerate.py \
  --root solver_outputs_cf1700_test \
  --model deepseek-v4-pro \
  --parallel_workers 6 \
  --overwrite
```

This script removes everything from the first `#include` in the official-editorial section to the end of that section, replaces it with:

```text
[code block omitted]
```

and regenerates `*_Solution.cpp` from the cleaned prompt.

### Result Analysis

After manually recording AC results, use a text file such as:

```text
CF1798C 111111
CF1792D 0011111
CF1777C 0111001
CF1765D 0000001
```

Each bit string means:

```text
first bit    = Hint0 / statement only
middle bits  = first k generated hints
last bit     = cleaned official solution/editorial condition
```

Run:

```bash
python3 analyze_hint_eval.py \
  --input eval_result.txt \
  --output_dir hint_eval_analysis
```

The script generates:

```text
hint_eval_analysis/
├── parsed_records.csv
├── percentage_bins_hint_only.csv
├── stage_summary.csv
├── length_summary.csv
├── skipped_problems.txt
├── bad_lines.txt
├── summary.json
├── percentage_accuracy_curve.png
├── stage_accuracy_bar.png
└── length_distribution.png
```

The percentage curve analyzes only the hint process:

```text
Hint0 → Hint1 → ... → HintAll
```

The official-solution condition is plotted separately as an upper-bound reference point.

### Current Statistical results(To be continued)

![](https://cdn.luogu.com.cn/upload/image_hosting/main7280.png)

## Notes

This repository studies progressive hint generation, not direct code generation.

The external solver model is only used to evaluate whether the generated hints are helpful. It is not part of the trained hint-generation model.

The current evaluation workflow still relies on manual Codeforces submission to determine whether generated code is accepted.

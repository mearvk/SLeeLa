# Code LLMs (procedural programming)

SLeeLa supports a **Code LLM** model type — a language model specialized for
source code (a.k.a. coding assistant / program-aided language model). Because
code is structured plain text, the model handles programming syntax much like
natural language and is effective at the sequential logic, state changes, loops,
and blocks of **procedural** languages (C, Fortran, Pascal, procedural Python).

This is the `code-language-model` entry in `models/architecture-catalog.json`,
made functional. As with every model here, **no weights and no model are
bundled** — the Code LLM is user-owned.

## What a Code LLM does here

| Task (`sllmcx_task`) | Meaning |
|---|---|
| `summarize` | Explain code in plain language, line by line |
| `dry-run` | Mentally execute; predict how variables/state change through loops and conditionals |
| `refactor` | Restructure monolithic code into clean, reusable procedures |
| `translate` | Port to another language/paradigm, preserving behavior |
| `debug` | Locate and explain a fault |
| `complete` | Complete/continue code (FIM-capable models) |

## Neural proposes, verifier decides (the honest part)

A Code LLM's claim about behavior is only as sound as the check behind it. The
model proposes; a **code verifier** supplies soundness. The strongest check the
caller can provide is declared as `check_mode`:

| `check_mode` | Soundness |
|---|---|
| `none` | No checker — behavior claims stay **UNKNOWN** (unverified) |
| `compile` | Syntactic/type correctness via a compiler |
| `test` | Behavior against tests / expected output |
| `trace` | Execute and compare the traced state — the dry-run truth |

In the reasoning layer, `SLLMR_SUBJECT_CODE` is treated like `MATH`: a
load-bearing `DEDUCTION` must be `SUPPORTED` for a `verified` result when
`require_verified` is set, and a `REFUTED` dry-run forces a **backtrack**. The
`reasoning/` demo shows this: a loop sum proposed as 10 is refuted by the trace
verifier ("adds 0..3, not 1..4"), the engine backtracks, and commits the
verified answer `sum == 6`.

## Declaring a Code LLM

A user declares a `sllmcx_profile` (C) / `CodeModelProfile` (C++), or a
`models/code-model*.jsonl` record validated by `models/code-model.schema.json`:

```c
sllmcx_profile p; sllmcx_profile_init(&p);
p.model_id = "user/c-assistant";
p.base_family = "code-llama";          /* informational */
p.primary_language = SLLMCX_LANG_C;
p.tasks = SLLMCX_TASK_SUMMARIZE | SLLMCX_TASK_DRY_RUN | SLLMCX_TASK_REFACTOR;
p.check_mode = SLLMCX_CHECK_COMPILE;   /* the user's compiler verifies steps */
```

It composes with the **capacity profile** (`sllmc_*`, 2–16 GB envelope) for the
reasoning budget, and with `SLLMR_SUBJECT_CODE` for the procedural-code persona.

## Files

| File | Role |
|---|---|
| `large_language_models_code.h/.c` | C11 Code LLM profile + task/language/check-mode enums (`sllmcx_*`, ABI v1) |
| `large_language_models_code.hpp` | C++17 `CodeModelProfile` helper |
| `models/code-model.schema.json` | JSON Schema for user-owned code-model records |
| `models/code-models.example.jsonl` | Example profiles: C, Fortran, Python, SLeeLa, Pascal |
| `tools/code_model_selftest.c` | Native self-test |

## What is NOT bundled

No code model, no weights, no compiler, no test runner. SLeeLa supplies the
model *type*, the reasoning policy, and the verifier *contract*; the user
supplies the actual Code LLM (within the 2–16 GB envelope) and the compiler /
tests / tracer that give its claims soundness.

# Reasoning demo

An end-to-end demonstration of the Reasoning Language Model layer
(`../large_language_models_reasoning.h`). See `../REASONING.md` for the design.

`reasoning_demo.c` wires two illustrative pieces — **no trained model, no
network, no weights** — to the reasoning engine:

- a **deterministic stub backend** (`sllm_backend`) that emits tagged chain-of-
  thought steps, deliberately proposing a wrong math deduction first; and
- a **tiny symbolic verifier** (`sllmr_verifier`) that knows a couple of ground
  facts and refutes/supports steps, returning `UNKNOWN` for open questions.

## Build and run

```sh
cc -std=c11 -O2 -Wall -Wextra -pedantic \
   ../large_language_models.c \
   ../large_language_models_reasoning.c \
   reasoning_demo.c -o reasoning_demo
./reasoning_demo
```

## What it shows

1. **Mathematics (verified, with backtrack).** The stub proposes "360 degrees";
   the verifier **refutes** it; the engine **backtracks**, proves "180 degrees"
   (SUPPORTED), and commits a **verified** conclusion. Run with
   `require_verified=1`, so an unsound path would fail rather than be accepted.
2. **Philosophy (nuanced, tentative).** The engine decomposes the question,
   surfaces an objection, and commits an honest, **tentative** position with its
   caveat intact — no proof-claim on a contestable question.

The demo is a schema/behavior fixture for the control loop, not a trained model.
To reason for real, implement `sllm_backend` against an actual model and supply
a `sllmr_verifier` (for math/logic, a bridge to `lib/deliberation`).

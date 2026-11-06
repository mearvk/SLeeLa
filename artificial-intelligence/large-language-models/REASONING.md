# Reasoning Language Model (RLM) layer

This document is the SLeeLa project's position on "can an LLM reason?", and the
contract of the reasoning layer that implements that position. It is normative
for the `sllmr_*` API.

## What reasoning is here (and is not)

A plain language model predicts the next token. "Reasoning" is **not a change to
the vocabulary** and **not a new kind of symbol** — the same token-ID table (see
`tokenizers/base-vocabulary.json`) is used whether the model chats or deduces.
What produces reasoning behavior is:

1. **A control procedure at inference time** — decompose the problem, emit an
   explicit chain of thought, reflect on and critique each step, and spend more
   compute on harder problems (the "test-time compute" / chain-of-thought
   effect). This layer owns that procedure.
2. **An external verifier** — the honest, reliable gains of a reasoning model
   come when step proposals are **checked** by something that can refute them,
   and the model **backtracks** on refutation. The language model supplies
   *proposals and plausibility*; the verifier supplies *soundness*.

This mirrors the current research consensus: chain-of-thought plus
reinforcement/verification turns a predictor into an effective problem-solver,
**but** the model has no built-in world model, can hallucinate fluent-but-wrong
logic, and costs more compute per answer. The layer is designed around those
limits rather than denying them.

### The neuro-symbolic split

| Half | Role | Where |
|---|---|---|
| Neural (LLM) | Propose steps, phrase nuance, generalize | caller's `sllm_backend` |
| Symbolic (verifier) | Refute/support steps, enforce soundness | caller's `sllmr_verifier` |

For deductive subjects (math), supply a **symbolic verifier** — e.g. a numeric
checker or a bridge to SLeeLa's `lib/deliberation` search/logic classes — and
set `require_verified`. For open subjects (philosophy, religion) a verifier
usually returns `UNKNOWN`, and the engine reports an honest, **tentative**
position rather than claiming proof.

## The control loop

For each candidate step the engine:

1. prompts the backend for the next **tagged** step (DECOMPOSE / ASSUMPTION /
   DEDUCTION / INDUCTION / OBJECTION / REFLECTION / REVISION / CONCLUSION);
2. inserts a **REFLECTION** every `reflection_every` steps to force self-critique;
3. **verifies** the step (`SUPPORTED` / `REFUTED` / `UNKNOWN`);
4. on `REFUTED`, **backtracks**: abandons the path, feeds the refutation back so
   the backend does not repeat it, and retries at greater depth (up to
   `max_backtracks`);
5. on `CONCLUSION`, commits the path if it satisfies `require_verified`.

The best path is kept even when no fully sound path is found; the result then
reports `tentative` with the honest confidence and `SLLMR_NO_SOUND_PATH`.

### Honesty rules (enforced, not advisory)

- `UNKNOWN` is never promoted to "verified". A verifier that cannot decide does
  not lower a step's standing dishonestly, nor raise it.
- For `SLLMR_SUBJECT_MATH`, a load-bearing `DEDUCTION` that is not `SUPPORTED`
  blocks a "verified" result when `require_verified` is set.
- For `SLLMR_SUBJECT_PHILOSOPHY` / `SLLMR_SUBJECT_RELIGION`, confidence is capped
  and results are reported `tentative`: the layer makes **no proof-claim** on
  contestable or interpretive questions, and the religion persona attributes
  claims to specific traditions/texts and separates faith from empirical claims.

## Subjects

`sllmr_subject` selects a controller **policy** (reflection depth, verifier
trust, confidence reporting, persona) over the *same* backend — not a different
model or vocabulary:

- `MATH` — seeks proof; expects a symbolic/numeric verifier.
- `PHILOSOPHY` — dialectic: surface assumptions, steelman the strongest
  objection, prefer a nuanced position to a slogan.
- `RELIGION` — interpretive: attribute to traditions, make no proof-claim.
- `GENERAL` — balanced default.

## Files

| File | Role |
|---|---|
| `large_language_models_reasoning.h/.c` | C11 reasoning controller (`sllmr_*`, ABI v1) |
| `large_language_models_reasoning.hpp/.cpp` | C++17 RAII facade (`sleela::ai::ReasoningModel`) with `std::function` verifier/observer |
| `reasoning/reasoning_demo.c` | End-to-end demo: a stub backend + tiny symbolic verifier run a math question (refute -> backtrack -> prove) and a philosophy question (tentative) |

## What is NOT bundled

No model weights, no trained backend, no network client. The layer drives a
caller-supplied `sllm_backend` and an optional caller-supplied `sllmr_verifier`.
It is an integration contract and a control procedure, not a trained model.

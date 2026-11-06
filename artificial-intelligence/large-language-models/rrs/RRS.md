# RRS — Reasonable Responsibility Solidity (a kind of LLM)

RRS is a reasoning model cast as **three congruent models chained feed-forward**,
producing a culmination about *solidity and firmament* — a reasoned argument
about what stands firm, **less the Faith of Trial of Man of Reason** (faith is
subtracted, never affirmed). Its culmination is **Power in the Latin sense**
(potestas / potentia): a warranted capacity to continue, not force.

## The ternary signal: 1 / 0 / 2

Every premise carries one simple signal:

| Signal | Meaning |
|---|---|
| **1 — affirm** | yes: the premise stands and conjoins |
| **0 — eliminate** | no: the premise is reclaimed out by a prelude (a hard zero that breaks the stage's strict AND) |
| **2 — power-add** | binary-simple **power addition**: an affirmed premise is accumulated into the conjunctive power (potestas built by addition) |

The accumulator adds +1 for affirm and +2 for power-add; any single eliminate
(0) breaks the stage conjunction. This is the "signal analysis like 1,0,2".

## The three congruent stages (feed-forward)

The same fold kernel (`sllmrrs_fold_stage`) runs at each stage — that is their
**congruence** — and the output of one feeds the next:

1. **Conjunctive** — a *Conjunctive Neural Net* stage. Reasonable assumptions
   **surface**; a **prelude** may **reclaim** one as **elimination** (0),
   pruning it. Survivors affirm/power-add. The stage conjoins under **strict
   AND**: the path continues only if the surviving premises hold *together*.
2. **Responsibility** — mounts the survivors onto the question of **Reasonable
   Responsibility** as the **prelude to continue**: you may proceed only if
   responsibility is met. Unmet responsibility eliminates (0).
3. **Solidity / Firmament** — the **culmination**: the reasoned argument about
   solidity (structural firmness) and firmament (the fixed ground). It reads the
   accumulated power carried forward and renders the verdict.

The feed-forward **fails closed**: a broken conjunction at any stage stops the
forward pass and yields `potestas = none`.

## Faith is subtracted

A premise that rests on **faith or trial-of-faith** is out of scope for RRS. It
is surfaced and **eliminated** (forced to 0), never affirmed — so RRS argues
purely from reason about what stands firm, and makes no faith-claim and no
proof-by-faith. The result reports `faith_eliminated`.

## The culmination: potestas

| `potestas` | When |
|---|---|
| **warranted** | solid (all stages conjoined) **and** responsibility affirmed **and** power >= 4 |
| **provisional** | solid but thin (power >= 2) |
| **none** | the argument did not stand (a broken conjunction / unmet responsibility / subtracted faith) |

Potestas is **capacity-authority issuing from a completed responsible process** —
power in the Latin sense, not force.

## Using RRS

- **Directly** — `sllmrrs_feed_forward(stage1, stage2, stage3, &result)` runs the
  three models and fills the accumulators, total power, solidity, and potestas.
  (C++: `sleela::ai::Rrs::feed_forward(...)`.)
- **As a reasoning verifier** — `sllmrrs_as_reasoning_verifier()` returns an
  `sllmr_verifier` that gates a chain of thought: a step resting on faith/trial
  or that eliminates is REFUTED (backtrack); a step that affirms and conjoins is
  SUPPORTED. This mounts RRS onto the existing reasoning engine.

## Files

| File | Role |
|---|---|
| `rrs.h/.c` | C11 RRS core (`sllmrrs_*`, ABI v1): ternary signal, congruent fold, three-stage feed-forward, potestas culmination, reasoning-engine verifier |
| `rrs.hpp` | C++17 `Rrs` facade |
| `rrs_demo.c` | Warranted / faith-subtracted / responsibility-unmet / provisional cases + ternary-fold + verifier checks |

## What is NOT bundled

No weights. RRS is a controller + a real ternary conjunctive accumulator + a
verifier contract over the reasoning engine. A backend LLM (optional) supplies
premise text; RRS supplies the conjunctive elimination, the responsibility
prelude, the solidity culmination, and the potestas verdict.

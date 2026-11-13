# advantage / artificial-intelligence — courseware track

A short courseware track that reads SLeeLa's AI code through the **advantage
model**: *which approach should I draw on, and what must I check before I do?*

It follows the house style of the [tutorial series](../../tutorial/README.md)
(standalone lessons, prev/next headers) and points at the real code in
[`/artificial-intelligence`](../../artificial-intelligence/README.md). Nothing
here claims a measured result; numbers in exercises are illustrative.

## The track

| # | Lesson | You will learn |
|--:|---|---|
| 01 | [Evidence before trust](01-evidence-before-trust.md) | Treat a model score as evidence: reward vs baseline, confidence `n/(n+prior)`, "unknown is not zero" |
| 02 | [Assumptions of a model](02-assumptions-of-a-model.md) | Record held/doubted assumptions for neural nets, character recognition and LLMs |
| 03 | [Counsel: choosing an AI approach](03-counsel-choosing-an-approach.md) | Rank approaches with `mean × confidence × trust` and read the cautions |
| 04 | [The high speed trader](04-the-high-speed-trader.md) | A runnable model: 5,000+ evals/s, about 2,000 trades/min (advanced-medium) |
| 05 | [Pragmatics and evolution](05-pragmatics-and-evolution.md) | The 24 pragmatics in three phases; online weight learning (advanced) |
| 06 | [Fail closed, and the brilliant](06-fail-closed-and-the-brilliant.md) | Safe halting, refusal over clamping, and the insight channel (advanced) |

Models live in [models/](models/README.md).

## Prerequisites

- [advantage README](../README.md) (math core, store, counsel)
- [SUPPORTED.MODELS.md](../SUPPORTED.MODELS.md) §1 (the foundation)
- Optional: [neural-nets](../../artificial-intelligence/neural-nets/README.md),
  [character-recognition](../../artificial-intelligence/character-recognition/README.md),
  [large-language-models](../../artificial-intelligence/large-language-models/)

## Honest limits

- Illustrative data only; replace with your own evaluation outcomes.
- Fetched/remote knowledge is untrusted and never scored (see `enrich`).
- Lessons 01-03 add no code; lessons 04-06 teach the courseware model in `models/`.

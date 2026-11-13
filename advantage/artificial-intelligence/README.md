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

## Prerequisites

- [advantage README](../README.md) (math core, store, counsel)
- [SUPPORTED.MODELS.md](../SUPPORTED.MODELS.md) §1 (the foundation)
- Optional: [neural-nets](../../artificial-intelligence/neural-nets/README.md),
  [character-recognition](../../artificial-intelligence/character-recognition/README.md),
  [large-language-models](../../artificial-intelligence/large-language-models/)

## Honest limits

- Illustrative data only; replace with your own evaluation outcomes.
- Fetched/remote knowledge is untrusted and never scored (see `enrich`).
- This track does not add new AI code; it teaches how to weigh the existing code.

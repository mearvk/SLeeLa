# Lesson 03 — Counsel: choosing an AI approach

> Series: [index](README.md) · prev: [02](02-assumptions-of-a-model.md)

`counsel` ranks sources by `score = mean × confidence × trust` and lists things
to verify and cautions:

- no evidence: scored 0 and flagged unknown;
- spread larger than the mean gain: flagged inconsistent;
- overflowing values: score withheld, never NaN.

```sh
cd advantage
make run-counsel
```

## Reading a ranking

A high mean with two runs and a doubted assumption can rank below a modest mean
with many runs and no doubts. That is intended: the module prefers a trusted
modest gain to an untested large one.

## Exercise

Model A: mean 0.4, 3 runs, trust 1.0. Model B: mean 0.5, 12 runs, trust 0.5.
Which ranks higher? *Answer:* A = 0.4×0.5×1.0 = 0.20; B = 0.5×0.8×0.5 = 0.20 — a tie.
Verify one assumption of B and it wins.

Next: the same discipline applies to trades; see [SUPPORTED.MODELS.md](../SUPPORTED.MODELS.md).

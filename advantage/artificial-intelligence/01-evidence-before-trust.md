# Lesson 01 — Evidence before trust

> Series: [index](README.md) · next: [02 — Assumptions of a model](02-assumptions-of-a-model.md)

An evaluation of an AI component is an **observation**: a `reward` (what you got)
against a `baseline` (what you would have got otherwise). The advantage math in
[`advantage.h`](../advantage.h) turns a list of observations into a mean, a
spread, and a confidence.

## The three rules

1. **Gain is relative.** A 92% score means little without the baseline (e.g. a
   majority-class guess or the previous model).
2. **Few observations earn little trust.** `confidence = n / (n + prior)`, prior 3
   by default: 1 run gives 0.25, 3 runs 0.5, 12 runs 0.8.
3. **No evidence is unknown, not zero.** An untested model is flagged, not scored 0.

## In XML

```xml
<sourceAdvantage id="neural-net-xor" kind="model">
  <evidence weight="1.0">
    <observation reward="0.97" baseline="0.50"/>
    <observation reward="0.95" baseline="0.50"/>
  </evidence>
</sourceAdvantage>
```

(The values above are illustrative, not measurements.)

## Exercise

Three runs give gains 0.4, 0.5, 0.45. What is the confidence? Then add nine more
similar runs: how does it change? *Answer:* 3/(3+3)=0.5; 12/(12+3)=0.8.

Try it: `cd advantage && make run-analyze`.

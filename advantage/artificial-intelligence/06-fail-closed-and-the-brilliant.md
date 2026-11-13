# Lesson 06 — Fail closed, and the brilliant (advanced)

> Series: [index](README.md) · prev: [05](05-pragmatics-and-evolution.md)

Some people are brilliant. A good system lets them contribute without letting a
single mistake, or a single brilliant mistake, defeat the safety gates.

## Fail closed

- `configure(7, ...)`, `configure(25, ...)`, or a non-positive rate: refused; the trader stays not ready and rejects everything (code 1).
- Malformed input (non-finite, out of range): vetoed by L1.
- Drawdown within 5% of the limit: the trader **halts**. Only `rearm()`, a human act, restarts it.

## The brilliant person channel

`insight(id, factor)` accepts a nudge of 0.5..2.0 on one weight. It cannot touch
hard minimums, so it cannot unlock a veto. Out-of-range requests are **refused**,
not clamped, so a typo never silently becomes a different instruction.

## Exercises

1. In section 4 of the demo, why can insight 2.0 on G2 not restart a halted trader?
2. A brilliant analyst wants to bypass G3 (price collar) for one trade. Argue for
   or against, and say what an auditable alternative looks like.
3. Why does `rearm()` reset the drawdown peak, and what is the risk of that?

## Honest limits

Everything here is synthetic. Real automated trading carries legal and
supervisory obligations this model does not cover.

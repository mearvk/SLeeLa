# Lesson 07 — The Moral Evaluator (advanced)

> Series: [index](README.md) · prev: [06](06-fail-closed-and-the-brilliant.md) · next: [08 — Tall Science](08-tall-science.md)

Module: `lib/advantage/modules/moral` (see the [modules README](../../lib/advantage/modules/README.md)). It asks of a trade: is it
centered on the human and on life, and how much merit is at risk?

## Veto before score

A trade can have 40 of 41 points yes and still be **NO** if the one miss is a
cardinal point (for example, it endangers life). Run the demo and read section 3:
the merit is 0.978 and the verdict is NO. Merit never buys a cardinal.

## Three verdicts

NO, YES, COMPLETE. COMPLETE means all 41 points are yes. Switch to 2-state mode
and COMPLETE folds into YES.

## Exercises

1. Set `configure(5, 3)`. What can the model no longer see?
2. Which dimension floor catches a trade that is unfair but lawful?
3. Add a 42nd point. What must change (arrays, orders, the COMPLETE rule)?
4. Argue whether "fair value" should be cardinal. What would it cost?

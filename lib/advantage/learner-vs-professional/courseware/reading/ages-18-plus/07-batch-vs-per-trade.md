# A Batch Total Is Not a Per-Trade Rate

A reader that finally gives a real, checked answer to a question raised
earlier: "what would 2,000 trades return?" Building
[`SLBatchReturn`](../../../../trading-actions-on-verb/SLBatchReturn.sleela)
made it possible to compute this honestly instead of asserting it. The one
idea this reader teaches: **a batch's total return and its average per-trade
return are very different numbers, and mixing them up changes a plausible
claim into an impossible one.**

## Part 1: Dividing the total

If a batch of 2,000 trades has a **total** return of 20% (2,000 basis points,
where 10,000 bp = 100%), the average per trade is simply the total divided by
the count:

> 2,000 bp / 2,000 trades = **1 bp per trade = 0.01%**

That is a small, plausible per-trade edge. It is also a completely different
claim from "20% per trade, 2,000 times."

## Part 2: What "20% every trade, 2,000 times" would actually mean

Using `SLBatchReturn.idealBatchProfit`, with every trade re-staking the same
$10,000 (so this is **not** compounding — see Part 3):

| assumption | result (on $10,000 capital, 2,000 trades) |
|---|---|
| total batch return of 20% | profit of $2,000; average 1 bp (0.01%) per trade |
| a flat 20% win on **every one** of the 2,000 trades, no losses | profit of $4,000,000,000 — **400 times the capital**, because 2,000 x 20% = 400 with no compounding at all |
| a 50/50 split of +20 bp wins and -20 bp losses | nets to exactly 0 bp total |

The middle row is not a typo: 2,000 trades, each a genuine 20% gain, summed
without any compounding, already reaches 400x capital. This is the same kind
of overflow the compounding table in
[reader 6](./06-single-trade-sensitivity.md) demonstrated for a daily rate,
arrived at a different way: even *without* compounding, repeating a real
single-trade-sized return (not a batch-average return) 2,000 times produces
a number with no economic meaning.

## Part 3: Why this class deliberately does not compound

`SLBatchReturn` re-stakes the **same** capital for every trade in a batch.
That models 2,000 *independent*, fixed-size trades — closer to "a trader
who risks the same $10,000 each time" than to a single account that lets
winnings ride. Compounding a batch (letting each trade's profit become the
next trade's capital) is a different, larger claim, and it is the mechanism
that made the daily-rate numbers in reader 6 explode to over 100x in 30 days.
Keeping batch totals uncompounded here makes the two readers consistent: a
batch's total is a sum, not a compounding growth curve.

## Part 4: How this connects to the original claim

[`ORGANIZED.md`](../../ORGANIZED.md) records a claim, stated as certain from
personal experience, that "day traders make about 20% profit per 2,000
trades." That claim is still not verified by this repository — nothing here
confirms it is true of real traders. What changed is that it is now
possible to state, exactly, **what the claim would mean if it were about a
batch total** (a 1 bp per-trade edge: small and not obviously absurd) versus
**what it would mean if "20%" were read as a per-trade return repeated 2,000
times** (400x capital: not plausible). The arithmetic does not decide which
reading the original claim intended, but it shows precisely why the two
readings cannot be treated as the same number.

## Questions

1. A batch of 500 trades has a total return of 10% (1,000 bp). What is the
   average per-trade return, in basis points and as a percent?
2. Why does "20% every trade, 2,000 times, no compounding" still reach 400x
   capital, when compounding is exactly what was excluded?
3. Is a 1 bp (0.01%) average per-trade edge, by itself, evidence that a
   trader achieves it? Why or why not?

**Answers:**

1. 1,000 / 500 = 2 bp per trade = 0.02%.
2. Because summing (not compounding) N copies of a fixed percentage of the
   *same* starting capital is still N times that percentage: 2,000 x 20% =
   400 (that is, 40,000%), which is 400x capital, with no multiplicative
   growth involved at all. Compounding would make it even larger; omitting
   compounding does not make repetition free.
3. No. `SLBatchReturn` computes what a stated assumption implies; it does not
   measure or confirm that any real trader achieves that assumption. The
   number is exact given the input, and the input is still unverified.

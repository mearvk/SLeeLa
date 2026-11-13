# Single-Trade Sensitivity, and Why Daily Rates Like These Are Not Real

A reader about a table that was requested earlier in planning this material:
a hypothetical trader's "good day" return of 2% to 17%, and a "microtrade
advantage" multiplier of 2x up to 12x or more. The table is built honestly,
with the real [`SLLongBook`](../../../../trading-actions-on-verb/SLLongBook.sleela),
and then used for the purpose it can actually serve: showing, by direct
computation, why a daily return in that range cannot be a sustained real-world
rate.

> No naming scheme such as "Status I" or "Convenient II" is used for this
> table. Those terms could not be verified as established usage anywhere
> checkable, and real academic terms that sound similar — "convenience
> yield," the "reserve-currency status" of a safe asset (Stanford economics
> literature on Treasury bonds and the dollar) — measure something
> unrelated: the premium investors accept for safety and liquidity, not a
> trader's return. Attaching either kind of label here would overstate what
> this table is.

## Part 1: One trade, several hypothetical returns

Each row below is **one stand-alone trade**: buy 10,000 units at 100 cents
each (capital = 1,000,000 cents = $10,000, entirely invented), sell the same
units the next day at a higher price, no repetition and no compounding. The
number of cents and the resulting multiple of capital follow from the return
percentage by simple arithmetic; nothing here is estimated.

| return assumed on this one trade | sell price (cents) | profit (cents) | capital afterward, as a multiple |
|---:|---:|---:|---:|
| 2% ("a good day," low end, as given) | 102 | 20,000 | 1.02x |
| 5% ("a good day," mid) | 105 | 50,000 | 1.05x |
| 10% ("a good day," high-ish) | 110 | 100,000 | 1.10x |
| 17% ("a good day," high end, as given) | 117 | 170,000 | 1.17x |
| 100% ("microtrade advantage," 2x) | 200 | 1,000,000 | 2.00x |
| 300% ("microtrade advantage," 4x) | 400 | 3,000,000 | 4.00x |
| 1,100% ("microtrade advantage," 12x) | 1,200 | 11,000,000 | 12.00x |

This is correct and uncontroversial: if a single trade genuinely returns 17%,
the profit on $10,000 is $1,700. The question is not whether this arithmetic
is right. It is whether a trader can **repeat** a return anywhere near this
range, day after day, as a matter of skill rather than luck.

## Part 2: What repeating it would mean (and why that is the tell)

Compounding is the test. If "2% on a good day" happened on every trading day,
starting from $10,000:

| after this many trading days | at 2 percent per day | at 17 percent per day |
|---:|---:|---:|
| 10 | not shown below (see Part 3's note) | $48,068.22 |
| 20 | — | $231,055.54 |
| 30 | $18,113.45 | $1,110,644.25 |
| 90 | $59,430.24 | (not computed; already absurd by day 30) |
| 365 (one year) | $13,773,765.85 | (not computed) |

These numbers were produced by running the compounding loop in SLeeLa and
reading the output directly, not by hand-estimating. They are shown precisely
**because** they are the argument against the assumption, not evidence for
it:

- **2% a day compounds to roughly 1,377x in one year.** A single trader or
  fund sustaining that would, within a few years, be larger than the entire
  world's investable capital. No market has room for a strategy that scales
  this way, because the trader's own buying and selling would move the price
  long before the rate could continue.
- **17% a day compounds to over 100x in 30 trading days.** A $10,000 stake
  becomes over a million dollars in about six weeks of trading days. This is
  not a "good trader" outcome; it is a mathematical signal that the 17%
  figure cannot be a recurring daily rate, only (at best) the return of one
  unusual trade that will not repeat on command.
- **The "2x to 12x microtrade advantage" figures are even larger single-trade
  returns** (100% to 1,100%), and compounding either of them for even a few
  days produces numbers with no economic meaning at all.

## Part 3: What this table is for

- It is a **worked illustration** for reading alongside
  [`02-position-books.md`](./02-position-books.md) and
  [`03-margin-exactly.md`](./03-margin-exactly.md): the same exact,
  checked arithmetic, applied to a hypothetical the reader can check by hand.
- It is **not** a claim that any real trader achieves these rates, a
  recommendation, or an estimate of what is achievable. The 2–17% and 2x–12x
  figures were supplied as an assumption to evaluate, not derived from any
  model, market data, or evidence in this library.
- It is **not** extended to a population, a city, a per-second rate, or any
  public office. See [`PROFITABILITY.SYSTEMS.INDEX.md`](../../../../../../advantage/PROFITABILITY.SYSTEMS.INDEX.md)
  §6 for why that kind of extrapolation is out of scope for this library, for
  the same underlying reason this reader demonstrates: an unrealistic rate
  does not become realistic by being multiplied by something large.

## Questions

1. At 2% a day, roughly how many trading days does it take to double $10,000?
   (Hint: 2% a day doubles in about 35 days — this is the "rule of 70":
   70 / 2 ≈ 35.)
2. Why does a 1,100% return on one trade not tell you anything about what
   the next trade will do?
3. If someone claims a trading strategy returns 10% to 280% per day
   consistently, what does Part 2's compounding argument say about that
   claim, independent of any details of the strategy itself?

**Answers:**

1. About 35 trading days (70 / 2). Compounding to 1,377x in a year is the
   same fact restated: doubling about ten times (2^10 ≈ 1,024) in 365 days.
2. A single trade's return describes that trade's specific price move. It
   says nothing about whether the next buyer and seller will agree to a
   similarly large move, which is what "returns 1,100% again" would require.
3. The claim does not need to be checked by its details: compounding any
   rate near the bottom of that range (even 10%) for a few months produces
   a result with no room in any real market. The claim can be set aside on
   arithmetic grounds alone, before asking anything about the strategy.

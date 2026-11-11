# Trading Actions on Verb

Trading expressed as **verbs** on a long-only book (`buy`, `sell`, `hold`,
`flatten`) and on its mirror, a short-only book (`shortSell`, `cover`, `hold`,
`coverAll`). Twenty-six runnable use cases show two families, plus the assurance cases:

- **Style: Trading Long** (use cases 1 to 7; the short side is 16 to 19, short margin is 23 to 25): how a long position is built,
  trimmed, closed, refused and rebalanced.
- **Time on Long** (use cases 8 to 13; the short side is 20 and 26): how long shares have been held, in whole
  days, and what that means for FIFO selling, averages, term and time stops.

Use case 13 joins both over a 500-day life. Use cases 14, 15, 21 and 22 are
the assurance cases: differential stress tests of both books, an exhaustive
test of the Euclid routines, and the long/short mirror law.

> Illustrative only. Nothing here places an order, reads a market or gives
> trading, tax or legal advice. All prices are invented integer cents.

## Reading of the request

Two phrases were interpreted; correct them if they were meant differently.

| phrase | meaning here |
|--------|--------------|
| "actions on verb" | each trading action is a method (a verb) with a status code, and a refused verb leaves the book exactly as it was |
| "careful looping Euclidean" | loops with a stated invariant and a termination argument, built on Euclid's algorithm (gcd, Bezout, exact fills) and Euclidean distance (squared distance and an integer square root) |

## The classes

| class | role |
|-------|------|
| [`SLLongBook`](./SLLongBook.sleela) | FIFO lots, the four verbs, money, time-on-long queries and `invariantsHold()` |
| [`SLShortBook`](./SLShortBook.sleela) | the mirror: short-only FIFO lots, `shortSell`/`cover`/`hold`/`coverAll`, a borrow cost per share-day, `markToMarket`, time queries and `invariantsHold()` |
| [`SLShortMargin`](./SLShortMargin.sleela) | illustrative short-margin arithmetic: exact ceiling requirement, equity and excess, call amount, fewest shares to cover (binary search), first call day under borrow cost |
| [`SLEuclid`](./SLEuclid.sleela) | `gcd`, `gcdAll`, `lcm`, `egcd` (Bezout), `canFill` (closed form) and `canFillScan` (reference), `isqrt`, `distSq`, `dist`, `nearestIndex` |

### Short book

`SLShortBook` is the mirror: it sells first and covers later, profit is sale price
minus cover price, and it is short only (a cover above the shares short is
refused, code 3). Codes 0 to 8 mean the same as below with `cover` for `sell`;
code 9 means the borrow rate cannot change while any share is short. Borrow cost
is `borrowRate` cents per share per day, charged on covered shares for the days
they were short; `borrowAccrued(today)` shows what is owed on open lots.
`realizedNet` for the short book is proceeds covered - cover cost - fees - borrow
cost paid. `markToMarket(price)` is proceeds held - shares short * price, which
can be below minus the proceeds, because a rising price has no upper limit.

**Mirror law.** For the same days, quantities, prices and fees, a short sale
matches a buy and a cover matches a sell; every status code, the shares held,
the lot count, the ages and the cost basis equal the sale proceeds are the same,
and the short profit is exactly minus the long profit. Use case 22 checks this
after every one of 1,200 random steps.

### Short margin (illustrative)

`SLShortMargin` is stateless arithmetic, not a broker rule: the percentage is a
parameter in basis points (10000 = 100 percent of the short value, at most
15000). Collateral is the cash held (proceeds plus any deposit). Equity =
collateral - shares * price. Required = the **ceiling** of value * bp / 10000,
computed as `q*bp + ceil(r*bp / 10000)` with `q = value / 10000` and
`r = value mod 10000`, so no product passes 6e15 and the ceiling is exact.
Excess = equity - required, and a call is due when excess < 0.

- `callAmount`: the deposit that brings excess back to zero.
- `sharesToCover`: covering at the current price leaves equity unchanged (fees
  ignored) and lowers the requirement, so "enough" is monotone in the shares
  covered. A binary search (invariant: the answer lies in `[lo, hi]`, and
  `hi - lo` shrinks) finds the fewest; `sharesToCoverScan` is the reference. If
  equity is negative no cover helps and the answer is -1.
- `daysToCall`: borrow cost drains `shares * rate` a day, so the first negative
  day is `floor(excess / burn) + 1` (0 if already due; -2 if nothing drains; -3
  beyond the day bound). `daysToCallScan` steps day by day.

### Verbs and status codes

| verb | meaning |
|------|---------|
| `buy(day, qty, price, fee)` | add a lot |
| `sell(day, qty, price, fee)` | sell the oldest lots first |
| `hold(day)` | let time pass, no trade |
| `flatten(day, price, fee)` | sell everything held |

| code | meaning |
|------|---------|
| 0 | ok |
| 1 | bad quantity |
| 2 | bad price |
| 3 | sell exceeds the holding (long only: no short) |
| 4 | book full (16 lots) |
| 5 | day goes backwards |
| 6 | bad fee |
| 7 | nothing held |
| 8 | a size bound would be passed (quantity, price, fee, day or a running total) |

## Use cases

| # | family | folder | what it proves |
|---|--------|--------|----------------|
| 1 | style | [`01-open-and-add`](./01-open-and-add/TaUse01OpenAndAdd.sleela) | lots, exact average cost with its remainder |
| 2 | style | [`02-trim-realized-pnl`](./02-trim-realized-pnl/TaUse02TrimRealizedPnl.sleela) | FIFO trim realizes profit on the true cost |
| 3 | style | [`03-flatten-and-reconcile`](./03-flatten-and-reconcile/TaUse03FlattenAndReconcile.sleela) | profit = proceeds - cost sold - fees; flat sells are refused |
| 4 | style | [`04-refuse-short-and-bad-input`](./04-refuse-short-and-bad-input/TaUse04RefuseShortAndBadInput.sleela) | every refusal leaves the book unchanged |
| 5 | style, Euclid | [`05-lot-ratio-by-euclid`](./05-lot-ratio-by-euclid/TaUse05LotRatioByEuclid.sleela) | gcd loop reduces 120:180:300 to 2:3:5 |
| 6 | style, Euclid | [`06-round-lot-fill-and-bezout`](./06-round-lot-fill-and-bezout/TaUse06RoundLotFillAndBezout.sleela) | gcd is necessary, not sufficient, for exact fills; Bezout certificates |
| 7 | style, Euclid | [`07-rebalance-distance`](./07-rebalance-distance/TaUse07RebalanceDistance.sleela) | nearest target by squared distance, floor square root, ties to the first |
| 8 | time | [`08-holding-days`](./08-holding-days/TaUse08HoldingDays.sleela) | age in days; time never runs backwards |
| 9 | time | [`09-fifo-aging`](./09-fifo-aging/TaUse09FifoAging.sleela) | a sell takes the oldest lots, so survivors are younger |
| 10 | time | [`10-weighted-average-age`](./10-weighted-average-age/TaUse10WeightedAverageAge.sleela) | quantity-weighted age, exact or floored |
| 11 | time | [`11-term-boundary`](./11-term-boundary/TaUse11TermBoundary.sleela) | long term only when held MORE than the threshold |
| 12 | time | [`12-time-stop`](./12-time-stop/TaUse12TimeStop.sleela) | a bounded loop sells everything past the age limit |
| 13 | both | [`13-full-lifecycle`](./13-full-lifecycle/TaUse13FullLifecycle.sleela) | 500 days, invariants after every action, independent reconciliation |
| 14 | both, assurance | [`14-differential-stress`](./14-differential-stress/TaUse14DifferentialStress.sleela) | 1,200 random verbs against an independent per-share shadow model; every observable agrees after every step, refusals change nothing |
| 15 | Euclid, assurance | [`15-euclid-exhaustive`](./15-euclid-exhaustive/TaUse15EuclidExhaustive.sleela) | each fast routine against a slower method over every small case, plus the exact-comparison ceiling |
| 16 | short, style | [`16-short-open-and-add`](./16-short-open-and-add/TaUse16ShortOpenAndAdd.sleela) | short lots, exact average sale price with its remainder |
| 17 | short, style | [`17-cover-realized-pnl`](./17-cover-realized-pnl/TaUse17CoverRealizedPnl.sleela) | FIFO cover, profit = sale price - cover price, borrow cost paid on cover |
| 18 | short, style | [`18-refuse-long-and-bad-input`](./18-refuse-long-and-bad-input/TaUse18RefuseLongAndBadInput.sleela) | a short-only book never goes long; refusals change nothing; the rate is frozen while short |
| 19 | both, style | [`19-loss-asymmetry`](./19-loss-asymmetry/TaUse19LossAsymmetry.sleela) | a long loss is capped by its cost, a short loss is not capped by its proceeds; the mirror law in one example |
| 20 | short, time | [`20-borrow-cost-and-time-stop`](./20-borrow-cost-and-time-stop/TaUse20BorrowCostAndTimeStop.sleela) | exact borrow accrual per share-day, paid on cover; a time stop for the short side |
| 21 | short, assurance | [`21-short-differential-stress`](./21-short-differential-stress/TaUse21ShortDifferentialStress.sleela) | 1,200 random verbs against an independent per-share shadow with borrow cost |
| 22 | both, assurance | [`22-mirror-law-stress`](./22-mirror-law-stress/TaUse22MirrorLawStress.sleela) | the same random verbs drive both books; every code equal, short profit exactly minus long profit |
| 23 | short margin | [`23-margin-requirement`](./23-margin-requirement/TaUse23MarginRequirement.sleela) | the requirement is the exact ceiling of value times percent, without a product that could pass 2^53; exhaustive against the direct form |
| 24 | short margin | [`24-margin-call-and-cover`](./24-margin-call-and-cover/TaUse24MarginCallAndCover.sleela) | a call, the deposit that restores margin, the fewest shares to cover; negative equity means a cover cannot help |
| 25 | margin, assurance | [`25-margin-differential`](./25-margin-differential/TaUse25MarginDifferential.sleela) | binary search and closed form against slow references over full grids, with minimality checks |
| 26 | short margin, time | [`26-margin-and-time`](./26-margin-and-time/TaUse26MarginAndTime.sleela) | borrow cost drains collateral; first call day 334, confirmed by stepping and by the book accrual |

## More

- [`ENGINEERING.md`](./ENGINEERING.md): invariants, the measured integer limits
  and every bound, fees, time queries, the Euclid loops, ties and the term rule.
- [`ASSURANCE.md`](./ASSURANCE.md): the six layers of checking, their limits, how
  to run the gate, and the review log.

```
bash test-suites/test-trading-actions.sh
```

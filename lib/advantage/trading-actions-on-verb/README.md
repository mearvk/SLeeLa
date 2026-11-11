# Trading Actions on Verb

Trading expressed as **verbs** on a long-only position book: `buy`, `sell`,
`hold` and `flatten`. Thirteen runnable use cases show two families:

- **Style: Trading Long** (use cases 1 to 7): how a long position is built,
  trimmed, closed, refused and rebalanced.
- **Time on Long** (use cases 8 to 13): how long shares have been held, in whole
  days, and what that means for FIFO selling, averages, term and time stops.

Use case 13 joins both over a 500-day life.

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
| [`SLEuclid`](./SLEuclid.sleela) | `gcd`, `gcdAll`, `lcm`, `egcd` (Bezout), `canFill`, `isqrt`, `distSq`, `dist` |

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

## Engineering notes

**Invariants.** `invariantsHold()` is asserted by the use cases after actions:
every lot has positive quantity and price; lot days never decrease and never
pass the book day; at most 16 lots; shares held = shares bought - shares sold;
cost held = cost bought - cost sold. The last two are conservation laws, so a
lost or invented share or cent is caught.

**Refusal is atomic.** A refused verb returns its code before changing any
field. Use case 4 sums shares, cost, fees and day before and after every
refusal.

**Integer only.** Money is integer cents, quantity whole shares, time whole
days. Average cost is rounded down and the remainder is reported, so
`average * shares + remainder = cost` exactly.

**Euclid loops.**
- `gcd`: invariant gcd(x, y) = gcd(a, b); the remainder strictly falls, so the
  loop ends. `steps` and `trail` expose every pass.
- `egcd`: keeps two Bezout rows; `bezoutHolds` verifies `a*x + b*y = g` with
  one multiplication.
- `canFill`: the gcd test is checked first because it is necessary; the search
  then runs at most `c / a + 1` passes. Use case 6 shows 10 shares passing the
  gcd test but failing the fill.
- `isqrt`: binary search with invariant `lo*lo <= n < (hi+1)*(hi+1)`, capped at
  46340 so `mid*mid` never passes the 32-bit range.
- `distSq`: refuses any difference above 10000 or more than 20 dimensions, so
  the sum stays below 2,000,000,000.

**Ties.** A tie between equal targets goes to the first, following the repo
tie norm (`.kiro/steering/advantage-tie-norm.md`).

**Term rule.** Long term means held strictly more than the threshold. The
model counts days only. Real tax rules (for example 26 U.S.C. 1222 uses a
holding period of more than one year) count calendar dates and vary by country
and year. Treat this as an illustration of the boundary, not as tax advice.

## Assurance

Each use case is self-checking: `expect` lines print `ok` or `FAIL` and a final
`checks passed=N failed=0`. The expected values were computed by a separate
Python model of the same rules, then compared with what SLeeLa printed.
`test-suites/test-trading-actions.sh` runs all thirteen, compares each with its
`.expected.txt`, and fails on any `FAIL` line.

```
bash test-suites/test-trading-actions.sh
```

Set `UPDATE=1` to rewrite the expected files after reviewing a deliberate change.

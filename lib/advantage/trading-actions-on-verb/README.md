# Trading Actions on Verb

Trading expressed as **verbs** on a long-only position book: `buy`, `sell`,
`hold` and `flatten`. Fifteen runnable use cases show two families, plus two stress cases:

- **Style: Trading Long** (use cases 1 to 7): how a long position is built,
  trimmed, closed, refused and rebalanced.
- **Time on Long** (use cases 8 to 13): how long shares have been held, in whole
  days, and what that means for FIFO selling, averages, term and time stops.

Use case 13 joins both over a 500-day life. Use cases 14 and 15 are the
assurance cases: a differential stress test of the book and an exhaustive test
of the Euclid routines.

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
| [`SLEuclid`](./SLEuclid.sleela) | `gcd`, `gcdAll`, `lcm`, `egcd` (Bezout), `canFill` (closed form) and `canFillScan` (reference), `isqrt`, `distSq`, `dist`, `nearestIndex` |

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

## Engineering notes

**Invariants.** `invariantsHold()` is asserted by the use cases after actions:
every lot has positive quantity and price; lot days never decrease and never
pass the book day; at most 16 lots; shares held = shares bought - shares sold;
cost held = cost bought - cost sold. The last two are conservation laws, so a
lost or invented share or cent is caught.

**Refusal is atomic.** A refused verb returns its code before changing any
field. Use case 4 sums shares, cost, fees and day before and after every
refusal.

**Integer only, with measured limits.** Money is integer cents, quantity whole
shares, time whole days. Average cost is rounded down and the remainder is
reported, so `average * shares + remainder = cost` exactly.

SLeeLa `int` is 64-bit and `+ - * / %` are exact over the whole range. The
comparison operators, however, were measured to be exact only up to
2^53 - 1 = 9,007,199,254,740,991: above that, `n` and `n - 1` compare equal. A
first version of `isqrt` assumed a 32-bit word and was therefore wrong in two
ways; after the review every value that is ever compared is held at or below
that limit by explicit bounds, and out-of-range input is **refused**, never
clamped or wrapped:

| bound | value | why |
|-------|-------|-----|
| quantity | 100,000,000 | with price, keeps a product at 1e15 |
| price | 10,000,000 cents | |
| fee | 100,000,000 cents | |
| running totals | 4e15 | leaves room for one more product below 2^53 |
| day | 10,000,000 | keeps share-day sums exact |
| gcd/lcm/egcd input | 2^53 - 1 | comparison limit |
| fill lot size | 90,000,000 | lot squared is 8.1e15 |
| distance difference / dimension | 10,000,000 / 64 | 64 * 1e14 = 6.4e15 |
| isqrt input | 2^53 - 1 | `sqrtCap` 94,906,265 squared fits |

**Fees.** Fees are expensed and are not added to a lot cost. `realizedNet` is a
cash view: proceeds - cost of shares sold - every fee paid so far, including the
buy fees of shares still held. `realizedGross` excludes fees.

**Time queries.** A query for a day before the book day answers -1 (false for
`isLongTerm`) instead of a negative age. A flat book also answers -1 to
`oldestAge`.

**Euclid loops.**
- `gcd`: invariant gcd(x, y) = gcd(a, b); the remainder strictly falls, so the
  loop ends. `steps` and `trail` expose every pass.
- `egcd`: keeps two Bezout rows; `bezoutHolds` verifies `a*x + b*y = g` with
  one multiplication.
- `canFill`: closed form in O(log) time. With g = gcd(a, b), a solution needs
  g | c; then x = (c/g mod b/g) * inverse(a/g mod b/g) mod b/g is the smallest
  number of a-lots, and y = (c - a*x) / b must be at least 0. `canFillScan`
  (the plain loop, at most c/a + 1 passes) is kept only as a reference; use
  cases 6 and 15 prove the two agree on every tested case. Use case 6 also
  shows 10 shares passing the gcd test but failing the fill.
- `isqrt`: binary search with invariant `lo*lo <= n < (hi+1)*(hi+1)`, capped at
  `sqrtCap`, so `mid*mid` is always compared exactly.
- `distSq`: refuses any difference above 10,000,000 or more than 64 dimensions.
- `nearestIndex`: nearest of several targets; a tie goes to the earlier one
  (the repo tie norm, `.kiro/steering/advantage-tie-norm.md`).

**Term rule.** Long term means held strictly more than the threshold. The
model counts days only. Real tax rules (for example 26 U.S.C. 1222 uses a
holding period of more than one year) count calendar dates and vary by country
and year. Treat this as an illustration of the boundary, not as tax advice.

## Assurance

Four independent layers:

1. **Self-checks.** Every use case prints `ok` or `FAIL` per check and ends with
   `checks passed=N failed=0`.
2. **Independent oracle.** Expected values for use cases 1 to 13 were computed
   by a separate Python model of the same rules and compared with what SLeeLa
   printed; one wrong expectation of mine (use case 11) was found and fixed that way.
3. **Differential testing.** Use case 14 runs 1,200 random verbs (four fixed
   seeds, one buy-heavy so the 16-lot capacity is reached) against a shadow
   model that stores one entry per share and shares no code with the lot-based
   book, and the outcome tallies were also reproduced in Python. Use case 15
   checks every Euclid routine against a slower method over all small cases.
4. **Expected-output gate.** `test-suites/test-trading-actions.sh` runs all
   fifteen, compares each with its `.expected.txt`, and fails on any `FAIL` line.

```
bash test-suites/test-trading-actions.sh
```

Set `UPDATE=1` to rewrite the expected files after reviewing a deliberate change.

Limits of the assurance: the stress cases do not generate code 8 (size bounds),
which use case 4 covers by hand; and the VM comparison behaviour above 2^53 was
measured on the committed `working-binaries/sleela`, not proven from its source.

## Review log (second look)

| finding | severity | fix |
|---------|----------|-----|
| `isqrt` assumed a 32-bit word and returned 46340 for large inputs | wrong answer | bounds rebuilt for the measured 64-bit arithmetic |
| VM comparisons are inexact above 2^53; `isqrt` returned 3037000499 for 3037000499^2 - 1 | wrong answer | every compared value bounded by `maxExact`; out-of-range refused; steering note added |
| no size bounds on quantity, price, fee, day or running totals | silent overflow risk | new code 8 and bounds |
| time queries accepted a day before the book day | negative ages | -1 answer |
| `canFill` scanned up to c/a times | cost grows with order size | O(log) closed form; scan kept as the reference |
| use case 4 compared a hash of the book | collision could hide a change | five values compared separately |
| `cap` was a mutable field beside fixed 16-slot arrays | out-of-bounds risk | fixed `maxLots()` |
| `realizedNet` meaning was implicit | misreading | documented as a cash view |
| README said "no sum can pass 2,000,000,000" | false claim | corrected |

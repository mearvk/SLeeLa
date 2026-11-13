# Engineering Notes

Part of [Trading Actions on Verb](./README.md). Illustrative only; not trading,
tax or legal advice.

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

**Batch return (`SLBatchReturn`).** Computes the total return over a batch of
N independent, non-compounding trades (each re-stakes the same capital) from
stated win-rate/size inputs or an explicit per-trade list, plus the exact
per-trade average a stated total implies. A quality review (below) found and
fixed two real bugs in an earlier version of this class, both instructive:

- A magnitude-based sentinel does not work for a signed quantity that can be
  legitimately as large as the sentinel itself. The original
  `idealBatchProfit` signaled "input rejected" by returning
  `-(maxCapital + 1)`, but a real, in-range batch of up to `maxTrades` trades
  can lose up to `maxTrades * maxCapital` in magnitude — far past that
  sentinel — so a genuine large loss was indistinguishable from a rejection.
  The fix: `idealBatchInRange(...)` is an explicit boolean check, called
  *before* reading a profit, never inferred from the profit's size.
- Bounds must be verified against **every** product the class actually forms,
  including ones several calls deep. The original `maxTrades`/`maxCapital`/
  `maxBp` were sized only against `idealBatchProfit`'s own internal products;
  `idealBatchReturnBp`'s conversion to basis points forms `profit * 10000`,
  a *larger* product that the original bounds let overflow silently (it
  returned a wrong number, not an error). The three bounds are now derived
  together against the single tightest constraint,
  `maxTrades * maxCapital * 10000 <= 2^53 - 1`, which is documented at the
  bound declarations rather than re-derived ad hoc per method. The explicit
  per-trade path has its own, separately derived `maxExplicitCapital` for the
  same reason, since it is capped at 64 trades rather than `maxTrades`.
  Use case 27's regression tests exercise both classes' true worst-case
  corner (maximum trades at maximum capital) to keep this from regressing
  silently again; a sweep of 500 random in-range inputs, independently
  checked against a Python model, additionally confirmed no other input
  combination overflows.

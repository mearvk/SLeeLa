# Margin, Exactly

An illustration of careful integer code. The percentage is a parameter in basis
points (10000 = 100 percent of the short value, at most 15000). It is not any
broker's or regulator's rule.

## Definitions

- collateral = cash held (proceeds plus any deposit)
- short value = shares x price
- equity = collateral - short value
- required = **ceiling** of short value x bp / 10000
- excess = equity - required; a **call** is due when excess < 0

## Why the ceiling, and how to compute it safely

Rounding the requirement down would favour the account holder by up to a cent.
Compute `q x bp + ceil(r x bp / 10000)` with `q = value / 10000` and
`r = value mod 10000`. No intermediate product exceeds 6 x 10^15, which keeps all
comparisons inside the exactly compared range of the runtime.

## Worked example

100 shares short at 5000, collateral 750 000, requirement 30 percent (bp 3000).

| price | value | required | equity | excess |
|-------|-------|----------|--------|--------|
| 5000 | 500 000 | 150 000 | 250 000 | +100 000 |
| 7000 | 700 000 | 210 000 | 50 000 | -160 000 (call) |

- **Deposit** to restore the margin: 160 000.
- **Or cover the fewest shares.** Covering at the mark leaves equity unchanged
  (fees ignored) and lowers the requirement, so "enough" is monotone in the shares
  covered. A binary search finds **77**; 76 is not enough. After covering 77
  at 7000, 23 shares remain, required = 48 300, equity = 50 000, excess = +1 700.
- At a price of 8000, equity is -50 000: **no cover can help**, only a deposit.

## Time wears it down

Borrow cost of 3 cents per share per day on 100 shares drains 300 a day. From an
excess of 100 000 the first negative day is floor(100 000 / 300) + 1 = **334**. Day
333 leaves +100; day 334 leaves -200.

## Questions

1. Required margin at 30 percent of a value of 333?
2. In the binary search, why is `enough after covering k` monotone in k?

**Answers:** 1. ceil(333 x 3000 / 10000) = ceil(99.9) = **100**. 2. Equity does not
change with k and the requirement falls as k grows, so once it is enough it stays
enough for every larger k.

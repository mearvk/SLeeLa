# Long-Term-Potencies

An adult courseware topic defining, precisely, what "potency" means for a
long-term position: **how much of what is held has actually crossed the
long-term line, and how concentrated the book's age is around its oldest
lot.** Both are read directly from `SLLongBook`'s own age and term queries —
nothing here invents a new fact about the book, only two named, exact ratios
over facts the book already exposes.

> Illustrative only. No real market, price or order is used, and this is not
> investment or tax advice. See [`../ORGANIZED.md`](../ORGANIZED.md) for the
> glossary of terms defined versus not defined elsewhere in this courseware
> (most importantly, "Flat 2" is unrelated to anything here and is still not
> defined).

## Why "potency" needed a definition before any code

"Potency" is not a term used anywhere else in this library. Rather than use
it loosely, [`SLTermPotency`](./SLTermPotency.sleela) defines it as exactly
two basis-point ratios:

| name | formula | reads |
|------|---------|-------|
| **term potency** | (units held that are long term) / (units held), in bp | `SLLongBook.qtyLongTerm` / `totalQty` |
| **age potency** | weightedAgeFloor / oldestAge, in bp | `SLLongBook.weightedAgeFloor` / `oldestAge` |

10,000 bp = 100%. Term potency answers "of what I still hold, what share has
crossed the long-term line?" Age potency answers "how close is the average
age of what I hold to the age of my single oldest lot?" — a book with many
small, much-younger lots alongside one old one has a *low* age potency even
though its term potency might already be partial.

## A real gap, stated rather than hidden

`SLLongBook` has `isLongTerm`/`qtyLongTerm`; `SLShortBook` does not — it only
has the three age queries (`oldestAge`, `weightedAgeFloor`, `qtyOlderThan`)
that both books share. So `termPotencyBp` only works on a `SLLongBook` today;
`agePotencyBp` is written to need nothing but the two age queries, so it
could be extended to `SLShortBook` without changing its formula, but that
extension is not built yet. This is recorded here rather than silently
assumed away.

## The four entries

| # | folder | what it shows |
|---|--------|----------------|
| 1 | [`01-ages-as-the-book-ages`](./01-ages-as-the-book-ages/TaPotency01AgesAsTheBookAges.sleela) | one lot checked at exactly its 365-day age (not yet long term — the rule is *strictly more than*), then a second lot added and the book checked again as both potencies rise |
| 2 | [`02-fifo-sell-and-potency`](./02-fifo-sell-and-potency/TaPotency02FifoSellAndPotency.sleela) | selling exactly the oldest lot removes its whole contribution to both potencies in one exact before/after comparison |
| 3 | [`03-honest-empty-and-invalid`](./03-honest-empty-and-invalid/TaPotency03HonestEmptyAndInvalid.sleela) | an empty book and a day before the book's own last action day are each answered honestly (-2 or -1), never guessed |
| 4 | [`04-cross-checked-portfolios`](./04-cross-checked-portfolios/TaPotency04CrossCheckedPortfolios.sleela) | ten randomly generated portfolios checked against an independent Python model of both formulas |

## A worked example, by hand

One lot of 40 units bought on day 0. Checked on day 365: age is exactly 365,
and `isLongTerm` requires **strictly more** than the threshold, so this lot
is *not yet* long term. Term potency is 0 bp; age potency is 10,000 bp (one
lot, so the weighted average age trivially equals the oldest lot's age). A
second lot of 60 units is bought on day 400. Checked on day 500: the day-0
lot (40 units) is now 500 days old, past the 365-day line, while the day-400
lot (60 units) is only 100 days old. Term potency is 40/100 = 4,000 bp (40%);
age potency is weightedAgeFloor(500)/oldestAge(500) = 260/500 = 5,200 bp.

## Assurance

Every entry prints `ok`/`FAIL` per check and ends `checks passed=N failed=0`.
Expected values for all four entries were computed by an independent Python
model of both formulas and compared with what SLeeLa printed; entry 4 does
this across ten randomly generated portfolios rather than hand-picked ones.
`test-suites/test-long-term-potencies.sh` runs all four and compares each
with its `.expected.txt`.

```
bash test-suites/test-long-term-potencies.sh
```

Set `UPDATE=1` to rewrite the expected files after reviewing a deliberate
change.

## Honest limits

- `SLTermPotency` is a read-only view; it never mutates the `SLLongBook` it
  is given.
- Term potency has no short-side counterpart yet (see above).
- `agePotencyBp` returns -1 for both a flat book and an invalid "today",
  because the `SLLongBook.oldestAge` method it reads from does not itself
  distinguish those two cases — this method does not invent a finer
  distinction than its source data supports.
- The long-term threshold (365 days in the worked example) is a parameter the
  caller supplies, not a tax or regulatory rule; see the existing term-boundary
  discussion in [`ENGINEERING.md`](../../../trading-actions-on-verb/ENGINEERING.md).

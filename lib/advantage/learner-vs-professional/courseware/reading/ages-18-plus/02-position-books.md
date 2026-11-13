# Position Books: Long, Short and the Mirror

## Two books, four verbs each

| long book | short book |
|-----------|------------|
| `buy` | `shortSell` |
| `sell` (oldest lots first) | `cover` (oldest lots first) |
| `hold` | `hold` |
| `flatten` | `coverAll` |

Each verb returns a status code. Codes 1 to 8 are refusals (bad quantity, bad
price, exceeds holding, book full, day goes backwards, bad fee, nothing held, a
size bound would be passed). The short book adds code 9 (the borrow rate cannot
change while any share is short).

## Accounting identities

Long book:

- shares held = shares bought - shares sold
- cost held = cost bought - cost sold
- realized gross = proceeds - cost sold
- realized net (cash view) = realized gross - every fee paid

Short book:

- shares short = shares sold short - shares covered
- proceeds held = proceeds opened - proceeds covered
- realized gross = proceeds covered - cover cost
- realized net (cash view) = realized gross - fees - borrow cost paid

These are checked by `invariantsHold()` after actions, so a lost share or cent is
caught.

## The mirror law

With the same days, quantities, prices and fees, every status code, the shares
held, the lot counts, the ages and cost basis versus sale proceeds are identical,
and the short realized gross equals **minus** the long realized gross. Use case
22 checks it after each of 1 200 random steps.

## Why risk is not symmetric

A long loss is bounded by its cost, since a price cannot go below one cent. A short
loss is bounded only by how high the price can go. Example: 100 shares short at
5000 cents (proceeds 500 000) covered at 15 000 loses 1 000 000 cents.

## Design choice: refuse, atomically

A refused verb must change nothing. This is tested by comparing shares, cost,
fees, day and lot count before and after each refusal. A book that "mostly"
refuses is a book that cannot be trusted.

## Questions

1. State the invariant linking cost held to purchases and sales.
2. Why is `flatten` on an empty book refused (code 7) instead of ignored?

**Answers:** 1. cost held = cost bought - cost sold. 2. Ignoring hides a mistaken
call; a code makes the mistake visible to the caller and costs nothing.

# Long-Medium-Averages

An adult courseware entry sitting between a single trade and a long-term hold:
**medium-term, actively-worked positions** — a near-term or short-term window
with several buys and a sell, each checked with exact FIFO arithmetic — plus
one concrete rule fixed in this session: **a trading day admits at most 2,000
actions.**

> Illustrative only. Coins, tokens, chips and similar invented units stand in
> for a tradable quantity; no real market, price or order is used. See
> [`../ORGANIZED.md`](../ORGANIZED.md) for the full glossary, including which
> terms from this topic are defined and which are explicitly **not** (most
> importantly, "Flat 2" has no definition yet and is not used here).

## What "medium" means here

"Medium" is read as covering **both** near-term and short-term trades: a
position built and closed over a short window (days), with more than one
action, as opposed to one buy-then-sell pair or a position held for a long
threshold of days. It is a point on the same spectrum as the long-term
material elsewhere in this library, not a new mechanism.

## The class

[`SLDailyCapBook`](./SLDailyCapBook.sleela) is `SLLongBook` (see
`../../../trading-actions-on-verb/SLLongBook.sleela`) with one addition: every
verb (`buy`, `sell`, `hold`, `flatten`) calls `admitAction(day)` before it
changes anything. Once a day has recorded 2,000 actions, any further action on
that same day is **refused** with status code 10, and the action count resets
to 0 when the day changes. All other status codes (0–8) and the exact-integer
bounds are unchanged from `SLLongBook`.

## The 17 entries

Entries 1–16 are short, self-checking stories: two buys a few days apart, then
a sell, covering 16 different invented goods (coins, tokens, chips, blanks,
foils and similar). Entry 17 proves the cap itself at its boundary.

| # | folder | goods | what it shows |
|---|--------|-------|----------------|
| 1 | `01-copper-coins` | copper coins | base case: two buys, FIFO sell, exact profit |
| 2 | `02-silver-tokens` | silver trading tokens | a partial sell across two lots |
| 3 | `03-gold-chips` | gold game chips | a wider day gap between buys and the sell |
| 4 | `04-brass-rounds` | brass rounds | a sell that exactly empties both lots |
| 5 | `05-bronze-medals` | bronze trading medals | a sell larger than either lot alone |
| 6 | `06-nickel-blanks` | nickel blanks | a falling second buy price |
| 7 | `07-tin-pucks` | tin pucks | equal-sized lots, unequal prices |
| 8 | `08-zinc-slugs` | zinc slugs | low unit price, large quantity |
| 9 | `09-steel-washers` | steel washers | the largest quantities in the set |
| 10 | `10-platinum-foils` | platinum foils | the highest unit price in the set |
| 11 | `11-titanium-tabs` | titanium tabs | a mid-size position with fees |
| 12 | `12-iron-tags` | iron tags | a quick two-day turn |
| 13 | `13-quartz-chips` | quartz counting chips | a wider window before the sell |
| 14 | `14-obsidian-flakes` | obsidian trading flakes | a loss on the second lot's share |
| 15 | `15-granite-counters` | granite counters | a rising price path |
| 16 | `16-slate-markers` | slate markers | a falling then rising price path |
| 17 | `17-daily-cap-boundary` | — | the 2000th action is accepted, the 2001st is refused and changes nothing, a new day resets to 0 |

## Assurance

Every entry prints `ok`/`FAIL` per check and ends `checks passed=N failed=0`.
Expected values for entries 1–16 were computed by an independent Python model
of the same FIFO rule and compared with what SLeeLa printed; entry 17 was
checked by hand against the stated rule (2000 accepted, 2001 refused, reset on
day change). `test-suites/test-long-medium-averages.sh` runs all 17 and
compares each with its `.expected.txt`.

```
bash test-suites/test-long-medium-averages.sh
```

## Honest limits

- The 2,000-actions-per-day figure is a teaching bound fixed in this session,
  not a market or regulatory rule.
- "Flat 2" and the other undefined terms from the planning conversation
  (see `../ORGANIZED.md`) are **not** used, approximated, or implied by
  anything in this folder.
- All goods, prices and quantities are invented for the exercise.

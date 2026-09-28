# NUMERAL

The **numerals** — the numbered entries whose values lead to the statistics in
[`STATS.md`](STATS.md). Two numbered sets are defined here: the goods & services
**basket** (atomic numbers) and the abstract **fiduciary accounts** (account
numbers). No real persons are numbered — only basket items and abstract accounts.

- **Basket source:** [`http-3.0/basket_data.json`](http-3.0/basket_data.json)
- **Currency:** USD (ISO 4217 840), values in micro-USD per gram (uUSD/g)
- **Generated:** 2026-09-20

## Basket numerals

Each basket item's **atomic number** is its numeral; its value is the ISO
USD-per-gram figure carried in every HTTP 3.0 packet.

| Numeral (atomic no.) | Name | Kind | uUSD / gram | USD / gram |
|---:|---|:--|---:|---:|
| 1 | Bleached pulp paper | good | 56,249,759 | $56.249759 |
| 2 | Broadband gigabyte | service | 34,243,099 | $34.243099 |
| 3 | Cane sugar | good | 69,987,683 | $69.987683 |
| 4 | Cobalt metal | good | 43,104,945 | $43.104945 |
| 5 | Cured tobacco leaf | good | 29,864,648 | $29.864648 |
| 6 | Freight ton-mile | service | 17,981,345 | $17.981345 |
| 7 | Gold bullion | good | 6,610,716 | $6.610716 |
| 8 | Legal counsel hour | service | 12,259,381 | $12.259381 |
| 9 | Managed cloud-compute | service | 65,041,790 | $65.041790 |
| 10 | Natural rubber | good | 31,566,795 | $31.566795 |
| 11 | Portland cement | good | 24,592,204 | $24.592204 |
| 12 | Raw cotton | good | 65,211,966 | $65.211966 |
| 13 | Roasted coffee | good | 35,009,362 | $35.009362 |
| 14 | Structural steel | good | 35,480,545 | $35.480545 |

## Account numerals

Abstract fiduciary accounts (see [`FIDUCIARY.md`](FIDUCIARY.md)) are numbered
`A-0001 …` and are **abstract positions of value, not persons**. Each account
numeral leads to the statistics defined over its wealth `W(t)` and rates:

| Numeral | Meaning | Statistic it feeds |
|---|---|---|
| `A-{k}` | account identifier | `W(t)` level, `dW/dt`, `d²W/dt²`, bound-crossings |

Populate this set per deployment; the numerals here are the keys, and
[`STATS.md`](STATS.md) defines the statistics computed from them.

# STATS

Statistics derived from the **numerals** in [`NUMERAL.md`](NUMERAL.md). The
basket statistics are computed directly from the 14 basket values; the account
statistics define what is measured over the abstract fiduciary accounts.

- **Generated:** 2026-09-20
- **Units:** micro-USD per gram (uUSD/g); USD shown for readability

## Basket statistics (per-gram value)

| Statistic | uUSD / gram | USD / gram |
|---|---:|---:|
| Count | 14 | — |
| Sum | 527,204,238 | $527.204238 |
| Mean | 37,657,445.6 | $37.657446 |
| Median | 34,626,230.5 | $34.626230 |
| Minimum | 6,610,716 | $6.610716 |
| Maximum | 69,987,683 | $69.987683 |
| Range (max − min) | 63,376,967 | $63.376967 |
| Std. dev. (population) | 19,305,422.7 | $19.305423 |

## Goods vs. services

| Group | Count | Sum (uUSD/g) | Sum (USD/g) | Mean (USD/g) |
|---|---:|---:|---:|---:|
| Goods | 10 | 397,678,623 | $397.678623 | $39.767862 |
| Services | 4 | 129,525,615 | $129.525615 | $32.381404 |
| **All** | **14** | **527,204,238** | **$527.204238** | **$37.657446** |

## Extremes

- **Cheapest per gram:** Gold bullion — $6.610716/g
- **Dearest per gram:** Cane sugar — $69.987683/g

## Account statistics (definitions)

Over the abstract fiduciary accounts (see [`FIDUCIARY.md`](FIDUCIARY.md)), the
following are the statistics each account numeral feeds — measured, not invented:

| Statistic | Definition |
|---|---|
| `W(t)` | current account wealth (bounded `W_min ≤ W ≤ W_max`) |
| `dW/dt` | wealth change per second (bounded `|dW/dt| ≤ R_max`) |
| `d²W/dt²` | wealth acceleration per second² (bounded `|d²W/dt²| ≤ A_max`) |
| lifetime spend | cumulative outflow vs. `W_life` (see Lifetime Bound) |
| bound-crossings | count of insignia raised (Lifetime/Velocity/Acceleration/Floor/Ceiling) |

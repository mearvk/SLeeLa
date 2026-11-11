# advantage/trading — three cross-asset trading models (C core + C++ layer)

The same three business models apply whether the asset is a token or a
mid-century dresser; only the numbers differ (fees, holding cost, days held,
fill odds). This directory implements them as **pure computation** and joins
them to the advantage model so `counsel` can rank approaches.

| Model | Idea | Digital example | Physical example |
|---|---|---|---|
| **Arbitrage** (`tm_arbitrage`) | Same asset, two venues, price gap | DEX → CEX | Regional listing → premium relist |
| **Market making** (`tm_marketmaking`, `tm_quote`) | Be the instant buyer and seller; earn the spread, bear stuck-inventory risk | Order book / AMM quoting | "Cash for estate lots" |
| **Refurbishment** (`tm_refurbishment`) | Buy raw/distressed, improve, sell to a higher tier; result is a *distribution* | Graded numismatic coins | Restored mid-century furniture |

## Files

| File | Role |
|---|---|
| `trade_models.{h,c}` | C core: the three models, validation, advantage bridge |
| `trade_models.{hpp,cpp}` | C++ layer: `Checked<T>` results, `vector` outcomes, the trade-off table as data (`profiles()`), `addEvidence` into an `AdvantageStore` |
| `trading_demo.cpp` | Six approaches (model × asset class), 21 illustrative scenarios, then a ranked counsel |
| `Makefile` | `make` / `make run` / `make clean`. No OpenSSL, no network |

## Build and run

```sh
make run
```

## How it connects to the advantage model

`tm_advantage` expresses a trade as **reward = expected proceeds** against
**baseline = total cost + opportunity cost of the capital** for `hold_days`.
That is the same `advantage_value` the rest of the module uses. `addEvidence`
appends the result as an `<observation>` on a source, so `counsel` scores it
with the usual `mean × confidence × trust`, and doubted assumptions (for
example "listings may be gathered within each platform's terms") discount it.

`Scale::PerUnitCost` divides reward and baseline by total cost, so a $20,000
token trade and a $100 dresser flip are compared per unit of capital instead of
the larger one winning on size. Use `Scale::Absolute` within a single approach.

## Care taken

- Every input must be finite and in range, else an error status is returned and
  the output is **left untouched**: prices/quantities/costs are capped at
  `TM_MAX_AMOUNT` (1e12), rates are in `[0,1)`, probabilities in `[0,1]`,
  `hold_days` ≤ 36,500, at most 16 refurbishment outcomes, and outcome
  probabilities must sum to 1 ± 1e-6.
- Results are re-checked for finiteness before being returned.
- Costs are explicit (fees, slippage, fixed, holding), so a spread that costs
  consume shows up as a loss. `tm_arbitrage_breakeven` gives the break-even
  sell price.
- Market making models the **unfilled** case (stuck inventory dumped at an
  unwind loss), which is where it actually loses.

## Honest limits

- **All demo numbers are illustrative.** They are not quotes, fee schedules or
  forecasts, and the demo's ranking mostly reflects those invented inputs.
- **Scores are per trade, not per year.** Digital trades turn over in seconds
  and physical ones in weeks, so a fair cross-asset comparison also needs
  trades-per-year. That is not modelled yet.
- The models take the *caller's* probabilities (fill odds, grade
  distribution). They do not estimate them.
- **No execution and no data collection.** There is no scraping and no order
  placement here. Feeding the models live quotes over HTTP is a separate
  adapter; it should use the module's allowlisted, TLS-verified transport and
  respect each platform's terms of service and API rules. High-frequency
  digital trading would also want a streaming (WebSocket) feed rather than
  request/response HTTP.
- This is arithmetic, not financial advice.

## Next

SLeeLa source versioning for these models is deferred (planned as a follow-up).

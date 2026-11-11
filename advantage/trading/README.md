# advantage/trading — seven cross-asset trading models (C core + C++ layer)

The same three business models apply whether the asset is a token or a
mid-century dresser; only the numbers differ (fees, holding cost, days held,
fill odds). This directory implements them as **pure computation** and joins
them to the advantage model so `counsel` can rank approaches.

| Model | Idea | Digital example | Physical example |
|---|---|---|---|
| **Arbitrage** (`tm_arbitrage`) | Same asset, two venues, price gap | DEX → CEX | Regional listing → premium relist |
| **Market making** (`tm_marketmaking`, `tm_quote`) | Be the instant buyer and seller; earn the spread, bear stuck-inventory risk | Order book / AMM quoting | "Cash for estate lots" |
| **Refurbishment** (`tm_refurbishment`) | Buy raw/distressed, improve, sell to a higher tier; result is a *distribution* | Graded numismatic coins | Restored mid-century furniture |
| **Regional arbitrage** (`tm_regional`, `tm_regional_premium`) | Buy low in one place, move it, sell high in another; the move can fail or damage the goods | Country-exclusive premium (capital-control barrier) | Estate sale → dense urban resale |
| **Supply aggregation** (`tm_aggregation`) | Collect many small items into a cohesive set; an incomplete set is worth only its parts | Sorted, attributed coin sets | Matched dining-chair sets |
| **Informational asymmetry** (`tm_asymmetry`, `tm_mispricing`) | A listing is mislabeled or misunderstood. One evaluation = one *candidate*: you pay to look every time, buy only if you win the race, and profit only if your read was right | Mispriced listings / panic discounts | "Old wooden table" that is a designer piece |
| **Consignment / zero inventory** (`tm_consignment`) | Match buyers and sellers for a commission; no inventory, almost no capital, but your time and counterparty risk are the real inputs | Local OTC / escrow desk | Staging and sales for people moving out |

## Files

| File | Role |
|---|---|
| `trade_models.{h,c}` | C core: the three models, validation, advantage bridge |
| `trade_models.{hpp,cpp}` | C++ layer: `Checked<T>` results, `vector` outcomes, the trade-off tables as data (`profiles()`, `advancedModelMatrix()`), `addEvidence` into an `AdvantageStore` |
| `trading_demo.cpp` | Fourteen approaches (seven models × digital/physical), 48 illustrative scenarios, both comparison tables, then a ranked counsel |
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
- Models that involve chance report **expected** values (probability-weighted
  cost and proceeds); one real trade lands above or below them.
- The race-and-read model charges the *attempt* every time but the purchase
  only on a win, so "bots win most races" correctly comes out as a loss.
- Consignment's return on cost explodes when little capital is tied up. That
  is real capital efficiency, but it ignores that your time is the scarce
  input (`labor_cost`) and that one reversed transfer (`chargeback_*`) can
  erase many commissions; both are explicit inputs. `counsel` further
  discounts it when its trust assumptions are doubted.
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

## Deliberately not built

- **Header spoofing / User-Agent and proxy rotation** to make automated requests
  look like human browsers and evade rate limits or IP blocks. That is evasion
  of a platform's access controls, so it is out of scope. Use official APIs,
  published feeds and webhooks, honor `robots.txt`, terms and rate limits, and
  identify your client honestly. The module's `bulkfetch` already provides
  parallel, rate-limited, TLS-verified fetching of sources you are authorized
  to read.
- **Automated mass offer messaging** (an auto-POST of pre-written offers to
  sellers' inboxes the moment something is flagged). Unsolicited messaging at
  machine speed is spam/abuse and violates most platforms' rules. A better
  design flags a candidate and **a person decides** what to send.

## Next

SLeeLa source versioning for these models is deferred (planned as a follow-up).

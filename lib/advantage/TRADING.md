# Trading — the Advantage™ trading-models subject

> The SLeeLa port of the eleven trading models, the rate budget, and the
> counsel that ranks them. It mirrors the C and C++ module in
> [`/advantage`](../../advantage) (see its
> [`SUPPORTED.MODELS.md`](../../advantage/SUPPORTED.MODELS.md) for what each
> model is and why it follows from the one before), and every figure here is
> checked against the same hand-computed anchors.

**Directory:** [`trading/`](./trading) · **Demo:** `trading/trading-demo.sleela`
(`TradingDemo`, 93 self-checks)

Everything here is **pure computation**. Nothing fetches a price, scrapes a
page, or places an order. A live operation belongs behind the
[`connect/`](./connect) medium and its host adapters; the `network/` subject
already models venues, scouting and cashflow receipts.

## How it fits the Advantage™ literature

The Advantage™ idea is to *advantage a dividend with respect to a model*. The
trading subject supplies eleven concrete, quantified models whose result is
always the same shape, so they can be compared and ranked:

```
 inputs (prices, fees, odds, days)
        |
        v
 SLTrading<Model>.evaluate(...)  --->  SLTradingResult
        |                              (cost, expected proceeds, profit,
        |                               return on cost, hold days)
        v
 result.advantage(costOfCapital)       reward minus (cost + opportunity cost)
        |
        v
 SLTradingApproach.addTrade(...)       becomes evidence (per unit of capital)
        |
        v
 SLTradingCounsel.rank(...)            score = mean x confidence x trust
```

## Classes

| File | Class | Purpose |
|------|-------|---------|
| `SLTradingResult.sleela` | `SLTradingResult` | Common result; `baseline(rate)` and `advantage(rate)` bridge to the advantage model |
| `SLTradingCheck.sleela` | `SLTradingCheck` | Shared fail-closed validation and finite bounds |
| `SLTradingArbitrage.sleela` | `SLTradingArbitrage` | 1 · Same asset, two venues; `breakeven(...)` |
| `SLTradingMarketMaking.sleela` | `SLTradingMarketMaking` | 2 · Quote both sides; prices the unfilled case; `bid`/`ask` quotes |
| `SLTradingRefurbishment.sleela` | `SLTradingRefurbishment` | 3 · Improve and sell up-tier; outcome distribution |
| `SLTradingRegional.sleela` | `SLTradingRegional` | 4 · Buy here, move, sell there; loss and salvage; `premium(...)` |
| `SLTradingAggregation.sleela` | `SLTradingAggregation` | 5 · Assemble many items into a set; completion probability |
| `SLTradingAsymmetry.sleela` | `SLTradingAsymmetry` | 6 · One candidate listing: pay to look, win the race, be right; `discount(...)` |
| `SLTradingConsignment.sleela` | `SLTradingConsignment` | 7 · Commission with no inventory; labor and chargeback risk |
| `SLTradingDistressed.sleela` | `SLTradingDistressed` | 8 · Buy from urgency; win probability, pursuit cost, unsold remainder |
| `SLTradingVirtual.sleela` | `SLTradingVirtual` | 9 · Sell before sourcing; fulfilment failure and price drift |
| `SLTradingSynthetic.sleela` | `SLTradingSynthetic` | 10 · One into many, or many into one; waste and fallback value |
| `SLTradingMomentum.sleela` | `SLTradingMomentum` | 11 · Event-driven entry; exit failure; `hypeDecay(...)` |
| `SLTradingRateBudget.sleela` | `SLTradingRateBudget` | Wait before the next request, from rate-limit numbers |
| `SLTradingApproach.sleela` | `SLTradingApproach` | One approach: evidence, assumptions held/doubted, honest score |
| `SLTradingCounsel.sleela` | `SLTradingCounsel` | Ranks approaches best-first with cautions |
| `trading-demo.sleela` | `TradingDemo` | Runnable demo + validation (`main`) |

Run it:

```sh
./working-binaries/sleela run lib/advantage/trading/trading-demo.sleela
```

(`run` auto-links the sibling classes, so no imports are needed. Expect
`passed 93, failed 0` and `ALL CHECKS PASSED`.)

## Conventions shared by every model

- **Expected values.** A model involving chance (loss, completion, winning a
  race, a sale happening) reports probability-weighted cost and proceeds, so a
  single real trade lands above or below them.
- **Fail closed.** Every input must be finite and inside its range. NaN fails
  every ordered comparison and infinity fails the finite test (`v - v` is
  NaN), so both are always rejected. A rejected evaluation has `isOk()` false,
  a reason in `getError()`, and every number zero, so it can never be mistaken
  for a real result.
- **Finite bounds.** Amounts up to 1e12, holding up to 36,500 days, at most 16
  outcome branches, 256 aggregation lots, 64 synthetic components.
- **Explicit costs.** Fees, slippage, fixed and holding costs are separate
  inputs, so a spread that costs consume shows up as a loss.
- **Sentinels.** Helper methods that return a bare number (`breakeven`,
  `premium`, `discount`, `bid`/`ask`, `hypeDecay`) return a documented
  out-of-range value (`-1.0`, `-2.0` or `-1000000.0`) on invalid input; use
  the matching `…Ok` check, or the `evaluate` result, to tell the cases apart.

## The advantage bridge

`result.advantage(costOfCapital)` is the expected proceeds minus the baseline,
where the baseline is the total cost **plus the opportunity cost** of the
capital tied up for `holdDays` at the yearly rate. A rate outside 0 to 10 (or
NaN) gives no advantage. `SLTradingApproach.addTrade(result, rate, perUnitCost)`
records it as evidence; with `perUnitCost` true, reward and baseline are
divided by total cost so a large token trade and a small furniture flip compare
per unit of capital instead of by size.

## Counsel

`SLTradingApproach.score(prior)` is `mean advantage x confidence x trust`:

- `confidence = n / (n + prior)`: a few results earn little trust.
- `trust` = assumptions still held / assumptions recorded.
- **No evidence is unknown, not zero:** it scores 0 and is flagged.
- Cautions are plain-language: thin evidence, a disadvantage on average,
  spread larger than the average gain, doubted assumptions.

`SLTradingCounsel.rank(prior)` orders approaches best-first; ties keep
insertion order, so the ranking is deterministic. The demo checks parity with
the C++ module (Genius 1.2, Source/Spring 0.4714, Kid 0.2286).

## The rate budget

`SLTradingRateBudget` turns what a server reports about its rate limit into a
wait: an explicit `Retry-After` always wins, a reserve is held back, the usable
budget is spread evenly over the rest of the window, and a floor interval is
kept. A required wait over the caller's cap means *stop*, not sleep. It
respects the limit; it does not evade one.

**Honest limit:** the SLeeLa `String` type exposes only concatenation and
equality (no character access), so header *text* cannot be parsed in-language.
The caller passes already-parsed numbers (`-1.0` means the header was absent);
strict string parsing lives in the C layer (`rb_parse_nonneg`).

## Honest limits

- **Illustrative anchors only.** No number here is market data, a quote, a fee
  schedule or a forecast.
- **Per trade, not per year.** Digital trades turn over in seconds and physical
  ones in weeks; a fair cross-asset comparison also needs trades per year. Not
  modelled yet.
- **The caller supplies the probabilities** (fill odds, win rates, completion,
  failure). The library does not estimate them.
- **Virtual inventory** models an *honest*, disclosed sale only. Its return on
  cost is on total outlay, not your own capital.
- **Event momentum** treats inability to exit as a first-class outcome; the
  numbers are not a reason to promote a token you hold.
- **Synthetic bundling** with coins: check local law before melting; some
  circulating coins are protected.
- Arithmetic, not financial advice.

## Deliberately not built

- Header spoofing, User-Agent or proxy rotation to evade rate limits or IP
  blocks.
- Automated mass offer messaging to sellers.
- Copying other sellers' photos, stripping text or watermarks, and
  re-uploading them.
- Persistent authenticated sessions and instant-checkout automation across
  third-party merchant dashboards.

## Authoring notes (verified against the toolchain)

- **New arrays start as `null`, not zero.** `new int[n]` / `new double[n]`
  elements must be assigned before they are compared or summed. (`rank` in
  `SLTradingCounsel` initialises its working arrays for this reason.)
- Integer division truncates: `held / total` of two ints is an int, so trust is
  computed as `(held * 1.0) / total`.
- A method may take 13+ parameters and may return a class instance.

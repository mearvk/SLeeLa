# Supported Models

Every model this module supports, what it does, and why it follows from the
one before it (*sequitur*: each model exists because the previous one left a
question open).

All models are **pure computation** in C with a C++ layer. None of them fetches
prices, scrapes anything, or places an order. Callers supply the inputs; the
models return an evaluation.

- Source: `advantage/` (math core, store, counsel), `advantage/trading/` (the
  eleven trading models and the rate budget).
- Everything shown in the demos is **illustrative**. No number in this repo is
  market data, a quote, a fee schedule or a forecast.

---

## 1. THE FOUNDATION: THE ADVANTAGE MODEL

### 1.1 Advantage (the math core)

`advantage.h`: `advantage_value`, `advantage_mean`, `advantage_stddev`,
`advantage_confidence`.

An advantage is what you gained (**reward**) beyond what you would have gained
anyway (**baseline**), scaled by a weight. Alongside it are two honesty
measures: **spread** (how consistent the gains are) and **confidence**
(`n / (n + prior)`, so a few results earn little trust).

*Why it comes first:* every other model is, at bottom, a way of estimating a
reward against a baseline. Without a single definition of "better than
otherwise," the models cannot be compared.

### 1.2 The knowledge store

`advantages.xml`, `advantage_store.*`.

Holds **source advantages**: kinds of leverage, each with the *assumptions* it
depends on, short *knowledge strings*, and recorded *evidence*
(`<observation reward baseline/>`). The five shipped sources are Source/Spring,
Mechanical Advantage, Genius, Kid and Simple Money. An assumption can be marked
`status="doubted"`.

*Why it follows the math core:* a number means little without the beliefs that
produce it. The store keeps those beliefs next to the evidence, and keeps
authored knowledge separate from fetched (untrusted) text.

The loader fails closed on malformed numbers, truncated documents, duplicate
ids and stray elements, and is bounded (16 MiB, 10,000 sources, 100,000
observations per source).

### 1.3 Counsel

`counsel.*`, run with `analyze --counsel`.

Ranks sources by `score = mean advantage × confidence × trust`, where *trust*
is the share of the source's assumptions still held. It also lists what to
verify first (doubted assumptions first) and states plain-language cautions.

*Why it follows the store:* the store holds evidence and assumptions; counsel
turns them into a decision without claiming more than the evidence supports.
No evidence is reported as **unknown, not zero**.

### 1.4 The bridge: trades become evidence

`tm_baseline`, `tm_advantage`, `addEvidence` (C++).

An evaluated trade is expressed as reward = expected proceeds against
baseline = total cost **plus the opportunity cost of the capital** tied up for
the holding time. `addEvidence` records it as an observation. With
`Scale::PerUnitCost` the reward and baseline are divided by total cost, so a
$20,000 token trade and a $100 dresser flip compare per unit of capital rather
than by size.

*Why it matters:* this is what makes the trading models part of one system
rather than a list. Each model below ends in the same place: a `tm_result` that
counsel can rank.

---

## 2. THE TRADING MODELS

All eleven share one result shape (`tm_result`: total cost, expected proceeds,
expected profit, return on cost, hold days) and one set of conventions:

- Models involving chance report **expected values** (probability-weighted),
  so a single real trade lands above or below them.
- **Fail closed:** every input must be finite and in range, results are
  re-checked for finiteness, and on any error the output is left untouched.
- **Finite bounds:** amounts up to 1e12, holding up to 36,500 days, at most 16
  outcome branches, 256 aggregation lots and 64 synthetic components.
- Costs are explicit (fees, slippage, fixed, holding), so a spread that costs
  consume shows up as a loss.

The models are grouped below by the **question each one answers**, in the order
they build on one another.

### A. Where does the edge come from? (price, quote, transformation)

#### 2.1 Arbitrage: `tm_arbitrage`, `tm_arbitrage_breakeven`
*Idea:* the same asset trades at two prices on two venues; buy low, sell high.
*Digital:* a token bought on a DEX and sold on a CEX. *Physical:* a regional
listing relisted on a premium venue.
*Captures:* both-leg fees, slippage, fixed costs, holding cost, and time.
`tm_arbitrage_breakeven` gives the lowest sell price that breaks even.
*Why it is first:* it is the simplest possible edge, a visible price gap, and
it establishes the cost vocabulary (fees, slippage, holding) every later model
reuses.

#### 2.2 Market making: `tm_quote`, `tm_marketmaking`
*Idea:* instead of waiting for a gap, create one by quoting both sides and
earning the spread.
*Digital:* order-book or AMM quoting. *Physical:* buying estate lots for cash,
reselling at market.
*Captures:* the **unfilled** case: inventory that does not turn over is dumped
at an unwind loss. A negative fee models a maker rebate.
*Why it follows arbitrage:* arbitrage needs someone else's mispricing;
market making manufactures the spread, which introduces the first genuinely
new risk, **inventory that does not sell**.

#### 2.3 Refurbishment: `tm_refurbishment`
*Idea:* buy raw or distressed, improve it, sell into a higher tier.
*Digital:* raw coins sent for professional grading. *Physical:* restoring
mid-century furniture.
*Captures:* the result is a **distribution** of outcomes (a list of
probability / value branches that must sum to 1), not a single number.
*Why it follows market making:* the edge here is created by *work*, not by a
quote. Because work can come out several ways, the model needs outcome
distributions, a structure later models reuse.

### B. What if the edge is in space, in pieces, or in what others miss?

#### 2.4 Regional arbitrage: `tm_regional`, `tm_regional_premium`
*Idea:* buy in one place, move it, sell in another.
*Digital:* a country-exclusive premium held up by capital controls.
*Physical:* estate-sale furniture sold in a dense urban market.
*Captures:* transport cost per unit, transit time, and the chance the move
**fails or damages the goods**, with a salvage rate.
*Why it follows arbitrage:* it is arbitrage where "the other venue" is far
away. Moving is itself a risky step, so loss and salvage become inputs.

#### 2.5 Supply aggregation: `tm_aggregation`
*Idea:* collect many small items from fragmented sellers into a cohesive set
and sell the set at a premium.
*Digital:* sorted, attributed coin sets. *Physical:* matched dining chairs.
*Captures:* **completion probability.** An incomplete set sells for little more
than its parts. Takes up to 256 individual lot prices.
*Why it follows refurbishment:* value is created by assembly instead of repair,
and the new risk is that the assembly never finishes.

#### 2.6 Informational asymmetry: `tm_asymmetry`, `tm_mispricing`
*Idea:* a listing is mislabeled, mispriced or misunderstood.
*Digital:* mispriced listings and panic discounts. *Physical:* "old wooden
table" that is a designer piece.
*Captures:* one evaluation is one **candidate**. You pay to look every time,
buy only if you win the race against other buyers, and profit only if your read
of it was right. This is why "bots win most races" comes out as a loss.
*Why it follows the previous two:* aggregation and regional arbitrage work when
the gap is visible; here the gap is *hidden*, so the model has to price the
cost of searching, not just the trade.

### C. What if you remove capital, or exploit someone's urgency?

#### 2.7 Consignment / zero inventory: `tm_consignment`
*Idea:* match buyers and sellers for a commission; hold no inventory.
*Digital:* a local OTC or escrow desk. *Physical:* staging and selling for
people moving out.
*Captures:* almost no capital, so return on cost can look enormous. The real
inputs are your **labor** and **counterparty risk** (a chargeback or reversal),
both explicit.
*Why it follows asymmetry:* after paying to search and compete, the natural
question is whether you can earn without owning anything. The honest answer is
that you swap capital risk for trust risk.

#### 2.8 Distressed inventory: `tm_distressed`
*Idea:* buy from sellers who value speed and certainty over price.
*Digital:* assets dumped by forced liquidations into a thin book.
*Physical:* office-mover, eviction or storage-auction truckloads.
*Captures:* a **win probability** against institutional liquidators, a cost to
pursue every lot, the sell-through rate, and what it costs to dispose of the
rest.
*Why it follows consignment:* consignment serves a seller who wants help;
distressed buying serves a seller who wants to be gone. It reuses the
race-and-pursuit structure from asymmetry and the unsold-remainder idea from
market making.

#### 2.9 Virtual inventory: `tm_virtual`
*Idea:* take the customer's payment first, source the item after.
*Digital:* dealer-fulfilled graded coins. *Physical:* resale of standard items
collected after the buyer has paid.
*Captures:* source price drift between sale and purchase and the probability
you **cannot fulfil**, with the loss when that happens. Return on cost here is
on total outlay, not your own capital, so the failure probability is the number
to stress.
*Why it follows distressed:* distressed buying commits capital early;
virtual inventory reorders the steps so the sale funds the purchase. It is the
logical extreme of capital efficiency, and its price is fulfilment risk.
It assumes an **honest, disclosed** sale.

### D. What if you can change the thing itself, or the clock is the edge?

#### 2.10 Synthetic bundling: `tm_synthetic`
*Idea:* turn one thing into many, or many into one.
*Digital:* wrap/unwrap spreads; breaking up a coin estate into collector pieces
and melt value. *Physical:* assembling salvaged parts into a bespoke piece.
*Captures:* input cost, transform cost per output, **waste**, and a fallback
value (for example melt) for outputs that do not sell. Up to 64 outputs.
*Why it follows virtual inventory:* it generalizes refurbishment (one in, one
better out) and aggregation (many in, one out) into a single structure that
also covers the reverse (one in, many out).

#### 2.11 Event momentum: `tm_momentum`, `tm_hype_decay`
*Idea:* enter on a news or trend shock and exit while demand is elevated.
*Digital:* newly listed or newly newsworthy tokens. *Physical:* vintage pieces
in a style that is going viral.
*Captures:* an exit-price **distribution**, the probability of being unable to
exit at all (often with near-zero recovery), and a half-life for how fast hype
decays.
*Why it comes last:* every earlier model has an edge that persists for the
holding period. This is the one where **time itself is the edge and the
risk**, and it extends the "inventory that does not sell" idea from market
making into a first-class exit-failure probability.
It treats inability to exit as a first-class outcome. In thin markets you can
be the exit liquidity for someone else's promotion; the numbers here are not a
reason to promote a token you hold.

---

## 3. SUPPORTING MODEL: THE RATE BUDGET

`rate_budget.h`: `rb_parse_nonneg`, `rb_observe`, `rb_next_delay_ms`
(C++: `RateBudget`).

Reads `X-RateLimit-Limit`, `-Remaining`, `-Reset` and `Retry-After` and returns
how long to wait before the next request: the usable budget (remaining minus a
reserve) spread evenly over the time left in the window, never below a minimum
interval, and **an explicit `Retry-After` always wins**. A required wait above
the caller's cap returns "stop." Malformed headers are rejected and leave the
state unchanged. It never sleeps and never makes a request.

*Why it is sequitur:* the models that depend on fast-moving data (asymmetry,
distressed, momentum) are exactly the ones tempted to read faster than a server
allows. The rate budget is the discipline that lets them read as fast as is
permitted, and no faster.

---

## 4. HOW THEY FIT TOGETHER

```
 inputs you supply (prices, fees, odds, days)
        |
        v
 one of 11 trading models  --->  tm_result (cost, expected proceeds, profit)
        |
        v
 tm_baseline / tm_advantage  (adds opportunity cost of capital)
        |
        v
 addEvidence  --->  AdvantageStore (observations; assumptions held/doubted)
        |
        v
 counsel:  score = mean advantage x confidence x trust
        |
        v
 ranked approaches, with what to verify first and why to be careful
```

The progression by axis, for orientation:

| Axis | Models |
|---|---|
| Price gap | Arbitrage, Regional arbitrage |
| Created spread | Market making |
| Transformation | Refurbishment, Aggregation, Synthetic bundling |
| Hidden information | Informational asymmetry |
| Seller urgency | Distressed inventory |
| Capital structure | Consignment, Virtual inventory |
| Time | Event momentum |

A reading, not a derivation: the five shipped source advantages are general
kinds of leverage, and the trading models are concrete, quantified instances
of the same idea (insight pays off in *asymmetry*, liquidity in *market making*
and *consignment*, leverage from position in *regional arbitrage*).

---

## 5. LIMITS THAT APPLY TO EVERY MODEL

- **Illustrative data only.** The demos' rankings mostly reflect invented
  inputs.
- **Per-trade, not per-year.** Digital trades turn over in seconds and physical
  ones in weeks; a fair cross-asset comparison also needs trades per year.
  That is not modeled yet.
- **The caller's probabilities.** Fill odds, win rates, completion rates and
  failure rates are inputs. The library does not estimate them.
- **Arithmetic, not advice.** Outputs are only as good as the inputs.
- **No execution and no data collection** are provided here.

### Deliberately not built

- Header spoofing, User-Agent or proxy rotation to evade rate limits or IP
  blocks.
- Automated mass offer messaging to sellers.
- Copying other sellers' photos, stripping text or watermarks, and
  re-uploading them.
- Persistent authenticated sessions and instant-checkout automation across
  third-party merchant dashboards.

### Planned

- SLeeLa source versioning of these models (deferred).
- Annualization (trades per year) for fair cross-asset ranking.

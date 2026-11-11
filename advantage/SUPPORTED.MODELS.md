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

## 1. The Foundation: the Advantage Model

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

## 2. The Trading Models

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

## 3. Supporting Model: the Rate Budget

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

## 4. How They Fit Together

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

## 5. Limits That Apply to Every Model

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

### SLeeLa port

All eleven models, the rate budget, and counsel are ported to SLeeLa in
[`lib/advantage/trading/`](../lib/advantage/trading) (see
[`lib/advantage/TRADING.md`](../lib/advantage/TRADING.md)). The port is
validated by a runnable demo of 93 self-checks against the same hand-computed
anchors, and matches this module's counsel scores. One honest difference: the
SLeeLa `String` type has no character access, so the rate budget there takes
already-parsed numbers instead of header text.

### Planned

- SLeeLa source *versioning* of these models (deferred; to be scoped).
- Annualization (trades per year) for fair cross-asset ranking.

---

## 6. SLeeLa-Native Position and Margin Models (lib/advantage)

A separate, newer family of SLeeLa-native models, distinct from the eleven
trading models above and from their 1:1 port. Where the models in sections 1-2
evaluate a **single prospective trade** against a baseline, this family models
an **ongoing position book**: a sequence of buy/sell (or short/cover) actions
over time, with exact FIFO lot accounting, and the margin arithmetic a short
position needs. They share this module's conventions (fail closed, finite
bounds, illustrative data only, no execution) but use verbs and status codes
rather than a `tm_result`/evidence/counsel pipeline, so they are documented
here as their own family rather than folded into the eleven above.

Location: [`lib/advantage/trading-actions-on-verb/`](../lib/advantage/trading-actions-on-verb)
(engineering detail: [`ENGINEERING.md`](../lib/advantage/trading-actions-on-verb/ENGINEERING.md),
checking: [`ASSURANCE.md`](../lib/advantage/trading-actions-on-verb/ASSURANCE.md)).
Companion courseware: [`lib/advantage/learner-vs-professional/`](../lib/advantage/learner-vs-professional/README.md).

### 6.1 Long position book: `SLLongBook`

A long-only FIFO lot book with four verbs: `buy`, `sell` (oldest lot first),
`hold` (let time pass) and `flatten` (sell everything held). A sell larger
than the holding is refused (status 3) and never becomes a short; every
refusal leaves the book unchanged. Tracks realized gross/net profit (net is a
cash view: proceeds minus cost sold minus every fee paid), and time-on-long
queries (age, FIFO-aware weighted average age, a strict "more than N days"
term boundary, and the shares held past an age limit for a time-stop loop).
`invariantsHold()` checks conservation of shares and cost after every action.

### 6.2 Short position book: `SLShortBook`

The mirror of `SLLongBook`: `shortSell`, `cover` (oldest lot first), `hold`,
`coverAll`. Short only (a cover above the shares short is refused, status 3,
never becomes a long). Adds a borrow cost (cents per share per day, frozen
while any share is short) and `markToMarket`. The **mirror law**: given the
same days, quantities, prices and fees, every status code, share/lot counts,
ages and cost-basis-versus-proceeds match the long book, and short realized
gross is exactly the negative of long realized gross — checked over 1,200
random actions per run (use case 22) against the long book itself, and
separately against an independent per-share shadow model (use case 21).

### 6.3 Short margin: `SLShortMargin`

Illustrative margin arithmetic, explicitly not a broker or regulator rule: the
required percentage is a caller-supplied basis-points parameter. Computes
equity (collateral minus short value), the required margin as an **exact
ceiling** of value times percent (computed without ever forming a product
that could exceed the runtime's exact-integer-comparison limit), the deposit
that clears a call, the fewest shares to cover to clear a call (binary search,
checked against a linear scan and for minimality — and against a negative-
equity case where no cover can help), and the first day a call becomes due
under ongoing borrow cost (closed form, checked against day-by-day stepping).

### 6.4 Careful integer toolkit: `SLEuclid`

Supporting integer routines built for this family and reused by it: `gcd`,
`gcdAll`, `lcm`, extended Euclid (`egcd`, with a Bezout-identity check),
exact two-lot-size order fills in logarithmic time (`canFill`, with a linear
`canFillScan` kept as a reference), an exact integer square root (`isqrt`),
and squared Euclidean distance / nearest-target selection (`distSq`, `dist`,
`nearestIndex`). Every routine states its loop invariant and termination
argument, and every bound is sized against the runtime's **measured**
integer-comparison exactness limit (exact only up to 2^53 - 1, not the full
64-bit range) rather than assumed; out-of-range input is refused, never
silently clamped or wrapped.

### 6.5 Daily action cap: `SLDailyCapBook`

A teaching variant of `SLLongBook` (in the courseware, not the core trading
folder) with one added rule: a single day may record at most 2,000 actions
across `buy`/`sell`/`hold`/`flatten`; the 2,001st is refused (status 10) and
changes nothing, and the count resets when the day changes. This is a fixed
teaching bound, not a market or regulatory limit.

### Checking

26 use cases exercise 6.1-6.4 (`test-suites/test-trading-actions.sh`); 17
courseware entries exercise 6.5 (`test-suites/test-long-medium-averages.sh`).
Expected values for the deterministic cases were independently computed in
Python and compared against the SLeeLa output; the random/differential cases
(6.1-6.2 mirror and shadow tests) were cross-checked the same way. Several
deliberately broken copies of these classes (an off-by-one in a rounding
step, a sign flip, an allowed-one-too-many bound, a non-terminating search)
were each confirmed to make the gate fail before the fix was accepted.

### Honest limits of this family

- Illustrative only: no order is placed, no market is read, and the margin
  model in particular ignores fees and real price paths.
- The "long term" boundary and the 2,000-actions-per-day cap are both fixed,
  stated constants for teaching, not real-world thresholds.
- The exact-integer-comparison limit (2^53 - 1) was measured against one
  build of the SLeeLa runtime, not proven from the runtime's own source.
- Not yet connected to the eleven-model/evidence/counsel pipeline in sections
  1-5; there is no current path from a `tm_result` into one of these books.

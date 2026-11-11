# Profitability Systems Index

An index of every profitability-related model that genuinely exists in this
repository: what it computes, where it lives, how it is checked, and —
explicitly — what it does **not** compute. This document indexes code that is
already written and tested. It does not introduce a new model or a new
number.

Written precisely and without padding, on the premise that a careful reader
deserves an accurate map rather than an impressive-sounding one.

## 1. Why this document exists, and a boundary it holds

A natural question after building several profitability models is: can one of
them be scaled up — to a city, a population, a time horizon of years, a
currency, a public office? This index answers that question directly: **no,
not as they stand, and not honestly.** Every model below computes a bounded,
specific quantity from caller-supplied inputs (a single trade's cost and
proceeds, a position book's realized profit, a margin requirement). None of
them estimates a population-wide or government-scale aggregate, a per-second
income rate, or a flow of money to a named office such as a mayor. Composing
several bounded, illustrative, single-trade models into a "profit per second
to a city of 380,000" figure would not be a derivation — it would be an
invented number wearing this repository's vocabulary. This index will not
produce one. It instead gives, as accurately as the actual code allows, what
each system is good for and where its scope ends.

## 2. Two implementations, one lineage

| | C/C++ core | SLeeLa port / native |
|---|---|---|
| single-trade evaluation | §3.1 `advantage/` | §3.2 `lib/advantage/trading/` |
| position books, margin | — (not ported; SLeeLa-native) | §4 `lib/advantage/trading-actions-on-verb/` |
| evidence + ranking | §3.1 `counsel.*` | §3.2 `SLTradingCounsel` |
| full model and lineage write-up | [`SUPPORTED.MODELS.md`](./SUPPORTED.MODELS.md) | same document, §6 |

This index is a map; [`SUPPORTED.MODELS.md`](./SUPPORTED.MODELS.md) is the
canonical description of why each model exists and how it follows from the
one before it. Where the two disagree, `SUPPORTED.MODELS.md` governs.

## 3. Single-trade evaluation (eleven models, one baseline, one ranking)

### 3.1 C/C++ core (`advantage/`)

Eleven models (`trade_models.{c,h}`, namespace by idea: arbitrage, market
making, refurbishment, regional arbitrage, supply aggregation, informational
asymmetry, consignment, distressed inventory, virtual inventory, synthetic
bundling, event momentum) each evaluate **one prospective trade** into a
common `tm_result`: total cost, expected proceeds, expected profit, return on
cost, holding days. `tm_baseline`/`tm_advantage` turn a result into an
*advantage* — reward above what capital would have earned otherwise — which
`addEvidence` records into `advantage_store` (`advantages.xml`). `counsel.*`
ranks recorded sources by `mean advantage x confidence x trust` and flags
assumptions to verify first. `rate_budget.*` paces repeated reads against a
server's own rate-limit headers; it never issues a request itself.

**Computes:** the expected profit of one trade, given inputs you supply, and
a ranking of recorded approaches by historical evidence. **Does not compute:**
a rate of income, a forecast, or anything about a geography or a population.

### 3.2 SLeeLa port (`lib/advantage/trading/`)

A 1:1 port of §3.1's eleven models plus `SLTradingCounsel`, validated by
`trading-demo` (93 self-checks against the same hand-computed anchors as the
C++ version; see [`TRADING.md`](../lib/advantage/TRADING.md)). Same inputs,
same `tm_result` shape (as `SLTradingResult`), same scope and the same limits.

### 3.3 Example tiers over the same models

`lib/advantage/examples/{novice,medium,advanced,professional}` and
`lib/advantage/medium-results/` apply §3.2's models to specific invented
scenarios (a US small business, cross-venue commodity routing, a guardrailed
first trade, twelve goods from lumber to smartphones). These are worked
examples of the same bounded single-trade arithmetic, not a new model and not
a scaled-up one; see [`EXAMPLES.md`](../lib/advantage/EXAMPLES.md).

## 4. Position books, verbs and margin (`lib/advantage/trading-actions-on-verb/`)

A separate SLeeLa-native family (not a port of §3) that models an **ongoing
position** — a sequence of buy/sell or short/cover actions over time — rather
than one prospective trade. Full detail in
[`SUPPORTED.MODELS.md` §6](./SUPPORTED.MODELS.md#6-sleela-native-position-and-margin-models-libadvantage)
and in the folder's own
[`README.md`](../lib/advantage/trading-actions-on-verb/README.md) /
[`ENGINEERING.md`](../lib/advantage/trading-actions-on-verb/ENGINEERING.md) /
[`ASSURANCE.md`](../lib/advantage/trading-actions-on-verb/ASSURANCE.md).

| class | computes | does not compute |
|---|---|---|
| `SLLongBook` | realized profit of a long-only FIFO lot book; age/term queries | anything beyond the book's own recorded actions |
| `SLShortBook` | the mirror: short-only FIFO book, borrow cost, mark-to-market | a loss ceiling (none exists on the short side; see §6.2 of `SUPPORTED.MODELS.md`) |
| `SLShortMargin` | equity, the ceiling-rounded margin requirement, a call amount, the fewest shares to cover, the first day a call falls due | any broker's or regulator's actual rule — the percentage is a caller-supplied parameter |
| `SLEuclid` | gcd/lcm/Bezout, exact two-lot order fills, integer square root, nearest-target distance — the arithmetic the books above depend on | anything about trading by itself |
| `SLDailyCapBook` (courseware) | the same as `SLLongBook`, plus a fixed 2,000-actions-per-day teaching rule | a real exchange's or regulator's throughput limit |

Checked by 26 use cases (`test-suites/test-trading-actions.sh`) plus 17
courseware entries (`test-suites/test-long-medium-averages.sh`): self-checks,
an independent Python oracle, differential tests against two independent
shadow implementations, and deliberately broken copies confirmed to fail the
gate before being fixed.

## 5. Grading, not payout: `SLSophisticationCircuit` (`lib/advantage/grading/`)

Worth naming explicitly because "grades" and "leading technician" appear in
how this topic has been discussed. `SLGradeBook`, `SLModuleGrade` and
`SLSophisticationCircuit` grade the **sophistication of a software module**
(a 1–10 tier and an S/A/B/C/D/F letter, derived from a raw score the caller
supplies) for use inside this library's own self-grading demos. This is a
code-quality rubric, not a trader-skill tier and not a payout schedule. It has
no relationship to a "Grade 1–5 trading technician" acting on a city's behalf;
no such construct exists in this codebase.

## 6. Why a city, a mayor, or a multi-year/multi-currency extrapolation is out of scope

Stated plainly, because an index that silently omits a question invites the
reader to assume an answer exists elsewhere. It does not. Specifically:

- **No model here has a per-second or continuous-time output.** Every result
  in §3–4 is per trade, per position-book action, or per margin check at a
  stated moment; none integrates to a rate.
- **No model has a population or jurisdiction as an input.** None of these
  types accepts a city, a population count, or a public office as a
  parameter, and extending one to accept such an input would require
  inventing the relationship between individual trades and a city's finances
  — a relationship this library does not model and should not pretend to.
- **Composing bounded models does not produce an unbounded claim honestly.**
  Chaining several single-trade evaluations and multiplying by 380,000 would
  produce a number that *looks* derived but encodes no real assumption about
  participation rate, capital available, market depth, or whether the trades
  could even occur at that scale without moving the market the models assume
  is fixed.
- **A flow of money to a named public office is not a thing to approximate.**
  Even framed as illustrative, a document stating a cents-per-second figure
  "to the City's Mayor" could be read, quoted, or reused as a real financial
  claim about a real official. That risk does not shrink by adding
  disclaimers, so the number is not produced here at all.
- **Currency and multi-decade extrapolation compound the same problem.**
  Converting an invented per-trade number into Korean won or Japanese yen, or
  projecting it across seconds through decades, would add apparent precision
  to a quantity that was never grounded in the first place.

If a real question sits behind this — for instance, "how would one scale a
*specific, named* trading strategy's expected return given a *specific*
capital base and a *specific* time horizon" — that is answerable with §3's
existing tools (supply real inputs to a specific model, read the `tm_result`,
and report the single-trade answer honestly scaled by the capital actually
deployed). It is not answerable by assuming a city's population, a mayor's
share, or a multi-decade compounding rate, because none of those quantities
exist anywhere in this codebase's inputs.

## 7. Reading paths

- **Want the full lineage and reasoning for each model?**
  [`SUPPORTED.MODELS.md`](./SUPPORTED.MODELS.md).
- **Want to run something?** `advantage/trading/trading_demo.cpp` (C++),
  `lib/advantage/trading/trading-demo.sleela` (SLeeLa port),
  `lib/advantage/trading-actions-on-verb/*/*.sleela` (position books and
  margin, run any of the 26 use cases directly).
- **Want the courseware version?**
  [`lib/advantage/learner-vs-professional/README.md`](../lib/advantage/learner-vs-professional/README.md).
- **Want the honest limits in one place?** §5 of
  [`SUPPORTED.MODELS.md`](./SUPPORTED.MODELS.md#5-limits-that-apply-to-every-model)
  and §6 of this document.

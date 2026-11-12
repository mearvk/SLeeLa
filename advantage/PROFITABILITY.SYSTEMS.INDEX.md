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
| `SLBatchReturn` | the exact total return over a stated batch of N trades (idealized win-rate/size inputs, or an explicit per-trade list), with no compounding, plus the exact per-trade average a stated total implies | whether any real trader achieves the stated inputs — see §6.1 |

Checked by 27 use cases (`test-suites/test-trading-actions.sh`) plus 20
courseware entries in `long-medium-averages`
(`test-suites/test-long-medium-averages.sh`): self-checks, an independent
Python oracle, differential tests against independent shadow
implementations, and deliberately broken copies confirmed to fail the gate
before being fixed. A later general code-quality review (not driven by any
use case) found and fixed two real overflow bugs in `SLBatchReturn` itself —
a magnitude-based rejection sentinel that could equal a real, legitimate
large-batch loss, and a basis-point conversion that still overflowed after
that fix — both confirmed genuine by reverting them and watching new
boundary-case regression tests fail with the original symptom; see
[`ASSURANCE.md`'s second review log](../lib/advantage/trading-actions-on-verb/ASSURANCE.md#review-log-general-code-quality-pass).

### 4.1 Long-term potencies (`lib/advantage/learner-vs-professional/courseware/long-term-potencies/`)

A small courseware-only class, `SLTermPotency`, that **defines** a term not
used anywhere else in this repository rather than letting it float free of a
computed meaning. Two basis-point ratios, each read-only over an existing
`SLLongBook` or `SLShortBook` (overloaded for both, resolved by argument
type):

| metric | formula | reads |
|---|---|---|
| **term potency** | (units held/short that are long term) / (units held/short), in bp | `qtyLongTerm` / `totalQty` or `totalShort` |
| **age potency** | `weightedAgeFloor` / `oldestAge`, in bp | both age queries, shared by either book |

`SLShortBook.isLongTerm`/`qtyLongTerm` did not exist until this class needed
them on the short side; they were added mirroring `SLLongBook`'s wording and
behavior exactly, and checked to produce identical readings to the long side
at identical lot structure and ages (entry 5 of 6 in that folder) before
being trusted. Checked by 6 entries
(`test-suites/test-long-term-potencies.sh`): self-checks, an independent
Python oracle (including ten randomly generated portfolios in entry 4), and
the long/short cross-check above.

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

## 6.0 `SLBatchReturn`: a real answer to "what would N trades return"

Built to replace an unverified claim with a checked computation. Given
stated inputs — either an idealized win-rate and per-trade win/loss size, or
an explicit list of per-trade percent moves (up to 64) — it computes the
exact batch total, in basis points, with no compounding (every trade
re-stakes the same capital, modeling N independent fixed-size trades). It
also computes the exact per-trade average a stated total implies
(`averagePerTradeBp`), and cross-checks an explicit batch against real
`SLLongBook` buy/sell pairs (`crossCheckOutcome`), distinguishing "the sums
matched" from "a trade could not be represented" (for example a -100% move,
which `SLLongBook` correctly refuses as a zero-cent sell price) from a
genuine mismatch. See use case 27
(`27-batch-return/TaUse27BatchReturn.sleela`) and
[reading/ages-18-plus/07-batch-vs-per-trade.md](../lib/advantage/learner-vs-professional/courseware/reading/ages-18-plus/07-batch-vs-per-trade.md).
It computes what a stated assumption implies; it does not and cannot confirm
the assumption is true of any real trader.

## 6.1 The estimated-return table that was asked for, and why it stops at one trade

A chart of hypothetical trading percentages was requested during planning:
a trader's "good day" return of 2% to 17%, and a "microtrade advantage"
multiplier of 2x up to 12x or more. It is included here, computed exactly
with the real [`SLLongBook`](../lib/advantage/trading-actions-on-verb/SLLongBook.sleela)
for one invented trade of $10,000 (1,000,000 cents), because the arithmetic
itself is the useful part — not because the percentages are realistic.

| return assumed on one trade | sell price (cents, from a 100-cent buy) | profit (cents) | capital afterward, as a multiple |
|---:|---:|---:|---:|
| 2% ("a good day," low end, as given) | 102 | 20,000 | 1.02x |
| 5% ("a good day," mid) | 105 | 50,000 | 1.05x |
| 10% ("a good day," high-ish) | 110 | 100,000 | 1.10x |
| 17% ("a good day," high end, as given) | 117 | 170,000 | 1.17x |
| 100% ("microtrade advantage," 2x) | 200 | 1,000,000 | 2.00x |
| 300% ("microtrade advantage," 4x) | 400 | 3,000,000 | 4.00x |
| 1,100% ("microtrade advantage," 12x) | 1,200 | 11,000,000 | 12.00x |

No row is estimated by a model; each is `profit = capital x return%` for one
stand-alone trade, with no repetition and no compounding. "Estimated" here
means *assumed as a hypothesis to test*, not derived from evidence.
No naming scheme (for example "Status I" or "Convenient II") is attached to
this table: that terminology could not be verified as established usage, and
real, unrelated academic terms that sound similar (the "convenience yield" /
reserve-currency "status" of a safe asset, in the Treasury-bond literature)
measure a safety-and-liquidity premium, not a trader's return, so borrowing
either name here would misstate what the table is.

**Why this does not become a daily rate, a yearly rate, or anything bigger:**
compounding any of these percentages across trading days shows the
assumption breaks down almost immediately — 2% a day compounds to about
1,377x in one year, and 17% a day compounds to over 100x in 30 trading days,
both figures outside what any real market or strategy sustains. The full
compounding table, independently cross-checked, and the reasoning behind it
are in
[reader 6 of the adult courseware](../lib/advantage/learner-vs-professional/courseware/reading/ages-18-plus/06-single-trade-sensitivity.md).
This is also why §6's refusal to extrapolate to a city, a mayor, a currency
or a decade stands: the per-trade numbers above are already shown, by direct
computation, not to survive being repeated — so there is no sustainable rate
left to extrapolate in the first place.

## 7. Reading paths

- **Want the full lineage and reasoning for each model?**
  [`SUPPORTED.MODELS.md`](./SUPPORTED.MODELS.md).
- **Want to run something?** `advantage/trading/trading_demo.cpp` (C++),
  `lib/advantage/trading/trading-demo.sleela` (SLeeLa port),
  `lib/advantage/trading-actions-on-verb/*/*.sleela` (position books, margin
  and batch returns, run any of the 27 use cases directly), or
  `lib/advantage/learner-vs-professional/courseware/long-term-potencies/*/*.sleela`
  (term and age potency, 6 entries).
- **Want the courseware version?**
  [`lib/advantage/learner-vs-professional/README.md`](../lib/advantage/learner-vs-professional/README.md).
- **Want the honest limits in one place?** §5 of
  [`SUPPORTED.MODELS.md`](./SUPPORTED.MODELS.md#5-limits-that-apply-to-every-model)
  and §6 of this document.

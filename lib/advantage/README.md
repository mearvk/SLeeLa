artifact;
pure;
gasoline;

<!-- preonem: this document is an enacted artifact of the advantage
     legislature, graded unto time and space. See PREONEM.md. -->

# Advantage™ — the SLeeLa library of advantage

> **Advantage™** is the SLeeLa standard-library family for *advantaging a
> unique dividend with respect to a model*. The name is a trademark of the
> SLeeLa project (MEARVK LLC). Where the toolchain prints its product identity
> to an interactive terminal, **Advantage™** is rendered in the customary
> **red** (ANSI SGR 31, red + classic), the same treatment given to **SLeeLa™**
> and **Sleelvac™** — presentation-only and TTY-gated, so captured or piped
> output stays plain and machine-parsable.

**Advantage™** turns a vague intuition — "work this to our advantage" — into a
concrete, inspectable, testable process. At its heart is the **Sequitur onion
model**: an ordered sequence of layers, wrapped like an onion around a value
(the *dividend*), that you navigate inward toward the core and outward toward
the skin, folding each layer's signed contribution into the running value. Over
that spine sit five subject families — trade economics, trust-field physics,
vertical integration, sophistication grading, and a routable output seam — each
a self-contained, validated module with its own literature.

**Documentation (the Advantage™ literature):**

- [`DEFINITIONS.md`](./DEFINITIONS.md) — the consolidated glossary of every
  term across the whole package.
- [`MATH.md`](./MATH.md) — the bilateral trade-economics subject (joint profit
  maximization, Nash bargaining, the bilateral newsvendor critical ratio).
- [`PHYSICS.md`](./PHYSICS.md) — the trust-field "physics" subject (metric
  tensor, transactional geodesic, announcement contact structure, trust-decay
  SDE), a faithful mathematics presented as an explicit modelling analogy.
- [`EXAMPLES.md`](./EXAMPLES.md) — runnable guide to all six subject demonstrations, expected outputs, and suggested learning sequence.
- [`TRADING.md`](./TRADING.md) — the trading-models subject: eleven models
  (arbitrage, market making, refurbishment, regional, aggregation, asymmetry,
  consignment, distressed, virtual, synthetic, momentum), a rate budget, and
  the counsel that ranks them; mirrors the C/C++ module in `/advantage`.
- [`trading-actions-on-verb/`](./trading-actions-on-verb) — trading as verbs on a
  long-only FIFO book and its short-only mirror: twenty-two use cases on the
  style of trading long and short and on time held, built on careful Euclid
  loops and Euclidean distance, with differential stress tests, an exhaustive
  Euclid check and a long/short mirror law.
- [`PREONEM.md`](./PREONEM.md) — the preonem standard under which every
  document here is an enacted `artifact; pure; gasoline;` of the Advantage™
  legislature, graded unto time and space.
- [`examples/`](./examples) — runnable examples organized by level of
  sophistication — [`novice/`](./examples/novice), [`medium/`](./examples/medium),
  [`advanced/`](./examples/advanced), [`professional/`](./examples/professional) —
  and within each level grouped by topic (onion model, trade economics, output
  routing, supply chains, trust fields, vertical integration, sophistication
  grading).

## The Sequitur onion

"Sequitur" means *it follows* — each layer follows the previous in logical
order. The layers form an **onion**, outer skin to inner core:

| depth | layer | kind | role |
|------:|-------|------|------|
| 0 | `dividend` | value | the value under consideration |
| 1 | `review` | validate | look again; confirm the premise |
| 2 | `concerns` | risk | surface risks and caveats |
| 3 | `maths` | quant | the quantitative core |
| 4 | `additions` | grow | what the layer adds to the dividend |
| 5 | `subtraction` | shrink | what the layer takes away |
| 6 | `regrouping` | reorg | reorganize concepts; carry/borrow across the model |
| 7 | `iteration` | loop | decide to go further down or back up the onion |

## The Advantage™ subjects at a glance

Advantage™ is organized as a spine (the onion) plus eight subject families. Each
subject is a directory of SLeeLa classes with a runnable demo and, for the two
quantitative ones, a full prose treatment.

| Subject | Directory | What it advantages | Literature |
|---------|-----------|--------------------|------------|
| **Onion model** | (root) | a unique dividend, by a layered, navigable process | this README · [`DEFINITIONS.md`](./DEFINITIONS.md) |
| **Trade economics** | [`math/`](./math) | a surplus shared between a buyer and a seller | [`MATH.md`](./MATH.md) |
| **Trust-field physics** | [`physics/`](./physics) | trust as geometry — distance, curvature, decay | [`PHYSICS.md`](./PHYSICS.md) |
| **Vertical integration** | [`integration/`](./integration) | a value chain, as recursively composable compartments | [`DEFINITIONS.md` §8](./DEFINITIONS.md#8-vertical-integration-subject-integration) |
| **Sophistication grading** | [`grading/`](./grading) | the ranking of modules and whole sets, A–D | [`DEFINITIONS.md` §9](./DEFINITIONS.md#9-sophistication-grading-subject-grading) |
| **Output seam** | [`output/`](./output) | where a module's own voice goes | [`DEFINITIONS.md` §10](./DEFINITIONS.md#10-output-seam-output) |
| **Network trades** | [`network/`](./network) | buy/sell opportunities found and negotiated across a venue network | [`examples/README.md`](./examples/README.md) |
| **Connectivity medium** | [`connect/`](./connect) | a uniform, fault-handled bridge to MySQL, national clocks, networks, and money sources | [`connect/README.md`](./connect/README.md) |
| **Trading models** | [`trading/`](./trading) | an asset position, across eleven models, ranked by evidence and trust | [`TRADING.md`](./TRADING.md) |

Read [`DEFINITIONS.md`](./DEFINITIONS.md) first for the vocabulary; then the
subject documents for depth; then run the demos and primers to see each one
prove itself.

## Classes

| File | Class | Purpose |
|------|-------|---------|
| `SLPackage.sleela` | `SLPackage` | Package facade (compiler / Loader / Nordshrift) |
| `SLAdvantageLayer.sleela` | `SLAdvantageLayer` | One concentric onion layer (depth, name, kind, contribution) |
| `SLAdvantageDividend.sleela` | `SLAdvantageDividend` | The unique dividend held with respect to a named model |
| `SLAdvantageSequitur.sleela` | `SLAdvantageSequitur` | The ordered, navigable onion; cursor moves inward/outward; runs passes |
| `SLAdvantageUserModel.sleela` | `SLAdvantageUserModel` | **User's model** — simple, outcome-facing encapsulation |
| `SLAdvantageDeveloperModel.sleela` | `SLAdvantageDeveloperModel` | **Developer's model** — full-control encapsulation of the process |
| `advantage-demo.sleela` | `AdvantageDemo` | Runnable demo + validation of the onion model (`main`) |
| `math/` (subfolder) | — | **Bilateral trade economics** subject — see below and [`MATH.md`](./MATH.md) |

### `math/` subfolder — trade economics subject

| File | Class | Purpose |
|------|-------|---------|
| `math/SLJointProfitMax.sleela` | `SLJointProfitMax` | Joint profit maximization — integrated surplus `Π_total(Q)` and `Q*` where `MR = MC` |
| `math/SLNashBargaining.sleela` | `SLNashBargaining` | Bilateral-monopoly Nash bargaining split by power index `α` over reservation profits |
| `math/SLBilateralNewsvendor.sleela` | `SLBilateralNewsvendor` | Overage/underage costs on both ends + the centralized critical ratio |
| `math/SLTradeMath.sleela` | `SLTradeMath` | Self-contained `expD` / `lnD` / `powAB` / `sqrtD` helpers (no cross-package imports) |
| `math/trade-math-demo.sleela` | `TradeMathDemo` | Runnable demo + validation of all three models (`main`) |

Full definitions, explanations, ASCII sketches, and worked examples live in
[`MATH.md`](./MATH.md).

### `physics/` subfolder — trust-field economics subject

A structural-economics subject that models trust and announcements **as**
differential-geometry / contact-topology / stochastic objects (an analogy,
flagged as such; the mathematics is implemented faithfully).

| File | Class | Purpose |
|------|-------|---------|
| `physics/SLTrustMetric.sleela` | `SLTrustMetric` | Trust as a metric tensor `g_μν`; conformal warping of operational distance |
| `physics/SLTransactionalGeodesic.sleela` | `SLTransactionalGeodesic` | Christoffel symbols + geodesic integration of the transaction path |
| `physics/SLAnnouncementContact.sleela` | `SLAnnouncementContact` | Contact 1-form `α`, the `α∧dα≠0` test, hyperplane restriction, homotopy-null check |
| `physics/SLTrustDecaySDE.sleela` | `SLTrustDecaySDE` | CIR mean-reverting diffusion + Poisson jump (announcement breach) |
| `physics/SLPhysicsMath.sleela` | `SLPhysicsMath` | Self-contained `expD`/`lnD`/`powAB`/`sqrtD` (distinct name avoids collision) |
| `physics/physics-demo.sleela` | `PhysicsDemo` | Runnable demo + validation of all four models (`main`) |

Full definitions, explanations, ASCII sketches, and worked examples are in
[`PHYSICS.md`](./PHYSICS.md).

### `integration/` subfolder — vertical-integration subject

A sophisticated modelling option for **vertical integration through
capitalism / retail**: a stack of value-chain tiers, each a recursively
composable **compartment** with its own capacitive space, meaning, and
relevance (OOD). Mainly a vertical arrangement (upstream → downstream); a tier
may embed its own sub-model.

| File | Class | Purpose |
|------|-------|---------|
| `integration/SLIntegrationKind.sleela` | `SLIntegrationKind` | Compartment-kind vocabulary: nested-model, onion, engineering-bay, trial, newton-tray, and plain holders (resources, mediums, hobbies, games, entertainment, processes, consumption, trading, gaining, selling, forcing, challenging) |
| `integration/SLCompartment.sleela` | `SLCompartment` | The recursive, self-similar node (kind, capacitive space, meaning, relevance, child compartments) |
| `integration/SLEngineeringBay.sleela` | `SLEngineeringBay` | Medium-capacity build/process compartment (load → build at a yield) |
| `integration/SLTrialCompartment.sleela` | `SLTrialCompartment` | Study/experiment compartment (log trials, success rate) |
| `integration/SLNewtonMedianTray.sleela` | `SLNewtonMedianTray` | Median-based enlightenment/insight tray |
| `integration/SLVerticalIntegration.sleela` | `SLVerticalIntegration` | The ordered tier stack: vertical flow, recursion, aggregate capacity/relevance, system-sophistication score |
| `integration/integration-demo.sleela` | `IntegrationDemo` | Runnable demo + validation (`main`) |

Defined term-by-term in [`DEFINITIONS.md`](./DEFINITIONS.md#8-vertical-integration-subject-integration).

### `grading/` subfolder — sophistication circuit grader

A grader that scores **each module** and rates **any set of modules** —
winning, losing, grader, and absolute sets — with letter + tier ratings.

| File | Class | Purpose |
|------|-------|---------|
| `grading/SLModuleGrade.sleela` | `SLModuleGrade` | One graded module: score → 1–10 tier + S/A/B/C/D/F letter; role (normal/grader/absolute) |
| `grading/SLGradeSet.sleela` | `SLGradeSet` | A named set of modules with its own aggregate rating (mean/best/worst, set letter+tier) |
| `grading/SLSophisticationCircuit.sleela` | `SLSophisticationCircuit` | The circuit grader: register modules, apply win/lose/absolute thresholds, partition into sets, find the champion |
| `grading/SLGradeBook.sleela` | `SLGradeBook` | Top-level: per-module & per-set ratings, overall field rating, win rate, rating of any supplied set |
| `grading/grading-demo.sleela` | `GradingDemo` | Runnable demo + validation (`main`) |

Defined term-by-term in [`DEFINITIONS.md`](./DEFINITIONS.md#9-sophistication-grading-subject-grading).

### `output/` subfolder — output seam (terminal / CommonRails / socket)

So modules need not stay silent: a selectable output sink that routes rendered
lines to the **terminal**, a **CommonRails**-formatted surface (via the
toolchain's `PrintRail`), a **socket/connector** (framed + buffered for a
native `slnet_write`), or a silent capture.

| File | Class | Purpose |
|------|-------|---------|
| `output/SLAdvantageSink.sleela` | `SLAdvantageSink` | The sink: target (terminal/commonrails/socket/silent), endpoint, capture buffer, `drain()`/`lastFrame()` |
| `output/SLAdvantageEmitter.sleela` | `SLAdvantageEmitter` | The module-facing API (`line`, `state`, `status`, `report`, `grade`); formats via `PrintRail` for the CommonRails target |
| `output/output-demo.sleela` | `OutputDemo` | Runnable demo + validation across all four targets (`main`) |

The three top-level models (`SLAdvantageDeveloperModel`, `SLVerticalIntegration`,
`SLGradeBook`) carry an `emitThrough(emitter)` method so they can report
themselves to any target. Defined in
[`DEFINITIONS.md`](./DEFINITIONS.md#10-output-seam-output).

### `trading/` subfolder — trading-models subject

Eleven cross-asset trading models, a rate budget, and counsel, ported from the
C/C++ module in `/advantage`. Pure computation; every figure is validated by
`trading-demo` against hand-computed anchors. See [`TRADING.md`](./TRADING.md).

| File | Class | Purpose |
|------|-------|---------|
| `trading/SLTradingResult.sleela` | `SLTradingResult` | Common result plus the advantage bridge (`baseline`, `advantage`) |
| `trading/SLTradingCheck.sleela` | `SLTradingCheck` | Shared fail-closed validation and finite bounds |
| `trading/SLTradingArbitrage.sleela` | `SLTradingArbitrage` | Same asset, two venues; break-even |
| `trading/SLTradingMarketMaking.sleela` | `SLTradingMarketMaking` | Quote both sides; the unfilled case |
| `trading/SLTradingRefurbishment.sleela` | `SLTradingRefurbishment` | Improve and sell up-tier; outcome distribution |
| `trading/SLTradingRegional.sleela` | `SLTradingRegional` | Move it and sell elsewhere; loss and salvage |
| `trading/SLTradingAggregation.sleela` | `SLTradingAggregation` | Assemble a set; completion probability |
| `trading/SLTradingAsymmetry.sleela` | `SLTradingAsymmetry` | One candidate: pay to look, win the race, be right |
| `trading/SLTradingConsignment.sleela` | `SLTradingConsignment` | Commission with no inventory |
| `trading/SLTradingDistressed.sleela` | `SLTradingDistressed` | Buy from urgency; pursuit cost and remainder |
| `trading/SLTradingVirtual.sleela` | `SLTradingVirtual` | Sell before sourcing; fulfilment failure |
| `trading/SLTradingSynthetic.sleela` | `SLTradingSynthetic` | One into many or many into one |
| `trading/SLTradingMomentum.sleela` | `SLTradingMomentum` | Event-driven entry; exit failure; hype decay |
| `trading/SLTradingRateBudget.sleela` | `SLTradingRateBudget` | Wait before the next request from rate-limit numbers |
| `trading/SLTradingApproach.sleela` | `SLTradingApproach` | Evidence, assumptions, honest score |
| `trading/SLTradingCounsel.sleela` | `SLTradingCounsel` | Ranks approaches best-first |
| `trading/trading-demo.sleela` | `TradingDemo` | Runnable demo + validation (`main`) |

## The two models

- **User's model** hides the onion. State a dividend and the model it respects,
  nudge a few plain dials (`grow`, `shrink`, `rethink`), press `advantage()`,
  and read the result. `advantageHarder(n)` lets the process iterate and keeps
  the best outcome. A curious user can still reach the full controls via
  `advanced()`.
- **Developer's model** exposes the whole machine: build the Sequitur, tune
  each layer's signed contribution (`setReview`, `setConcerns`, `setMaths`,
  `setAdditions`, `setSubtraction`, `setRegrouping`, `setIteration`), walk the
  cursor (`deeper`, `shallower`, `seek`), single-step (`step`), run a full
  inward pass (`runPass`), and iterate (`iterate`).

Both operate on the same `SLAdvantageSequitur` + `SLAdvantageDividend` engine —
the user's model is a thin, friendly skin over the developer's model.

## Dynamically altering the model (to the owner's advantage)

The model can be re-shaped, re-pointed, and fully restored at runtime:

- `setOwner(name)` / `ownedBy(name)` — name the party the model is tuned to
  favor. This also re-snapshots, folding the owner into the restitution point.
- `updateModel(model, label, base)` (developer) / `updateModel(model, base)`
  (user) — re-point the dividend onto a new frame of reference and starting
  value at runtime. The onion shape is preserved and the original identity is
  retained for restitution. Pass `null` to leave a field unchanged.
- `favorOwner(strength)` (developer) / `favorMe(strength)` (user) — a dynamic
  alteration that tips the model toward the owner: the growth-side layers
  (`maths`, `additions`, `regrouping`) each gain `+strength` and a cost held in
  `subtraction` is softened toward zero by up to `strength`. It never flips a
  cost into a gain, and it leaves `review`, `concerns`, and `iteration`
  untouched — favoring must not fabricate a clean review or silence a concern.
  Returns the net contribution gained.
- `commitBaseline()` — adopt the current (altered) state as the new restitution
  point, so a later `restitute()` returns here instead of to the `init()` state.
- `restitute()` — **full restitute**: restore *both* the dividend's identity
  (label, model, owner, base) *and* the entire onion shape to the captured
  baseline, and clear the pass counter. A complete, known-good undo of every
  dynamic alteration.

The demo's final sections exercise all of this: a baseline advantage of 10,
`favorOwner(4)` raising it to 26 (shape gain +16), an `updateModel` onto a
promo model, and a `restitute` that returns the model and advantage to their
committed baseline.

## Bilateral trade economics (`math/` subject)

Math for advantaging a dividend across a **buyer** and a **seller** as one
coordinated system, then dividing the surplus, now organized as the
[`math/`](./math) subject: `SLJointProfitMax` (joint surplus + `Q*`),
`SLNashBargaining` (the `α`-weighted split with a closed-form Nash price), and
`SLBilateralNewsvendor` (overage/underage costs + the centralized critical
ratio that avoids double marginalization). `SLTradeMath` supplies self-contained
`expD`/`lnD`/`powAB` helpers so the subject needs no cross-package imports.

Full definitions, explanations, ASCII sketches, and worked examples are in
**[`MATH.md`](./MATH.md)**.

## Run it

```sh
./working-binaries/sleela run lib/advantage/advantage-demo.sleela        # onion model
./working-binaries/sleela run lib/advantage/math/trade-math-demo.sleela  # trade math subject
./working-binaries/sleela run lib/advantage/physics/physics-demo.sleela  # trust-field subject
./working-binaries/sleela run lib/advantage/integration/integration-demo.sleela  # vertical integration
./working-binaries/sleela run lib/advantage/grading/grading-demo.sleela  # sophistication grader
./working-binaries/sleela run lib/advantage/output/output-demo.sleela  # output seam (terminal/rails/socket)
```

`run` auto-links the sibling classes in each directory (no imports needed).
The onion demo validates the layered arithmetic (developer pass = 25, user
advantage = 30). The trade-math demo validates every model against hand-computed
anchors: Q* = 20, Π_total = 800, Nash price = 60 (400/400 split), critical
ratio = 80/95 ≈ 0.8421.

## Notes

- Every class carries the mandatory **Metadata Class metapattern**
  (`.kiro/steering/metadata-class-standard.md`).
- Editing files here touches the `lib/**` gates: after changes, regenerate
  `lib/LIBRARY.SYMBOLS.md` (`tools/generate-library-symbols.py`), update the
  four `EXPECTED_*` in `test-suites/test-library-inventory.sh` and the header
  counts in `lib/LIBRARY.INDEX.md`, and regenerate `security/sha256-manifest.json`.
- The quantitative subjects cite their real sources: Cournot (1838) for the
  `MR = MC` rule, Nash (1950) for the bargaining solution, Arrow–Harris–Marschak
  (1951) for the newsvendor critical ratio, and Cox–Ingersoll–Ross (1985) for
  the trust-decay diffusion. The economic reading of the `physics/` subject is
  an explicit modelling analogy, not a claim that trust is a literal field.

## Acknowledgments

With thanks to **NC State University (NCSU)**, the **University of North
Carolina at Wilmington (UNCW)**, the **University of North Carolina (UNC)**, and
**Duke University** — the North Carolina research community whose teaching and
scholarship inform the economics, geometry, stochastic-calculus, and
software-design thinking behind the Advantage™ library. The library's models are
the SLeeLa project's own constructions; the gratitude is for the education that
made them possible, and no endorsement by any institution is implied.

---

**Advantage™** — a trademark of the SLeeLa project, MEARVK LLC. Part of the
SLeeLa™ standard library; built, checked, and run by **Sleelvac™** /
**SLeeLa™ Language/Runtime 1.3.2-pro**. Rendered in the customary red where the
toolchain prints its product identity to a terminal. SLeeLa — MEARVK LLC — 2026.

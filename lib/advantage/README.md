# lib/advantage — the Sequitur onion model

The `advantage` package encapsulates the process of **advantaging a unique
dividend with respect to a model**: an ordered, layered procedure you can
navigate inward (deeper) and outward (shallower), and iterate up or down.

**Documentation:** [`DEFINITIONS.md`](./DEFINITIONS.md) — consolidated glossary
of every term across the whole package · [`MATH.md`](./MATH.md) — trade
economics · [`PHYSICS.md`](./PHYSICS.md) — trust-field physics.

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

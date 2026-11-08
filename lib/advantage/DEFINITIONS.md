# Definitions — the `advantage` package

> A single consolidated glossary for everything in `lib/advantage`: the
> **Sequitur onion model**, the **trade math** subject (`math/`), and the
> **trust-field physics** subject (`physics/`). Each entry gives the term, a
> plain definition, and where it is implemented.
>
> Companion docs: [`README.md`](./README.md) (overview),
> [`MATH.md`](./MATH.md) (trade economics), [`PHYSICS.md`](./PHYSICS.md)
> (trust fields).

## Contents

1. [Core concepts](#1-core-concepts)
2. [The Sequitur onion model](#2-the-sequitur-onion-model)
3. [Dynamic alteration & restitute](#3-dynamic-alteration--restitute)
4. [Trade math subject (`math/`)](#4-trade-math-subject-math)
5. [Trust-field physics subject (`physics/`)](#5-trust-field-physics-subject-physics)
6. [Cross-cutting conventions](#6-cross-cutting-conventions)
7. [Class index](#7-class-index)

---

## 1. Core concepts

| Term | Definition | Where |
|------|------------|-------|
| **Advantage** | A source of leverage: something you can draw on to know, decide, or act with more effect than you otherwise could. The net movement a dividend makes away from its base. | `SLAdvantageDividend.advantage()` |
| **Dividend** | The value being advantaged. "Unique" because it carries its own identity (a label); held "with respect to a model" (a named frame of reference). | `SLAdvantageDividend` |
| **Model** | The frame of reference a dividend is evaluated against (e.g. `quarterly-plan`, `pricing-model`). Distinct from the two *encapsulation* models (user's / developer's). | `SLAdvantageDividend.model` |
| **Owner** | The party to whose advantage a model is tuned. Folded into the restitution snapshot. | `SLAdvantageDividend.owner` |
| **Base** | The dividend's starting value before any layer is applied. | `SLAdvantageDividend.base` |
| **Current** | The dividend's running value as layers are applied. | `SLAdvantageDividend.current` |
| **User's model** | A simple, outcome-facing encapsulation: state a dividend, nudge a few plain dials, press a button, read the result. Hides the onion. | `SLAdvantageUserModel` |
| **Developer's model** | A full-control encapsulation: build the onion, tune every layer, navigate the cursor, run passes, iterate. | `SLAdvantageDeveloperModel` |

---

## 2. The Sequitur onion model

| Term | Definition | Where |
|------|------------|-------|
| **Sequitur** | Latin "it follows". An ordered list of layers in which each follows the previous in logical order. The engine's name for the onion. | `SLAdvantageSequitur` |
| **Onion** | A set of concentric layers navigated by **depth**: depth 0 is the outermost skin, higher depth is further inward toward the core. | `SLAdvantageSequitur.layers` |
| **Layer** | One stage of the process: a name, a kind, and a signed numeric contribution it folds into the running dividend when applied. | `SLAdvantageLayer` |
| **Depth** | A layer's position in the onion (0 = outermost). Also the cursor's current location. | `SLAdvantageLayer.depth`, `SLAdvantageSequitur.cursor` |
| **Contribution** | The signed amount a layer applies to the dividend's running value when the layer is applied. | `SLAdvantageLayer.contribution` |
| **Cursor** | The current depth. Moving it is how the model "iterates further down or up" the onion. | `SLAdvantageSequitur.cursor` |
| **Inward / outward** | Navigation directions: inward (`goInward`) increases depth toward the core; outward (`goOutward`) decreases it toward the skin. | `SLAdvantageSequitur.goInward` / `goOutward` |
| **Pass** | One full inward sweep applying every layer from skin to core in order, folding each contribution into the dividend. | `SLAdvantageSequitur.runInwardPass` |
| **Iteration** | Running further passes (rebasing each time), keeping the best net advantage found, up to a cap. | `SLAdvantageDeveloperModel.iterate` |

### The eight standard layers (outer → inner)

| Depth | Layer | Kind | Meaning |
|------:|-------|------|---------|
| 0 | `dividend` | value | the value under consideration |
| 1 | `review` | validate | look again; confirm the premise |
| 2 | `concerns` | risk | surface risks and caveats |
| 3 | `maths` | quant | the quantitative core |
| 4 | `additions` | grow | what the layer adds to the dividend |
| 5 | `subtraction` | shrink | what the layer takes away |
| 6 | `regrouping` | reorg | reorganize concepts; carry/borrow across the model |
| 7 | `iteration` | loop | decide to go further down or back up the onion |

---

## 3. Dynamic alteration & restitute

| Term | Definition | Where |
|------|------------|-------|
| **Update model** | Dynamically re-point the dividend onto a new model / label / base at runtime. The onion shape is preserved; the original identity is retained for restitution. | `SLAdvantageDeveloperModel.updateModel`, `SLAdvantageUserModel.updateModel` |
| **Favor the owner** | A dynamic alteration biasing the onion toward the owner: `maths`, `additions`, `regrouping` each gain `+strength`; a cost in `subtraction` is softened toward zero (never flipped); `review`/`concerns`/`iteration` are left untouched. | `SLAdvantageSequitur.favorOwner`, `SLAdvantageDeveloperModel.favorOwner`, `SLAdvantageUserModel.favorMe` |
| **Snapshot** | The captured restitution point — the dividend identity (`snapshot()`) and the onion shape (`snapshotShape()`) that `restitute()` restores to. | `SLAdvantageDividend.snapshot`, `SLAdvantageSequitur.snapshotShape` |
| **Commit baseline** | Adopt the current (altered) state as the new restitution point, so a later `restitute()` returns here instead of to the `init()` state. | `SLAdvantageDeveloperModel.commitBaseline` |
| **Full restitute** | Restore *both* the dividend's identity (label, model, owner, base) *and* the whole onion shape to the captured baseline, clearing the pass counter. A complete, known-good undo of every dynamic alteration. | `SLAdvantageDeveloperModel.restitute`, `SLAdvantageDividend.restitute`, `SLAdvantageSequitur.restitute` |

**Honesty guardrail.** `favorOwner` never fabricates a clean review or silences
a concern, and never turns a cost into a gain — biasing is an honest
re-weighting, consistent with the package's metadata-honesty posture.

---

## 4. Trade math subject (`math/`)

Bilateral trade economics: treat buyer and seller as one coordinated system,
maximize the total surplus, then divide it. Curves are quadratic
(`R_B(Q)=rbA·Q−rbB·Q²`, `C_S(Q)=csA·Q+csB·Q²`) so every optimum is closed-form.
Full treatment in [`MATH.md`](./MATH.md).

### 4.1 Joint Profit Maximization — `SLJointProfitMax`

| Term | Definition |
|------|------------|
| **Integrated surplus `Π_total(Q)`** | The total available pool of profit, `R_B(Q) − C_S(Q)`, when both parties act as one system. |
| **Buyer revenue `R_B(Q)`** | The buyer's revenue / utility valuation from `Q` units. |
| **Seller cost `C_S(Q)`** | The seller's total cost to produce/supply `Q` units. |
| **Marginal revenue** | `dR_B/dQ` — the extra revenue from one more unit. |
| **Marginal (supply) cost** | `dC_S/dQ` — the extra cost of one more unit. |
| **Decision rule (`Q*`)** | The optimal quantity where marginal revenue equals marginal supply cost: `MR = MC ⇒ Q* = (rbA − csA)/(2·(rbB + csB))`. |
| **Max joint profit** | `Π_total(Q*)` — the surplus evaluated at the optimal quantity. |

### 4.2 Bilateral monopoly & Nash bargaining — `SLNashBargaining`

| Term | Definition |
|------|------------|
| **Wholesale price `P`** | The transfer price that determines how the fixed pool at `Q*` is split. |
| **Seller profit curve** | `Π_S(P,Q) = P·Q − C_S(Q)`. |
| **Buyer profit curve** | `Π_B(P,Q) = R_B(Q) − P·Q`. |
| **Reservation profit `Π_S⁰ / Π_B⁰`** | The disagreement point — what each party makes if the deal falls through (alternative vendors, scrap value). |
| **Bargainable surplus `S`** | `Π_total(Q*) − Π_S⁰ − Π_B⁰` — the value available to split beyond both fallbacks. |
| **Bargaining power `α`** | An index in `[0,1]`; the seller's share of `S`. `α = 0.5` splits evenly. |
| **Nash objective** | The geometric product maximized by the solution: `[Π_S − Π_S⁰]^α · [Π_B − Π_B⁰]^(1−α)`. |
| **Nash price `P*`** | The closed-form price awarding the seller a fraction `α` of `S`: `P* = (Π_S⁰ + α·S + C_S(Q*)) / Q*`. |

### 4.3 Bilateral newsvendor — `SLBilateralNewsvendor`

| Term | Definition |
|------|------------|
| **Overage cost `c_o`** | The cost of one unit too many. Seller: `cs − vs`; buyer: `P − vb`. |
| **Underage cost `c_u`** | The cost of one unit too few. Seller: `P − cs`; buyer: `r − P`. |
| **Salvage `vs / vb`** | Recovered value per unsold unit — seller salvage `vs`, buyer salvage `vb`. |
| **Retail price `r`** | The end-market price per unit. |
| **Production cost `cs`** | The seller's unit production cost. |
| **Centralized critical ratio `F(Q*)`** | The system-wide service level that avoids double marginalization: `(r − cs) / ((r − cs) + (cs − vb))`. |
| **Double marginalization** | The inefficiency when both parties hedge separately and restrict inventory too much, shrinking the pool. |
| **Coordination gap** | How far the buyer's standalone ratio falls short of the system ratio — how much a buyback / revenue-share contract must lift it. |

### 4.4 Helpers — `SLTradeMath`

| Term | Definition |
|------|------------|
| `expD` / `lnD` | Range-reduced Taylor / atanh series for `eˣ` and `ln y`. |
| `powAB(a,b)` | General power `a^b = exp(b·ln a)` for `a > 0` (used for the fractional Nash exponents). |
| `sqrtD` | Square root via `powAB(a, 0.5)`. |

---

## 5. Trust-field physics subject (`physics/`)

A structural-economics **analogy**: trust and announcements are modelled *as*
geometric / topological / stochastic objects. The mathematics is implemented
faithfully; the economic reading is an analogy (flagged in every class). Full
treatment in [`PHYSICS.md`](./PHYSICS.md).

### 5.1 Trust metric — `SLTrustMetric`

| Term | Definition |
|------|------------|
| **Metric tensor `g_μν`** | The object that defines distance and curvature on the economic manifold. Here a conformal 2×2 `g = f(τ)·I` in a flat `double[4]`. |
| **Trust level `τ`** | A trust parameter in `(0,1]` that sets the conformal factor. |
| **Conformal factor `f(τ)`** | `1/τ` — the scaling applied to distances. `f → 1` at full trust (Euclidean); `f → ∞` as trust → 0. |
| **Line element `ds²`** | `f(τ)·(dx² + dy²)` — the squared length of a displacement under the metric. |
| **Effective distance** | `√(ds²)` — the operational cost to cross a displacement; inflates as trust falls. |
| **Warping** | `f(τ) − 1 ≥ 0` — the field's departure from flatness; 0 at full trust. |
| **Flat / Euclidean field** | Full-trust regime where distances behave normally. |
| **Warped / hyperbolic field** | Low-trust regime where friction and distance inflate. |

### 5.2 Transactional geodesic — `SLTransactionalGeodesic`

| Term | Definition |
|------|------------|
| **Geodesic** | The shortest path between buyer and seller through the warped manifold; a straight line only when the field is flat. |
| **Geodesic equation** | `d²xᵘ/dλ² + Γᵘ_αβ (dxᵃ/dλ)(dxᵝ/dλ) = 0`. |
| **Christoffel symbols `Γᵘ_αβ`** | The "gravitational pull" of market friction, systemic risk, or political instability; derived from `φ = −½ ln τ`. |
| **Trust field `τ(x,y)`** | `τ₀ + gx·x + gy·y` (clamped) — a trust level that varies across the plane. |
| **Curvature rule** | As trust → 0 the Christoffel symbols balloon and the path bends; crossing the same distance costs exponentially more (capital, legal overhead, insurance). |
| **Geodesic length** | The accumulated trust-metric arc length along the integrated path. |

### 5.3 Announcement contact structure — `SLAnnouncementContact`

| Term | Definition |
|------|------------|
| **Announcement** | A sovereign/corporate declaration (tariff, currency peg, verified manifest) that imposes a boundary constraint on an open topological space. |
| **Phase space** | The 3-D space `(Volume, Price, Trust)` the market moves through. |
| **Contact manifold `(M, α)`** | A 3-D space with a 1-form `α` satisfying the contact condition. |
| **Contact 1-form `α`** | Here `α = dz − y·dx`; its kernel defines the allowed moves. |
| **Contact condition `α ∧ dα ≠ 0`** | Non-integrability — timing, sequence, and momentum matter. The coefficient of `α ∧ dα` is reported as the "contact volume". |
| **Hyperplane restriction** | The announcement confines allowed instantaneous moves to the 2-D distribution `ker(α)` — a slice of phase space. |
| **Homotopy deformation `H(x,t)`** | A continuous warp (`t: 0 → 1`) from the announced surface back to the hidden cost reality. |
| **Topologically null** | An announcement with no binding force — when credibility is 0 the residual `credibility·(1−t)` collapses to 0. |

### 5.4 Stochastic trust decay — `SLTrustDecaySDE`

| Term | Definition |
|------|------------|
| **SDE** | The stochastic differential equation `dτ_t = κ(τ̄ − τ_t)dt + σ√τ_t dW_t − δ dJ_t`. |
| **Mean-reversion `κ(τ̄ − τ_t)`** | Pull back toward baseline institutional trust `τ̄` at speed `κ`. |
| **Baseline `τ̄`** | The long-run historical institutional trust level. |
| **Brownian motion `dW_t`** | Standard random market noise; diffusion scales with `σ√τ_t`. |
| **Diffusion `σ`** | The noise scale (CIR square-root volatility). |
| **Poisson jump `δ dJ_t`** | A breach: a sudden catastrophic drop of size `δ` when an announced contract is broken. |
| **Expected path** | The deterministic trajectory (`dW = 0`, no jumps) — converges to `τ̄`. |
| **Breach** | A single fired jump: `τ → τ − δ` (clamped at 0). |

### 5.5 Helpers — `SLPhysicsMath`

Self-contained `expD` / `lnD` / `powAB` / `sqrtD` / `absD`, named distinctly
from `math/SLTradeMath` to avoid cross-directory class-name ambiguity.

---

## 6. Cross-cutting conventions

| Term | Definition |
|------|------------|
| **Metadata Class metapattern** | The mandatory descriptive-metadata block (eleven catalogued attributes with get/set pairs) every new SLeeLa class carries. See `.kiro/steering/metadata-class-standard.md`. |
| **`metaNationalSchedule`** | The real, citable instrument governing a class, or `"none"` — never invented. The trade/physics subjects cite Cournot (1838), Nash (1950), Arrow–Harris–Marschak (1951), Einstein (1915), Cox–Ingersoll–Ross (1985). |
| **Subject** | A focused topic area within the package, organized as a subfolder (`math/`, `physics/`). The inventory generator keys the *package* on the first path component, so subfolders remain under the `advantage` package. |
| **Sibling auto-linking** | `sleela run` links all `.sleela` files in the *same* directory with no imports; each demo validates its own siblings. |
| **Flat matrix storage** | 2-D data is stored row-major in a flat `double[]` with `r*cols + c` indexing (house style, cf. `lib/math/SLMatrix`); SLeeLa does not use `double[][]`. |
| **Modelling analogy** | The physics subject models economics *as* physics; the math is real, the economic interpretation is an analogy, declared in each class and doc. |

---

## 7. Class index

| Class | Subject | File |
|-------|---------|------|
| `SLPackage` | package facade | `SLPackage.sleela` |
| `SLAdvantageLayer` | onion | `SLAdvantageLayer.sleela` |
| `SLAdvantageDividend` | onion | `SLAdvantageDividend.sleela` |
| `SLAdvantageSequitur` | onion | `SLAdvantageSequitur.sleela` |
| `SLAdvantageUserModel` | onion | `SLAdvantageUserModel.sleela` |
| `SLAdvantageDeveloperModel` | onion | `SLAdvantageDeveloperModel.sleela` |
| `SLJointProfitMax` | trade math | `math/SLJointProfitMax.sleela` |
| `SLNashBargaining` | trade math | `math/SLNashBargaining.sleela` |
| `SLBilateralNewsvendor` | trade math | `math/SLBilateralNewsvendor.sleela` |
| `SLTradeMath` | trade math | `math/SLTradeMath.sleela` |
| `SLTrustMetric` | physics | `physics/SLTrustMetric.sleela` |
| `SLTransactionalGeodesic` | physics | `physics/SLTransactionalGeodesic.sleela` |
| `SLAnnouncementContact` | physics | `physics/SLAnnouncementContact.sleela` |
| `SLTrustDecaySDE` | physics | `physics/SLTrustDecaySDE.sleela` |
| `SLPhysicsMath` | physics | `physics/SLPhysicsMath.sleela` |

Runnable demos (sibling-linked, each validates its subject):
`advantage-demo.sleela`, `math/trade-math-demo.sleela`,
`physics/physics-demo.sleela`.

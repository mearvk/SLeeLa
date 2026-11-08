artifact;
pure;
gasoline;

<!-- preonem: this document is an enacted artifact of the advantage
     legislature, graded unto time and space. See PREONEM.md. -->

# The Physics of Trust Fields — a structural-economics subject

> **Modelling analogy, stated up front.** This subject models *trust* and
> *announcements* **as** objects from differential geometry, contact topology,
> and stochastic calculus. The mathematics is implemented faithfully (real
> Christoffel symbols, a real geodesic integrator, a real contact-form test, a
> real CIR+jump step), but the economic reading — "trust is a metric tensor",
> "an announcement is a hyperplane" — is an **analogy**, not literal physics.
> Each class says so in its metadata and cites the real source of the math.
>
> Implemented under [`lib/advantage/physics/`](./physics) and validated by
> [`physics/physics-demo.sleela`](./physics/physics-demo.sleela).

| Subject | Class | Models |
|---------|-------|--------|
| Trust metric tensor | `SLTrustMetric` | trust as the metric `g_μν` of an economic manifold |
| Transactional geodesic | `SLTransactionalGeodesic` | the transaction path through a warped field |
| Announcement contact structure | `SLAnnouncementContact` | announcements as boundary constraints in phase space |
| Stochastic trust decay | `SLTrustDecaySDE` | trust fluctuation + breach via a CIR+jump SDE |
| Helpers | `SLPhysicsMath` | self-contained `expD`/`lnD`/`powAB`/`sqrtD` |

---

## 1. Differential Geometry & General Relativity: the trust metric

Instead of a vague qualitative score, trust is modelled as a **metric tensor
`g_μν`** that sets distance and curvature on an economic manifold.

```
     [ Zero Trust Field ]                      [ High Trust Field ]
  Friction/Distance Inflates                Friction/Distance Flattens
   (Warped Hyperbolic Space)                     (Euclidean Plane)
        ┌───────────┐                              ┌───────────┐
        │   \   /   │                              │ ─── ─── ───│
        │  ── X ──  │                              │ ───  X  ───│
        │   /   \   │                              │ ─── ─── ───│
        └───────────┘                              └───────────┘
```

**Implementation.** A conformal 2×2 metric `g = f(τ)·I` with `f(τ) = 1/τ`,
stored row-major in a flat `double[4]` (house style, cf. `lib/math/SLMatrix`):

```
ds² = f(τ)·(dx² + dy²)        effective distance = √f · √(dx²+dy²)
```

As `τ → 1` the field is Euclidean (`f → 1`, warping 0); as `τ → 0` distances
inflate without bound (`f → ∞`). The reported **warping** is `f(τ) − 1 ≥ 0`.

**Example.** Displacement `(3,4)` (Euclidean length 5):

```
τ = 1.00 :  distance = √(1·25) = 5    warping = 0   (flat)
τ = 0.25 :  distance = √(4·25) = 10   warping = 3   (doubled — costly)
```

### The transactional geodesic equation

Under varying trust, a transaction follows a **geodesic** — the shortest path
through the warped manifold:

```
d²xᵘ/dλ²  +  Γᵘ_αβ · (dxᵃ/dλ)(dxᵝ/dλ)  =  0
```

- `Γᵘ_αβ` — the **Christoffel symbols**: the "gravitational pull" of market
  friction, systemic risk, or political instability.
- **Curvature rule.** As trust → 0 the Christoffel symbols balloon, warping the
  coordinates toward hyperbolic space; crossing the same operational distance
  then takes exponentially more energy (capital, legal overhead, insurance).

For the conformal metric with `φ = −½ ln τ` and a trust field
`τ(x,y) = τ₀ + gx·x + gy·y`, the 2-D symbols are

```
Γˣ_xx =  φ_x   Γˣ_xy =  φ_y   Γˣ_yy = −φ_x
Γʸ_xx = −φ_y   Γʸ_xy =  φ_x   Γʸ_yy =  φ_y        φ_x = −½ · gx/τ
```

When trust is uniform (`gx = gy = 0`) the symbols vanish and the geodesic is a
straight line (Euclidean). The integrator (`geodesicLength`) accumulates the
trust-metric arc length along the path.

**Example.** Start `(0,0)` → target `(3,4)`:

```
flat field   (gx=gy=0)       geodesic length = 5.000   (= Euclidean)
warped field (gx=gy=−0.1)    geodesic length ≈ 5.256   (> Euclidean)
```

```sleela
SLTrustMetric metric = new SLTrustMetric();
double d = metric.effectiveDistance(0.25, 3.0, 4.0, m);   // 10  (τ = 0.25)

SLTransactionalGeodesic geo = new SLTransactionalGeodesic();
geo.configureField(0.9, -0.1, -0.1);
double len = geo.geodesicLength(0.0, 0.0, 3.0, 4.0, 200, m);   // > euclidean
```

---

## 2. Information Topology: announcements as boundaries

A sovereign or corporate **announcement** (a future tariff, a currency peg, a
verified inventory manifest) imposes a **boundary constraint** on an open
topological space.

**Contact geometry.** The announced space is a 3-D **contact manifold
`(M, α)`** in phase space `(Volume, Price, Trust)`, where the 1-form `α`
satisfies the contact condition

```
α ∧ dα ≠ 0
```

which prevents full integrability — so **sequence, timing, and momentum
matter**. The standard form `α = dz − y·dx` gives `α ∧ dα = dz∧dx∧dy` with
coefficient `1 ≠ 0`.

- **Announcement as a hyperplane.** The announcement confines the market's
  allowed instantaneous moves to the 2-D distribution `ker(α)` — a hyperplane
  slice of the full 3-D phase space.
- **Homotopy & information deformation.** If the entity lacks credibility,
  players use a homotopy deformation `H(x,t)` to smoothly warp the announced
  space back to the hidden cost reality, rendering the announcement
  **topologically null**. Modelled as a residual `credibility · (1 − t)`: at
  `t = 1` or `credibility = 0` the announced value is 0.

**Example** (`α = dz − 0.5·dx`):

```
α ∧ dα           = 1        → genuine contact structure (non-integrable)
move (2, 0, 1)   allowed    → 1·1 + (−0.5)·2 = 0, lies in ker(α)
move (1, 0, 1)   blocked    → 1·1 + (−0.5)·1 = 0.5 ≠ 0, off the hyperplane
residual(cred 0.9, t 0) = 0.9     residual(cred 0, t 0) = 0  → null
```

```sleela
SLAnnouncementContact ann = new SLAnnouncementContact();
ann.standardForm(0.5);
boolean real = ann.isContact(1e-6);                 // true  (α∧dα = 1)
boolean ok   = ann.moveAllowed(2.0, 0.0, 1.0, 1e-6); // true  (in ker α)
double  res  = ann.homotopyResidual(0.0, 0.0);       // 0     (null)
```

---

## 3. Mechanism Design in 3-D information space

Trust is tied to cost-sharing via 3-D surface modelling over
`X = Stated Cost`, `Y = Actual Cost`, `Z = Trust parameter τ`. A fully
incentive-compatible mechanism forms a stable **saddle-point** surface.

```
            Z [Trust Parameter (τ)]
                       ▲
                       │     .───.   ← saddle point (optimal)
                       │   .'     '.
                       │  /         \
                       │ /           \
                       └───────────────► Y [Actual Cost]
                      /
                     /
                    ▼
            X [Stated Cost]
```

### The stochastic differential equation for trust decay

Trust fluctuates with how well verified actions match prior announcements —
modelled as a CIR square-root diffusion with a downward jump:

```
dτ_t = κ(τ̄ − τ_t) dt  +  σ·√τ_t · dW_t  −  δ·dJ_t
```

- `κ(τ̄ − τ_t) dt` — **mean-reversion** to baseline institutional trust.
- `σ·√τ_t · dW_t` — Brownian market noise (variance scales with `τ`).
- `δ·dJ_t` — a **Poisson jump**: an announcement breach, a sudden catastrophic
  drop when an announced contract is broken.

Because `run` is deterministic, `SLTrustDecaySDE` exposes the pieces: `step`
takes the noise increment `dW` and jump indicator `dJ` as inputs; the
**expected** path (`dW = 0`, no jumps) is closed-form-checkable and converges
to `τ̄`; `breach` fires a single `−δ` jump.

**Example** (`κ = 0.5`, `τ̄ = 0.8`, `σ = 0.1`, `δ = 0.4`):

```
expected one step from 0.3  = 0.3 + 0.5·(0.8−0.3)·0.1 = 0.325
expected path (50 steps)    → 0.7615  (reverting up toward τ̄ = 0.8)
breach of 0.8               = 0.8 − 0.4 = 0.4   (catastrophic drop)
```

```sleela
SLTrustDecaySDE sde = new SLTrustDecaySDE();
sde.configure(0.5, 0.8, 0.1, 0.4);
double s = sde.expectedStep(0.3, 0.1);   // 0.325
double b = sde.breach(0.8);              // 0.4
double n = sde.step(0.5, 0.1, 0.2, 0.0, m);  // one noisy step, dW=0.2, no jump
```

---

## Comprehensive synthesis of paradigms

| Discipline | Core object | How "trust" is modelled | Role of the "announcement" |
|------------|-------------|-------------------------|----------------------------|
| Physics (field theory) | Metric tensor `g_μν` (`SLTrustMetric`, `SLTransactionalGeodesic`) | curvature / resistance of the medium | a mass injection that bends market geodesics |
| Pure mathematics | 3-D contact manifold `(M, α)` (`SLAnnouncementContact`) | topological connectivity / openness of neighbourhoods | a hyperplane restriction on the allowed phase space |
| Global economics | Stochastic control & signalling (`SLTrustDecaySDE`) | a dynamic process over an agent's true type | a strategic vector that separates efficient from inefficient types |

## Run the subject

```sh
./working-binaries/sleela run lib/advantage/physics/physics-demo.sleela
```

`run` auto-links the sibling classes in `lib/advantage/physics/`. The demo
prints each model and validates all results (metric distances 5 and 10, flat vs
warped geodesic, `α∧dα = 1`, allowed/blocked moves, SDE step 0.325 and breach
to 0.4) — ending in `ALL CHECKS PASSED`.

## Notes and honest limits

- Every class carries the **Metadata Class metapattern** and cites the real
  source of its mathematics — Riemannian geometry / general relativity (Einstein
  1915), contact geometry and homotopy (pure topology), and the Cox–Ingersoll–
  Ross square-root diffusion (1985). The economic interpretation is flagged as
  an analogy; no `metaNationalSchedule` is invented.
- The geodesic uses a **conformal** 2-D metric and explicit Euler integration —
  enough to show straight-line vs warped behaviour, not a full numerical-
  relativity solver.
- The SDE is stepped **deterministically** (the caller supplies `dW` and `dJ`);
  a Monte-Carlo driver with a real RNG and Poisson arrivals would sit on top of
  these primitives.
- The "saddle-point revelation surface" of §3 is presented as a conceptual
  sketch; the shipped code models its *dynamics* (the trust SDE), not a fitted
  3-D surface.

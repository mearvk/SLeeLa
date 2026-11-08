artifact;
preonem: [PREONEM.md](./PREONEM.md)
standard: legislature-grade; evaluated unto time and space

# EXAMPLES — Advantage™ Library

This guide is the practical entry point to the Advantage™ Library in
`lib/advantage/`. It maps each subject to its runnable demonstration, gives
the command to execute it, and explains what to inspect in the output.

The examples are demonstrations of the package's implemented models. They are
not financial, legal, investment, or scientific advice, and the trust-field
"physics" is a mathematical modelling analogy rather than a claim that trust
is a literal physical field.

## 1. Run the demonstrations

Run commands from the repository root. The examples below use the committed
convenience runtime; for the current release, build and use the source-built
SLeeLa runtime as described in the repository's build documentation.

| Subject | Command | What it demonstrates |
|---|---|---|
| Sequitur onion | `./working-binaries/sleela run lib/advantage/advantage-demo.sleela` | Dividend, eight ordered layers, user/developer models, iteration, owner bias, update and restitution |
| Bilateral trade math | `./working-binaries/sleela run lib/advantage/math/trade-math-demo.sleela` | Joint-profit optimum, Nash bargaining split, bilateral newsvendor critical ratio |
| Trust-field physics | `./working-binaries/sleela run lib/advantage/physics/physics-demo.sleela` | Trust metric, transaction geodesic, announcement contact structure, stochastic trust decay |
| Vertical integration | `./working-binaries/sleela run lib/advantage/integration/integration-demo.sleela` | Recursive compartments, tier ordering, engineering bays, trials, median tray |
| Sophistication grading | `./working-binaries/sleela run lib/advantage/grading/grading-demo.sleela` | Module scores, letter/tier ratings, winning/losing/grader/absolute sets |
| Output routing | `./working-binaries/sleela run lib/advantage/output/output-demo.sleela` | Terminal, CommonRails, socket-frame buffering, and silent capture targets |

Each demo has a `main()` entry point. The runtime's `run` command
auto-links sibling classes in the demo's directory; these examples do not
require manually listing each sibling source file.

## 2. Sequitur onion: a dividend through ordered layers

**Source:** `advantage-demo.sleela`

The core model evaluates one dividend relative to a named model. A standard
Sequitur contains eight layers, from outer skin to inner core:

1. `dividend` — the value being considered.
2. `review` — validate the premise.
3. `concerns` — expose risks and caveats.
4. `maths` — apply quantitative reasoning.
5. `additions` — record growth.
6. `subtraction` — record costs or losses.
7. `regrouping` — reorganize or recover value.
8. `iteration` — decide whether to make another pass.

The demo exercises two interfaces over the same engine:

- **User model:** `start`, `grow`, `shrink`, `rethink`,
  `advantage()`, and `advantageHarder(n)`.
- **Developer model:** `init`, individual layer setters, cursor navigation,
  `step`, `runPass`, and `iterate`.

The developer example configures contributions of
`0 + 0 - 5 + 20 + 15 - 8 + 3 + 0`, yielding a net advantage of **25**.
The user example configures `30 - 12 + 10 + 2`, yielding **30**.
These are deterministic arithmetic fixtures, not claims about real-world
returns.

### Dynamic owner adjustment and restoration

The demo also illustrates controlled dynamic alteration:

- Establish a baseline dividend and onion shape.
- Call `commitBaseline()` to make that known-good state the restitution point.
- Call `favorOwner(4)`: the maths, additions, and regrouping layers each
  increase by four; a negative subtraction is softened toward zero by up to
  four. Review and concern layers are not altered.
- Call `updateModel(...)` to point the same model at a different frame.
- Call `restitute()` to restore the captured dividend identity and layer
  contributions.

The sample's baseline advantage is **10**; the owner-adjusted result is **26**
(a shape gain of **16**); restitution returns the advantage to **10** and
restores the original model identity. Favoring an owner is a model operation,
not a substitute for independent review: it deliberately does not erase
risk or validation layers.

## 3. Bilateral trade economics

**Source:** `math/trade-math-demo.sleela`  
**Further reading:** [MATH.md](./MATH.md)

The trade subject treats buyer and seller as one coordinated system:

- `SLJointProfitMax` computes integrated surplus and the optimal quantity
  where marginal revenue equals marginal cost.
- `SLNashBargaining` divides gains according to a bargaining-power index
  `alpha`, subject to reservation profits.
- `SLBilateralNewsvendor` models overage and underage costs on both sides
  and computes a centralized critical ratio.
- `SLTradeMath` supplies the subject's self-contained numerical helpers.

The demo's documented hand-check anchors are:

| Check | Expected result |
|---|---:|
| Joint-profit optimal quantity `Q*` | 20 |
| Integrated surplus `Pi_total` | 800 |
| Nash bargaining price | 60 |
| Buyer / seller split | 400 / 400 |
| Centralized critical ratio | `80/95 ≈ 0.8421` |

These values validate the sample parameter set; they are not universal
market outcomes.

## 4. Trust-field modelling

**Source:** `physics/physics-demo.sleela`  
**Further reading:** [PHYSICS.md](./PHYSICS.md)

This subject explores trust and announcements through mathematical analogies:

- `SLTrustMetric` represents operational trust with a metric tensor and
  conformal distance weighting.
- `SLTransactionalGeodesic` integrates a transaction path using
  Christoffel-symbol/geodesic machinery.
- `SLAnnouncementContact` evaluates a contact 1-form and related structural
  conditions.
- `SLTrustDecaySDE` models mean-reverting stochastic decay with jump events
  representing announcement breaches.

Read the equations and assumptions in `PHYSICS.md` before interpreting
outputs. The model is a formal analogy and does not establish an empirically
validated physical law of trust.

## 5. Vertical integration

**Source:** `integration/integration-demo.sleela`

The integration subject represents a value chain as an ordered stack of
recursive compartments. Each compartment can carry a kind, capacity/space,
meaning, relevance, and child compartments. The demo covers:

- upstream-to-downstream tier ordering;
- nested models and onion-like compartments;
- engineering bays for loading and producing at a yield;
- trial compartments for recording experiments and success rates; and
- a median tray for summarizing observations.

Use this subject to prototype a structured value-chain model. Treat its
capacity, relevance, and sophistication scores as model outputs whose
definitions and input assumptions must be reviewed for each application.

## 6. Sophistication grading

**Source:** `grading/grading-demo.sleela`

The grading circuit rates individual modules and named sets:

- `SLModuleGrade` represents a module score, tier, letter, and role.
- `SLGradeSet` aggregates a named collection using mean, best, or worst
  scoring.
- `SLSophisticationCircuit` registers modules and partitions the field into
  winning, losing, grader, and absolute sets.
- `SLGradeBook` exposes module, set, and overall-field results.

A module's grade is only as meaningful as the rubric, score normalization,
and thresholds supplied to the circuit. The grader is an organizational
instrument, not an objective certification authority.

## 7. Output routing

**Source:** `output/output-demo.sleela`

The output seam separates a module's reporting API from its destination:

- **Terminal:** ordinary terminal output.
- **CommonRails:** formatted output through the toolchain's `PrintRail`.
- **Socket:** framed and buffered output for a native host to forward.
- **Silent:** capture without visible output.

The socket demonstration validates frame construction and buffering; the
SLeeLa `run` environment does not itself provide a live socket transport.
A native host must perform the actual forwarding.

## 8. Suggested learning sequence

For a first-time reader:

1. Run `advantage-demo.sleela` and compare the two model interfaces.
2. Read [DEFINITIONS.md](./DEFINITIONS.md) to learn the vocabulary.
3. Run `trade-math-demo.sleela` and check its numerical anchors against
   [MATH.md](./MATH.md).
4. Explore vertical integration and grading as composable higher-level
   structures.
5. Read [PHYSICS.md](./PHYSICS.md) before running the trust-field analogy.
6. Finish with the output demo to see how models can report through different
   sinks.

## 9. Related documentation

- [README.md](./README.md) — package architecture and class map.
- [DEFINITIONS.md](./DEFINITIONS.md) — consolidated glossary.
- [MATH.md](./MATH.md) — bilateral trade economics and worked calculations.
- [PHYSICS.md](./PHYSICS.md) — trust-field mathematical models.
- [PREONEM.md](./PREONEM.md) — documentation and drafting standard.


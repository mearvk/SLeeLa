# SLeeLa

## Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Ubuntu.Determinant.Beta.Restricted/blob/main/images/Bitcoin_and_wallet_in_slots_2K_202609042306%20(1).jpeg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh

US Democratic and US Policy.

**Sleela** is a Java-like programming language running on a Turing-complete,
thread-friendly **C/C++ execution core**, with **Nordshrift** — a `.sst`
transpiler driver (spec `SST.model`, NS-SST-0001) — sitting on top to drive the
triplet of targets (**Java**, **Sleela**, **C**).

> **Wrapper™** — A Sleela source file (extension `.sleela`) is a **Wrapper™**:
> the program unit that carries the metadocument addend, i.e. it is governed by
> and consistent with the Sleela Language Metadocument (SL-META-0001). Throughout
> this repository, "a `.sleela` file", "a Sleela source file", and "a Wrapper™"
> all name the same file type.

## Current implementation — native domains and persistent artifacts

The implementation now includes a native-domain pipeline in which domain
libraries are lowered into Sleela Core bytecode rather than delegated to a
host-language runtime at execution time.

```text
Sleela source (.sleela)
        │
        ▼
     Sleelvac™
        │
        ├── math
        ├── physics
        ├── economics
        ├── chemistry
        └── financial
        │
        ▼
  native/domain lowering
        │
        ▼
   Sleela Core bytecode
        │
        ▼
 persistent .sleela artifact
        │
        ▼
    Sleela Runtime
```

The compiled `.sleela` artifact is persistent Core bytecode. The runtime can
load the artifact directly; it does **not** invoke Sleelvac again merely to
execute an already compiled artifact. The artifact format has its own magic
header and serialized constants, strings, globals, functions, bytecode, and
entry-point information.

The working implementation lives under [`impl/`](impl/) — see
[`impl/README.md`](impl/README.md) for the full guide and
[`impl/DESIGN.md`](impl/DESIGN.md) for the architecture.

### Native libraries

The executable native library layer currently includes:

- **Math** — constants, elementary functions, powers, roots, logarithms,
  trigonometric functions, `hypot`, and floating-point remainder. See
  [`MATH.md`](MATH.md).
- **Physics** — physical constants, kinematics, mechanics, gravitation,
  relativity, electrical relations, idealized gas relations, wave relations,
  and a simplified neutrino-oscillation model. See
  [`PHYSICS.md`](PHYSICS.md).
- **Economics** — time value of money, elasticity, real/nominal rates,
  doubling time, continuous growth, profit, margin, and the GDP identity. See
  [`ECONOMICS.md`](ECONOMICS.md).
- **Chemistry** — a dependency-light domain model for chemical subjects,
  properties, ratios, similarity, stochastic evaluation, and structured
  inference. Chemistry is deliberately treated as a model/inference layer,
  not as an assertion of experimental fact. See
  [`CHEMISTRY.md`](CHEMISTRY.md).
- **Financial** — algebraic quantitative-finance primitives including time
  value of money, annuities, NPV, bond pricing, CAPM, WACC, ratios,
  determinants, two-by-two linear systems, and quadratic equations. See
  [`FINANCIAL.md`](FINANCIAL.md).

These five subject documents are the primary data/reference layer for their
respective executable domains; `NATIVE_API.md` remains the cross-domain API
and lowering overview.

The current executable examples use:

```sleela
import math;
import chemistry;
import financial;
```

where the relevant domain is needed. Chemistry and financial lowering uses the
existing executable math kernels as a dependency. The integration path is
already wired for both source execution and persistent `.sleela` artifacts.

The detailed API records are maintained in [`NATIVE_API.md`](NATIVE_API.md) and
the five subject references listed above.

### Financial library design

The financial library uses **QuantLib** as a conceptual benchmark for the
breadth and seriousness of quantitative-finance modeling, while keeping the
SLeeLa implementation original and dependency-light. No QuantLib source is
copied into this repository.

The internal dependency chain is:

```text
quantitative-finance concepts
        ↓
original SLeeLa algebra and formulas
        ↓
impl/finance/financial.cpp
        ↓
Sleelvac financial lowering
        ↓
Sleela Core
        ↓
runnable .sleela artifact
```

The financial explanation model follows:

**Financial Subject → Quantity and Unit → Algebraic Relation → Financial
Formula → Transformation → Result → Comparative Norm → Interpretation**.

Domain assumptions remain explicit: rates, timing, compounding, units, tax
parameters, and model conditions are not silently converted into empirical
claims or investment recommendations.

### Chemistry and scientific inference

Chemistry follows the same disciplined distinction between a represented
subject and a verified observation. A chemical or experimental subject can be
represented through composition, properties, ratios, structural relationships,
bond/valence concepts, symmetry, comparative similarity, and stochastic
inference. The intended chain is:

**Subject Identity → Formula / Composition → Valence and Bond Norm →
Symmetry / Structural Norm → Physical-Chemical Properties → Comparative
Similarity → Astronomical / Elemental Origin → Inference and Uncertainty**.

Astronomical grounding is scientific: it refers to elemental origin,
nucleosynthesis, planetary context, and related physical models. It is not a
claim that a symbolic model constitutes direct observation.

### Nordshrift integration and IQ Conservators

Nordshrift now has declarative financial integration components in
[`impl/nordshrift/FINANCIAL.model`](impl/nordshrift/FINANCIAL.model),
[`impl/nordshrift/IQ_CONSERVATORS.FINANCIAL.model`](impl/nordshrift/IQ_CONSERVATORS.FINANCIAL.model),
and [`impl/nordshrift/SST.FINANCIAL.md`](impl/nordshrift/SST.FINANCIAL.md).

The financial integration preserves a causal explanation chain:

```text
subject
  → quantities
  → units
  → assumptions
  → algebra
  → equation
  → transformation
  → numerical_result
  → interpretation
```

The **IQ Conservator** preserves relative meaning across related subjects:

```text
PriorSubject
  → CurrentSubject
  → ReferenceSubject
  → ComparativeNorm
```

Conserved items include:

- `PriorSubject`
- `SubjectIdentity`
- `QuantityIdentity`
- `UnitIdentity`
- `FormulaIdentity`
- `AssumptionIdentity`
- `ComparativeRatio`
- `AlgebraicInvariant`
- `ResultTrace`
- `ExplanationTrace`

Here **IQ** means system insight/quality and preservation of inspectable
meaning. It is **not a psychometric measurement of a person**. Stochastic
scoring can rank or compare modeled alternatives, but it cannot manufacture
missing evidence.

The same conservation principle is intended for future chemistry, physics,
economics, and other domain integrations: preserve the subject identity,
quantities, units, assumptions, transformations, invariants, result trace,
and explanation trace while allowing the domain-specific calculation to vary.

### Network support

Sleela Core also contains native network opcodes for listening, accepting,
connecting, socket reads/writes, and socket close operations, with a bounded
socket model. Nordshrift's network model represents endpoints, NICs, links,
packets, queues, switches, routers, fabrics, listeners, connectors, gateways,
load balancers, services, TLS, and DNS. The network contract is documented in
[`impl/nordshrift/NETWORK.model`](impl/nordshrift/NETWORK.model) and
[`impl/nordshrift/SST.NETWORK.md`](impl/nordshrift/SST.NETWORK.md).

### `.sleela` artifact commands

The current driver supports source compilation and artifact execution:

```sh
sleela compile <file.sleela> -o <program.sleela>
sleela run <file.sleela>
sleela run <program.sleela>
sleela run <file.xclass> [more...]
sleela xclass ...
sleela check ...
sleela version
```

The compiler therefore provides a clear separation between **source
interpretation/compilation** and **persistent executable artifact** handling.

- [`SHEET.sheet`](SHEET.sheet) is the catalog of common system objects (129
  objects across 16 role categories) that backs Sleela's *conducted methods*
  and Nordshrift's *object compatibility list*.
- [`SST.model`](SST.model) is the normative `.sst` format specification.
- [`SLEELA.md`](SLEELA.md) documents the `.sleela` filetype (**Wrapper™**), the
  configurable Sigil **QR code**, and the deterministic 248×48
  **steganographic frame** — generated by the dependency-free tool in
  [`tools/sigil/`](tools/sigil/).
- [`VERSION.md`](VERSION.md) is the single record of all versions (toolchain
  **0.1.2**, language syntax **1.0**, Nordshrift **1.0**, and the governing
  specs).
- [`COMPILER.md`](COMPILER.md) describes the Sleela compiler — its pipeline,
  version awareness (the `#sleela` pragma), and the versions it implements.
- [`SOURCE.md`](SOURCE.md) describes the Sleela source file (the `.sleela`
  **Wrapper™**) and its characteristics.
- [`NORDSHRIFT.md`](NORDSHRIFT.md) gives a concise overview of the `.sst`
  transpiler driver (authoritative detail in `SST.model` and
  `impl/nordshrift/NORDSHRIFT.md`).
- [`GLOSSARY.md`](GLOSSARY.md) defines all project terms — Part A (science &
  engineering) and Part B (the United States evidentiary method).
- [`SUMMARY.md`](SUMMARY.md) is a single-file accounting of the whole
  repository — every document, source tree, specification, method, and special
  function, in one place.
- [`LENS.md`](LENS.md) applies the Sleela *Lens* idea (a focused get/set view)
  to how the United States was created — who built it, who bought it, and the
  early reliefs of 1967 — as dated, sourced facts kept apart from framing.

```sh
cd impl && make
./build/sleela run examples/hello.sleela
./build/sleela run examples/financial.sleela
./build/nordshrift build nordshrift/examples/demo/build.sst
```

## The Constitution — ordained constraints, in order

This is the ordered constitution of governing constraints ("congrains") the
system honors. Every rule here is enforced or expressible today; each is drawn
from `SHEET.sheet` (the `congrains` and `limits` sections and the `system`
invariants) or from the NS-SST-0001 diagnostics. Constraints are organized into
named **groupings**. Two of the groupings — **Health** and **IQ** — are
*system* metric groupings (system health and system insight/quality); they
describe the software system's own condition, nothing about people.

### Article I — System invariants (ordained, always in force)

In order of precedence:

1. **Depth** — the relevant system depth is **3024**: the maximum
   nesting/reachability from `System` to any leaf object (`SHEET.sheet` →
   `system.depth`; the `DepthLimit` congrain).
2. **Connectivity** — up to **3024** congruent-linear systems may be connected
   (`system.congruent-linear-systems-max`).
3. **Complexity degree** — the connection order between systems must be **≤ 4**
   (`system.complexity-degree-max`; the `ComplexityDegree` congrain). Degrees:
   `1` direct · `2` mediated · `3` brokered · `4` federated.
4. **The connect rule** — `connect(n)` is permitted iff `n ≤ 3024` **and**
   `degree(topology) ≤ 4`. These are surfaced in Sleela as `sysdepth()` (3024)
   and `degreemax()` (4).

### Article II — Congrains grouping (the binding constraints)

In `SHEET.sheet` order (`section congrains`):

| # | Constraint      | Ordinance                                                        |
|---|-----------------|-----------------------------------------------------------------|
| 1 | `Invariant`     | a condition that must always hold for an object                   |
| 2 | `Precondition`  | what must be true before an operation runs                        |
| 3 | `Postcondition` | what the operation promises afterward                             |
| 4 | `Constraint`    | a restriction on legal object states/relations                    |
| 5 | `Dependency`    | a binding one object has on another (co-grained)                  |
| 6 | `Coupling`      | strength of inter-object binding (keep it loose)                   |
| 7 | `Congruence`    | structural sameness enabling congruent-linear connection          |
| 8 | `Contract`      | the bundle of congrains an object honors                           |

### Article III — Limits grouping (the bounding constraints)

In `SHEET.sheet` order (`section limits`):

| # | Limit             | Ordinance                                                      |
|---|-------------------|-----------------------------------------------------------------|
| 1 | `Bound`           | an inclusive/exclusive extent on a value                        |
| 2 | `Quota`           | a permitted amount over a scope/time                            |
| 3 | `RateLimit`       | cap on operations per unit time                                 |
| 4 | `Timeout`         | a limit on how long an operation may take                       |
| 5 | `Capacity`        | the maximum a container/resource may hold                       |
| 6 | `Threshold`       | a boundary value that changes behavior when crossed             |
| 7 | `DepthLimit`      | maximum reachable depth — here the System depth is **3024**     |
| 8 | `ComplexityDegree`| the connection order between systems — capped at **4**          |

### Article IV — Health grouping (system-health metrics)

System **Health** is the grouping of runtime-condition constraints the system
watches (drawn from the catalog's `attention` and `gain` roles). A system is
*healthy* while all hold:

| Metric        | Constraint                                                          |
|---------------|--------------------------------------------------------------------|
| `Watchdog`    | no line of execution is stalled (heartbeat within its `Timeout`)   |
| `Monitor`     | observed metrics stay within their `Threshold`s                    |
| `Throughput`  | work completed per unit time stays above its floor                 |
| `Capacity`    | no `Resource` exceeds its `Capacity` / high-water mark             |
| `Exception`   | unhandled exceptions do not cross a system boundary                |

### Article V — IQ grouping (system insight / quality)

System **IQ** is the grouping of insight/quality constraints — how well the
system *understands and expresses itself* (drawn from the `gain` and
`congruence` roles and Sleela's conducted-method insights). Higher IQ = more of
these satisfied:

| Metric        | Constraint                                                          |
|---------------|--------------------------------------------------------------------|
| `Insight`     | every conducted object resolves an `insight(...)` (a known gloss)  |
| `Congruence`  | related objects `route(...)` to a known congruence (no dead ends)  |
| `Coupling`    | inter-object coupling stays loose (Article II·6)                   |
| `Contract`    | each object's pre/post/invariant `Contract` is stated              |
| `Optimization`| a defined objective (`gain`) is being measured and improved        |

Query these from Sleela via the conducted methods (`insight`, `role`,
`congruent`, `route`, `sysdepth`, `degreemax`) and from Nordshrift via
`nordshrift objects` / `nordshrift relevance`.

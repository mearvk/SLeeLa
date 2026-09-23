# NORDSHRIFT.md — the `.sst` transpiler driver

**Nordshrift** is the **transpiler driver and semantic coordination layer for Sleela**. It reads a **`.sst` Scripting Sheet** — the human-authored control surface defined by the normative specification **NS-SST-0001** — and drives the transpilation of Sleela source files while preserving explicit subject semantics.

A `.sst` file remains a build-control sheet, not a program. In Nordshrift 2.0 it may additionally declare semantic subjects, quantities, units, assumptions, relations, transformations, evidence status, comparative norms, dependencies, explanations, and work-plan items.

## Version 2.0

Nordshrift 2.0 introduces a common semantic layer shared by the first-class subject libraries:

- **Math** — algebra, numerical transformation, approximation, and mathematical invariants.
- **Physics** — quantities, units, constants, laws, domain conditions, and modeled physical relations.
- **Economics** — rates, flows, accounting identities, equilibrium assumptions, scenarios, and comparative statics.
- **Chemistry** — composition, valence, bonds, symmetry, properties, signals, conferrers, and qualified inference.
- **Financial** — cash flows, valuation, discounting, instruments, portfolios, risk, scenarios, and financial algebra.

The canonical semantic chain is:

**Subject → Quantity → Unit → Assumption → Relation → Formula → Transformation → Result → ComparativeNorm → Evidence → Explanation → Validation**

The normative 2.0 specification is [`SST-2.0.model`](SST-2.0.model). The original [`SST.model`](SST.model) remains the 1.0 compatibility specification and historical grammar reference.

## Common semantic models

The implementation defines these models in [`subject_model.h`](impl/nordshrift/subject_model.h):

| Model | Purpose |
|---|---|
| `Subject` | common identity, domain, dependencies, semantic collections, and work plan |
| `Quantity` | value/expression, unit, dimension, domain, and status |
| `Assumption` | explicit conditions governing model validity |
| `Relation` | inputs, outputs, and algebraic/structural formula |
| `Transformation` | inspectable operation and provenance chain |
| `ComparativeNorm` | prior/current/reference relationship for comparison |
| `Evidence` | observed, specified, derived, modeled, inferred, or assumed status |
| `Explanation` | structured rendering of the semantic chain |
| `Todo` | declarative work item, dependencies, expected result, validation, and status |

Health and IQ remain **system metrics**. IQ means system insight/quality, not human psychometric intelligence.

## Domain ideals

1. Identity before calculation.
2. Quantity before formula.
3. Unit and dimension before interpretation.
4. Assumption before extrapolation.
5. Relation before conclusion.
6. Transformation before result.
7. Provenance before trust.
8. Comparison before ranking.
9. Uncertainty before certainty claims.
10. Validation before completion.
11. Explicit dependency before hidden coupling.
12. Computation remains distinguishable from observation.

A numerical result is therefore not silently promoted to an empirical fact, and an inference is not silently promoted to an observation.

## WorkPlan / TODO

2.0 also gives Nordshrift a declarative work-plan vocabulary:

`PLANNED → READY → ACTIVE → BLOCKED → VALIDATING → COMPLETE` with `DEFERRED` available for intentionally postponed work.

A TODO records what is intended, what it depends on, what result is expected, and how completion will be validated. It does not itself constitute evidence that the proposed work has been completed.

## Network and Finance component series

A `.sst` sheet may declare two first-class, closed object series as input:

- A **`network:`** block — the network component series (`Endpoint`, `NIC`,
  `Link`, `Packet`, `Queue`, `Switch`, `Router`, `Fabric`, `Listener`,
  `Connector`, `Gateway`, `LoadBalancer`, `Service`, `TLS`, `DNS`), plus
  `transports`, `address-family`, and `tls`. See
  [`impl/nordshrift/SST.NETWORK.md`](impl/nordshrift/SST.NETWORK.md).
- A **`finance:`** block — the finance component series (`FutureValue`,
  `PresentValue`, `AnnuityPresent`, `AnnuityFuture`, `NetPresentValue`,
  `BondPrice`, `CAPM`, `WACC`, `Determinant2x2`, `LinearSolve2x2`,
  `QuadraticEquation`, `Ratio`), plus `currency`, `period`, and `discounting`.
  See [`impl/nordshrift/SST.FINANCE.md`](impl/nordshrift/SST.FINANCE.md).

Both series are also carried in the repo-root [`SHEET.sheet`](SHEET.sheet)
catalog (sections `network` and `finance`), so every declared object has a home
in the shared object-compatibility / relevance structure. Unknown members are
rejected with structured diagnostics (`NSS-E-NET-00x` / `NSS-E-FIN-00x`).

A sheet may also declare one or more **`subject:`** blocks — the 2.0 semantic
layer (`Subject → Quantity → Unit → Assumption → Relation → Formula →
Transformation → Result → ComparativeNorm → Evidence → Explanation →
Validation`) — which fill the shared `nordshrift::semantic::Subject` model. See
[`impl/nordshrift/SST.SUBJECT.md`](impl/nordshrift/SST.SUBJECT.md).

## Build effect: the component manifest

On `nordshrift build`, the declared `network:`, `finance:`, and `subject:`
series are emitted into a **component manifest** artifact for the selected
target — a Java class of static arrays, C name arrays with counts, or a Sleela
class. For the `sleela` target it is written next to the sheet as
`build/ComponentManifest.sleela`; for `java` / `c` it is emitted to stdout with
the transpiled sources. Declaring a component series therefore has a real,
inspectable build effect rather than being inert metadata.

## The triplet

The `target-language` directive selects one of three targets — **`java`**, **`sleela`**, or **`c`**. For `target-language sleela`, the emitted program is also executed on the Sleela core.

## CLI

```sh
nordshrift check <sheet.sst>
nordshrift build <sheet.sst>
nordshrift version
```

## Authoritative references

- [`SST-2.0.model`](SST-2.0.model) — normative 2.0 semantic/control-sheet specification.
- [`SST.model`](SST.model) — NS-SST-0001 1.0 compatibility specification.
- [`impl/nordshrift/NORDSHRIFT.md`](impl/nordshrift/NORDSHRIFT.md) — implementation guide.
- [`MATH.md`](MATH.md), [`PHYSICS.md`](PHYSICS.md), [`ECONOMICS.md`](ECONOMICS.md), [`CHEMISTRY.md`](CHEMISTRY.md), [`FINANCIAL.md`](FINANCIAL.md) — subject-level references.


## Subject Libraries

The Nordshrift semantic layer now carries the expanded SLeeLa Subject Library family, including **Astrophysics** and **Sociology**. Subject definitions are represented through the common Subject/Quantity/Assumption/Relation/Transformation/Evidence vocabulary and can be traced to native C/C++ implementations and XML model/procedure definitions.

Astrophysics declares Math and Physics dependencies. Sociology declares Math as its mathematical foundation and keeps descriptive statistics separate from causal or normative conclusions.

The implementation target is **Excellent**: explicit dependencies, units/dimensions, provenance, evidence status, bounded procedures, native tests, SLeeLa examples, and XML declarations must remain connected rather than becoming parallel undocumented systems.

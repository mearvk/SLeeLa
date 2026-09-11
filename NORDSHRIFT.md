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

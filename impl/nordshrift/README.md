# Nordshrift

**Nordshrift 2.0** is the transpiler driver and semantic coordination layer for Sleela. It reads a **`.sst` control sheet** and drives the transpilation of Sleela sources into the target selected by the sheet — the **triplet**: **Java**, **Sleela** (executed on the C core), or **C**.

The `.sst` file remains the *control surface*, not the program. In 2.0 it can also carry explicit semantic models shared by the Math, Physics, Economics, Chemistry, and Financial libraries.

## Build & run

```sh
cd impl
make

./build/nordshrift check <sheet.sst>
./build/nordshrift build <sheet.sst>
```

The Nordshrift build now includes `subject_model.cpp`, providing the common
semantic model implementation.

## 2.0 specification

- [`../../SST-2.0.model`](../../SST-2.0.model) — normative 2.0 specification.
- [`../../SST.model`](../../SST.model) — 1.0 compatibility specification.
- [`NORDSHRIFT.md`](NORDSHRIFT.md) — implementation architecture and semantics.
- [`subject_model.h`](subject_model.h) — C++ semantic model vocabulary.

## Common semantic models

Nordshrift 2.0 defines a shared vocabulary so each subject library can remain
domain-specific while participating in the same inspectable structure:

| Model | Meaning |
|---|---|
| `Subject` | identity, domain, dependencies, semantic collections, work plan |
| `Quantity` | value/expression, unit, dimension, domain, status |
| `Assumption` | explicit model condition |
| `Relation` | inputs, outputs, and formula |
| `Transformation` | operation and provenance |
| `ComparativeNorm` | prior/current/reference comparison |
| `Evidence` | epistemic status and source |
| `Explanation` | ordered semantic explanation |
| `Todo` | declarative work item and validation plan |

Canonical chain:

**Subject → Quantity → Unit → Assumption → Relation → Formula → Transformation → Result → ComparativeNorm → Evidence → Explanation → Validation**

## Evidence status

The model distinguishes:

`Observed`, `Specified`, `Derived`, `Modeled`, `Inferred`, `Assumed`.

A numerical result is not automatically an observation. A model or inference
must retain its assumptions and supporting relationships where available.

## WorkPlan / TODO

A `Todo` records identity, subject, priority, dependencies, preconditions,
action, expected result, validation, and status.

Statuses are:

`Planned`, `Ready`, `Active`, `Blocked`, `Validating`, `Complete`, `Deferred`.

## Domain ideals

1. Identity before calculation.
2. Quantity before formula.
3. Unit/dimension before interpretation.
4. Assumption before extrapolation.
5. Relation before conclusion.
6. Transformation before result.
7. Provenance before trust.
8. Comparison before ranking.
9. Uncertainty before certainty claims.
10. Validation before completion.
11. Explicit dependency before hidden coupling.
12. Computation remains distinguishable from observation.

## The `.sst` sheet

Indentation-significant, pragma-first. Existing 1.0 sections remain available:

`sheet`, `import`, `source`, `target`, `pipeline`, `rules`, `effects`, `derive`, `guards`, `interop`, and `profile`.

The 2.0 semantic extension adds subject-oriented sections for quantities,
assumptions, relations, transformations, comparison, evidence, and work plans.

## Self-contained demo

```sh
./build/nordshrift check nordshrift/examples/demo/build.sst
./build/nordshrift build nordshrift/examples/demo/build.sst
```

## Object compatibility list (SHEET.sheet)

Nordshrift also carries every object from the repo-root `SHEET.sheet` catalog
(129 objects, 16 role categories, the `System` root with depth 3024 /
complexity degree 4) in an **object compatibility list**, and converts each
object into a per-target relevance:

- **direct** — the object maps to a concrete target construct.
- **model** — the object is realized as an abstract role/pattern.
- **none** — the object is not on the compatibility list.

Commands:

```sh
nordshrift objects
nordshrift relevance --target=c
nordshrift relevance --target=c Thread
```

The catalog is parsed by the shared `../catalog/` module so the compiler and
Nordshrift remain aligned.

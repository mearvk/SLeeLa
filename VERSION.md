# Sleela — Version

This file is the single place of record for the versions of everything in this
repository. Sleela versions several things independently (the metadocument
distinguishes language syntax, language semantics, the implementation, and the
tooling), so they do not share one number. Update this file whenever any of the
versions below changes, and keep it consistent with the source-of-truth
locations listed in each row.

## Current versions (at a glance)

| Component | Version | Status | Source of truth |
|-----------|---------|--------|-----------------|
| **Sleela toolchain / implementation** (`sleela` CLI) | **0.1.2** | Working (pre-1.0) | `impl/frontend/driver.cpp` (`kVersion`) |
| **Sleela language syntax** | **1.0** | Supported range `1.0 .. 1.0` | `impl/frontend/version.h` (`min`/`maxSupportedSyntax`) |
| **Nordshrift** (`.sst` transpiler driver) | **2.0** | Semantic model layer added; legacy 1.0 sheets retained | `NORDSHRIFT.md`, `impl/nordshrift/subject_model.h` |
| **NS-SST-0001** (`.sst` format specification) | **2.0.0** | Normative 2.0 semantic/control extension | `SST-2.0.model` |
| **NS-SST-0001 legacy** | **1.0.0** | Compatibility / historical grammar | `SST.model` |
| **SL-META-0001** (Sleela Language Metadocument) | **1.0.0** | Pre-Normative | `src/Sleela.manifest` (Revision) |

> **Short answer:** the Sleela toolchain remains **0.1.2**, implementing
> **Sleela syntax 1.0**, while **Nordshrift is now specified at 2.0** with a
> common semantic layer for the Math, Physics, Economics, Chemistry, and
> Financial libraries.

## What each version means

### Sleela toolchain / implementation — `0.1.2`
The version of the actual C/C++ implementation in `impl/` (the `sleela` CLI:
lexer → parser → compiler → C core). This is a semantic `MAJOR.MINOR.PATCH`
number and is what `./build/sleela version` reports. It is **pre-1.0**.

### Sleela language syntax — `1.0`
The grammar version a `.sleela` file (a **Wrapper™**) declares with its
`#sleela MAJOR.MINOR` pragma. The compiler is version aware and accepts only
its supported range.

### Nordshrift — `2.0`
Nordshrift remains the `.sst`-driven transpiler for Java / Sleela / C, and now
adds a common semantic coordination model. The 2.0 model defines Subject,
Quantity, Unit, Assumption, Relation, Formula, Transformation,
ComparativeNorm, Evidence, Explanation, Validation, and Todo/WorkPlan concepts.
The implementation vocabulary is in `impl/nordshrift/subject_model.h` and is
compiled into the Nordshrift target.

### NS-SST-0001 — `2.0.0` (Normative)
The normative 2.0 specification is `SST-2.0.model`. It extends the original
`.sst` control surface with subject semantics, evidence status, comparative
norms, explicit dependencies, explanations, validation, and declarative work
plans. `SST.model` remains available as the 1.0 compatibility specification.

### SL-META-0001 — `1.0.0` (Pre-Normative)
The Sleela Language Metadocument (`src/Sleela.manifest`) remains the
constitutional document governing the language's design and syntax versioning.

## Nordshrift 2.0 semantic ideals

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

## Versioning policy

- **Semantic versioning** (`MAJOR.MINOR.PATCH`) applies to the implementation
  and normative specifications.
- A specification minor version may add constructs while preserving existing
  meanings. A major version may introduce breaking grammar changes.
- A Nordshrift implementation must reject unsupported 2.0 directives rather
  than silently ignoring them.
- A change that affects `.sleela` acceptance still requires the corresponding
  compiler version update and tests.

## How to bump a version

1. Change the source-of-truth location for the component.
2. Update the matching row and prose in this file.
3. Update implementation version constants where applicable.
4. Run `make test` and confirm the reported versions match this file.

## History

| Date | Toolchain | Syntax | Nordshrift | NS-SST | Notes |
|------|-----------|--------|------------|--------|-------|
| 2026-09 | 0.1.2 | 1.0 | 2.0 | 2.0.0 | Added common Subject/Quantity/Unit/Assumption/Relation/Transformation/Evidence/Comparison/Explanation/WorkPlan semantic layer while retaining 1.0 compatibility. |
| 2026-09 | 0.1.2 | 1.0 | 1.0 | 1.0.0 | Version-aware compiler; Nordshrift triplet; original NS-SST-0001 specification. |

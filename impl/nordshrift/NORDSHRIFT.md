# Nordshrift — `.sst` transpiler driver and semantic layer

Nordshrift is the **transpiler driver and semantic coordination layer for Sleela**. It reads a **`.sst` Scripting Sheet**, validates its control sections, resolves Sleela Wrapper™ sources, and drives the selected target. Version 2.0 also gives the sheet a common semantic vocabulary for the Math, Physics, Economics, Chemistry, and Financial libraries.

## Version 2.0 architecture

```text
.sst control surface
       │
       ├── source / target / pipeline / rules / effects / derive / guards / interop
       │
       └── subject / quantities / units / assumptions / relations
                         │
                         ▼
              transformations / evidence / comparison
                         │
                         ▼
                 explanation / validation / workplan
                         │
                         ▼
                 Sleela front end → Java | Sleela | C
```

The normative semantic extension is [`/SST-2.0.model`](../../SST-2.0.model). The legacy 1.0 grammar remains in [`/SST.model`](../../SST.model).

## Common subject model

[`subject_model.h`](subject_model.h) defines the implementation vocabulary:

- `Subject`
- `Quantity`
- `Assumption`
- `Relation`
- `Transformation`
- `ComparativeNorm`
- `Evidence`
- `Explanation`
- `Todo`
- `EvidenceStatus`
- `WorkStatus`

The implementation intentionally keeps these structures dependency-light so that they can be used by the Nordshrift driver without coupling the semantic layer to a particular subject library.

### Canonical chain

**Subject → Quantity → Unit → Assumption → Relation → Formula → Transformation → Result → ComparativeNorm → Evidence → Explanation → Validation**

This chain is the principal bridge between declarative `.sst` sheets and executable subject libraries.

## Evidence semantics

`EvidenceStatus` is explicitly epistemic:

- `Observed`
- `Specified`
- `Derived`
- `Modeled`
- `Inferred`
- `Assumed`

The implementation must not silently convert a modeled or inferred result into an observation.

## Work-plan semantics

`Todo` provides an explicit plan with identity, subject, priority, dependencies, preconditions, action, expected result, validation, and `WorkStatus`.

`WorkStatus` values are:

`Planned`, `Ready`, `Active`, `Blocked`, `Validating`, `Complete`, `Deferred`.

Completion should be tied to a validation record rather than merely setting the status to `Complete`.

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

## Subject relationships

Math is foundational. Physics, Economics, Chemistry, and Financial may declare Math as a dependency. Other dependencies must be declared when they are genuinely required; semantic declarations must not imply empirical causation.

## Existing `.sst` pipeline

The 1.0 control sections remain available and continue to describe the build process:

| Section | Purpose |
|---|---|
| `sheet` | manifest and metadata |
| `import` | composition |
| `source` | Wrapper™ source selection |
| `target` | output and target language |
| `pipeline` | processing phases |
| `rules` | rule activation |
| `effects` | effect policy |
| `derive` | derivation configuration |
| `guards` | failure behavior |
| `interop` | language interoperability |
| `profile` | named configuration variants |

2.0 adds semantic sections without removing these build-control concepts.

## Files

```text
nordshrift/
  NORDSHRIFT.md
  README.md
  diagnostics.h
  sst_lexer.{h,cpp}
  sheet_model.h
  sst_parser.{h,cpp}
  source_resolve.{h,cpp}
  sleela_emit.{h,cpp}
  object_compat.cpp
  subject_model.h
  subject_model.cpp
  nordshrift.cpp
  examples/
```

The build now compiles `subject_model.cpp` into the Nordshrift executable.

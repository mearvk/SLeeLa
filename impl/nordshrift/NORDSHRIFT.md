# Nordshrift — `.sst` transpiler driver and semantic layer

Nordshrift is the **transpiler driver and semantic coordination layer for Sleela**. It reads a **`.sst` Scripting Sheet**, validates its control sections, resolves Sleela Wrapper™ sources, and drives the selected target. Version 2.0 also gives the sheet a common semantic vocabulary for the Math, Physics, Economics, Chemistry, Financial, Astrophysics, Sociology, and other Subject Libraries.

## Version 2.0 architecture

The sheet now accepts a unified `object` input layer covering the SLeeLa source architecture: Core, IO, System, Network, Application, Data, Science, Security, and Deployment. Specialized sections retain stronger domain-specific contracts where applicable.

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

## Generic input-object model

`input_object.h` defines `InputObjectCategory` and `InputObject`. Each object has an identity and optional type, source, target, value, inputs, outputs, and named properties. The parser and validator enforce closed categories, known members, and bounded sizes.

The object layer is a validated data boundary. It does not imply network transmission, process execution, filesystem mutation, database access, email submission, or deployment. See [`INPUT-OBJECTS.md`](INPUT-OBJECTS.md).

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

Math is foundational. Physics, Economics, Chemistry, Financial, Astrophysics, and Sociology may declare Math as a dependency. Domain dependencies must remain explicit. Other dependencies must be declared when they are genuinely required; semantic declarations must not imply empirical causation.

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
  INPUT-OBJECTS.md
  diagnostics.h
  sst_lexer.{h,cpp}
  sheet_model.h
  sst_parser.{h,cpp}
  source_resolve.{h,cpp}
  sleela_emit.{h,cpp}
  object_compat.{h,cpp}
  input_object.{h,cpp}
  subject_model.h
  subject_model.cpp
  nordshrift.cpp
  examples/
```

The build now compiles `subject_model.cpp` into the Nordshrift executable.


## Subject Library coverage

Nordshrift now treats the expanded Subject Library set as one semantic family rather than unrelated APIs:

| Subject | Native/source layer | XML model/procedure | Nordshrift relationship |
|---|---|---|---|
| Math | C/C++ core | shared contract | foundational |
| Physics | C/C++ subject layer | shared contract | depends on Math |
| Chemistry | native subject layer | shared contract | domain model |
| Economics | C/C++ subject layer | shared contract | depends on Math |
| Finance | C/C++ subject layer | shared contract | quantitative domain |
| Inference | native subject layer | shared contract | evidence-sensitive |
| Data Analytics | native/API layer | shared contract | statistical/data layer |
| Astrophysics | C/C++ numerical layer | model, procedure, observation | depends on Math and Physics |
| Sociology | C/C++ statistical layer | model, procedure, observation | depends on Math |

A Subject Library addition is complete only when its executable mathematics, semantic declaration, evidence discipline, XML representation, SLeeLa example, and focused validation can be traced together.

## Excellent implementation standard

For the Subject Library system, **Excellent** means:

1. the subject has a stable identity and declared dependencies;
2. quantities have units or explicit dimensions;
3. formulas have named inputs and outputs;
4. assumptions are separate from observations;
5. modeled, inferred, derived, and observed values remain distinct;
6. XML describes models and procedures without becoming arbitrary executable code;
7. native C/C++ provides deterministic implementations where the semantics are mature;
8. SLeeLa source demonstrates the subject vocabulary;
9. Nordshrift can carry the subject through Subject → Quantity → Unit → Assumption → Relation → Formula → Transformation → Result → ComparativeNorm → Evidence → Explanation → Validation;
10. validation and provenance are explicit.

The standard is a software/source-quality target. It does not turn a computed statistic into a causal claim, nor a model into an observation.

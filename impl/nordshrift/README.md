# Nordshrift

**Nordshrift 2.0** is the transpiler driver and semantic coordination layer for Sleela. It reads a **`.sst` control sheet** and drives the transpilation of Sleela sources into the target selected by the sheet — the **triplet**: **Java**, **Sleela** (executed on the C core), or **C**.

The `.sst` file remains the *control surface*, not the program. In 2.0 it can also carry explicit semantic models shared by the Math, Physics, Economics, Chemistry, Financial, Astrophysics, and Sociology libraries.

## Build & run

```sh
cd impl
make

./build/nordshrift check <sheet.sst>
./build/nordshrift build <sheet.sst>
```

The Nordshrift build includes `subject_model.cpp` and `input_object.cpp`, providing the common semantic model and cross-domain SLeeLa input-object model.

## Unified SLeeLa input objects

Nordshrift accepts a generic `object` declaration for SLeeLa architecture inputs. Supported categories are `core`, `io`, `system`, `network`, `application`, `data`, `science`, `security`, and `deployment`.

An input object carries an identity plus optional `type`, `source`, `target`, `value`, `inputs`, `outputs`, and named properties. Categories, members, string sizes, and collection counts are validated. Unknown categories and members are rejected.

Generic objects complement the specialized `network`, `finance`, `reach`, `measure`, and `subject` structures. They are descriptive input contracts and do not implicitly open sockets, execute processes, access files, send email, connect to databases, or deploy services.

See [`INPUT-OBJECTS.md`](INPUT-OBJECTS.md) and [`examples/input-objects.sst`](examples/input-objects.sst).

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

Three first-class series blocks are also available:

- `network` — the closed network object series (see [`SST.NETWORK.md`](SST.NETWORK.md)).
- `finance` — the closed finance object series (see [`SST.FINANCE.md`](SST.FINANCE.md)).
- `subject` — the 2.0 semantic subject layer with nested `quantity`,
  `assumption`, `relation`, `transformation`, `comparison`, `evidence`,
  `explanation`, and `todo` blocks (see [`SST.SUBJECT.md`](SST.SUBJECT.md)).

On `build`, these three series are emitted into a **component manifest**
artifact for the selected target (a Java class, C arrays, or a Sleela class), so
declaring a series has a real, inspectable build effect.

## Self-contained demo

```sh
./build/nordshrift check nordshrift/examples/demo/build.sst
./build/nordshrift build nordshrift/examples/demo/build.sst
```

## Object compatibility list (SHEET.sheet)

Nordshrift also carries every object from the repo-root `SHEET.sheet` catalog
(156 objects across 18 role categories — including the `network` and `finance`
component series — with the `System` root at depth 3024 / complexity degree 4)
in an **object compatibility list**, and converts each object into a per-target
relevance:

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


## SLeeLa ↔ Nordshrift from the `sleela` compiler

The `sleela` compiler can drive the Nordshrift emitter directly, so you can go
**from SLeeLa to Nordshrift and back** without the standalone `nordshrift`
binary or an `.sst` sheet:

```sh
# SLeeLa -> Nordshrift transpile (choose the triplet target)
sleela nordshrift --emit --target=sleela  prog.sleela   # canonical Sleela
sleela nordshrift --emit --target=java     prog.sleela   # Java
sleela nordshrift --emit --target=c        prog.sleela   # C
sleela nordshrift --emit --target=java --package=com.acme prog.sleela

# SLeeLa -> Nordshrift(Sleela) -> re-parse -> run (round trip, on the core)
sleela nordshrift --roundtrip prog.sleela
```

`--roundtrip` transpiles the program to the Nordshrift **Sleela** target,
re-parses that output, verifies the program's **structure is preserved** (same
classes, and per class the same field/method counts), and runs the re-parsed
program on the C core. It reports whether the round trip is an *exact fixed
point* or merely *canonical-form stable* (the emitter fully parenthesizes and
normalizes blocks, so a second pass can differ cosmetically while remaining
structurally identical). The `sleela` binary links only the Nordshrift **emitter**
for this (not the standalone driver), so there is no second `main`.


## Subject Library coverage

The shared subject layer currently documents and carries:

- Math — foundational numerical vocabulary.
- Physics — mathematical physical relations.
- Chemistry — domain-specific chemical models.
- Economics — quantitative economic relations.
- Finance — financial calculations.
- Inference — evidence-sensitive inference structures.
- Data Analytics — statistical/data operations.
- Astrophysics — astronomical numerical relations plus observed-data/provenance XML.
- Sociology — population, rate, distribution, transition, inequality and group-separation statistics.

Astrophysics declares Math and Physics dependencies. Sociology declares Math. The .sst subject layer keeps these dependencies explicit and preserves evidence status.

## Excellent standard

A Subject Library reaches the repository's **Excellent** engineering standard when native mathematics, semantic declarations, XML model/procedure definitions, SLeeLa examples, provenance, evidence status, focused tests, and documented interpretation limits are all connected and inspectable.

## Nordshrift Complete

**Nordshrift Complete** is the application/service authoring edition built on the existing Nordshrift 2.0 pipeline and the SLeeLa-Complete base-class library.

It provides reusable designer surfaces for Core/Runtime, API contracts, Platform, Network, Security, Data, Database, HTTP/Web, Server/Service, I/O, Synchronization, UI, XML, Email, Media, Telephony/VoIP, Memory Management, Reflection, AI/Inference, Analytics, Regex, Science/Subject Libraries, Terminal, and developer tooling.

See [`NORDSHRIFT-COMPLETE.md`](NORDSHRIFT-COMPLETE.md) for the complete application/service class catalog and authoring rules.

The canonical base-class definitions remain in [`../../api/sleela-complete/`](../../api/sleela-complete/), with the module matrix in [`../../api/sleela-complete/MODULE-MATRIX.md`](../../api/sleela-complete/MODULE-MATRIX.md). Native Memory Manager and Reflection foundations are documented under [`../../api/native/memory/`](../../api/native/memory/) and [`../../api/native/reflection/`](../../api/native/reflection/).

Nordshrift Complete is an **authoring-level completeness** designation. It does not claim that every native backend or platform implementation is production-complete.
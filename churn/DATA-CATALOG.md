# Churn Data Catalog and Integration Plan

Max Rupplin - MEARVK LLC - 2026

## Purpose

This catalog defines how the five Churn dimensions fit together and how new data should be added without creating duplicate, untraceable, or dimensionally incompatible records. It complements the set-specific READMEs and BASIS documents; it does not replace them.

## Existing data inventory

The current repository already contains representative data in multiple formats:

| Set | Dimension | Existing examples | Integration role |
|---|---|---|---|
| [1 — Longs](1/README.md) | Time series | `longs.csv`, assumptions CSV and SLeeLa model | Period-based measurements and assumptions |
| [2 — Structures](2/README.md) | Scalar/value with unit or evidence kind | Math and physics constants, SI base units, chemical elements, solar-system and social-labor data | Canonical values and measurement definitions |
| [3 — Facets](3/README.md) | Property of an identified object | Math/physics facets, moons, electronegativity, SI derived units, social-labor facets | Object-to-property relationships |
| [4 — Areas](4/README.md) | Two-dimensional extent | Geometry areas, country areas, Earth surface, Great Lakes, continents, US density | Area values and spatial context |
| [5 — Lines](5/README.md) | One-dimensional length or count | Integers, geometry lengths, physical lengths, US lengths, common counts | Count/length values and derived line measures |

These examples establish useful coverage, but the files must be checked individually before being treated as authoritative. File presence alone does not prove correctness, completeness, or currentness.

## Shared record contract

New machine-readable records should provide the following fields where applicable:

- `id`: stable, unique identifier within the dataset.
- `label`: human-readable name.
- `dimension`: `LONG`, `STRUCTURE`, `FACET`, `AREA`, or `LINE`.
- `domain`: `MATH`, `PHYSICS`, or `SOCIAL` (or a documented extension).
- `value` or `values`: numeric or structured value; never encode a numeric quantity only in prose.
- `unit`: explicit unit or `1` for dimensionless values.
- `evidence_kind`: `EXACT`, `MEASURED`, `OBSERVED`, `ASSUMED`, or `DERIVED`.
- `source_uri`: stable source URL or a repository-relative source document.
- `retrieved_at` / `valid_period`: retrieval date or period for values that change.
- `uncertainty`: measurement uncertainty or a documented reason it is unavailable, when relevant.
- `derived_from`: source record IDs for derived values; empty for primary facts.

This is a proposed data contract. Existing records need not be bulk-rewritten until a migration and compatibility plan is reviewed.

## Cross-dimensional relationships

Use explicit identifiers rather than relying on filenames or matching display labels.

- **Long → Structure:** a time-series observation references the scalar definition, unit, and series identity it measures.
- **Structure → Facet:** a facet references its subject object and the property definition/value it describes.
- **Facet → Area:** an area can reference the geographic or geometric object whose extent it represents.
- **Structure/Facet → Line:** a line record references the count or length definition, source, and unit.
- **Area ↔ Line:** a relationship is valid only when the operation is defined (for example, perimeter is a length derived from a boundary; area divided by length has units of length). Do not infer relationships from similar names alone.

A cross-dimensional relationship should record at least `from_id`, `relationship`, `to_id`, and, for derived relationships, the method/formula and unit handling.

## Inheritance and composition

Prefer composition for cross-dimensional records: a measurement references a Structure, a Facet references an object and a property definition, and derived records reference their inputs. This avoids pretending that a time series, an area, and a line are interchangeable types.

If actual SLeeLa class inheritance is introduced later, keep a small shared evidence/provenance base contract and make the five dimension types distinct descendants only if the language/compiler semantics support that design. Do not claim runtime inheritance merely because the documentation describes conceptual relationships. The existing Java constraint-report classes are build/test utilities, not the data-model inheritance hierarchy.

## Import policy

Before importing a dataset:

1. Identify the publisher and canonical source URL.
2. Record retrieval date, license/terms, version, and any attribution requirements.
3. Preserve the source file or a reproducible retrieval procedure; do not silently overwrite original data.
4. Normalize units and document conversion formulas.
5. Map records to stable IDs and check for duplicates against existing canonical records.
6. Label data as primary, observed, assumed, or derived; never present assumptions as measurements.
7. Add source-specific notes and a small fixture/test for parsing and key invariants.
8. Update the relevant set README and this catalog in the same change.

Do not add bulk external data merely to increase file count. Import only when it adds a clear dimension/domain, has verifiable provenance, and can be validated.

## Validation roadmap

The current `validate_data.py` checks JSON/XML parsing, CSV column counts, and nonempty TXT files. Expand validation incrementally to include:

1. Required headers/fields and duplicate IDs within each dataset.
2. Referential integrity for `derived_from` and cross-dimensional links.
3. Unit presence and compatible dimensions before arithmetic or comparison.
4. Range and invariant checks that are justified by the dataset's source/specification.
5. Provenance and license metadata for imported data.
6. Deterministic smoke tests for each SLeeLa model and each Java constraint report.

Keep validation fail-closed for malformed records, but report warnings separately for missing optional metadata in legacy datasets so migration can proceed deliberately.

## Build entry points

From the repository root:

```sh
make churn
make churn-data-check
make churn-test
```

From this directory:

```sh
make all
make data-check
make test
make clean
```

The root Makefile already dispatches `churn`, `churn-data-check`, and `churn-test`. Avoid duplicating the Churn compiler commands in the root Makefile; keep the local Makefile authoritative.

## Immediate priorities

- [ ] Add this catalog to the set documentation links.
- [ ] Review the existing JSON/CSV/XML schemas and document source/license/retrieval metadata.
- [ ] Add cross-dimensional IDs and reference checks where the current records support them.
- [ ] Check mathematical invariants and unit consistency with source-backed expected values.
- [ ] Only then import additional source-backed data in targeted gaps.

# Churn Data Catalog and Integration Plan

Max Rupplin - MEARVK LLC - 2026

## Purpose

This catalog defines how the five Churn dimensions fit together and how new data should be added without creating duplicate, untraceable, or dimensionally incompatible records. It complements the set-specific READMEs and BASIS documents; it does not replace them.

## Existing data inventory

| Set | Dimension | Existing examples | Integration role |
|---|---|---|---|
| [1 — Longs](1/README.md) | Time series | `longs.csv`, assumptions CSV and SLeeLa model | Period-based measurements and assumptions |
| [2 — Structures](2/README.md) | Scalar/value with unit or evidence kind | Math and physics constants, SI base units, chemical elements, solar-system and social-labor data | Canonical values and measurement definitions |
| [3 — Facets](3/README.md) | Property of an identified object | Math/physics facets, moons, electronegativity, SI derived units, social-labor facets | Object-to-property relationships |
| [4 — Areas](4/README.md) | Two-dimensional extent | Geometry areas, country areas, Earth surface, Great Lakes, continents, US density | Area values and spatial context |
| [5 — Lines](5/README.md) | One-dimensional length or count | Integers, geometry lengths, physical lengths, US lengths, common counts | Count/length values and derived line measures |

The repository has broad representative coverage. File presence alone does not prove correctness, completeness, licensing, or currentness; do not label a dataset authoritative without checking its publisher and source document.

## Shared record contract

For newly normalized machine-readable datasets, use the following fields where applicable:

- `id`: stable, unique identifier within the dataset (prefer publisher identifiers where available; otherwise use a documented namespace such as `churn.line.us-state-count`).
- `label`: human-readable name.
- `dimension`: `LONG`, `STRUCTURE`, `FACET`, `AREA`, or `LINE`.
- `domain`: `MATH`, `PHYSICS`, or `SOCIAL` (or a documented extension).
- `value` or `values`: numeric or structured value; do not encode a numeric quantity only in prose.
- `unit`: explicit unit or `1` for dimensionless values.
- `evidence_kind`: `EXACT`, `MEASURED`, `OBSERVED`, `ASSUMED`, or `DERIVED`.
- `source_uri`: canonical publisher URL or repository-relative source document.
- `retrieved_at`, `published_at`, or `valid_period`: date/period where the value can change.
- `license` / `attribution`: license or terms and required attribution for imported data.
- `uncertainty`: uncertainty or a documented reason it is unavailable, when relevant.
- `derived_from`: source record IDs for derived values; empty for primary facts.

New normalized JSON datasets may opt into the validator's strict contract by setting top-level `"record_contract": "churn-1"` and supplying top-level `source_uri` and `license`. This is an opt-in migration path; legacy data is not silently rewritten.

## Cross-dimensional relationships

Use explicit identifiers rather than relying on filenames or matching display labels. A relationship record should contain `from_id`, `relationship`, and `to_id`; derived relationships also need `method` or `formula` and explicit unit handling.

- **Long → Structure:** a time-series observation references the scalar definition, unit, series identity, and period it measures.
- **Structure → Facet:** a facet references its subject object and the property definition/value it describes.
- **Facet → Area:** an area can reference the geographic or geometric object whose extent it represents.
- **Structure/Facet → Line:** a line references the count/length definition, source, and unit.
- **Area ↔ Line:** only connect when the operation is defined (for example, perimeter is a length derived from a boundary; area divided by length has units of length). Never infer links from similar labels alone.

## Formulas and dimensional compatibility

Store formula identity separately from observed values. A formula record should document its inputs, output unit, and assumptions. Validate unit compatibility before comparing or combining quantities. Examples of expected dimensional checks:

- Area: length × length → length².
- Perimeter/circumference: length → length.
- Density: count ÷ area → count/area.
- A time-series correlation is dimensionless and descriptive, not causal.
- Unit conversions must record the conversion factor and source; do not compare unlike units before conversion.

## Inheritance and composition

Prefer composition for cross-dimensional records: a Long observation references a Structure definition, a Facet references an object and a property, and derived records reference their inputs. This avoids pretending that a time series, area, and line are interchangeable types.

If actual SLeeLa class inheritance is introduced later, keep a small shared evidence/provenance contract and make the five dimension types distinct descendants only if compiler semantics support that design. The Java constraint-report classes are build/test utilities, not the data-model inheritance hierarchy. The current sets should therefore be described as related by composition and shared validation, not as implemented subclasses.

## Import policy

Before importing a dataset:

1. Identify the publisher and canonical source URL.
2. Record retrieval date, license/terms, version, and attribution requirements.
3. Preserve the original source file or a reproducible retrieval procedure.
4. Normalize units and document conversion formulas.
5. Map records to stable IDs and check duplicates against canonical records.
6. Label primary, observed, assumed, or derived data correctly.
7. Add a source-specific note and a parsing/invariant test.
8. Update the relevant set README and this catalog in the same change.

Do not import bulk data merely to increase file count. Prefer authoritative publisher sources, but import only when the data adds a clear dimension/domain, provenance is verifiable, and the data can be validated. Respect source terms and licenses.

## Validation and test coverage

`validate_data.py` currently checks JSON/XML parseability, CSV header/row consistency, duplicate CSV IDs where an `id` column exists, duplicate IDs in record-array JSON, known evidence-kind values, and the opt-in `churn-1` JSON contract. These structural checks do not establish scientific correctness or that a source remains current.

The validation roadmap is:

1. Required fields and stable IDs for normalized datasets.
2. Referential integrity for `derived_from` and cross-dimensional relationship records.
3. Unit compatibility before arithmetic or comparison.
4. Source-backed range/invariant checks.
5. Provenance, license, and retrieval metadata for imports.
6. Deterministic smoke tests for each SLeeLa model and Java constraint report.

Legacy files may remain outside the strict contract until migrated; do not turn missing optional legacy metadata into false validation failures. Invalid syntax and explicit contract violations fail the check.

## Build entry points

From repository root:

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

The root Makefile already dispatches these targets. Keep the local Makefile authoritative and avoid duplicating compiler commands at the repository root.

## Immediate implementation status

- [x] Document the five dimensions and composition-first relationship model.
- [x] Strengthen structural checks while preserving a deliberate legacy migration path.
- [x] Define stable-ID, provenance, evidence-kind, formula, and license expectations.
- [x] Keep build/test entry points connected through the root Makefile.
- [ ] Add normalized cross-dimensional link files only when their endpoint IDs are grounded in existing source records.
- [ ] Run the complete build/test suite in a checkout with the required toolchain and verify branch parity.

# Churn — data, dimensions, constraints, and inheritance

Churn organizes five complementary ways to model a magnitude. Each numbered set
has a distinct primary dimension, but all sets share provenance and quality
rules, the three-domain grouping (math, physics, social), and a constraint layer.

See [DATA-CATALOG.md](DATA-CATALOG.md) for the cross-dimensional data catalog,
shared record contract, import policy, and integration roadmap.

## Set index

| Set | Dimension | Core model | Question |
|---|---|---|---|
| [1 — Longs](1/README.md) | temporal series | `Long` and pairwise relevance | How do measurements co-vary across periods? |
| [2 — Structures](2/README.md) | standing scalar | `Structure` | What is the value, unit, and evidence kind? |
| [3 — Facets](3/README.md) | object + selected face | `Facet` | Which property of which object is described? |
| [4 — Areas](4/README.md) | two-dimensional extent | `Area` | What extent is occupied, and relative to what? |
| [5 — Lines](5/README.md) | one-dimensional count/length | `Line` | How far does a count or length reach, and what Spark guides its reading? |

## Organization

Each set keeps its model source and local explanation beside the data it
interprets. Sets /2–/5 group inputs under `data/` and domain explanations under
`models/`; /1 keeps its annual series in `longs.csv` and its separate
assumptions layer under `assumptions/`.

- `N/README.md`: purpose, sources, run instructions, and file index.
- `N/*.sleela`: SLeeLa model for the set's dimension.
- `N/Constraints.java` and `N/constraints.sleela`: JVM and SLeeLa constraint
  representations. Java report classes share a fully-qualified class name, so
  each set is compiled into a separate `build/N/` directory.
- `N/data/`, `N/models/`: input facts and domain-level explanations.
- `N/BASIS.md`: basis document for sets that define one.
- `5/SPARKS.md`: interpretive guidance for Lines; Sparks are not evidence.

## Shared data contract and relationships

These are conceptual/data-contract relationships, not claims that SLeeLa
currently implements class inheritance between the five model structs.

1. **Shared evidence contract.** Factual records should identify their name,
   value(s), unit or measurement basis, evidence kind (`EXACT`, `MEASURED`,
   or `OBSERVED` where applicable), source/provenance, and date or period when
   values can change.
2. **Domain is cross-cutting.** Math, physics, and social classify the subject;
   they do not replace Long/Structure/Facet/Area/Line.
3. **Derived relationships must be explicit.** A Facet should identify its
   source object(s); an Area derived from geometry should identify its formula
   and inputs; a Line derived from a count or length should retain its unit and
   source. Do not conflate formulas, measurements, and observations.
4. **Temporal joins need matching periods.** Longs should document period,
   frequency, units, and fiscal/calendar-year differences before series are
   compared. Correlation is descriptive, not causal.
5. **No speculative imports.** Add data only with a verifiable source,
   retrieval/update date, units, evidence kind, and an explanation of how it
   enriches the model. Prefer updating a canonical dataset over duplicates.

## Build and validation

From the repository root:

```sh
make churn
make churn-data-check
make churn-test
```

Or from this directory: `make all`, `make test`, `make clean`.

The data check parses JSON/XML/CSV/TXT, checks CSV headers and row widths, detects duplicate IDs in supported record arrays/CSV ID columns, validates evidence-kind values, and enforces the opt-in `churn-1` JSON contract. It does not establish scientific correctness, unit compatibility, referential integrity, or that sources are still current. Java
constraint reports are generated outputs and remain ignored by Git. See
[DATA-CATALOG.md](DATA-CATALOG.md) for the recommended validation roadmap.

## Evidence discipline

- `EXACT` means definitional/formula-based, not merely a rounded decimal.
- `MEASURED` values should retain source uncertainty where available.
- `OBSERVED` values need a stated period or retrieval context.
- Record units explicitly; convert before comparing unlike units.
- Keep assumptions, derived values, and published observations distinguishable.
- A Spark is explanatory guidance, never a factual source.

# SLeeLa — SOCIALISM.md

## Constitutional Socialism Reference — neutral, sourced registry indicator

This companion document specifies the final column of `BANKS4.md` for the
project's 391-country/jurisdiction registry. It records **one observable,
citable fact** about each jurisdiction and is deliberately *descriptive*, not
evaluative.

> **The question:** Does the jurisdiction's own current constitution / basic law
> explicitly reference socialism (a socialist state, society, or economy) in its
> text?

It replaces an earlier four-value "Socialism Status" scheme
(`YES` / `NO` / `CONVINCED` / `INDEED`) that assigned an opaque, editorializing
label to every row from a hardcoded, uncited country list. That scheme was
subjective and — by inventing a value for every jurisdiction with no source —
contradicted this registry's own data-integrity rule (below). The neutral
indicator restores that discipline.

### The three permitted values

| Value | Definition |
|---|---|
| `CONSTITUTIONAL` | The jurisdiction's current constitution explicitly references socialism, per a cited source. |
| `NONE` | The current constitution contains no such reference, per a cited source. |
| `UNASSESSED` | Not yet verified against a cited constitutional source (the default). |

No other value is permitted. A jurisdiction that has not been verified against a
cited constitutional source remains `UNASSESSED` — it is never guessed.

### Required companion fields

Each non-`UNASSESSED` assignment should retain the following evidence fields
(the generator records the value and source inline; richer records may add the
rest):

| Field | Purpose |
|---|---|
| `ID` | Stable registry identifier. |
| `Country/Jurisdiction` | Jurisdiction name (exactly as in the registry). |
| `Constitutional Socialism Reference` | One of `CONSTITUTIONAL`, `NONE`, `UNASSESSED`. |
| `Constitutional Provision` | The article/preamble clause evaluated, when applicable. |
| `Source` | The jurisdiction's own constitution / basic law supporting the value. |
| `Verification Date` | Date the constitutional text was checked. |

### Data integrity rule

This is a constitutional-**text** indicator, not a judgment about a country's
economy, policy, or political character. A value MUST be supported by a citation
to the jurisdiction's own constitution and MUST NOT be inferred from geography,
country name, income level, an arbitrary numerical identifier, or a maintainer's
opinion. Absent a cited source, the value is `UNASSESSED`.

### Tooling

The column is populated by
[`tools/banking/update_banks4_socialism.py`](tools/banking/update_banks4_socialism.py)
and titled **"Constitutional Socialism Reference"** in `BANKS4.md`
(via [`tools/banking/update_banks4.py`](tools/banking/update_banks4.py)). See
[`tools/banking/CONSTITUTIONAL_SOCIALISM_REFERENCE.md`](tools/banking/CONSTITUTIONAL_SOCIALISM_REFERENCE.md)
for the full description and how to extend the sourced table. The
`Annotate BANKS4 Constitutional Socialism Reference` workflow applies it and
validates that every row holds one of the three permitted values.

### Relationship to BANKS.md

`BANKS.md` remains the principal national banking, economic and historical
registry. This document specifies only the neutral constitutional-reference
indicator carried in the final `BANKS4.md` column, preserving all existing
banking, currency, GDP, trade, supervision and other registry fields.

### Versioning

Any country-level value change should preserve the reason for the change and its
supporting constitutional source in Git history or an accompanying evidence
record.

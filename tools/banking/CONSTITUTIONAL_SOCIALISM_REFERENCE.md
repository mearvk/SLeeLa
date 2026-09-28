# BANKS4 — "Constitutional Socialism Reference" column

The final column of `BANKS4.md` records a single, neutral, observable fact for
each jurisdiction, populated by
[`update_banks4_socialism.py`](update_banks4_socialism.py):

> **Does the jurisdiction's own current constitution / basic law explicitly
> reference socialism (a socialist state, society, or economy) in its text?**

## Permitted values

| Value | Meaning |
|---|---|
| `CONSTITUTIONAL` | The current constitution explicitly references socialism, per a cited source. |
| `NONE` | The current constitution contains no such reference, per a cited source. |
| `UNASSESSED` | Not yet verified against a cited constitutional source (the default). |

## Why this replaced the earlier "Socialism Status"

An earlier version assigned every one of the 391 rows one of four opaque,
editorializing labels (`YES` / `NO` / `CONVINCED` / `INDEED`) from a hardcoded,
uncited country list. That was subjective, non-neutral, and — because it invented
a value for every jurisdiction with no source — it contradicted the registry's
own evidence discipline (see [`BANKS.md`](../../BANKS.md): political and
historical values are not invented unless present in the registry or an
explicitly sourced enrichment).

The replacement is:

- **Neutral & descriptive.** It reports a fact about constitutional *text*, not a
  judgment about a country's economy, policy, or political character.
- **Sourced.** Each non-`UNASSESSED` value carries a citation in the script's
  `CONSTITUTIONAL_REFERENCE` table (the jurisdiction's own constitution; official
  texts are hosted by the neutral Constitute Project, constituteproject.org).
- **Honest about coverage.** Jurisdictions not yet verified against a cited
  source remain `UNASSESSED` rather than being guessed. Historical entities are
  left `UNASSESSED` because the question is about a *current* constitution.

## Extending the table

To classify another jurisdiction, add an entry to `CONSTITUTIONAL_REFERENCE` in
[`update_banks4_socialism.py`](update_banks4_socialism.py) as
`"<country>": ("CONSTITUTIONAL"|"NONE", "<citation>")`, using the country name
exactly as it appears in the registry. Do not assign a value without a citation.

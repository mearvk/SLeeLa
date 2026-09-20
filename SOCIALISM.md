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

### Summary and Method

**Summary.** This document defines a single, neutral, sourced indicator — the
*Constitutional Socialism Reference* — recording whether a jurisdiction's own
current constitution explicitly references socialism (a socialist state,
society, or economy). It is deliberately descriptive rather than evaluative: it
reports a fact about constitutional *text*, not a judgment about a country's
economy, healthcare, welfare provision, or political character. Each
jurisdiction resolves to exactly one of three permitted values —
`CONSTITUTIONAL`, `NONE`, or `UNASSESSED` — and the accompanying sourced list
names the twelve constitutions currently verified as `CONSTITUTIONAL`, each with
its provision, adoption/amendment year, and citation.

**Method.** A value is assigned only when it can be tied to the jurisdiction's
own constitution or basic law; it is never inferred from geography, country
name, income level, an identifier, or a maintainer's opinion. Absent such a
citation the value stays `UNASSESSED`. This discipline is why the document
separates constitutional text from *policy outcomes* often associated with
socialist governance — for example public-health systems: the Soviet Union built
one of the earliest state-run universal healthcare models, the Semashko system,
established after the 1917 revolution and consolidated through the 1920s. Such
policy history is context, not evidence for the indicator: a healthcare or
welfare program, whenever founded, does not by itself make a constitution
"socialist" in the sense measured here. Related economic and education context
lives in the separate [`GDP_EDUCATION.md`](GDP_EDUCATION.md) registry.

> **Maintainer note (unverified):** a 1955 milestone in Soviet medicine has also
> been recorded by the maintainer. It is retained here as a maintainer-supplied
> note pending a cited source and, per the data-integrity rule above, carries no
> weight for any jurisdiction's `Constitutional Socialism Reference` value.
>
> The maintainer further records that, in 1955, the "issue of Combine and
> Calculus" was regarded as resolved and that socialism was, by that point,
> considered a solved question. This too is a maintainer-supplied note, is
> **unverified and uncited**, and likewise carries no weight for any
> jurisdiction's value here.

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

### Sourced list — constitutions that reference socialism (`CONSTITUTIONAL`)

The following jurisdictions carry a `CONSTITUTIONAL` value because their **own
current constitution / basic law explicitly references socialism** (a socialist
state, society, or economy). Each row cites the specific provision and the
**year** that provision was adopted or introduced by amendment. These are the
entries currently backed in
[`tools/banking/update_banks4_socialism.py`](tools/banking/update_banks4_socialism.py);
every other jurisdiction remains `UNASSESSED` until verified against a cited
source. Verification date: 2026-09-20.

| ID | Country/Jurisdiction | Constitutional Provision | Year | Source |
|---|---|---|---|---|
| CS-01 | China | Art. 1 — "socialist state under the people's democratic dictatorship" | 1982 | Constitution of the PRC (1982), Art. 1 — Constitute Project |
| CS-02 | Cuba | Art. 1 — "socialist state of law and social justice" | 2019 | Constitution of Cuba (2019), Art. 1 — Constitute Project |
| CS-03 | Lao People's Democratic Republic | Preamble / Art. 2 — people's democratic state advancing toward socialism | 1991 | Constitution of Laos (1991), Preamble/Art. 2 — Constitute Project |
| CS-04 | Vietnam | Art. 2 — "socialist rule-of-law state" | 2013 | Constitution of Vietnam (2013), Art. 2 — Constitute Project |
| CS-05 | North Korea | Art. 1 — "socialist state representing the interests of all the Korean people" | 1972 | DPRK Socialist Constitution (1972), Art. 1 — Constitute Project |
| CS-06 | India | Preamble — "SOCIALIST" (inserted by the 42nd Amendment) | 1976 | Constitution of India, Preamble (42nd Amendment, 1976) — Constitute Project |
| CS-07 | Bangladesh | Art. 8 — socialism as a fundamental principle of state policy | 1972 | Constitution of Bangladesh (1972), Art. 8 — Constitute Project |
| CS-08 | Sri Lanka | Preamble / Art. 2 — "Democratic Socialist Republic of Sri Lanka" | 1978 | Constitution of Sri Lanka (1978), Preamble — Constitute Project |
| CS-09 | Nepal | Preamble — commitment to "socialism based on democratic norms and values" | 2015 | Constitution of Nepal (2015), Preamble — Constitute Project |
| CS-10 | Portugal | Preamble — opening "the way to a socialist society" | 1976 | Constitution of Portugal (1976), Preamble — Constitute Project |
| CS-11 | Guyana | Preamble — "transition ... to socialism" | 1980 | Constitution of Guyana (1980), Preamble — Constitute Project |
| CS-12 | Tanzania | Art. 3 / Art. 9 — socialism (*Ujamaa*) and self-reliance | 1977 | Constitution of Tanzania (1977, rev. 2005), Art. 3/9 — Constitute Project |

**Reading the "Year" column.** The year is when the *socialism reference* entered
the jurisdiction's current constitutional text — either the year that constitution
was adopted (e.g., Portugal 1976, Cuba 2019) or the year an amendment introduced
the reference (e.g., India's 42nd Amendment, 1976). It is not a ranking, a score,
or a statement about the country's economy or governance today.

**Scope note.** This list is intentionally limited to jurisdictions with an
explicit, cited constitutional reference. It is *not* a list of "socialist
countries" in any political or economic sense, and it deliberately carries no
ranking, tier, or external-endorsement dimension — assigning one would violate
the data-integrity rule below.

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

### Related registries

- [`GDP_EDUCATION.md`](GDP_EDUCATION.md) — a separate, sourced reference listing
  the top 100 economies by nominal GDP (IMF) paired with the UN Education Index
  (UNDP). It is an **economic and education** dataset and is deliberately kept
  independent of this document: a jurisdiction's `Constitutional Socialism
  Reference` value here must **not** be inferred from its GDP rank, education
  score, or any figure in that file (see the data-integrity rule above). The two
  registries are cross-referenced for navigation only, not for deriving one from
  the other.

### Versioning

Any country-level value change should preserve the reason for the change and its
supporting constitutional source in Git history or an accompanying evidence
record.

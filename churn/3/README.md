# /3 — Facets

**Facets** is the third *relevance set* (a sibling of `/1` [Longs](../1/README.md)
and `/2` [Structures](../2/README.md)). Where a **Long** is a magnitude that
*runs over time* and a **Structure** is a magnitude that *stands*, a **Facet** is
**one face of a many-sided object shown from a chosen angle**: a derived physical
constant, a special mathematical value, a moving face of the labor market.

This folder is the data + models for one relevance set. It gathers **real,
sourced** facets across three domains, holds them as JSON / XML / TXT data
models, and rests them on a **basis document** — the *Medium requirement for
Workers, Work and Excellence — Workers*.

> **Long vs. Structure vs. Facet.**
> A **Long** extends period by period — we ask how two lengths *co-occupy*.
> A **Structure** stands — we ask its magnitude and kind.
> A **Facet** is *one face of a thing* — we name the **object** it is a face of,
> the **face** itself, and its **kind**:
>
> | Kind | Meaning | Example |
> |---|---|---|
> | **EXACT** | a definition or exact derivation — no uncertainty, no date | ζ(2) = π²/6, the molar gas constant `R` |
> | **MEASURED** | an experimental face with a standard uncertainty | fine-structure constant α, Rydberg constant |
> | **OBSERVED** | a published figure for a stated period | US quits rate, 2024 |
>
> `EXACT ≠ MEASURED ≠ OBSERVED`; `ASSOCIATION ≠ CAUSATION`; `ASSUMPTION ≠ FACT`.

## The three models

| Model | Domain | Facets it holds |
|---|---|---|
| [`models/PHYSICS_MODEL.md`](models/PHYSICS_MODEL.md) | physics | CODATA 2022 derived constants + SI derived units |
| [`models/MATH_MODEL.md`](models/MATH_MODEL.md) | mathematics | special values + identity faces |
| [`models/SOCIAL_MODEL.md`](models/SOCIAL_MODEL.md) | society | US labor-market flow faces (BLS JOLTS / CES) |

## The data models

All data are **real and sourced** (each file embeds its provenance), stored as
JSON, XML, or TXT so they read cleanly on GitHub or a public server:

| File | Format | What it is | Source |
|---|---|---|---|
| [`data/physics-facets.json`](data/physics-facets.json) | JSON | derived & related physical constants | CODATA 2022 / NIST |
| [`data/si-derived-units.xml`](data/si-derived-units.xml) | XML | SI coherent derived units (named) | BIPM SI Brochure (2019) |
| [`data/math-facets.json`](data/math-facets.json) | JSON | special values + identity faces | public domain / DLMF / OEIS |
| [`data/social-labor-facets.json`](data/social-labor-facets.json) | JSON | US labor-market flow faces | BLS JOLTS / CES |
| [`data/chemistry-electronegativity.txt`](data/chemistry-electronegativity.txt) | TXT | Pauling electronegativity (a chemical face) | Pauling scale / CRC / IUPAC |
| [`data/moons.json`](data/moons.json) | JSON | largest natural satellites (planetary faces) | NASA / JPL |

## The model

[`facets.sleela`](facets.sleela) (syntax 1.2) loads Facets from all three
domains as `Facet` records, and prints:

1. a **mathematical** listing — each facet's object, its face, value, order of
   magnitude, and kind, and
2. an **evaluation** — what each facet's kind (`EXACT` / `MEASURED` /
   `OBSERVED`) means for how far it can be trusted.

Run it from the repository root:

```sh
export SLEELA_SHA256_MANIFEST=security/sha256-manifest.json
export SLEELA_SHEET=SHEET.sheet
impl/build/sleela run churn/3/facets.sleela
```

## The basis

[`BASIS.md`](BASIS.md) is the basis document this set rests on: the **Medium
requirement for Workers, Work and Excellence — Workers**. It treats a working
society as an *assailable and firm* medium (the repository's subject-medium idea,
[`../../FILE.SYSTEM.md`](../../FILE.SYSTEM.md)) and reads its **moving faces** —
openings, quits, hours, earnings — to state the condition the medium must
satisfy: a **fluid** market of **workers**, putting in real **work**, rewarded
with rising **excellence**.

## Files

| File | What it is |
|---|---|
| [`BASIS.md`](BASIS.md) | the Medium requirement for Workers, Work and Excellence — Workers |
| [`facets.sleela`](facets.sleela) | the Facets model over all three domains |
| [`constraints.sleela`](constraints.sleela) / [`Constraints.java`](Constraints.java) | the Constitution congrains/limits applied to Facets (shared across `/1`–`/4`) |
| [`models/`](models/) | the physics, math, and social model docs |
| [`data/`](data/) | the sourced data models (JSON / XML / TXT) |

*The facets here are real and sourced. EXACT facets are definitions or exact
derivations, MEASURED facets carry uncertainty, and OBSERVED facets are
published figures for a stated period. Any relation drawn between facets is a
model, not a causal claim.*

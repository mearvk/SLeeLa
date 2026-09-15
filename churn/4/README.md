# /4 — Areas (Real Model Area)

**Areas** is the fourth *relevance set* (a sibling of `/1` [Longs](../1/README.md),
`/2` [Structures](../2/README.md), and `/3` [Facets](../3/README.md)). It is the
**Real Model Area**: data modelled around **area** — a magnitude that **spans**,
a real two-dimensional extent a model occupies.

The churn relevance sets read a magnitude four ways:

| Set | A magnitude that… | We ask… |
|---|---|---|
| `/1` **Longs** | *runs over time* | how two lengths co-occupy |
| `/2` **Structures** | *stands* | its magnitude and kind |
| `/3` **Facets** | shows *one face* | which face, of which object |
| `/4` **Areas** | *spans* | how much ground it covers, and its kind |

This folder is the data + models for one relevance set. It gathers **real,
sourced** areas across three domains, holds them as JSON / XML / TXT, rests them
on a **basis document** — the *Medium requirement for Workers, Work and
Excellence — Workers* — and carries a **constraint layer** encoding the
repository's Constitution of congrains.

> **Kinds of Area.**
> | Kind | Meaning | Example |
> |---|---|---|
> | **EXACT** | defined by an area formula — no uncertainty | circle area `πr²` |
> | **MEASURED** | a measured extent with an uncertainty | Earth's total surface `4πR²` |
> | **OBSERVED** | a surveyed / published extent | Russia's total area; Lake Superior surface |
>
> `EXACT ≠ MEASURED ≠ OBSERVED`; `ASSOCIATION ≠ CAUSATION`; `ASSUMPTION ≠ FACT`.

## The three models

| Model | Domain | Areas it holds |
|---|---|---|
| [`models/PHYSICS_MODEL.md`](models/PHYSICS_MODEL.md) | physics / geography | country, continent, Earth-surface, and lake extents |
| [`models/MATH_MODEL.md`](models/MATH_MODEL.md) | mathematics | planar & surface area formulas |
| [`models/SOCIAL_MODEL.md`](models/SOCIAL_MODEL.md) | society | US land area & population density |

## The data models

All data are **real and sourced** (each file embeds its provenance), stored as
JSON, XML, or TXT so they read cleanly on GitHub or a public server:

| File | Format | What it is | Source |
|---|---|---|---|
| [`data/country-areas.json`](data/country-areas.json) | JSON | largest countries by total area | UN Statistics Division / CIA |
| [`data/earth-surface.xml`](data/earth-surface.xml) | XML | Earth surface + land/water split | geophysics / Wikipedia |
| [`data/geometry-areas.json`](data/geometry-areas.json) | JSON | planar & surface area formulas | public-domain geometry |
| [`data/social-density-us.json`](data/social-density-us.json) | JSON | US land area & density | US Census / UN / CIA |
| [`data/continents.txt`](data/continents.txt) | TXT | the seven continents by area | standard geographic figures |
| [`data/great-lakes.json`](data/great-lakes.json) | JSON | Great Lakes by surface area | US EPA / Britannica |

## The model

[`areas.sleela`](areas.sleela) (syntax 1.2) loads Areas from all three domains as
`Area` records and prints:

1. a **mathematical** listing — each area's extent, order of magnitude, and kind;
2. **shares** — geographic extents as a share of Earth's land; and
3. an **evaluation** — what each area's kind (`EXACT` / `MEASURED` / `OBSERVED`)
   means for how far it can be trusted.

```sh
export SLEELA_SHA256_MANIFEST=security/sha256-manifest.json
export SLEELA_SHEET=SHEET.sheet
impl/build/sleela run churn/4/areas.sleela
```

## The constraint layer (constraints & theory)

Every churn set is built around the repository's **Constitution of congrains**
(see [`../../README.md`](../../README.md), "The Constitution"): the governing
constraints and limits the system honors. `/4` carries that theory in two forms:

- [`constraints.sleela`](constraints.sleela) — the congrains/limits expressed as
  a Sleela model, applied to Areas (bounds, thresholds, capacity, and the system
  invariants **DepthLimit = 3024** and **ComplexityDegree ≤ 4**).
- [`Constraints.java`](Constraints.java) — the same theory as a small, dependency-free
  Java class (`com.mearvk.sleela.churn`), so the constraints can be checked from
  the JVM side too.

The sibling sets `/1`–`/3` carry the same layer, so all four share one constraint
theory over their different magnitudes (Longs, Structures, Facets, Areas).

## The basis

[`BASIS.md`](BASIS.md) is the basis document this set rests on: the **Medium
requirement for Workers, Work and Excellence — Workers**, read through the
**ground** the working substrate spans — enough land, filled firmly enough to be
*assailable and firm*.

## Files

| File | What it is |
|---|---|
| [`BASIS.md`](BASIS.md) | the Medium requirement for Workers, Work and Excellence — Workers |
| [`areas.sleela`](areas.sleela) | the Areas model over all three domains |
| [`constraints.sleela`](constraints.sleela) / [`Constraints.java`](Constraints.java) | the Constitution congrains/limits applied to Areas |
| [`models/`](models/) | the physics, math, and social model docs |
| [`data/`](data/) | the sourced data models (JSON / XML / TXT) |

*The areas here are real and sourced. EXACT areas are formula definitions,
MEASURED areas carry uncertainty, and OBSERVED areas are surveyed/published
extents. Any relation drawn between areas is a model, not a causal claim.*

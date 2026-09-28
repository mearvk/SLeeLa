# /5 — Lines

**Lines** is the fifth *relevance set* (a sibling of `/1` [Longs](../1/README.md),
`/2` [Structures](../2/README.md), `/3` [Facets](../3/README.md), and `/4`
[Areas](../4/README.md)). A **Line** is the **simplest magnitude**: a
one-dimensional count or length that *reaches* from 0 out along a single axis —
an **integer**, a **length**, a **common count**.

The churn relevance sets read a magnitude five ways:

| Set | A magnitude that… | We ask… |
|---|---|---|
| `/1` **Longs** | *runs over time* | how two lengths co-occupy |
| `/2` **Structures** | *stands* | its magnitude and kind |
| `/3` **Facets** | shows *one face* | which face, of which object |
| `/4` **Areas** | *spans* (2D) | how much ground it covers |
| `/5` **Lines** | *reaches* (1D) | its count/length, and the **Spark** it teaches |

This folder gathers **real, sourced** Lines across three domains, holds them as
JSON / XML / TXT, rests them on a **basis document**, carries the shared
**constraint layer** (the Constitution of congrains), and adds `/5`'s own
guiding layer — **Sparks** (see [`SPARKS.md`](SPARKS.md)).

> **Kinds of Line.**
> | Kind | Meaning | Example |
> |---|---|---|
> | **EXACT** | a defined integer or a length formula — no uncertainty | a dozen (12); circle circumference `2πr` |
> | **MEASURED** | a measured physical length with an uncertainty | the Bohr radius |
> | **OBSERVED** | a counted/surveyed/published extent | 50 US states; the US–Canada border |
>
> `EXACT ≠ MEASURED ≠ OBSERVED`; a **Spark** guides but is not evidence
> (`VALUE ≠ SPARK`); `ASSOCIATION ≠ CAUSATION`; `ASSUMPTION ≠ FACT`.

## The three models

| Model | Domain | Lines it holds |
|---|---|---|
| [`models/MATH_MODEL.md`](models/MATH_MODEL.md) | mathematics | canonical integers / common counts + line & perimeter formulas |
| [`models/PHYSICS_MODEL.md`](models/PHYSICS_MODEL.md) | physics | defined & measured lengths in metres |
| [`models/SOCIAL_MODEL.md`](models/SOCIAL_MODEL.md) | society | common counts + US national lengths |

## The data models

All data are **real and sourced** (each file embeds its provenance), stored as
JSON, XML, or TXT so they read cleanly on GitHub or a public server:

| File | Format | What it is | Source |
|---|---|---|---|
| [`data/integers.json`](data/integers.json) | JSON | canonical integers & common counts (dozen, gross, perfect numbers…) | public-domain math / OEIS |
| [`data/geometry-lengths.json`](data/geometry-lengths.json) | JSON | line & perimeter formulas (segment, circumference, hypotenuse) | public-domain geometry |
| [`data/physical-lengths.xml`](data/physical-lengths.xml) | XML | defined & measured physical lengths (metre, AU, Earth radius) | BIPM / CODATA / NASA / IAU |
| [`data/common-counts.json`](data/common-counts.json) | JSON | common counts of a society (states, senators, population) | US Census / Senate / House |
| [`data/us-lengths.txt`](data/us-lengths.txt) | TXT | US national lengths (borders, rivers, trails, interstates) | CBP / USGS / NPS / Census |

## The model

[`lines.sleela`](lines.sleela) (syntax 1.2) loads Lines from all three domains as
`Line` records — each with a **Spark** — and prints:

1. a **mathematical** listing — each Line's value, order of magnitude, whether it
   is a whole count, and its kind;
2. the **Sparks** — the guiding maxim each Line carries; and
3. an **evaluation** — what each Line's kind (`EXACT` / `MEASURED` / `OBSERVED`)
   means for how far it can be trusted.

```sh
export SLEELA_SHA256_MANIFEST=security/sha256-manifest.json
export SLEELA_SHEET=SHEET.sheet
impl/build/sleela run churn/5/lines.sleela
```

## The constraint layer (constraints & theory)

Like every churn set, `/5` is built around the repository's **Constitution of
congrains** (see [`../../README.md`](../../README.md), "The Constitution"):

- [`constraints.sleela`](constraints.sleela) — the congrains/limits as a Sleela
  model applied to Lines (a count is a non-negative Line; Bound / Threshold /
  Capacity; the system invariants **DepthLimit = 3024** and
  **ComplexityDegree ≤ 4**).
- [`Constraints.java`](Constraints.java) — the same theory as a small,
  dependency-free Java class (`com.mearvk.sleela.churn`).

### Running it — the dated Markdown report

```sh
# from churn/5/
javac -d out Constraints.java
java -cp out com.mearvk.sleela.churn.Constraints
# => writes ./DD-MM-YYYY/churn-5-lines.md (the run date)
```

The `DD-MM-YYYY/` output folder is a build artifact and is git-ignored (see
[`../.gitignore`](../.gitignore)).

## The basis

[`BASIS.md`](BASIS.md) is the basis document this set rests on: the **Medium
requirement for Workers, Work and Excellence — Workers**, read through the
**reach** of the working substrate — the plain lines and counts by which a
society measures what it has and how far it extends.

## The Sparks

[`SPARKS.md`](SPARKS.md) defines `/5`'s own guiding layer: a **Spark** is a short
maxim attached to a Line — what the Line *teaches*. A Spark guides; it is not
evidence (`VALUE ≠ SPARK`).

## Files

| File | What it is |
|---|---|
| [`README.md`](README.md) | this file |
| [`BASIS.md`](BASIS.md) | the Medium requirement for Workers, Work and Excellence — Workers |
| [`SPARKS.md`](SPARKS.md) | the guiding-Sparks definition and the set's Sparks |
| [`lines.sleela`](lines.sleela) | the Lines model over all three domains (with Sparks) |
| [`constraints.sleela`](constraints.sleela) / [`Constraints.java`](Constraints.java) | the Constitution congrains/limits applied to Lines |
| [`models/`](models/) | the math, physics, and social model docs |
| [`data/`](data/) | the sourced data models (JSON / XML / TXT) |

*The Lines here are real and sourced. EXACT Lines are defined integers or
formulas, MEASURED Lines carry uncertainty, and OBSERVED Lines are
counted/surveyed extents. A Spark guides but does not measure; any relation
drawn between Lines is a model, not a causal claim.*

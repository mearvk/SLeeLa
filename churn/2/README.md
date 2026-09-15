# /2 — Structures

**Structures** is the second *relevance set* (a sibling of `/1` [Longs](../1/README.md)).
Where a **Long** is a magnitude that *runs over time* (a length series), a
**Structure** is a magnitude that *stands*: a named, standing quantity in some
domain, with a value, a unit, and a **kind** that says how far it can be trusted.

This folder is the data + models for one relevance set. It gathers **real,
sourced** structures across three domains, holds them as JSON / XML / TXT data
models, and rests them on a **basis document** — the *Medium requirement for
Workers, Work and Excellence — Workers*.

> **Long vs. Structure.**
> A **Long** extends period by period and we ask how two lengths *co-occupy*.
> A **Structure** stands, and we ask what its magnitude, its order of magnitude,
> and its **kind** are:
>
> | Kind | Meaning | Example |
> |---|---|---|
> | **EXACT** | a definition — no uncertainty, no date | π, the speed of light `c` |
> | **MEASURED** | an experimental value with a standard uncertainty | `G`, electron mass |
> | **OBSERVED** | a published figure for a stated period | US labor force participation, 2024 |
>
> `EXACT ≠ MEASURED ≠ OBSERVED`; `ASSOCIATION ≠ CAUSATION`; `ASSUMPTION ≠ FACT`.

## The three models

| Model | Domain | Structures it holds |
|---|---|---|
| [`models/PHYSICS_MODEL.md`](models/PHYSICS_MODEL.md) | physics | CODATA 2022 fundamental constants + SI base units |
| [`models/MATH_MODEL.md`](models/MATH_MODEL.md) | mathematics | mathematical constants + canonical integer sequences |
| [`models/SOCIAL_MODEL.md`](models/SOCIAL_MODEL.md) | society | US labor & social figures (BLS / US Census) |

## The data models

All data are **real and sourced** (each file embeds its provenance), stored as
JSON, XML, or TXT so they read cleanly on GitHub or a public server:

| File | Format | What it is | Source |
|---|---|---|---|
| [`data/physics-constants.json`](data/physics-constants.json) | JSON | fundamental physical constants | CODATA 2022 / NIST |
| [`data/si-base-units.xml`](data/si-base-units.xml) | XML | the 7 SI base units + defining constants | BIPM SI Brochure (2019) |
| [`data/math-constants.json`](data/math-constants.json) | JSON | math constants + integer sequences | public domain / OEIS |
| [`data/social-labor-us.json`](data/social-labor-us.json) | JSON | US labor & social figures | BLS / US Census |
| [`data/chemical-elements.txt`](data/chemical-elements.txt) | TXT | first 20 elements + atomic weights | IUPAC (2021) / NIST |
| [`data/solar-system.json`](data/solar-system.json) | JSON | the 8 planets (mass, radius, orbit) | NASA NSSDCA |

## The model

[`structures.sleela`](structures.sleela) (syntax 1.2) loads Structures from all
three domains as `Structure` records, and prints:

1. a **mathematical** listing — each structure's magnitude, order of magnitude,
   and kind, and
2. an **evaluation** — what each structure's kind (`EXACT` / `MEASURED` /
   `OBSERVED`) means for how far it can be trusted.

Run it from the repository root:

```sh
export SLEELA_SHA256_MANIFEST=security/sha256-manifest.json
export SLEELA_SHEET=SHEET.sheet
impl/build/sleela run churn/2/structures.sleela
```

## The basis

[`BASIS.md`](BASIS.md) is the basis document this set rests on: the **Medium
requirement for Workers, Work and Excellence — Workers**. It treats a working
society as an *assailable and firm* medium (the repository's subject-medium idea,
[`../../FILE.SYSTEM.md`](../../FILE.SYSTEM.md)) and states the condition the
medium must satisfy — enough **workers**, laying down real **work**, done with
**excellence** — with the social-model structures as its evidence.

## Files

| File | What it is |
|---|---|
| [`BASIS.md`](BASIS.md) | the Medium requirement for Workers, Work and Excellence — Workers |
| [`structures.sleela`](structures.sleela) | the Structures model over all three domains |
| [`constraints.sleela`](constraints.sleela) / [`Constraints.java`](Constraints.java) | the Constitution congrains/limits applied to Structures (shared across `/1`–`/4`) |
| [`models/`](models/) | the physics, math, and social model docs |
| [`data/`](data/) | the sourced data models (JSON / XML / TXT) |

*The structures here are real and sourced. EXACT structures are definitions,
MEASURED structures carry uncertainty, and OBSERVED structures are published
figures for a stated period. Any relation drawn between structures is a model,
not a causal claim.*

# /5 · BASIS — the Medium requirement for Workers, Work and Excellence — Workers

This is the **basis document** for `/5` [Lines](README.md). Where `/1` (Longs)
reasons about magnitudes that *run over time*, `/2` (Structures) about magnitudes
that *stand*, `/3` (Facets) about *faces*, and `/4` (Areas) about *extents*, `/5`
reasons about the **simplest reach** — a Line: an integer, a length, a common
count. This document states the **basis** on which the Lines set rests: the
**Medium requirement for Workers, Work and Excellence**, and — this document's
scope — its first pillar, **Workers**, read through the **reach** of the working
substrate: the plain lines and counts by which a society measures what it has.

> **Discipline.** This is a *basis* — a stated foundation to be argued with, not
> a decree. Empirical claims are tagged to their source and period; a **Spark**
> is a guide, not evidence. `ASSUMPTION ≠ FACT`, `ASSOCIATION ≠ CAUSATION`,
> `VALUE ≠ SPARK`.

## 1. What "the Medium" is here

In this repository a **medium** is a working substrate that is *assailable and
firm* — it can be worn or attacked, yet holds together and stays reconstructible
from its neighbors (see [`../../FILE.SYSTEM.md`](../../FILE.SYSTEM.md), "the
subject medium"). The Lines set reads that same working society **by its plainest
measures**: not its stock (`/2`), its flow (`/3`), or its ground (`/4`), but the
**bare counts and lengths** it can state without interpretation — how many, how
far. A medium is firm only if it can *count itself* and *measure its own reach*
honestly; a society that cannot say how many of a thing it has cannot keep it
firm.

A medium therefore carries a **requirement**: the condition it must satisfy to
remain firm. For the working substrate that requirement has three pillars.

## 2. The Medium requirement — three pillars

| Pillar | The medium must guarantee | Read through Lines (this set) |
|---|---|---|
| **Workers** | that the substrate can *count itself* — how many workers, in how many places | head counts, structural counts |
| **Work** | that its *reach is real* — the lengths it actually spans, laid down and used | network lengths, borders, routes |
| **Excellence** | that each count *earns its place* — a count encodes a principle, not just a number | counts that carry a rule (equal voice) |

Together: **Workers · Work · Excellence** — the medium is firm when it can be
*counted*, *measured*, and *principled*. This document covers the first pillar;
the other two are named here and reasoned about in the social model
([`models/SOCIAL_MODEL.md`](models/SOCIAL_MODEL.md)).

## 3. Workers — the requirement in scope

**Requirement (Workers).** *The medium is firm only while the working substrate
can count itself honestly — a stated number of workers across a stated number of
places — so that a loss can be seen (the count falls) and a neighbor can make it
good (the count is restored).*

This is the "assailable and firm" property read through **counting**:

- **Firm** — the substrate keeps an honest, checkable count of itself; a missing
  unit shows up as a fallen tally, the way the file medium reconstructs a lost
  cell from a neighbor.
- **Assailable** — the counts are *not* frozen; they can rise and fall as the
  substrate changes. A count that can never move is not measuring a living
  society.

### The Workers lines (evidence)

The requirement is checked against the real, sourced **Lines** in
[`data/common-counts.json`](data/common-counts.json):

| Line | Value | What it tells the requirement |
|---|---|---|
| US states | 50 | the number of places the substrate spans |
| US resident population | 331,449,281 (2020 Census) | how many fill them |
| US counties (and equivalents) | 3,143 | the granularity at which the count can be checked |

**Reading (a model, not a verdict).** A substrate that reports a stable count of
places (50) at a fine checking granularity (3,143 counties) and a large, dated
head count reads as *countable and firm* by this basis: it can see itself, so it
can see a loss and repair it. This is an interpretation of published figures, not
a causal or normative claim about any person or place.

## 4. How Workers connects to Work and Excellence

The Workers pillar is a **precondition**, not the whole requirement:

- **Workers → Work.** Only a substrate that can count itself can measure its
  reach; head counts and place counts are the base on which measured lengths
  (the Work pillar — borders, routes, networks in
  [`data/us-lengths.txt`](data/us-lengths.txt)) rest.
- **Work → Excellence.** Reach becomes Excellence when a count *earns its place*
  by encoding a principle, not merely a quantity — e.g. **two senators per
  state**, a count that carries *equal voice*. Excellence is a Line that
  *teaches* (its Spark) as well as *tallies*.

So the basis reads top to bottom: **countable Workers** make **measured reach**
possible, and **counts that carry a principle** are **Excellence**. Across the
five sets, the same requirement is tested five ways — over *time* (`/1`), by
*stock* (`/2`), by *flow* (`/3`), over *ground* (`/4`), and by *plain count and
length* (`/5`).

## 5. Files this basis rests on

| File | Role in the basis |
|---|---|
| [`README.md`](README.md) | what the Lines set is |
| [`SPARKS.md`](SPARKS.md) | the guiding Sparks (guide, not evidence) |
| [`models/SOCIAL_MODEL.md`](models/SOCIAL_MODEL.md) | the Workers/Work/Excellence counts and lengths, with sources |
| [`data/common-counts.json`](data/common-counts.json) / [`data/us-lengths.txt`](data/us-lengths.txt) | the sourced Workers evidence |
| [`lines.sleela`](lines.sleela) | the model that loads and grades Lines |
| [`constraints.sleela`](constraints.sleela) / [`Constraints.java`](Constraints.java) | the Constitution congrains/limits applied to Lines |

*This basis is a stated foundation, offered to be examined and revised. The
Workers requirement is a modelling condition on a working medium; the figures
that test it are published statistics cited to the U.S. Census Bureau and other
U.S. primary sources.*

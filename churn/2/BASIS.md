# /2 · BASIS — the Medium requirement for Workers, Work and Excellence — Workers

This is the **basis document** for `/2` [Structures](README.md). Where `/1`
(Longs) reasons about magnitudes that *run over time*, `/2` reasons about
magnitudes that *stand* — Structures. This document states the **basis** on
which the Structures set rests: the **Medium requirement for Workers, Work and
Excellence**, and — this document's scope — its first pillar, **Workers**.

> **Discipline.** This is a *basis* — a stated foundation to be argued with, not
> a decree. Empirical claims are tagged to their source and period; everything
> else is an explicit modelling choice. `ASSUMPTION ≠ FACT`,
> `ASSOCIATION ≠ CAUSATION`.

## 1. What "the Medium" is here

In this repository a **medium** is a working substrate that is *assailable and
firm* — it can be worn or attacked, yet holds together and stays reconstructible
from its neighbors (see [`../../FILE.SYSTEM.md`](../../FILE.SYSTEM.md), "the
subject medium"). The Structures set generalizes that idea from files to a
**working society**: the medium is the standing body of **workers** and the
**work** they lay down, and it must be firm — able to take load and recover —
while remaining assailable — open to change, replacement, and growth.

A medium is therefore not neutral storage. It carries a **requirement**: the
condition it must satisfy to remain firm. For the working substrate that
requirement has three pillars.

## 2. The Medium requirement — three pillars

| Pillar | The medium must guarantee | Measured by (Structures) |
|---|---|---|
| **Workers** | that enough of the society is *at work* — the substrate is populated and participating | participation rate, employment–population ratio, population |
| **Work** | that the work laid down is *real extent*, not churn — output that stands | productivity (output per hour), output growth |
| **Excellence** | that the work is done *well* — quality rises, not merely quantity | productivity growth, reward of work (median income) |

Together: **Workers · Work · Excellence** — the medium is firm when it is
*populated*, *productive*, and *improving*. This document covers the first
pillar; the other two are named here for completeness and reasoned about in the
social model ([`models/SOCIAL_MODEL.md`](models/SOCIAL_MODEL.md)).

## 3. Workers — the requirement in scope

**Requirement (Workers).** *The medium is firm only while a sufficient share of
the society is a worker — present in the labor force and employed — so that the
substrate is neither depopulated nor idle.*

This is the "assailable and firm" property applied to people-as-substrate:

- **Firm** — enough workers are at work that the society can bear load and
  recover from loss (a retirement, a layoff, a shock) from its neighbors, the
  way the file medium reconstructs a lost cell from a cheap neighbor.
- **Assailable** — the worker body is *not* frozen: workers enter and leave,
  roles are substituted, and that turnover is healthy churn so long as extent is
  preserved (lengthening, in the `/1` assumptions vocabulary), not merely
  swapped away.

### The Workers structures (evidence)

The requirement is checked against the real, sourced **Workers** structures in
[`data/social-labor-us.json`](data/social-labor-us.json):

| Structure | Value | Period | What it tells the requirement |
|---|---|---|---|
| Labor force participation rate | 62.5% | 2024 | how much of the eligible society *shows up* as workers |
| Employment–population ratio | 59.9% | 2024 | how much of the society is *actually at work* |
| Unemployment rate | 4.2% | 2024 Q4 | slack: willing workers not yet placed |
| Resident population estimate | 341,784,857 | 2025 | the size of the substrate the workers populate |

**Reading (a model, not a verdict).** With roughly three in five people at work
(59.9%) and five in eight participating (62.5%), the worker substrate is
*populated and firm* by this basis, with modest slack (4.2% unemployment)
that keeps it *assailable* — open to placing more workers without over-tightening.
This is an interpretation of published figures over a stated period, not a
causal or normative claim about any person.

## 4. How Workers connects to Work and Excellence

The Workers pillar is a **precondition**, not the whole requirement:

- **Workers → Work.** Only a populated substrate can lay down work; participation
  and employment are the base on which output (the Work pillar) rests.
- **Work → Excellence.** Work becomes Excellence when its *quality* rises —
  captured here by **productivity growth** (+2.7%, 2023→2024): more real extent
  laid down per working hour. Excellence is work that *lengthens* (adds durable
  extent) rather than *substitutes* (churns without added reach) — the same
  distinction the `/1` assumptions layer draws for code-about-code.

So the basis reads top to bottom: **enough Workers** make **real Work** possible,
and **Work done well** is **Excellence**. `/2`'s job is to hold the standing
Structures — social, mathematical, physical — that let each pillar be checked
against evidence rather than asserted.

## 5. Files this basis rests on

| File | Role in the basis |
|---|---|
| [`README.md`](README.md) | what the Structures set is |
| [`models/SOCIAL_MODEL.md`](models/SOCIAL_MODEL.md) | the Workers/Work/Excellence structures, with sources |
| [`data/social-labor-us.json`](data/social-labor-us.json) | the sourced Workers evidence |
| [`structures.sleela`](structures.sleela) | the model that loads and grades Structures |

*This basis is a stated foundation, offered to be examined and revised. The
Workers requirement is a modelling condition on a working medium; the figures
that test it are published statistics cited to BLS and the US Census Bureau.*

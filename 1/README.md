# /1 — Longs

**Longs** is the first *relevance set*: the basic **lengths** of US economic data —
each a magnitude that runs over time (a "long") — together with the
**relevances** they hold to one another as a matter of **material nature**: how
the lengths co-relate and occupy the same economic space.

This folder is the data + model for one relevance set. Further sets live in
sibling numbered folders (`/2`, `/3`, …).

> **What a "long" is here.** A *long* is a length series: a quantity with size
> that extends period by period (GDP, federal debt, the price level). We read
> the series as lengths and ask how two lengths *co-occupy* — do they extend
> together, materially, or not.

> **What a "relevance" is here.** Following this repository's Nordshrift meaning
> (`impl/nordshrift/object_compat.h`), a **relevance** is a classified relation.
> Nordshrift classifies an object's relevance to a target as **Direct / Model /
> None**; here we classify the material co-relation between two economic lengths
> the same way, using the Pearson correlation `r` from the native `inference`
> library ([`../INFERENCE.md`](../INFERENCE.md)):
>
> | Relevance | Meaning (material co-occupation) | Threshold |
> |---|---|---|
> | **Direct** | the two lengths move together directly; they occupy the same material extent | `|r| ≥ 0.85` |
> | **Model**  | they co-relate only through a modeled/looser association | `0.5 ≤ |r| < 0.85` |
> | **None**   | no material co-relation is evidenced over this window | `|r| < 0.5` |
>
> The **sign** of `r` records direction (co-extending vs. opposing); the
> **class** records strength. A relevance is a measured association over the
> supplied window — `ASSOCIATION ≠ CAUSATION`.

## The lengths (this set)

Annual series, 2019–2024, from public primary sources (see `provenance` in
[`longs.csv`](longs.csv)):

| Long | Symbol | Units | Source |
|---|---|---|---|
| Nominal GDP | `GDP` | $ trillion | BEA (nominal GDP, annual) |
| Total federal debt | `DEBT` | $ trillion | US Treasury / GAO (total federal debt, fiscal year) |
| Consumer price level | `CPI` | index, 1982–84=100 | BLS (CPI-U, annual average) |

Values are the published annual figures rounded to the precision shown; they are
**real, dated, and sourced**, not placeholders. Consult `longs.csv` for the
per-series provenance and retrieval note.

## The model

[`relevances.sleela`](relevances.sleela) (syntax 1.2) loads the three lengths as
`Long` structs, computes every pairwise relevance with `inference.correlation`,
classifies each **Direct / Model / None**, and prints:

1. a **mathematical** relevance matrix (each pair's `r` and class), and
2. a **paragraph** reading of how the lengths co-relate and occupy the same
   material extent.

Run it from the repository root:

```sh
export SLEELA_SHA256_MANIFEST=security/sha256-manifest.json
export SLEELA_SHEET=SHEET.sheet
impl/build/sleela run 1/relevances.sleela
```

## Files

| File | What it is |
|---|---|
| [`longs.csv`](longs.csv) | The length series, with a per-series provenance column. |
| [`relevances.sleela`](relevances.sleela) | The relevance model over the lengths. |
| [`assumptions/`](assumptions/) | The **assumptions** layer: a moral model over foundational human-nature / construction lengths (code-about-code + ethics + Longs). See [`assumptions/ASSUMPTIONS.md`](assumptions/ASSUMPTIONS.md). |

*The relevances reported are measured associations over 2019–2024. Direction and
strength are descriptive; material co-occupation is an interpretation of the
association, not a causal claim.*

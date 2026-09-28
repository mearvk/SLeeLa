# US Infrastructure — Economic Model

A small, transparent economic model of United States **infrastructure**: what
each sector represents, how investment in it flows into the economy, and how a
renewal backlog accumulates when reinvestment lags depreciation. The model is
written in **SLeeLa** (syntax 1.2) and leans on two existing pieces of this
repository:

- the **economics identities** documented in [`../ECONOMICS.md`](../ECONOMICS.md)
  (the GDP identity `Y = C + I + G`, time-value/NPV, and rates), and
- the **`struct` feature** documented in [`../STRUCTS.md`](../STRUCTS.md), used
  here to model each infrastructure sector as a typed record.

> **Method note (Part B discipline).** Following this repository's evidentiary
> rule (`METAPHOR ≠ FACT`, `ASSOCIATION ≠ CAUSATION`), every numeric input in
> this model is **illustrative / placeholder** unless it carries an explicit,
> dated primary source. A model output is a *computation over assumptions*, not
> an empirical measurement or a forecast. See [`SOURCES.md`](SOURCES.md).

---

## 1. What the model is

The model treats US infrastructure as a set of **sectors** (roads & bridges,
transit, water, energy grid, aviation, rail, broadband, …). For each sector it
carries a few economic quantities and derives a few results. It answers
questions of the form:

- How much does annual infrastructure investment contribute to output under a
  stated **spending multiplier**?
- Given an asset base, an annual **depreciation** rate, and actual annual
  **reinvestment**, does the **renewal backlog** grow or shrink?
- What is the **benefit–cost ratio** and **net present value** of a candidate
  investment at a stated discount rate and horizon?

These are the classic public-infrastructure economics relationships, kept
deliberately explicit so the assumptions are visible.

## 2. Data model (structs)

Each sector is a `Sector` record; a scenario bundles the shared macro
assumptions. (See [`model.sleela`](model.sleela) for the runnable definitions.)

```sleela
struct Sector {
    String name;          // e.g. "Roads & Bridges"
    double assetBase;     // estimated replacement value of in-service assets ($B)
    double annualInvest;  // actual annual investment into the sector ($B/yr)
    double depreciation;  // annual depreciation rate of the asset base (fraction)
    double conditionIdx;  // 0..1 condition proxy (1 = state of good repair)
    double bcRatio;       // benefit-cost ratio of marginal investment (unitless)
}

struct Scenario {
    String label;         // scenario name
    double multiplier;    // output multiplier on infrastructure spend (Y per $)
    double discountRate;  // real discount rate for NPV (fraction/yr)
    int    horizonYears;  // evaluation horizon
}
```

## 3. Relationships computed

All formulas are stated so a reader can check them; the SLeeLa program computes
exactly these.

| Result | Formula | Meaning |
|---|---|---|
| **Output contribution** | `contribution = annualInvest × multiplier` | Spend times the assumed multiplier (a modelled effect, not a measured one). |
| **Required renewal** | `renewalNeed = assetBase × depreciation` | Annual reinvestment needed just to hold the asset base steady. |
| **Backlog change** | `backlogDelta = renewalNeed − annualInvest` | Positive ⇒ the backlog grows this year; negative ⇒ it shrinks. |
| **Investment gap** | `gap = max(0, backlogDelta)` | The annual shortfall against a state-of-good-repair path. |
| **Marginal NPV** | `NPV = Σ_{t=1..H} (annualInvest × (bcRatio − 1)) / (1+r)^t` | Present value of net benefits of the marginal spend over the horizon. |
| **National rollup** | `Σ contribution`, `Σ gap`, `Σ NPV` | Program totals across all sectors. |

The GDP identity `Y = C + I + G` frames where infrastructure investment sits:
public infrastructure spend is part of `I` (and, for government capital, `G`).
The model reports the sector `contribution` as a component estimate, not a claim
about the whole economy.

## 4. Assumptions vs. facts

The model separates three layers explicitly, matching `ECONOMICS.md`'s
`observation → measurement → accounting identity → behavioral model →
estimate → interpretation` chain:

1. **Identities** (true by definition): `Y = C + I + G`; `backlogDelta =
   renewalNeed − annualInvest`. These hold regardless of data quality.
2. **Behavioral assumptions** (hypotheses): the spending `multiplier` and each
   sector's `bcRatio`. Reasonable ranges are discussed in `SOURCES.md`; the
   defaults in the program are round, conservative placeholders.
3. **Data** (must be sourced): `assetBase`, `annualInvest`, `depreciation`,
   `conditionIdx`. The committed values are **illustrative placeholders**; swap
   in sourced figures (with dates) before drawing any real conclusion.

## 5. Files

| File | What it is |
|---|---|
| [`model.sleela`](model.sleela) | The runnable SLeeLa model (structs + identities + rollup). |
| [`sectors.csv`](sectors.csv) | Illustrative per-sector inputs with a provenance column. |
| [`SOURCES.md`](SOURCES.md) | Where real figures come from, and the assumption ranges, kept apart from framing. |

## 6. Running it

The model is an ordinary SLeeLa program. From the repository root, with the
verified-build manifest configured (see [`../security/BUILD-VERIFICATION.md`](../security/BUILD-VERIFICATION.md)):

```sh
export SLEELA_SHA256_MANIFEST=security/sha256-manifest.json
export SLEELA_SHEET=SHEET.sheet
impl/build/sleela run us-infrastructure/model.sleela
```

*This model is a descriptive, transparent computation. It asserts no official
status and no forecast; its outputs are only as good as the assumptions and
sourced data supplied to it.*

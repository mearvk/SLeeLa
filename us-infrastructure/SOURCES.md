# US Infrastructure Model — Sources & Assumptions

This file keeps the model's **data** and **assumptions** apart from its framing,
following the repository's evidentiary discipline (see
[`../politico/SAKES.md`](../politico/SAKES.md) and the rule that a model output
is a *computation over assumptions*, not a measurement). Nothing here is a
forecast or an official figure.

## Status of the committed numbers

**Every value in [`sectors.csv`](sectors.csv) and the defaults in
[`model.sleela`](model.sleela) is an `ILLUSTRATIVE_PLACEHOLDER`.** They are round
numbers chosen to exercise the model and to be plausible in order of magnitude —
not sourced measurements. The `provenance` column in `sectors.csv` records this
explicitly; replace it with a citation (author, publication, date, table) when a
figure is backed by a primary source, and only then treat that row as data.

## The three layers (what can and cannot be trusted)

| Layer | Items | Trust |
|---|---|---|
| **Identity** (true by definition) | `Y = C + I + G`; `renewalNeed = assetBase × depreciation`; `backlogDelta = renewalNeed − annualInvest`; `gap = max(0, backlogDelta)` | Exact given the inputs. |
| **Behavioral assumption** (hypothesis) | output `multiplier`; per-sector `bcRatio`; `discountRate` | Defensible only within a cited range; results are sensitive to these. |
| **Data** (must be sourced) | `assetBase`, `annualInvest`, `depreciation`, `conditionIdx` | Placeholder until a dated primary source is attached. |

## Where real figures come from (to be filled in per row)

When sourcing the placeholders, draw each quantity from a named primary release
and record the retrieval date. Suitable primary sources for US infrastructure
economics include (consult and cite the specific table/vintage — do not treat
this list as data):

- **BEA** — National Income and Product Accounts (GDP components `C`, `I`, `G`)
  and Fixed Assets Accounts (asset stocks, depreciation).
- **Census Bureau** — Annual Value of Construction Put in Place (public/private
  investment by category).
- **CBO** — public spending on transportation and water infrastructure.
- **DOT / FHWA, EIA, FCC** — sector-specific asset condition and investment
  series for transport, energy, and broadband respectively.
- **ASCE Infrastructure Report Card** — condition proxies and investment-gap
  framing (note: an advocacy source; treat its gap figures as claims to test,
  per `METAPHOR ≠ FACT`).

## Assumption ranges (behavioral layer)

Documented so a reader can vary them deliberately rather than accept the
defaults:

- **Spending multiplier** — public-infrastructure fiscal multipliers are commonly
  discussed in a wide band (roughly 0.5–2.0+ depending on slack, financing, and
  horizon). The model's default `1.5` is a mid-range illustrative choice.
- **Benefit–cost ratio (`bcRatio`)** — appraisal BCRs for maintenance and
  targeted expansion frequently exceed 1; the per-sector values here (2.2–3.8)
  are illustrative and vary widely by project.
- **Discount rate** — the default `0.03` (3% real) is an illustrative long-horizon
  public-appraisal rate; sensitivity to this choice should be reported.

## Reproducing / extending

1. Replace `sectors.csv` values with sourced figures and update the `provenance`
   column per row.
2. Mirror the sourced inputs into the sector definitions in `model.sleela`
   (the program is self-contained; the CSV is the human-readable companion).
3. Re-run and report results **with the assumption values stated inline**, so the
   output is never mistaken for a measurement.

*Per project discipline: the existence of a computed number is not proof of a
claim. Cite the source, state the assumptions, and keep interpretation separate
from the identity-level arithmetic.*

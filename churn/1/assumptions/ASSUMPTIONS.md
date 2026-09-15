# /1 · Assumptions — a moral model over Longs

These are the **assumptions** for `/1` [Longs](../README.md). Where `longs.csv`
holds *measured, sourced* economic lengths, this set holds *foundational human
assumptions* — the everyday "lengths" of human nature and construction — and
uses them to build a small **moral model**: a model whose job is to reason about
**code about code**, **ethics**, and **Longs** together.

> **Discipline.** Everything here is explicitly **ASSUMED**, not measured. The
> point is not to assert facts about people; it is to make the modelling
> assumptions *visible and checkable* so the moral reasoning built on them is
> transparent (`ASSUMPTION ≠ FACT`, `ASSOCIATION ≠ CAUSATION`). Change a row and
> the model's conclusions change with it — by design.

## 1. What a length is here

A **length** (a "long") is a count that *extends* — a quantity with size that a
process lays down over time. In `longs.csv` a length is an economic magnitude
(GDP, debt, price level). Here a length is a **human/construction count-as-length**:

| Assumption (example) | The length it names |
|---|---|
| Human lengths per high‑school education | how many life‑norm spans go into finishing HS |
| Life norms per college education | how many lifetime‑radius study passes go into a degree |
| 2×4s per family‑man‑year at Home Depot | is he **lengthening** (building/growing) or **substituting**? |
| Dates per pretty girl in a small town | a continuation / procreation signal |

The full seed set (with many more like these) is in
[`assumptions.csv`](assumptions.csv).

## 2. The three quantities each assumption carries

1. **Length** — how many units go into one whole (the magnitude of the span).
2. **Speed** — the rate the process runs, in units per year. Speed is "the speed
   of the process over such concerns": speed of data, of inference, of test. A
   long span traversed quickly is a different moral object than the same span
   traversed slowly.
3. **Lengthening vs. substituting** — a flag: does the behavior **add real
   extent** to base reality (build, grow, continue) or merely **swap one thing
   for another** (substitute)? Lengthening is treated as growth; substitution is
   treated as churn.

## 3. What the model computes

For each assumption the model (`assumptions.sleela`) derives:

| Quantity | Formula (stated so it can be checked) | Meaning |
|---|---|---|
| **duration** | `length / speed` (years) | how long the process takes to lay the length down |
| **habituality** | `speed / length`, clamped to `[0,1]` | how habitual/repeated the act is relative to its span |
| **growth sign** | `+1` if lengthening else `−1` | does it inhabit base reality or churn it |
| **moral score** | `moral_weight · growth · (0.5 + 0.5·habituality)` | signed contribution to the model's moral aggregate |
| **relevance** | Direct / Model / None on `|moral score|` | how strongly it bears on base reality |

The **relevance** classes reuse the `/1` (Nordshrift) vocabulary — **Direct**
(`≥ 0.85`), **Model** (`≥ 0.50`), **None** (else) — so an assumption's bearing on
base reality is graded the same way a Long's material co‑occupation is.

## 4. Why this is a moral model of *code about code*

The moral aggregate ranks assumptions by whether they **lengthen** (build,
continue, habituate a good) or **substitute** (churn without extent). Applied to
software, the same shape scores *code about code*:

- code that **adds durable extent** (tests, inference, documented growth) scores
  like lengthening;
- code that merely **substitutes** (rewrites without added reach, churn) scores
  like substitution.

So the model **optimizes plan, growth, and habituality**: it prefers processes
that lay down real length at a sustainable speed and habituate a good, over ones
that only swap parts. That preference — made explicit and adjustable — is the
ethics layer sitting over Longs.

## 5. Files

| File | What it is |
|---|---|
| [`assumptions.csv`](assumptions.csv) | The seed assumptions (all ASSUMED), with length, speed, category, lengthening flag, moral weight, and a note. |
| [`assumptions.sleela`](assumptions.sleela) | The moral model over the assumptions (math summary + paragraph evaluation). |
| [`OUTPUT.md`](OUTPUT.md) | The synthesized **opinion** the model informs — an overall reading of the health of the human condition (as a product). |

Run it from the repository root:

```sh
export SLEELA_SHA256_MANIFEST=security/sha256-manifest.json
export SLEELA_SHEET=SHEET.sheet
impl/build/sleela run 1/assumptions/assumptions.sleela
```

*These are foundational modelling assumptions, stated to be argued with. The
moral scores are a transparent function of the assumed inputs, not a judgment of
any person.*

# /1 · Assumptions — OUTPUT: an opinion on the health of the human condition

This document is the **output** of the `/1` assumptions moral model: from the
datums in [`assumptions.csv`](assumptions.csv), scored by
[`assumptions.sleela`](assumptions.sleela), it forms an **overall opinion about
the health of a product — the human condition** (read here as a product we build
and maintain, alongside code, ethics, and Longs).

> **This is an opinion derived from assumptions, not a measurement of people.**
> Every input is explicitly ASSUMED (see [`ASSUMPTIONS.md`](ASSUMPTIONS.md)).
> The opinion is a transparent function of those assumptions and their weights;
> change a datum and the opinion moves with it. `ASSUMPTION ≠ FACT`.

---

## 1. The health verdict (headline)

**The human condition reads as broadly healthy but not excellent — "growing, not
yet thriving."**

| Health signal | Value |
|---|---|
| Datums considered | **12** |
| Datums that *lengthen* base reality (build/grow/continue) | **10 of 12 (83%)** |
| Datums that *substitute* (churn without added extent) | **2 of 12** |
| Aggregate moral score | **+5.769** |
| Average moral score per datum | **+0.481** |
| Average habituality | **0.645** |
| **Health index (0–100)** | **≈ 74** |
| Model posture | **mildly growth-oriented** |

The **health index** maps the average moral score from its range `[−1, +1]` onto
`0–100`: `index = (avg + 1) / 2 × 100 = (0.481 + 1)/2 × 100 ≈ 74`. Seventy-four
is a solid, upper-middle reading: the condition is being *built* more than it is
being *churned*, but nothing yet reaches the **Direct** tier (a co-occupation of
base reality so strong it is unmistakable).

## 2. Why (the reasoning over the datums)

The condition is healthy because the **strongest, best-habituated datums are all
lengthening** — they add durable extent at a sustainable speed:

- **Wholeplayers wondering about procreation — 0.825 (Model).** The single
  strongest human signal: well, whole players deliberately continuing the line.
  Continuation is the deepest lengthening of the human product.
- **Paying taxes on time — 0.800 (Model).** On-time civic contribution keeps the
  shared order intact; a healthy population honors the common frame.
- **Tests written per feature — 0.800 (Model).** The code-about-code mirror of
  the same virtue: work that adds durable reach rather than swapping parts.
- **On-time taxpayers who are degreed — 0.700 (Model).** Education and civic
  reliability *co-occupy*; the responsible and the educated overlap.
- **A family man's 2×4s — 0.700 (Model).** He is lengthening (building) rather
  than substituting — the household is under construction, not merely maintained.
- **Children per family — 0.524 (Model).** At replacement, base reality is
  continued, though slowly (its low speed is the one caution in the growth story).
- **Education (HS 0.5625, College 0.5625), sports-car hospitality (0.500).**
  Steady lengthening: schooling extends the person; opening the home to better
  company extends the circle.

The condition is **not excellent** because two datums pull the other way, and
several sit only at the modest edge:

- **Rewrites without added reach — −0.400 (None).** Pure churn: motion that adds
  no extent. The code-about-code warning sign for any living system.
- **Job changes across a life — −0.205 (None).** Substitution more than growth;
  restlessness that does not lay down length.
- **Dates per pretty girl — 0.400 (None).** Lengthening in intent but too diffuse
  to count as material co-occupation on its own — promise, not yet product.

## 3. Reading it as product health

Treating the human condition as a product we ship and maintain:

- **Growth vs. churn ratio.** Ten lengthening datums (sum **+6.37**) against two
  substituting ones (sum **−0.60**) — a strong build-to-churn balance. A healthy
  product ships more durable extent than it rewrites.
- **Habituation.** Average habituality **0.645**: the good behaviors are
  *repeated*, not one-off. Health that is habitual is health that compounds.
- **The ceiling.** No datum reaches **Direct** (≥ 0.85). The product is
  dependable but has headroom: to move from *healthy* to *excellent*, the
  lengthening virtues (continuation, civic reliability, durable work) would need
  to deepen until at least one co-occupies base reality unmistakably.
- **The failure mode to watch.** Both negatives are **substitution**. The clearest
  path to declining health is not weakness but **churn** — activity that swaps
  parts without extending the whole (rewrites, restless job-hopping). Optimizing
  the product means converting substitution into lengthening.

## 4. The opinion, in one paragraph

> On the assumptions supplied, the human condition is a **healthy product in the
> upper-middle band (~74/100), growing but not yet thriving.** Its strength is
> that its deepest and most habitual behaviors — continuing the line, honoring
> the civic frame, doing durable work — all *lengthen* base reality rather than
> merely churn it, and they are repeated rather than one-off. Its limit is that
> none of these virtues is yet strong enough to be unmistakable (no **Direct**
> co-occupation), and two behaviors — rewrites without reach and restless
> substitution — quietly subtract. The plan that follows is clear and it is the
> same plan the model prefers for code and for ethics: **lengthen, at a
> sustainable speed, and habituate the good** — convert churn into extent, and
> deepen the continuation-, civic-, and craft-virtues until the condition reads
> not merely healthy but excellent.

## 5. Provenance of this output

- **Inputs:** [`assumptions.csv`](assumptions.csv) — all ASSUMED.
- **Model:** [`assumptions.sleela`](assumptions.sleela) — `moralScore =
  weight · growth · (0.5 + 0.5·habituality)`; Direct/Model/None on `|score|`.
- **Aggregate reproduced by the model:** aggregate **5.76895**, average
  **0.480746**, average habituality **0.644881**, posture *mildly
  growth-oriented*. The 0–100 health index is this document's monotonic
  restatement of the average.

*An opinion about a product's health, computed from stated assumptions. It judges
processes, not persons, and it is meant to be argued with by editing the datums.*

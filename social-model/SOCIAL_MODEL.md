# Social Model — Systems of Social Design, Remedy, Method

A **socialing model**: a small, transparent model (written in SLeeLa) for
reasoning about **systems of social design** — how a social arrangement is
*proposed* to work, how it is *corrected* when it drifts, and how it is
*practiced and measured*. Its job is to draw a **boundary** around what is
proposed as *normal*, then check whether what we actually find still lives inside
that boundary — or whether the **base assumptions have gone stale or fallen out
of use**.

It builds on the native [`inference`](../INFERENCE.md) library for the
statistics and on SLeeLa `struct`s ([`../STRUCTS.md`](../STRUCTS.md)) for the
data model.

> **Discipline.** The "proposed normal" values are **assumptions**, and the
> "observed" values here are **illustrative**. The model does not assert social
> facts; it makes the *gap between assumption and observation* explicit and
> measurable, so a stale assumption can be seen and argued with
> (`ASSUMPTION ≠ FACT`, `ASSOCIATION ≠ CAUSATION`).

---

## 1. The three systems

| System | What it is | Question it answers |
|---|---|---|
| **Design** | how the arrangement is *proposed* to work — its normal center and tolerance | *What did we assume normal looks like?* |
| **Remedy** | how the arrangement is *corrected* when observation drifts from design | *How far off are we, and does it need fixing?* |
| **Method** | how the arrangement is *practiced and measured* — the observed readings | *What do we actually find?* |

Design proposes a normal; Method reports the observed; Remedy is the response the
deviation calls for. The model computes the third from the first two.

## 2. Boundaries — 2D and 3D

Each social measure carries a **proposed-normal center** and a **tolerance**
(a radius / half-width per axis). The boundary is the region a measure is allowed
to occupy while still counting as "normal." The model tests membership two ways,
in two dimensionalities:

- **2D box** — observed `(x,y)` within `±tol` of center on each axis.
- **2D circle** — observed `(x,y)` within Euclidean radius `tol` of center.
- **3D box** — observed `(x,y,z)` within `±tol` on each axis.
- **3D sphere** — observed `(x,y,z)` within Euclidean radius `tol` of center.

The 2D form suits a measure with two facets (e.g. *access* × *quality*); the 3D
form adds a third (e.g. *access* × *quality* × *cost*). A point **inside** the
boundary means the base assumption still holds; **outside** means it has drifted.

## 3. Staleness — the deviation math

Boundaries give a yes/no; the **deviation** gives a magnitude. Using the
`inference` library, the model computes the **standard-score gap between what is
proposed as normal and what we find**:

```
z = (observed − proposedNormal) / tolerance        (inference.zscore)
```

Here the tolerance plays the role of a standard deviation: it is the assumed
spread of "still normal." Across a measure's axes the model takes the overall
distance (a Euclidean combine of per-axis z's) and classifies it — reusing the
familiar three-tier grading:

| Class | Condition | Reading |
|---|---|---|
| **In-band** | distance ≤ 1 tolerance | the base assumption still holds |
| **Drifting** | 1 < distance ≤ 2 | the assumption is aging; Remedy is advisable |
| **Stale** | distance > 2 | the assumption is stale / out of use; Remedy is due |

A large deviation is the signal that a *proposed normal* no longer describes the
world — the base assumption has become stale.

## 4. "The State has so many workers. What do they do?"

A second sub-model asks the worker question directly. A State employs a total
headcount distributed across **functions** (administration, education, health,
safety, infrastructure, social services, …). **Design** proposes a *normal
allocation share* per function; **Method** reports the *observed share*. The
model computes, per function:

```
proposedHeadcount = totalWorkers × proposedShare
observedHeadcount = totalWorkers × observedShare
deviation         = observedShare − proposedShare      (z against a share tolerance)
```

and reports who is **over-** or **under-staffed** relative to the assumed normal —
i.e. *what the workers actually do* versus what the design assumes they should.
The sum of |share deviations| is a single "allocation drift" number for the State.

## 5. Files

| File | What it is |
|---|---|
| [`measures.csv`](measures.csv) | Social measures: proposed-normal center + tolerance + observed (2D/3D), all ASSUMED/illustrative. |
| [`workers.csv`](workers.csv) | State functions: proposed vs. observed workforce share, illustrative. |
| [`social_model.sleela`](social_model.sleela) | The model: boundaries, deviation/staleness, and the worker-allocation report. |

Run it from the repository root:

```sh
export SLEELA_SHA256_MANIFEST=security/sha256-manifest.json
export SLEELA_SHEET=SHEET.sheet
impl/build/sleela run social-model/social_model.sleela
```

*A socialing model of design, remedy, and method. Boundaries and deviations are
computed from stated assumptions and illustrative observations; they locate stale
assumptions, they do not certify social facts.*

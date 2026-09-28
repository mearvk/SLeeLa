# SPARKS.md — the guiding Sparks of /5 Lines

A **Spark** is `/5`'s own addition to the churn relevance sets: a short **guiding
maxim** attached to a Line. Where `/3` names the *face* of an object, `/5` names
what a Line *teaches* — the small, transferable insight a count or a length hands
to the next mind.

> **What a Spark is.** A Spark is a one-sentence guiding note carried in the
> `spark` field of each `Line` (see [`lines.sleela`](lines.sleela)). It is not a
> measurement and not a claim about the world; it is a *guide* — the reason this
> particular Line is worth keeping in the set. Sparks are how a bare number
> becomes instructive.

## Why Lines carry Sparks

A Line is the simplest magnitude — an integer, a length, a common count. On its
own, `12` or `40075017` teaches nothing. A Spark supplies the *guiding* layer:
it says what discipline or habit of thought the Line illustrates, so the data
set reads as instruction, not inventory.

This keeps `/5` honest about the difference between the two things it holds:

- the **value** — EXACT / MEASURED / OBSERVED, sourced, checkable;
- the **Spark** — a guide, explicitly *not* evidence.

`VALUE ≠ SPARK`, just as elsewhere `ASSOCIATION ≠ CAUSATION` and
`ASSUMPTION ≠ FACT`.

## The Sparks in this set

| Line | Domain | Guiding Spark |
|---|---|---|
| dozen | math | a count is a line laid unit by unit; name it and it becomes a tool. |
| gross | math | structure compounds: a dozen dozen is easier to hold than 144 loose. |
| first perfect number (6) | math | a whole equal to the sum of its own parts is rare and worth noticing. |
| byte range count (256) | math | powers of two are where counting meets the machine. |
| 3-4-5 hypotenuse | math | the shortest line between two truths often runs through a right angle. |
| circle circumference | math | a curved line still has an exact length; pi makes it knowable. |
| metre | physics | a unit is a line everyone agrees to share; agreement makes measurement possible. |
| Earth equatorial circumference | physics | even the whole world is a line you can walk end to end, given time. |
| US states (50) | social | a union is a count held together by a rule, not by proximity. |
| US senators (100) | social | a count can encode a principle (equal voice) as well as a quantity. |
| US-Canada border | social | the longest shared line in the world is a border no one guards in anger. |

## Reading the Sparks at runtime

`lines.sleela` prints a `[sparks]` section that emits each Line's Spark. Run it
from the repository root:

```sh
export SLEELA_SHA256_MANIFEST=security/sha256-manifest.json
export SLEELA_SHEET=SHEET.sheet
impl/build/sleela run churn/5/lines.sleela
```

A Spark guides; it does not measure. Keep the two apart.

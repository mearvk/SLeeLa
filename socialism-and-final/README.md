# socialism-and-final

A small set of **elegant SLeeLa source documents** that let a creator render
excellent **design**, **math**, and even **moral concepts** — each as a
first-class, reasoned-with value rather than a guess. The classes follow the
`lib/` conventions: the executive-grade **Metadata Class metapattern**
(`SLEELA.syntax` §15, [`.kiro/steering/metadata-class-standard.md`](../.kiro/steering/metadata-class-standard.md)),
angle-bracket comments, and the one hard rule — **values faithful to real
origin; unknowns stay `"none"`/`"unknown"`, never invented**.

They also carry the two integrity modifiers
([`.kiro/steering/integrity-modifiers.md`](../.kiro/steering/integrity-modifiers.md)):
**1751 — unified, not single** (one set built of honest parts) and
**1491 — a bond is kept by its symmetry** (reach balanced with regard).

## The documents

| File | Renders | What it gives a creator |
|------|---------|-------------------------|
| [`SFDesignPrinciple.sleela`](./SFDesignPrinciple.sleela) | **design** | a named principle with a statement, a default renderer weight (1..10), and its two failure modes, so a layout is reasoned, not guessed |
| [`SFGoldenMeasure.sleela`](./SFGoldenMeasure.sleela) | **math** | integer-exact proportional math — Fibonacci, the golden section (`goldenMajor`/`goldenMinor`), phi (×1000), a golden check, and a modular scale — no floating-point drift |
| [`SFMoralConcept.sleela`](./SFMoralConcept.sleela) | **moral concepts** | an ethical concept modeled as an Aristotelian mean: a faithful statement flanked by its excess and deficiency, with the tradition credited |
| [`SFCreator.sleela`](./SFCreator.sleela) | **all three** | the runnable sibling demo that composes the set and reads it aloud |

Each class is deliberately a **reasoning tool, not a verdict**: design and moral
classes name *both* failure modes at the extremes so the creator keeps the
symmetry (1491) instead of tilting the work; the math class supplies an exact,
widely-used measure without asserting it is the only beautiful one.

## Design intent

- **Elegant.** Small, well-named methods; clear single responsibilities; no
  cleverness that hides meaning.
- **Honest.** The moral class asserts no ideology; it models concepts as the
  mean and credits the tradition (Aristotle, and the repo Integrity Goal). The
  math is ownerless classical mathematics, credited as such.
- **Composable.** `SFCreator` shows the three working as one unified set.

## Validate

These are `#sleela 2.1` library-style classes; validate through the sibling demo
`SFCreator`. Because the folder lives at the repo root (not under `lib/`), point
`SLEELA_LIB` at it so the sibling records resolve:

```sh
# build the source compiler first if needed (prebuilt binary tops out at 1.11)
cd impl && SLEELA_SHA256_MANIFEST="$PWD/../security/sha256-manifest.json" make build/sleela
cd ..

SLEELA_LIB="$PWD/socialism-and-final" impl/build/sleela check socialism-and-final/SFCreator.sleela
SLEELA_LIB="$PWD/socialism-and-final" impl/build/sleela run   socialism-and-final/SFCreator.sleela
```

Verified output (abridged): a Contrast design principle at weight 80%, the
golden split of 800 → major 494 / minor 306, `fib(10) = 55`, and Fairness stated
as the measure between its two failures.

> The SHA-256 verified-build manifest covers the toolchain and `lib/`; user
> programs at the repo root (like this folder) are outside that gate, so no
> manifest regeneration is required.

## A note on the name

The folder name is the one given for this working set. The contents are
general-purpose creator tools — design, math, and an ethics-as-mean model — and
assert no political doctrine; the moral class is explicitly a neutral reasoning
frame that credits its sources.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

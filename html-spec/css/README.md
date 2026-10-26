# CSS Specifications Archive (1996 → present)

A chronological archive of **Cascading Style Sheets** — the web's styling language — from CSS
Level 1 (1996) through the modular CSS of today. It mirrors the structure of the HTML archive one
level up: version subfolders, each with the retrieved **specification source** and a
**feature catalogue** (`FEATURES.md`) listing that level's selectors, properties, at-rules,
functions, values and units. File **signatures** (size + SHA-256) and provenance are in
[`SIGNATURES.md`](./SIGNATURES.md).

## Timeline &amp; layout

| Level | Year | Folder | Feature catalogue | Source archived |
|-------|------|--------|-------------------|-----------------|
| **CSS1** | 1996 | [`css1-1996/`](./css1-1996) | [`FEATURES.md`](./css1-1996/FEATURES.md) | ✅ `REC-CSS1-19961217.html` (verbatim 1996 REC) |
| **CSS 2 / 2.1** | 1998 / 2011 | [`css2.1-2011/`](./css2.1-2011) | [`FEATURES.md`](./css2.1-2011/FEATURES.md) | ✅ `CSS21-Overview.bs` (W3C Bikeshed source) |
| **CSS3+ modules** | 1999 → now | [`css3-modules/`](./css3-modules) | [`FEATURES.md`](./css3-modules/FEATURES.md) | ✅ 12 module sources (Selectors, Color, Backgrounds, Fonts, Text, Values, Media Queries, Cascade, Flexbox, Grid, Transitions, Animations) |
| **CSS Snapshot** | 2023 | [`snapshot/`](./snapshot) | [`FEATURES.md`](./snapshot/FEATURES.md) | ✅ `CSS-Snapshot-2023.bs` |

## How CSS versioning works (important)

- **CSS1 (1996)** and **CSS2 / CSS 2.1 (1998 / 2011)** were each a *single document*.
- From **CSS3** onward there is **no single "CSS3" spec**. CSS is developed as **independent
  modules**, each with its own *Level* (Selectors Level 3/4, Color Level 3/4/5, Grid Level 1/2, …).
- The W3C publishes a dated **CSS Snapshot** listing the modules that collectively *are* "CSS"
  at that time — the nearest modern equivalent of a version number.

```
CSS1 ──▶ CSS2 / 2.1 ──▶ CSS3+ (many modules) ──▶ CSS Snapshot (dated roll-up)
```

## What each FEATURES.md contains

The "objects, items, selectors, nouns, verbs, treatments, functions" of each level, organized:

- **Selectors** — type, class, id, combinators, attribute, pseudo-classes, pseudo-elements.
- **At-rules** — `@import`, `@media`, `@font-face`, `@keyframes`, `@page`, `@supports`, …
- **Properties** — grouped by role (box model, text, color/background, layout, animation, …).
- **Functions** — `url()`, `rgb()`/`hsl()`/`oklch()`, `calc()`, `var()`, `repeat()`,
  `minmax()`, `cubic-bezier()`, `steps()`, gradients, …
- **Value types & units** — lengths (`px`/`em`/`rem`/`vw`/`fr`/…), colors, angles, time,
  percentages, keywords (`inherit`/`initial`/`unset`/`revert`).

## Provenance note

Sources were retrieved via the GitHub API from `w3c/csswg-drafts` (the authoritative CSS Working
Group repository) and, for CSS1, the `slebok/zoo` language archive (a byte-for-byte mirror of the
1996 W3C REC). The sandbox is `INTEGRATIONS_ONLY`, so `w3.org/TR` is not reachable directly. See
[`SIGNATURES.md`](./SIGNATURES.md) for exact repos, commits, sizes and SHA-256 digests. Nothing
was fabricated: feature lists are descriptive write-ups grounded in the archived sources; only the
files under the version folders are verbatim upstream material.

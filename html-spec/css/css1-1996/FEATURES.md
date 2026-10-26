# CSS Level 1 (1996) — complete feature catalogue

| | |
|---|---|
| **Formal title** | Cascading Style Sheets, level 1 |
| **Publisher** | W3C |
| **Published** | 17 December 1996 (revised 11 January 1999) |
| **Authors** | Håkon Wium Lie, Bert Bos |
| **Status** | Superseded (by CSS 2.1 / CSS Snapshots) |
| **Canonical reference** | <https://www.w3.org/TR/REC-CSS1> |
| **Source in this archive** | [`REC-CSS1-19961217.html`](./REC-CSS1-19961217.html) — the verbatim 17 Dec 1996 Recommendation. See [`../SIGNATURES.md`](../SIGNATURES.md). |

CSS1 is the origin of the language: a single flat specification. Everything below is grounded in
the archived REC.

## Selectors

| Selector | Meaning |
|----------|---------|
| `E` | type (element) selector |
| `E F` | descendant (contextual) selector |
| `.class` | class selector |
| `#id` | id selector |
| `E:link`, `E:visited`, `E:active` | anchor pseudo-classes |
| `E:first-line` | pseudo-element: first formatted line |
| `E:first-letter` | pseudo-element: first letter |
| grouping with `,` | one rule for several selectors |

> CSS1 had **no** child (`>`), sibling (`+`), attribute (`[attr]`), or universal (`*`) selectors
> — those arrive in CSS2.

## At-rules

| At-rule | Purpose |
|---------|---------|
| `@import` | pull in another style sheet |

## Properties (all 53)

### Font
`font-family`, `font-style`, `font-variant`, `font-weight`, `font-size`, `font` (shorthand)

### Color & background
`color`, `background-color`, `background-image`, `background-repeat`,
`background-attachment`, `background-position`, `background` (shorthand)

### Text
`word-spacing`, `letter-spacing`, `text-decoration`, `vertical-align`, `text-transform`,
`text-align`, `text-indent`, `line-height`

### Box model — margin / padding / border
`margin-top`, `margin-right`, `margin-bottom`, `margin-left`, `margin` (shorthand);
`padding-top`, `padding-right`, `padding-bottom`, `padding-left`, `padding` (shorthand);
`border-top-width`, `border-right-width`, `border-bottom-width`, `border-left-width`,
`border-width`; `border-color`, `border-style`;
`border-top`, `border-right`, `border-bottom`, `border-left`, `border` (shorthands)

### Dimensions
`width`, `height`, `float`, `clear`

### Classification / display
`display` (`block | inline | list-item | none`), `white-space`,
`list-style-type`, `list-style-image`, `list-style-position`, `list-style` (shorthand)

## Value types & units

| Category | Values |
|----------|--------|
| Keywords | `auto`, `none`, `normal`, `inherit`-like cascade (no explicit `inherit` keyword yet) |
| Lengths — relative | `em`, `ex`, `px` |
| Lengths — absolute | `in`, `cm`, `mm`, `pt`, `pc` |
| Percentages | `%` |
| Colors | keyword names, `#rgb`, `#rrggbb`, `rgb(r,g,b)`, `rgb(r%,g%,b%)` |
| URLs | `url(...)` |
| Strings | `"..."` / `'...'` |

## Core concepts introduced

- **The cascade** — the rules for combining author, user and UA style sheets, with specificity
  and `!important`.
- **Inheritance** — properties flow from parent to child unless overridden.
- **The box model** (content / padding / border / margin) — CSS1's foundational layout concept.
- **Forward-compatible parsing** — UAs must ignore properties/values they don't understand.

## What CSS1 did *not* have (added later)

No positioning, no `float`-based page layout beyond simple floats, no media types, no
`@media`/`@page`, no child/sibling/attribute selectors, no `z-index`, no generated content, no
tables model — all added in **CSS2** (see [`../css2.1-2011/FEATURES.md`](../css2.1-2011/FEATURES.md)).

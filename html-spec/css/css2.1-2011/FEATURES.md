# CSS Level 2 Revision 1 (CSS 2.1, 2011) — complete feature catalogue

| | |
|---|---|
| **Formal title** | Cascading Style Sheets Level 2 Revision 1 (CSS 2.1) Specification |
| **Publisher** | W3C |
| **Published** | CSS2: 12 May 1998 · CSS 2.1: 7 June 2011 (W3C Recommendation) |
| **Status** | Superseded as a monolith by the CSS3 modules + CSS Snapshots; still the stable "core". |
| **Canonical reference** | <https://www.w3.org/TR/CSS2/> |
| **Source in this archive** | [`CSS21-Overview.bs`](./CSS21-Overview.bs) — the W3C Bikeshed source (≈79 property definitions). See [`../SIGNATURES.md`](../SIGNATURES.md). |

CSS 2.1 is the mature "classic CSS" baseline: CSS1 plus positioning, media types, the table and
paged-media models, and a far richer selector set. Everything below is present in the archived
source.

## Selectors (new in CSS2 over CSS1)

| Selector | Meaning |
|----------|---------|
| `*` | universal selector |
| `E > F` | child combinator |
| `E + F` | adjacent-sibling combinator |
| `E[attr]`, `E[attr=val]`, `E[attr~=val]`, `E[attr\|=val]` | attribute selectors |
| `E:first-child` | structural pseudo-class |
| `E:hover`, `E:focus` | dynamic pseudo-classes (added to `:link`/`:visited`/`:active`) |
| `E:lang(c)` | language pseudo-class |
| `:before`, `:after` | generated-content pseudo-elements |
| `E:first-line`, `E:first-letter` | (from CSS1) |

## At-rules

| At-rule | Purpose |
|---------|---------|
| `@import` | import another style sheet (now with optional media list) |
| `@media` | styles scoped to a media type/query |
| `@page` | paged-media page box + margins |
| `@charset` | declare the style sheet's character encoding |
| `@font-face` | (defined in CSS2; dropped in 2.1, returns in CSS3 Fonts) |

## Properties — by group (CSS 2.1 defines ~ 115 properties)

### Box model
`margin`, `margin-top/right/bottom/left`;
`padding`, `padding-top/right/bottom/left`;
`border`, `border-top/right/bottom/left`, `border-width`, `border-color`, `border-style`
and all per-side longhands; `width`, `height`, `min-width`, `max-width`, `min-height`,
`max-height`

### Visual formatting & positioning (new in CSS2)
`display`, `position` (`static|relative|absolute|fixed`), `top`, `right`, `bottom`, `left`,
`float`, `clear`, `z-index`, `direction`, `unicode-bidi`

### Visual effects
`overflow`, `clip`, `visibility`

### Generated content (new in CSS2)
`content`, `quotes`, `counter-reset`, `counter-increment`, `list-style-type`,
`list-style-image`, `list-style-position`, `list-style`

### Color & background
`color`, `background-color`, `background-image`, `background-repeat`,
`background-attachment`, `background-position`, `background`

### Font
`font-family`, `font-style`, `font-variant`, `font-weight`, `font-size`, `font`

### Text
`text-indent`, `text-align`, `text-decoration`, `letter-spacing`, `word-spacing`,
`text-transform`, `white-space`, `line-height`, `vertical-align`

### Tables (new in CSS2)
`caption-side`, `table-layout`, `border-collapse`, `border-spacing`, `empty-cells`

### User interface (new in CSS2)
`cursor`, `outline`, `outline-width`, `outline-style`, `outline-color`

### Paged media (new in CSS2)
`page-break-before`, `page-break-after`, `page-break-inside`, `orphans`, `widows`

### Aural / speech (CSS2; largely deferred)
`volume`, `speak`, `pause`, `cue`, `voice-family`, `pitch`, `richness`, … (removed in 2.1, moved
to CSS Speech module)

## Value types, units & functions

| Category | Values |
|----------|--------|
| Lengths | `em`, `ex`, `px`, `in`, `cm`, `mm`, `pt`, `pc`, `%` |
| Angles/time (aural) | `deg`, `grad`, `rad`, `ms`, `s`, `Hz`, `kHz` |
| Keywords | `inherit` (now an explicit universal keyword), `auto`, `none`, `normal`, … |
| Colors | names, `#rgb`, `#rrggbb`, `rgb()`, 16 + `orange`, plus system colors |
| Functions | `url()`, `rgb()`, `counter()`, `counters()`, `attr()`, `rect()` (for `clip`) |

## Core concepts added over CSS1

- **The visual formatting model** — block/inline/positioned boxes, containing blocks,
  stacking contexts (`z-index`), floats and clearance.
- **Positioning** — `relative`, `absolute`, `fixed`.
- **Media types** (`@media screen`, `print`, …) — one style sheet, many output targets.
- **The table formatting model** and **paged media** (`@page`, page breaks, orphans/widows).
- **Generated content** via `:before`/`:after` + `content` + counters.
- The explicit **`inherit`** keyword.

## What came after (CSS3+)

CSS stopped being one document after 2.1. Everything from here — advanced selectors, flexbox,
grid, transitions, animations, media queries, new color syntaxes, web fonts — lives in
independent **modules**: see [`../css3-modules/`](../css3-modules/).

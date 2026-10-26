# CSS3+ — modular feature catalogue

From CSS3 onward there is no single "CSS3 specification." The language is developed as
**independent modules**, each versioned at its own "level." A given year's state of CSS is
captured by a **CSS Snapshot** (see [`../snapshot/`](../snapshot/)). This document catalogues the
selectors, properties, at-rules, functions and values of the major modules archived here. Each
heading links to the archived source; signatures are in [`../SIGNATURES.md`](../SIGNATURES.md).

---

## Selectors Level 3 &mdash; [`selectors-3.src.html`](./selectors-3.src.html)

Everything new over CSS 2.1 selectors.

**Combinators:** `E ~ F` (general sibling).

**Attribute selectors:** `[attr^=val]` (starts-with), `[attr$=val]` (ends-with),
`[attr*=val]` (contains).

**Structural pseudo-classes:** `:root`, `:nth-child(n)`, `:nth-last-child(n)`, `:nth-of-type(n)`,
`:nth-last-of-type(n)`, `:first-of-type`, `:last-of-type`, `:last-child`, `:only-child`,
`:only-of-type`, `:empty`.

**Other pseudo-classes:** `:target`, `:enabled`, `:disabled`, `:checked`, `:not(s)`,
`:lang(c)` (extended).

**Pseudo-elements with `::`:** `::before`, `::after`, `::first-line`, `::first-letter`
(double-colon syntax introduced to distinguish pseudo-elements from pseudo-classes).

---

## CSS Color Level 3 &mdash; [`css-color-3.html`](./css-color-3.html)

**Properties:** `color`, `opacity`.

**Color syntaxes:** `rgba(r,g,b,a)`, `hsl(h,s%,l%)`, `hsla(h,s%,l%,a)`, the `transparent`
keyword as a real color, `currentColor`, and the extended **SVG/X11 named-color** list (~147
names).

---

## CSS Backgrounds & Borders Level 3 &mdash; [`css-backgrounds-3.bs`](./css-backgrounds-3.bs)

**Backgrounds:** multiple backgrounds (comma-separated layers), `background-size`,
`background-origin`, `background-clip`, plus the CSS1/2 `background-*` set.

**Borders:** `border-radius` (+ per-corner longhands), `border-image` (`border-image-source`,
`-slice`, `-width`, `-outset`, `-repeat`), `box-shadow`.

---

## CSS Fonts Level 3 &mdash; [`css-fonts-3.html`](./css-fonts-3.html)

**At-rule:** `@font-face` (web fonts) with descriptors `src`, `font-family`, `font-style`,
`font-weight`, `unicode-range`, `font-feature-settings`.

**Properties:** `font-size-adjust`, `font-stretch`, `font-kerning`, `font-feature-settings`,
`font-variant-*` (ligatures, caps, numeric, …), `font-synthesis`.

---

## CSS Text Level 3 &mdash; [`css-text-3.bs`](./css-text-3.bs)

**Properties:** `text-align` (extended), `text-align-last`, `text-justify`, `text-indent`
(extended), `word-spacing`, `letter-spacing`, `white-space`, `tab-size`, `word-break`,
`overflow-wrap` (`word-wrap`), `line-break`, `hyphens`, `text-transform` (extended).

---

## CSS Values & Units Level 3 &mdash; [`css-values-3.bs`](./css-values-3.bs)

**New length units:** `rem`, `ch`, `vw`, `vh`, `vmin`, `vmax`, `q`.

**Functions:** `calc()` (arithmetic mixing units), `attr()` (extended), `toggle()`,
`var()`-adjacent groundwork.

**Value keywords:** `initial`, `unset` (added to `inherit`).

**Other units:** resolution (`dpi`, `dpcm`, `dppx`), angle (`turn`), time, frequency.

---

## Media Queries Level 4 &mdash; [`mediaqueries-4.bs`](./mediaqueries-4.bs)

**At-rule:** `@media` with feature queries.

**Media features:** `width`/`min-width`/`max-width`, `height`, `aspect-ratio`, `orientation`,
`resolution`, `hover`, `any-hover`, `pointer`, `any-pointer`, `prefers-color-scheme`,
`prefers-reduced-motion`, `prefers-contrast`, range syntax (`(400px <= width <= 700px)`),
logical `and` / `or` / `not` / `only`.

---

## CSS Cascade & Inheritance Level 4 &mdash; [`css-cascade-4.bs`](./css-cascade-4.bs)

**At-rules:** `@import` (with `supports()`/media conditions), `@media`-layering groundwork.

**Keywords:** `initial`, `inherit`, `unset`, `revert`.

**Concepts:** the cascade order, origins (author/user/UA), importance, specificity, and the
groundwork later extended by cascade layers (`@layer`, Level 5).

---

## CSS Flexible Box Layout Level 1 (Flexbox) &mdash; [`css-flexbox-1.bs`](./css-flexbox-1.bs)

**Display:** `display: flex | inline-flex`.

**Container properties:** `flex-direction`, `flex-wrap`, `flex-flow` (shorthand),
`justify-content`, `align-items`, `align-content`.

**Item properties:** `order`, `flex-grow`, `flex-shrink`, `flex-basis`, `flex` (shorthand),
`align-self`.

---

## CSS Grid Layout Level 1 &mdash; [`css-grid-1.bs`](./css-grid-1.bs)

**Display:** `display: grid | inline-grid`.

**Container properties:** `grid-template-columns`, `grid-template-rows`, `grid-template-areas`,
`grid-template` (shorthand), `grid-auto-columns`, `grid-auto-rows`, `grid-auto-flow`, `grid`
(shorthand), `row-gap`, `column-gap`, `gap`, `justify-items`, `align-items`, `justify-content`,
`align-content`.

**Item properties:** `grid-column-start/end`, `grid-row-start/end`, `grid-column`, `grid-row`,
`grid-area`, `justify-self`, `align-self`.

**Functions / values:** `repeat()`, `minmax()`, `fit-content()`, the `fr` flexible-length unit,
`min-content`, `max-content`, `auto-fill`, `auto-fit`, named grid lines/areas.

---

## CSS Transitions Level 1 &mdash; [`css-transitions-1.bs`](./css-transitions-1.bs)

**Properties:** `transition-property`, `transition-duration`, `transition-timing-function`,
`transition-delay`, `transition` (shorthand).

**Timing functions:** `ease`, `linear`, `ease-in`, `ease-out`, `ease-in-out`,
`cubic-bezier()`, `steps()`.

---

## CSS Animations Level 1 &mdash; [`css-animations-1.bs`](./css-animations-1.bs)

**At-rule:** `@keyframes name { from/to / 0%..100% { … } }`.

**Properties:** `animation-name`, `animation-duration`, `animation-timing-function`,
`animation-delay`, `animation-iteration-count`, `animation-direction`, `animation-fill-mode`,
`animation-play-state`, `animation` (shorthand).

---

## Beyond what is archived here

The module set is large and ongoing. Other significant modules (not source-archived here but part
of modern CSS, tracked in the CSS Snapshot) include: **Transforms** (`transform`, `translate()`,
`rotate()`, `scale()`, `perspective`), **Color 4/5** (`oklch()`, `color()`, relative color),
**Cascade 5** (`@layer`), **Containment & Container Queries** (`@container`, `container-type`),
**Custom Properties** (`--var`, `var()`), **Writing Modes**, **Shapes**, **Masking**, **Filters**
(`filter`, `blur()`), and **Nesting** (`&`). See [`../snapshot/FEATURES.md`](../snapshot/FEATURES.md)
for the year-by-year "what counts as CSS" picture.

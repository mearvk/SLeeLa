# Structural Integrity Appraisal — `div`, `span`, `table`

_An appraisal of HTML's three core structural primitives against the capabilities assumed of
international web technology, with a safeguarding checklist. Grounded in the specifications
archived under [`specifications/`](./specifications): the HTML 3.2 / 4.01 / XHTML DTDs and the WHATWG HTML
Living Standard `source`._

> **Context.** The concern raised was that HTML "has become unwieldy." The finding below is
> the opposite of what that fear implies: the three structural elements are **stable, small,
> and well-specified**. What is unwieldy is not the elements — it is the *practice* of using
> `div`/`span` as a substitute for meaningful structure. The checklist targets exactly that.

---

## 1. Executive appraisal

| Element | Introduced | Semantic role | International support | State |
|---------|-----------|---------------|-----------------------|-------|
| `div` | HTML 3.2 (1997) | Generic **block** container; "no special meaning at all" | `dir` + `lang` since HTML 4.0 | ✅ Great |
| `span` | **HTML 4.0 (1998)** | Generic **inline** container | `dir` + `lang` since HTML 4.0 | ✅ Great |
| `table` | HTML 3.2 (flat) → HTML 4.0 (full model) | **Tabular data** (NOT layout) | `dir` + `lang` + `scope`/`headers` | ✅ Great, with conditions |

**Verdict: coherent and fit for purpose.** All three are present, stable across 25+ years,
and fully carry the internationalization attributes. The only *missing* capabilities are ones
that **by design do not belong to HTML at all** — visual sizing and layout live in CSS (see §5).

---

## 2. What the archived specs actually say

### `div`
- **HTML 3.2:** `<!ELEMENT DIV - - %body.content>` — block container, but its only attribute is
  `align (left|center|right)`. **No `dir`, no `lang`, no `style`.** So 3.2 `div` has *no*
  international text support.
- **HTML 4.01:** `<!ELEMENT DIV - - (%flow;)* -- generic language/style container -->`, and its
  attribute list is `%attrs;` = `%coreattrs` (`id/class/style/title`) + **`%i18n` (`lang`,
  `dir=ltr|rtl`)** + events. This is the turning point: `div` becomes internationally aware.
- **Living Standard:** verbatim — _"The `div` element has no special meaning at all. It
  represents its children."_ Authors are "strongly encouraged to view the `div` element as an
  element of **last resort**, for when no other element is suitable."

### `span`
- **HTML 3.2:** **not declared** — `span` did not exist. (Confirmed: `grep` finds no `SPAN` in
  `HTML32.dtd`.)
- **HTML 4.01:** `<!ELEMENT SPAN - - (%inline;)* -- generic language/style container -->`, carries
  the same `%i18n` attributes. The inline counterpart to `div`.

### `table`
- **HTML 3.2 (flat model):** `<!ELEMENT table - - (caption?, tr+)>`, `tr` → `(th|td)*`. That is
  **all** — no `thead`, `tbody`, `tfoot`, `col`, or `colgroup`.
- **HTML 4.01 (full model):** `(CAPTION?, (COL*|COLGROUP*), THEAD?, TFOOT?, TBODY+)` with
  `THEAD/TFOOT/TBODY`, `COL/COLGROUP`, a `summary` attribute for speech output, and the
  `%i18n` group. This is the mature, accessible table.

---

## 3. International technology — capability matrix

"Assumed functionality of international technology" = the capabilities a globally-deployed page
is expected to have: bidirectional text, language tagging, Unicode, assistive-tech semantics.

| Capability | `div` | `span` | `table` | Notes |
|------------|:-----:|:------:|:-------:|-------|
| Unicode content (UTF-8) | ✅ | ✅ | ✅ | Character set is a document-level guarantee since HTML 2.0 / 4.0 |
| Language tagging (`lang` / `xml:lang`) | ✅ (4.0+) | ✅ (4.0+) | ✅ | BCP 47 language codes |
| Bidirectional text (`dir="ltr\|rtl"`) | ✅ (4.0+) | ✅ (4.0+) | ✅ | Essential for Arabic, Hebrew, etc. |
| `dir="auto"` (heuristic direction) | ✅ (HTML5+) | ✅ | ✅ | Added by the Living Standard; not in 4.01 |
| Isolation of bidi runs (`<bdi>`, `unicode-bidi: isolate`) | n/a | use `<bdi>` | n/a | `span` alone does **not** isolate; use `<bdi>` or CSS |
| Assistive-tech structure | ⚠️ none intrinsically | ⚠️ none | ✅ `scope`, `headers`, `caption`, `summary` | `div`/`span` convey *no* semantics (see §4) |
| Logical reading order | ✅ (source order) | ✅ | ✅ | Preserved as long as CSS does not reorder visually (see §5 risk) |

**Great:** language + direction support on all three is complete from HTML 4.0 onward, and the
Living Standard adds `dir="auto"`.
**Condition:** `div`/`span` are *presentation/scripting hooks with zero semantics* — they are
international-text-capable but **accessibility-neutral**. A `table` is the only one of the three
that carries structural meaning assistive technology can use.
**Missing (by design, not a defect):** neither `div` nor `span` isolates a bidi run on its own;
that is the job of `<bdi>` or CSS `unicode-bidi`.

---

## 4. Conditions: great / missing / unsupported

### ✅ Great (works as assumed)
- `div` and `span` are the complete block/inline generic-container pair from HTML 4.0 on, with
  full `lang`/`dir` support — the correct styling and scripting hooks.
- `table` provides a genuinely rich, accessible tabular model (header association via
  `scope`/`headers`, `caption`, grouping via `thead`/`tbody`/`tfoot`, column grouping).
- All three are **backward- and forward-compatible**: a 4.01 `div` is still valid in the Living
  Standard. Nothing here is deprecated.

### ⚠️ Missing / conditional
- **No semantics in `div`/`span`.** The spec itself calls `div` an "element of last resort."
  Overusing them ("div soup") is the real source of the "unwieldy" feeling — the fix is to
  prefer `article`, `section`, `nav`, `header`, `footer`, `main`, `aside`, `figure`.
- **`span` absent before HTML 4.0.** Documents targeting HTML 3.2 cannot use it.
- **HTML 3.2 `div` has no `lang`/`dir`.** International support on `div` begins at 4.0.
- **`table` has no intrinsic sort/filter/resize.** Those are CSS/JS concerns, not table features.
- **Bidi isolation** needs `<bdi>`/CSS, not a bare `span`.

### ❌ Unsupported (and correctly so — out of scope for HTML)
- **Visual sizing / layout is not an HTML capability.** HTML defines *structure*; it does not
  define width, height, or how a box resizes. The Living Standard's rendering section is
  explicitly non-normative ("default rendering rules") and it *references* CSS modules
  (e.g. `CSSFLEXBOX`) rather than defining layout. So "does HTML let a div resize?" is answered
  in §5 — the honest answer is **HTML no, CSS yes**.
- **Using `table` for page layout is unsupported practice** (not a DTD error, but a conformance
  anti-pattern): a layout table misrepresents non-tabular content to assistive technology.

---

## 5. "Can divs resize dynamically?" — the direct answer

**Not through HTML. Yes — richly — through CSS, which is the correct layer for it.**

- **HTML's role:** a `div` is just a block box that "represents its children." The specification
  assigns it **no width, height, or resize behavior**. There is no HTML attribute that sizes a
  `div` responsively. (The legacy `align` attribute on `div` is presentational and obsolete.)
- **CSS's role (this is where dynamic resize lives):**
  - **Fluid/relative units** — `%`, `vw`/`vh`, `em`/`rem`, `ch` → a `div` grows/shrinks with its
    container or viewport.
  - **Intrinsic sizing** — `min-content`, `max-content`, `fit-content`, `clamp()`.
  - **Flexbox** (`display:flex`, `flex-grow/shrink/basis`) — children resize to share space.
  - **Grid** (`display:grid`, `fr` units, `minmax()`, `auto-fit`/`auto-fill`) — two-dimensional
    dynamic tracks.
  - **Container queries & media queries** — a `div` can restyle/resize based on viewport *or*
    its own container's size.
  - **User-draggable resize** — the CSS `resize` property (`resize: both; overflow:auto`) lets a
    user drag-resize a `div`.
- **Conclusion:** dynamic resizing is **fully supported** for `div` (and `span`, as an inline
  box, within inline-layout limits) — but it is a **CSS capability the HTML spec references, not
  one it provides.** This separation is a feature, not a gap: structure (HTML) stays stable while
  presentation (CSS) evolves independently.

> **Risk flag for the safeguarding checklist:** CSS layout can *visually reorder* flex/grid
> children away from source order. That breaks logical reading order for screen readers and
> keyboard users. Safeguard: keep DOM source order meaningful; never rely on `order`/grid
> placement to convey sequence.

---

## 6. Safeguarding checklist (page-structure integrity)

Use this as a review gate for any page or component.

### Semantic structure
- [ ] `div`/`span` used only where **no semantic element** fits (not in place of `article`,
      `section`, `nav`, `header`, `footer`, `main`, `aside`, `figure`, `ul/ol`, headings).
- [ ] Each page has exactly one `<main>`; landmark regions are real landmark elements, not `div`s.
- [ ] No "div soup": nesting depth of generic containers is justified, not accidental.
- [ ] `span` used only for inline phrasing hooks; block content is never placed inside a `span`.

### Tables
- [ ] `table` is used for **tabular data only** — never for page layout.
- [ ] Every data table has a `<caption>`; header cells use `<th>` with `scope` (or `headers`).
- [ ] Complex tables use `thead`/`tbody`/`tfoot` and, where helpful, `col`/`colgroup`.
- [ ] No reliance on `border`/`cellpadding`/`cellspacing`/`align` presentational attributes —
      style with CSS (these are legacy/obsolete).

### Internationalization
- [ ] Document declares a `lang`; sub-sections in another language carry their own `lang`.
- [ ] `dir="rtl"` (or `dir="auto"`) is set wherever right-to-left or mixed-direction text appears.
- [ ] Bidirectional runs that could mis-order are wrapped in `<bdi>` (not a bare `span`).
- [ ] Content is UTF-8; no reliance on named entities for basic characters.

### Layout / dynamic resize (CSS layer)
- [ ] Sizing uses CSS, never HTML presentational attributes.
- [ ] Responsive behavior uses relative units / flexbox / grid / container queries — not fixed
      pixel widths that overflow small viewports.
- [ ] Visual order from `flex`/`grid`/`order` **matches** DOM source (reading) order, or any
      divergence is deliberate and accessibility-tested.
- [ ] Any user-resizable `div` pairs CSS `resize` with `overflow:auto` and a sensible
      `min-/max-` constraint.

### Validation / conformance
- [ ] Markup validates (for XHTML, against the self-resolving bundle in
      [`specifications/dtd/`](./specifications/dtd) via `catalog.xml`; for HTML5, against the Living Standard).
- [ ] Elements are properly nested and closed; inline (`span`) never wraps block (`div`).
- [ ] No deprecated/obsolete structural attributes remain.

---

## 7. Bottom line

The structural primitives are **not the source of unwieldiness** — they are small, stable, and
internationally capable. `div` dates to 1997, `span` to 1998, the full accessible `table` to
1998, and all three remain valid and unchanged in today's Living Standard. Dynamic resizing is
fully available, delivered by CSS exactly as the architecture intends. The real risk is
*discipline*: generic-container overuse and layout misuse of tables. The checklist in §6 is the
safeguard for keeping page structure coherent.

# HTML 4.0 / 4.01 (1997 / 1999)

| | |
|---|---|
| **Formal title** | HTML 4.0 / HTML 4.01 Specification |
| **Code name** | "Cougar" |
| **Publisher** | W3C |
| **Published** | HTML 4.0 — 18 December 1997 (revised 24 April 1998); **HTML 4.01 — 24 December 1999** |
| **Status** | Superseded |
| **Canonical reference** | <https://www.w3.org/TR/html401/> |
| **Source in this archive** | ✅ Normative DTDs + entity sets — [`specifications/dtd/html-4.0/`](../specifications/dtd/html-4.0/) (strict/loose/frameset) and [`specifications/dtd/html-4.01/`](../specifications/dtd/html-4.01/) (strict/loose/frameset + HTMLlat1/HTMLspecial/HTMLsymbol entities). ✅ **Complete prose** now archived — [`specifications/prose/html-4.01/`](../specifications/prose/html-4.01/): every canonical chapter (`about`, `intro/*`, `struct/*` incl. `lists`, `present/*`, `interact/*` incl. `forms`, `sgml/*`, `appendix/*`, `index/*`) plus the figure images. The published 24 December 1999 Recommendation. |

> 📖 **Tag reference:** see [`tags/04-html-4.01.tags.md`](./tags/04-html-4.01.tags.md) for this version's elements and their definitions (Strict / Transitional / Frameset).

## Summary

HTML 4.x was the long-lived mature version of classic HTML, and the baseline for the web for roughly a decade. Its central theme was the **separation of structure from presentation**: markup describes the document, while **CSS** handles appearance. HTML 4.01 is a minor bug-fix revision of 4.0.

## Three DTD "flavors"

- **Strict** — excludes presentational and deprecated elements (encourages CSS)
- **Transitional** (a.k.a. Loose) — allows the deprecated presentational markup for backwards compatibility
- **Frameset** — for documents using `<frameset>` / `<frame>`

## Key features

- First-class **scripting** (`<script>`) and **styling** (`<style>`, `class`, `id`) hooks
- **Accessibility**: `<label>`, `<fieldset>`, `<legend>`, `accesskey`, `tabindex`, table header associations
- **Internationalization**: the `lang` and `dir` attributes, Unicode as the document character set
- `<object>` as a general embedding mechanism; `<iframe>` (Transitional)
- **Deprecated** many HTML 3.2 presentational features (`<font>`, `<center>`) in favor of CSS

## Significance

Defined "proper" semantic HTML authoring for the CSS era and stood as the stable standard until HTML5. XHTML 1.0 is essentially HTML 4.01 reformulated as XML.

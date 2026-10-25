# Worked examples — one well-formed demo document per HTML version

Each file here is a **self-contained, well-formed HTML (or XHTML) document** that showcases the
elements a given version introduced, each in its usual use case. Every example carries:

- a **Title** and a **Date** (2026-10-25),
- the live markup for that use case, and
- a **"Performs as expected"** verdict in a modern browser: **&#x2713; Yes** or **&#x2717; No**
  (with a one-line reason when it is No).

No CSS is used anywhere — just the markup and the plain &#x2713; / &#x2717; status glyph with its
label, as requested.

| Demo | Version | Doctype / form | Well-formed check |
|------|---------|----------------|-------------------|
| [`01-html-tags-1991.demo.html`](./01-html-tags-1991.demo.html) | HTML Tags (1991) | HTML5 doctype, 1991 elements | HTML parse OK |
| [`02-html-2.0.demo.html`](./02-html-2.0.demo.html) | HTML 2.0 | HTML5 doctype | HTML parse OK |
| [`03-html-3.2.demo.html`](./03-html-3.2.demo.html) | HTML 3.2 | HTML5 doctype | HTML parse OK |
| [`04-html-4.01.demo.html`](./04-html-4.01.demo.html) | HTML 4.01 | HTML 4.01 Strict doctype | HTML parse OK |
| [`05-xhtml-1.0.demo.html`](./05-xhtml-1.0.demo.html) | XHTML 1.0 | XHTML 1.0 Strict, XML | `xmllint` well-formed XML ✓ |
| [`06-xhtml-1.1.demo.html`](./06-xhtml-1.1.demo.html) | XHTML 1.1 | XHTML 1.1, XML | `xmllint` well-formed XML ✓ |
| [`07-html5.demo.html`](./07-html5.demo.html) | HTML5 (2014) | HTML5 doctype | HTML parse OK |
| [`10-html-living-standard.demo.html`](./10-html-living-standard.demo.html) | Living Standard | HTML5 doctype | HTML parse OK |

## How "performs as expected" is judged

The verdict is measured against **a current browser**, so it is meaningful and ties into the
deprecated/obsolete data in [`../md/tags/CHANGES.md`](../md/tags/CHANGES.md):

- **&#x2713; Yes** — the element still renders and behaves as its spec intended (e.g. `<table>`,
  `<ul>`, `<form>`, `<video>`, `<details>`).
- **&#x2717; No** — the element is obsolete / removed / repurposed and no longer performs its
  original job (e.g. `<font>`, `<center>`, `<applet>`, `<isindex>`, framesets, `<acronym>`,
  complex-ruby `<rb>`, and the still-experimental `<selectedcontent>`).

## Notes

- The XHTML demos (05, 06) use **numeric** character references (`&#8212;`, `&#160;`) rather than
  named ones, because named entities like `&mdash;`/`&nbsp;` are **not predefined in XML** and
  would make the document not-well-formed. Both validate cleanly with `xmllint --noout --nonet`.
- Where an element cannot be shown live without breaking the page (framesets, which replace
  `<body>`; `<isindex>`, which no longer renders), the markup is shown **escaped** inside a
  `<pre>` block and the reason is stated.
- Media/applet/script resources (`movie.mp4`, `Clock.class`, etc.) are placeholders — the point
  is the element and its usual use case, not the asset.
- HTML5 / 5.1 / 5.2 share one demo set with the Living Standard where their element sets overlap;
  `07-html5.demo.html` covers the 2014 additions and `10-...` covers what came after.

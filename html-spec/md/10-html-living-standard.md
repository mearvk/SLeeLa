# HTML Living Standard (current)

| | |
|---|---|
| **Formal title** | HTML Standard (the "Living Standard") |
| **Publisher** | WHATWG (Web Hypertext Application Technology Working Group) |
| **First published** | 2011 (as the Living Standard model); continuously updated ever since |
| **Status** | ✅ **The current, authoritative version of HTML** |
| **Canonical reference** | <https://html.spec.whatwg.org/> (multipage: <https://html.spec.whatwg.org/multipage/>) |
| **Source in this archive** | ✅ `specs/whatwg-html-living-standard.source.html` — full 8.0 MB source; see PROVENANCE.txt for the exact commit |

## Summary

The HTML Living Standard is a **continuously-maintained, unversioned** specification — there are no numbered editions; the standard *is* whatever the current document says. Rather than periodic snapshots, changes land continuously as features reach interoperable implementation. Since the **2019 W3C/WHATWG agreement**, this is the single authoritative HTML specification, and the W3C no longer publishes competing versions.

## What it covers

Far broader than any numbered HTML spec — it is both the HTML vocabulary and a large body of associated platform semantics:

- The complete element/attribute vocabulary and content model
- The **HTML parser** algorithm (tokenization + tree construction), defined precisely enough for bit-identical DOMs across browsers
- Core DOM interfaces, event loop and microtask processing model
- Browsing contexts, navigation, session history, origins and the same-origin policy
- Embedded content (media, `<canvas>`, images), forms and form validation
- Platform APIs historically bundled with HTML5: Web Storage, Web Workers, `postMessage`, drag-and-drop, offline, server-sent events, and more

## About the archived source

The archived `source` file is the single hand-authored file from the [`whatwg/html`](https://github.com/whatwg/html) repository that generates the entire published standard. It is ~8 MB of annotated HTML. (It is too large for GitHub's own blame tool; WHATWG points contributors to searchfox for history.)

## Significance

The definitive, living definition of the web's markup language today. Every modern browser implements against this document, and all earlier numbered specs (1991 → HTML 5.2) are now historical context leading up to it.

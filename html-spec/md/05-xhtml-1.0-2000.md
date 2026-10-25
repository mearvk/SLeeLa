# XHTML 1.0 (2000)

| | |
|---|---|
| **Formal title** | XHTML 1.0: The Extensible HyperText Markup Language |
| **Publisher** | W3C |
| **Published** | 26 January 2000 (W3C Recommendation; revised 1 August 2002) |
| **Status** | Superseded |
| **Canonical reference** | <https://www.w3.org/TR/xhtml1/> |
| **Source in this archive** | ✅ Normative DTDs — [`specs/dtd/xhtml-1.0/`](../specs/dtd/xhtml-1.0/) (strict / transitional / frameset). ✅ **Full prose** now archived — [`specs/prose/xhtml-1.0/Overview.html`](../specs/prose/xhtml-1.0/Overview.html) (the complete XHTML 1.0 Second Edition single-page Recommendation). |

## Summary

XHTML 1.0 is a **reformulation of HTML 4.01 as an application of XML 1.0** rather than SGML. The vocabulary (the elements and attributes) is essentially identical to HTML 4.01; what changed are the syntax rules, which XML makes strict.

## XML well-formedness rules

- Elements and attributes must be **lowercase**
- Every element must be **closed** — including empty elements written as `<br />`, `<img ... />`
- Attribute values must always be **quoted**
- Attributes cannot be minimized (`disabled="disabled"`, not just `disabled`)
- Elements must nest properly; a single root element is required

Like HTML 4.01, it offered **Strict, Transitional, and Frameset** DTDs.

## Significance

Brought XML's rigor and tooling (XML parsers, namespaces, XSLT) to the web and promoted cleaner, more consistent authoring. It set up the later, stricter XHTML 1.1 — and the eventual industry pivot away from XML-based HTML toward HTML5.

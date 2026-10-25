# XHTML 1.1 (2001)

| | |
|---|---|
| **Formal title** | XHTML 1.1 — Module-based XHTML |
| **Publisher** | W3C |
| **Published** | 31 May 2001 (W3C Recommendation; 2nd edition 23 November 2010) |
| **Status** | Superseded |
| **Canonical reference** | <https://www.w3.org/TR/xhtml11/> |
| **Source in this archive** | ✅ Normative DTD + content module — [`specs/dtd/xhtml-1.1/`](../specs/dtd/xhtml-1.1/) (`xhtml11.dtd`, `xhtml11-model-1.mod`). Prose is at the w3.org URL above. |

## Summary

XHTML 1.1 took XHTML 1.0 Strict and removed the transitional/legacy allowances, defining the language in terms of **modularized XHTML** — a set of reusable modules (text, forms, tables, etc.) assembled into the full language. This modular approach was intended to let constrained devices support defined subsets.

## Key points

- Based strictly on **XHTML 1.0 Strict** — no Transitional or Frameset variant
- Removed the `lang` attribute in favor of XML's `xml:lang`
- Added the **Ruby annotation** module (`<ruby>`, `<rt>`, `<rp>`) for East Asian typography
- Intended to be served with an XML MIME type (`application/xhtml+xml`)

## Significance

The purest expression of the "HTML as strict XML" vision. In practice its strictness (notably draconian XML error handling — a single syntax error stops the page from rendering) limited adoption, and the community direction shifted decisively toward the forgiving, backward-compatible HTML5. Work on XHTML 2.0 was later abandoned.

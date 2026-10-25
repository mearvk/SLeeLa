# HTML Tags (1991–1992)

| | |
|---|---|
| **Common name** | "HTML Tags" / the first HTML |
| **Author** | Tim Berners-Lee (CERN) |
| **Publisher** | CERN (informal; no standards body) |
| **Published** | Late 1991 (document circulated), described publicly ~1992 |
| **Status** | Historical — never a formal standard |
| **Canonical reference** | <https://www.w3.org/History/19921103-hypertext/hypertext/WWW/MarkUp/Tags.html> |
| **Source in this archive** | ✅ [`specs/html-tags-1991/Tags.html`](../specs/html-tags-1991/Tags.html) — the original note, verbatim |

> 📖 **Tag reference:** see [`tags/01-html-tags-1991.tags.md`](./tags/01-html-tags-1991.tags.md) for this version's elements and their definitions.

## Summary

The first public description of HTML was an informal document titled **"HTML Tags"**, listing roughly **18–20 elements** that the earliest web browsers would support. It had no DTD, no formal grammar, and no versioning — it was a working note, not a specification in the modern sense.

HTML was conceived as an application of **SGML** (Standard Generalized Markup Language, ISO 8879:1986), borrowing SGML's angle-bracket tag syntax.

## Key elements introduced

As enumerated by the [original note](../specs/html-tags-1991/Tags.html) itself:

- Document/structure: `<title>`, `<nextid>` (a NeXT-browser document-wide id counter, "not good
  SGML"; obsolete), `<base>` ("format … not yet specified. NOT CURRENTLY USED"), `<p>`
  (originally a separator, not a container)
- Headings: `<h1>`–`<h6>`
- Hypertext anchor: `<a>` (with `HREF` / `NAME`) — the defining feature of the web; the note
  shows `HREF="http://info.cern.ch/hypertext/WWW/MarkUp/XXX"`
- Lists: `<ul>`, `<menu>`, `<dir>` (three flavors of list, each with `<li>`) and the
  glossary/definition list `<dl>` / `<dt>` / `<dd>`. _(Note: there is **no** `<ol>` yet — ordered
  lists came later; a `<ul>` was merely "typically" rendered bulleted.)_
- Literal/quoting: `<xmp>`, `<listing>`, `<plaintext>` (all now obsolete)
- Highlighting: `<hp1>`, `<hp2>`, … ("NOT CURRENTLY USED" at the time), and `<address>`

## Significance

Defined the hyperlink and the basic document outline that every later version built on. There was no `<html>`, `<head>`, or `<body>` wrapper yet, no images, no tables, and no forms. Several listed tags were explicitly marked *"NOT CURRENTLY USED"* or *"Obsolete"* even in 1991 — a reminder that this was a living working note, not a frozen standard.

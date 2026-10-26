# XHTML 1.0 (2000, 2nd Ed. 2002) — element reference

Sources: [`specifications/dtd/xhtml-1.0/`](../../specifications/dtd/xhtml-1.0/)
(`xhtml1-strict.dtd` = 77 elements, `xhtml1-transitional.dtd`, `xhtml1-frameset.dtd`) and the
archived prose [`specifications/prose/xhtml-1.0/Overview.html`](../../specifications/prose/xhtml-1.0/Overview.html).

XHTML 1.0 is **HTML 4.01 reformulated as XML** — the *element and attribute vocabulary is the
same as HTML 4.01*, split into the same three flavours (Strict / Transitional / Frameset). What
changes is **syntax, not tags**. So rather than repeat the catalogue, this page lists the
element set and the syntactic rules that differ.

## Element set = HTML 4.01's

The full tag list, grouped by role and with per-element definitions, is in
[`04-html-4.01.tags.md`](./04-html-4.01.tags.md) and applies unchanged here:

- **Strict** (77 elements): the modern core — structure (`html`/`head`/`body`/`title`/…),
  grouping (`div`/`span`/`p`/`h1`–`h6`), phrase (`em`/`strong`/`abbr`/`acronym`/…),
  presentational-kept (`tt`/`i`/`b`/`big`/`small`/`sub`/`sup`), edits (`ins`/`del`), `bdo`,
  lists (`ul`/`ol`/`li`/`dl`/`dt`/`dd`), the full table model
  (`table`/`caption`/`thead`/`tbody`/`tfoot`/`col`/`colgroup`/`tr`/`th`/`td`), forms
  (`form`/`input`/`button`/`select`/`optgroup`/`option`/`textarea`/`label`/`fieldset`/`legend`),
  and embedding (`a`/`img`/`object`/`param`/`map`/`area`).
- **Transitional** adds the deprecated presentational tags: `font`, `basefont`, `center`, `u`,
  `s`, `strike`, `dir`, `menu`, `isindex`, `applet`, `iframe`.
- **Frameset** adds `frameset`, `frame`, `noframes` (and `iframe`).

## What is genuinely different in XHTML 1.0 — the XML rules

Because XHTML is XML, the *same* tags must be written to stricter rules:

| Rule | HTML 4.01 | XHTML 1.0 |
|------|-----------|-----------|
| Tag/attribute case | case-insensitive | **lower-case only** (`<P>` → `<p>`) |
| Empty elements | `<br>`, `<img ...>` | **must be closed**: `<br/>`, `<img ... />` |
| Non-empty empties | `<p>` could be left open | **every element must close**: `<p>…</p>` |
| Attribute values | some may be unquoted | **always quoted**: `width="80"` |
| Attribute minimisation | `<input disabled>` | **must be expanded**: `disabled="disabled"` |
| Nesting | lenient | **must be well-formed XML** (strictly nested) |
| Root | — | needs `xmlns="http://www.w3.org/1999/xhtml"` on `<html>` |
| `id` vs `name` | `name` on many elements | `id` is the fragment identifier; `name` deprecated on `a`/`map` |
| Script/style content | CDATA | should be wrapped in `<![CDATA[ … ]]>` or externalised |

## Significance

XHTML 1.0 introduced **no new elements** over HTML 4.01 — its contribution was to make documents
parseable by generic XML tools and to enforce well-formedness. The vocabulary is identical; only
the authoring discipline is tighter. (XHTML **1.1** then *modularised* that vocabulary — see
[`06-xhtml-1.1.tags.md`](./06-xhtml-1.1.tags.md).)

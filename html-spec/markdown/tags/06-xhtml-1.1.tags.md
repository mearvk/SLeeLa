# XHTML 1.1 (2001, 2nd Ed. 2010) — element reference

Sources: the module set under [`specifications/dtd/xhtml-1.1/`](../../specifications/dtd/xhtml-1.1/) (driver
`xhtml11.dtd` + 47-file Modularization library, validates offline) and the archived prose
[`specifications/prose/xhtml-1.1/xhtml11.html`](../../specifications/prose/xhtml-1.1/xhtml11.html).

XHTML 1.1 is **"Module-based XHTML"**: the same vocabulary as XHTML 1.0 **Strict**, but assembled
from the W3C *Modularization* modules rather than one monolithic DTD. At the tag level it is
**XHTML 1.0 Strict, plus the Ruby Annotation module**, and it drops the few remaining
HTML-compatibility affordances (notably the `lang` attribute — replaced by `xml:lang` — and the
`name` attribute on `a`/`map`, replaced by `id`).

## Modules that make up XHTML 1.1

From the prose, the document type is composed of (abbreviated): Structure, Text, Hypertext,
List, Object, Presentation, Edit, Bidirectional Text, Forms, Table, Image, Client-side Image Map,
Server-side Image Map, Intrinsic Events, Metainformation, Scripting, Style Sheet, Style
Attribute, Link, Base, **Ruby Annotation**, plus the Legacy exclusions removed.

## Element set = XHTML 1.0 Strict + Ruby

The core tags and their definitions are exactly as in
[`05-xhtml-1.0.tags.md`](./05-xhtml-1.0.tags.md) / [`04-html-4.01.tags.md`](./04-html-4.01.tags.md)
(Strict flavour): `html`, `head`, `body`, `title`, `base`, `meta`, `link`, `style`, `script`,
`noscript`, `div`, `span`, `p`, `h1`–`h6`, `hr`, `br`, `pre`, `blockquote`, `q`, `address`,
phrase (`em`/`strong`/`dfn`/`code`/`samp`/`kbd`/`var`/`cite`/`abbr`/`acronym`), presentational
(`tt`/`i`/`b`/`big`/`small`/`sub`/`sup`), `ins`/`del`, `bdo`, lists
(`ul`/`ol`/`li`/`dl`/`dt`/`dd`), the full table model
(`table`/`caption`/`thead`/`tbody`/`tfoot`/`col`/`colgroup`/`tr`/`th`/`td`), forms
(`form`/`input`/`button`/`select`/`optgroup`/`option`/`textarea`/`label`/`fieldset`/`legend`),
and embedding (`a`/`img`/`object`/`param`/`map`/`area`).

## New in XHTML 1.1 — the Ruby Annotation module

Ruby is the small inter-linear annotation used above/beside East-Asian text (e.g. furigana over
kanji):

| Tag | Definition |
|-----|------------|
| `<ruby>` | Container associating base text with its ruby annotation. |
| `<rb>` | **Ruby base** — the text being annotated. |
| `<rt>` | **Ruby text** — the annotation (e.g. pronunciation). |
| `<rp>` | **Ruby parenthesis** — fallback brackets shown by UAs that cannot render ruby. |
| `<rbc>` | **Ruby base component** — groups bases in complex (two-level) ruby. |
| `<rtc>` | **Ruby text component** — groups annotations in complex ruby. |

## Removed / changed vs XHTML 1.0 Strict

- **No `lang`** attribute — XHTML 1.1 uses **`xml:lang`** exclusively.
- **No `name`** on `a` and `map` — use **`id`** for fragment identifiers.
- Strict-only to begin with: none of the Transitional/Frameset presentational tags
  (`font`, `center`, `u`, `s`, `strike`, `dir`, `menu`, `isindex`, `applet`, frames) are present —
  as in XHTML 1.0 Strict.

## Significance

XHTML 1.1's contribution is **modularity** (a vocabulary you can subset/extend — the basis for
XHTML Basic and mobile profiles) and **internationalisation** (Ruby + `xml:lang`). Only the
six Ruby elements are genuinely new tags over XHTML 1.0 Strict.

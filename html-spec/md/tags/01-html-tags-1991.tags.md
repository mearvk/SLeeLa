# HTML Tags (1991) — element reference

Source: the original CERN note [`specs/html-tags-1991/Tags.html`](../../specs/html-tags-1991/Tags.html).
There was **no DTD** in 1991 — this is the informal set of tags the note describes. Several were
already flagged *"NOT CURRENTLY USED"* or *"Obsolete"* even then.

## Document / structure

| Tag | Definition |
|-----|------------|
| `<TITLE>` | The title of the document — an attribute of the node, not body text; only one per node. |
| `<NEXTID>` | A NeXT-browser document-wide counter for the next numeric anchor id. "Not good SGML"; browsers may ignore it. |
| `<BASE>` | Base address for resolving relative anchor addresses. "Format … not yet specified. NOT CURRENTLY USED." |
| `<ISINDEX>` | Marks the document as searchable (a keyword index the reader can query). |
| `<P>` | Paragraph **separator** (a break between paragraphs — not yet a container). |

## Headings

| Tag | Definition |
|-----|------------|
| `<H1>`–`<H6>` | Six levels of heading. H1 is the most prominent; the note advises not to skip levels. |

## Hypertext

| Tag | Definition |
|-----|------------|
| `<A>` | Anchor — the defining element of the web. `HREF` makes it the start of a link; `NAME` makes it a link destination. Shown as `HREF="http://info.cern.ch/hypertext/WWW/MarkUp/XXX"`. |

## Lists

| Tag | Definition |
|-----|------------|
| `<UL>` | Unordered list — multi-line paragraphs, typically rendered with bullets/whitespace. |
| `<MENU>` | A list of smaller items, typically one line each, more compact than `UL`. |
| `<DIR>` | A directory list of short elements (< one line), e.g. arranged in columns. |
| `<LI>` | List element — the item inside `UL` / `MENU` / `DIR`. |
| `<DL>` | Glossary / definition list (a set of named paragraphs). |
| `<DT>` | The term being defined, inside `DL`. |
| `<DD>` | The definition paragraph for the preceding `DT`. |

> Note: there is **no `<OL>` (ordered list) yet** — ordered lists arrived in HTML 2.0.

## Literal / preformatted

| Tag | Definition |
|-----|------------|
| `<XMP>` | "Example" — monospaced literal text, tags inside are not interpreted. (Obsolete.) |
| `<LISTING>` | Like `XMP`, for code listings, rendered in a fixed-width font. (Obsolete.) |
| `<PLAINTEXT>` | Everything after this tag is plain text to end of document; no closing tag. (Obsolete.) |

## Inline / other

| Tag | Definition |
|-----|------------|
| `<ADDRESS>` | Address / authorship information for the document. |
| `<HP1>`, `<HP2>`, … | "Highlighted phrase" tags (emphasis levels), may nest. Marked *"NOT CURRENTLY USED"* in 1991. |

## Shape of the language in 1991

~20 tags, no `<HTML>`/`<HEAD>`/`<BODY>` wrapper, no images (`<IMG>` came in HTML 2.0), no tables,
no forms, no ordered lists. A working note, not a frozen standard.

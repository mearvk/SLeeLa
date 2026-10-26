# Tag reference — elements and their definitions, per version

A per-version catalogue of HTML/XHTML **elements (tags)** with a one-line definition of each.
Every list is derived from the authoritative source **archived in this repository** — not from
memory — so each entry can be traced back to a file under [`../../specs/`](../../specs):

| Version | Reference | Element inventory derived from |
|---------|-----------|--------------------------------|
| HTML Tags (1991) | [`01-html-tags-1991.tags.md`](./01-html-tags-1991.tags.md) | the original note [`specs/html-tags-1991/Tags.html`](../../specs/html-tags-1991/Tags.html) |
| HTML 2.0 (1995) | [`02-html-2.0.tags.md`](./02-html-2.0.tags.md) | the DTD embedded in [`specs/rfc1866-html-2.0.txt`](../../specs/rfc1866-html-2.0.txt) |
| HTML 3.2 (1997) | [`03-html-3.2.tags.md`](./03-html-3.2.tags.md) | [`specs/dtd/html-3.2/HTML32.dtd`](../../specs/dtd/html-3.2/HTML32.dtd) |
| HTML 4.01 (1999) | [`04-html-4.01.tags.md`](./04-html-4.01.tags.md) | [`specs/dtd/html-4.01/`](../../specs/dtd/html-4.01/) (strict + loose) + prose |
| XHTML 1.0 (2000) | [`05-xhtml-1.0.tags.md`](./05-xhtml-1.0.tags.md) | [`specs/dtd/xhtml-1.0/`](../../specs/dtd/xhtml-1.0/) + [`prose`](../../specs/prose/xhtml-1.0/) |
| XHTML 1.1 (2010) | [`06-xhtml-1.1.tags.md`](./06-xhtml-1.1.tags.md) | [`specs/dtd/xhtml-1.1/`](../../specs/dtd/xhtml-1.1/) module set + prose |
| HTML Living Standard | [`10-html-living-standard.tags.md`](./10-html-living-standard.tags.md) | [`specs/whatwg-html-living-standard.source.html`](../../specs/whatwg-html-living-standard.source.html) |

> HTML5 (2014) / 5.1 / 5.2 are not given separate tag files: their element set is a subset of
> the Living Standard's, and this archive has no published-REC prose snapshot for them (only the
> 5.3 Editor's Draft Bikeshed source). The Living Standard reference is the authoritative tag
> list for the modern era; [`CHANGES.md`](./CHANGES.md) notes which elements entered or left at
> HTML5 and after.

## How to read these

- Elements are grouped by role (document structure, text, lists, tables, forms, …) to make the
  shape of each version legible, not just an alphabetical dump.
- Each row gives the **tag**, a **short definition**, and — where useful — a note on when it
  arrived or that it is **deprecated/obsolete**.
- Where a version's DTD declares elements only through parameter-entity groups (e.g. HTML 3.2's
  `%heading` = `H1..H6`, 4.01's `%phrase` = `EM STRONG DFN …`), those groups are expanded so the
  catalogue lists real tags.
- See [`CHANGES.md`](./CHANGES.md) for the cross-version added/removed/deprecated deltas.

## A note on scope and fidelity

These files are **descriptive write-ups** (like the rest of [`../`](../)), not verbatim spec
text. The *element names* are extracted mechanically from the archived DTDs / RFC / source; the
*definitions* summarise each element's role as given by its spec. Nothing is invented — if an
element's meaning is uncertain in a given version it is marked as such.

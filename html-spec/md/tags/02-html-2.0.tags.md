# HTML 2.0 (RFC 1866, 1995) — element reference

Source: the DTD embedded in [`specs/rfc1866-html-2.0.txt`](../../specs/rfc1866-html-2.0.txt).
HTML 2.0 was the first *formal* HTML specification (IETF). Element names below are taken from the
RFC's `<!ELEMENT>` declarations; the heading / font / phrase groups are expanded.

## Document structure

| Tag | Definition |
|-----|------------|
| `<HTML>` | Root element wrapping the whole document (start/end tags optional). |
| `<HEAD>` | Container for document metadata. |
| `<BODY>` | Container for the displayed content. |
| `<TITLE>` | Document title (required, in `HEAD`). |
| `<BASE>` | Base URI for resolving relative links. |
| `<LINK>` | A document-level relationship to another resource. |
| `<META>` | Generic metadata (name/value or HTTP-equivalent). |
| `<ISINDEX>` | Marks the document as searchable via a keyword query. |
| `<NEXTID>` | Legacy next-anchor-id hint (carried over from 1991; obsolescent). |

## Headings & paragraphs

| Tag | Definition |
|-----|------------|
| `<H1>`–`<H6>` | Six heading levels. |
| `<P>` | Paragraph. |
| `<BR>` | Forced line break. |
| `<HR>` | Horizontal rule / thematic divider. |
| `<ADDRESS>` | Authorship / contact information. |
| `<BLOCKQUOTE>` | A block of quoted material. |
| `<PRE>` | Preformatted text — whitespace preserved, monospaced. |

## Lists

| Tag | Definition |
|-----|------------|
| `<UL>` | Unordered (bulleted) list. |
| `<OL>` | **Ordered (numbered) list** — new in 2.0. |
| `<DIR>` | Directory list (short items). |
| `<MENU>` | Menu list (single-line items). |
| `<LI>` | List item (for `UL`/`OL`/`DIR`/`MENU`). |
| `<DL>` | Definition/glossary list. |
| `<DT>` | Definition term. |
| `<DD>` | Definition description. |

## Text-level (phrase & font)

| Tag | Definition |
|-----|------------|
| `<EM>` | Emphasis. |
| `<STRONG>` | Strong emphasis. |
| `<CODE>` | A fragment of computer code. |
| `<SAMP>` | Sample output from a program. |
| `<KBD>` | Text to be typed by the user. |
| `<VAR>` | A variable or program argument. |
| `<CITE>` | A citation / title of a work. |
| `<TT>` | Teletype / monospaced (presentational). |
| `<B>` | Bold (presentational). |
| `<I>` | Italic (presentational). |

## Hypertext & images

| Tag | Definition |
|-----|------------|
| `<A>` | Anchor — hyperlink (`HREF`) and/or link target (`NAME`). |
| `<IMG>` | **Embedded image** — new in 2.0 (with `SRC`, `ALT`). |

## Forms (new in 2.0)

| Tag | Definition |
|-----|------------|
| `<FORM>` | A form for user input, submitted to a server. |
| `<INPUT>` | A form control (text, checkbox, radio, submit, …). |
| `<SELECT>` | A drop-down / list selection control. |
| `<OPTION>` | An option within a `SELECT`. |
| `<TEXTAREA>` | A multi-line text input control. |

## Literal (legacy)

| Tag | Definition |
|-----|------------|
| `<XMP>` | Literal example text (obsolescent). |
| `<LISTING>` | Code listing (obsolescent). |
| `<PLAINTEXT>` | Plain text to end of document (obsolescent). |

## What changed from 1991

**Added:** the `<HTML>`/`<HEAD>`/`<BODY>` document skeleton, `<IMG>`, forms
(`FORM`/`INPUT`/`SELECT`/`OPTION`/`TEXTAREA`), ordered lists `<OL>`, `<META>`, `<LINK>`,
`<HR>`, `<BLOCKQUOTE>`, `<PRE>`, and the named phrase/font elements.
**Still absent:** tables, `<DIV>`/`<SPAN>`, `<FONT>`/`<CENTER>` presentational tags, scripting,
and frames — all arrive in 3.2 / 4.0.

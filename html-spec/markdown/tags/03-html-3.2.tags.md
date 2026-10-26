# HTML 3.2 "Wilbur" (1997) — element reference

Source: [`specifications/dtd/html-3.2/HTML32.dtd`](../../specifications/dtd/html-3.2/HTML32.dtd). First W3C
Recommendation. Element names below come from the DTD's `<!ELEMENT>` declarations; the
`%heading`, `%font` and `%phrase` parameter-entity groups are expanded. 3.2 consolidated the
*presentational* tags browsers had already shipped.

## Document structure

| Tag | Definition |
|-----|------------|
| `<HTML>` | Document root. |
| `<HEAD>` | Metadata container. |
| `<BODY>` | Content container (now takes `bgcolor`, `text`, `link` colour attributes). |
| `<TITLE>` | Document title. |
| `<BASE>` | Base URI. |
| `<LINK>` | Document-level link relationship. |
| `<META>` | Metadata. |
| `<STYLE>` | **Embedded style information** (CSS hook — new). |
| `<SCRIPT>` | **Script placeholder** reserved (new; no scripting model yet). |
| `<ISINDEX>` | Searchable-document marker. |

## Headings, blocks & dividers

| Tag | Definition |
|-----|------------|
| `<H1>`–`<H6>` | Six heading levels (with `align`). |
| `<P>` | Paragraph (with `align`). |
| `<DIV>` | **Generic block container** — new in 3.2, but its only attribute is `align` (no `lang`/`dir`/`style` yet). |
| `<CENTER>` | **Centre block/inline content** (presentational; = `DIV align=center`). |
| `<BLOCKQUOTE>` | Quoted block. |
| `<PRE>` | Preformatted text. |
| `<ADDRESS>` | Authorship info. |
| `<HR>` | Horizontal rule (with `size`, `width`, `align`, `noshade`). |
| `<BR>` | Line break (with `clear`). |

## Lists

| Tag | Definition |
|-----|------------|
| `<UL>` / `<OL>` | Unordered / ordered list (with `type`, `start`, `compact`). |
| `<DIR>` / `<MENU>` | Directory / menu lists. |
| `<LI>` | List item (with `type`, `value`). |
| `<DL>` / `<DT>` / `<DD>` | Definition list, term, description. |

## Text-level — phrase

| Tag | Definition |
|-----|------------|
| `<EM>` | Emphasis. |
| `<STRONG>` | Strong emphasis. |
| `<DFN>` | The defining instance of a term. |
| `<CODE>` | Computer code. |
| `<SAMP>` | Sample program output. |
| `<KBD>` | Keyboard input. |
| `<VAR>` | Variable. |
| `<CITE>` | Citation. |

## Text-level — font/presentational (new emphasis in 3.2)

| Tag | Definition |
|-----|------------|
| `<TT>` | Teletype / monospace. |
| `<I>` | Italic. |
| `<B>` | Bold. |
| `<U>` | Underline. |
| `<STRIKE>` | Strikethrough. |
| `<BIG>` | Larger text. |
| `<SMALL>` | Smaller text. |
| `<SUB>` | Subscript. |
| `<SUP>` | Superscript. |
| `<FONT>` | **Font face / size / colour** (presentational — the hallmark of the 3.2 era). |
| `<BASEFONT>` | Base font size for following text (presentational). |

## Images, imagemaps & applets

| Tag | Definition |
|-----|------------|
| `<IMG>` | Embedded image (with `align`, `border`, `hspace`, `vspace`, `usemap`). |
| `<MAP>` | A client-side image map. |
| `<AREA>` | A clickable region within a `MAP`. |
| `<APPLET>` | **Embedded Java applet** — new in 3.2. |
| `<PARAM>` | A named parameter for an `APPLET`. |

## Tables (new in 3.2 — flat model)

| Tag | Definition |
|-----|------------|
| `<TABLE>` | A table (with `border`, `cellpadding`, `cellspacing`, `width`, `align`). |
| `<CAPTION>` | Table caption. |
| `<TR>` | Table row. |
| `<TH>` | Header cell. |
| `<TD>` | Data cell. |

> 3.2 tables are **flat**: just `CAPTION?, TR+` with `TH`/`TD`. No `THEAD`/`TBODY`/`TFOOT`,
> no `COL`/`COLGROUP` — those arrive in 4.0.

## Forms

| Tag | Definition |
|-----|------------|
| `<FORM>` | Input form. |
| `<INPUT>` | Form control. |
| `<SELECT>` | Selection control. |
| `<OPTION>` | Option within `SELECT`. |
| `<TEXTAREA>` | Multi-line text control. |

## Legacy literal

| Tag | Definition |
|-----|------------|
| `<XMP>` / `<LISTING>` / `<PLAINTEXT>` | Literal/preformatted legacy tags (obsolescent). |

## What changed from 2.0

**Added:** `<DIV>`, `<CENTER>`, tables (`TABLE`/`TR`/`TH`/`TD`/`CAPTION`), image maps
(`MAP`/`AREA`), `<APPLET>`/`<PARAM>`, `<STYLE>`, `<SCRIPT>` (reserved), and a wave of
presentational text tags (`FONT`, `BASEFONT`, `U`, `STRIKE`, `BIG`, `SMALL`, `SUB`, `SUP`) plus
`<DFN>`. **No `<SPAN>` yet** (added in 4.0). The abandoned HTML 3.0 maths features were dropped.

# HTML 4.01 "Cougar" (1999) — element reference

Sources: [`specs/dtd/html-4.01/strict.dtd`](../../specs/dtd/html-4.01/strict.dtd),
[`loose.dtd`](../../specs/dtd/html-4.01/loose.dtd), [`frameset.dtd`](../../specs/dtd/html-4.01/frameset.dtd),
with definitions cross-checked against the archived prose under
[`specs/prose/html-4.01/`](../../specs/prose/html-4.01/). Parameter-entity groups
(`%heading`, `%fontstyle`, `%phrase`, `%special`, `%list`) are expanded.

HTML 4.01 has **three flavours**: **Strict** (modern, no presentation), **Transitional/Loose**
(Strict + deprecated presentational tags), and **Frameset** (Transitional + frames). Elements
below are grouped as **Strict core** and then the **Transitional-only** and **Frameset-only**
additions.

---

## Strict core

### Document structure

| Tag | Definition |
|-----|------------|
| `<HTML>` | Document root. |
| `<HEAD>` | Metadata container. |
| `<BODY>` | Content container. |
| `<TITLE>` | Document title. |
| `<BASE>` | Base URI for relative links. |
| `<LINK>` | Document-level link relationship (stylesheets, alternates, …). |
| `<META>` | Metadata (name/value, HTTP-equiv). |
| `<STYLE>` | Embedded CSS. |
| `<SCRIPT>` | Embedded or linked script. |
| `<NOSCRIPT>` | Fallback content when scripting is off. |

### Grouping & sections

| Tag | Definition |
|-----|------------|
| `<DIV>` | Generic **block** container — now carries the full `%attrs` incl. `lang`/`dir`/`style` (the turning point for i18n). |
| `<SPAN>` | Generic **inline** container — **new in 4.0**, the inline counterpart to `DIV`. |
| `<P>` | Paragraph. |
| `<H1>`–`<H6>` | Six heading levels. |
| `<HR>` | Thematic horizontal rule. |
| `<BR>` | Line break. |
| `<PRE>` | Preformatted text. |
| `<BLOCKQUOTE>` | Block quotation (with `cite`). |
| `<Q>` | **Inline quotation** — new. |
| `<ADDRESS>` | Authorship/contact info. |

### Text-level — phrase

| Tag | Definition |
|-----|------------|
| `<EM>` | Emphasis. |
| `<STRONG>` | Strong emphasis. |
| `<DFN>` | Defining instance of a term. |
| `<CODE>` | Computer code. |
| `<SAMP>` | Sample output. |
| `<KBD>` | Keyboard input. |
| `<VAR>` | Variable. |
| `<CITE>` | Citation. |
| `<ABBR>` | **Abbreviation** — new. |
| `<ACRONYM>` | **Acronym** — new. |

### Text-level — presentational (kept in Strict)

| Tag | Definition |
|-----|------------|
| `<TT>` | Monospace. |
| `<I>` | Italic. |
| `<B>` | Bold. |
| `<BIG>` | Larger text. |
| `<SMALL>` | Smaller text. |
| `<SUB>` | Subscript. |
| `<SUP>` | Superscript. |

### Edits & bidi

| Tag | Definition |
|-----|------------|
| `<INS>` | **Inserted text** (edit marking) — new. |
| `<DEL>` | **Deleted text** (edit marking) — new. |
| `<BDO>` | **Bidirectional override** of text direction — new. |

### Lists

| Tag | Definition |
|-----|------------|
| `<UL>` / `<OL>` | Unordered / ordered list. |
| `<LI>` | List item. |
| `<DL>` / `<DT>` / `<DD>` | Definition list, term, description. |

### Tables (full model — new in 4.0)

| Tag | Definition |
|-----|------------|
| `<TABLE>` | A table (with `summary` for accessibility). |
| `<CAPTION>` | Table caption. |
| `<THEAD>` | **Row group: header** — new. |
| `<TBODY>` | **Row group: body** — new. |
| `<TFOOT>` | **Row group: footer** — new. |
| `<COLGROUP>` | **A group of columns** — new. |
| `<COL>` | **Column properties** — new. |
| `<TR>` | Table row. |
| `<TH>` | Header cell (with `scope`, `headers`). |
| `<TD>` | Data cell (with `headers`, `axis`). |

### Forms (expanded in 4.0)

| Tag | Definition |
|-----|------------|
| `<FORM>` | Input form. |
| `<INPUT>` | Form control. |
| `<BUTTON>` | **A rich push button** — new. |
| `<SELECT>` | Selection control. |
| `<OPTGROUP>` | **A group of options** in a `SELECT` — new. |
| `<OPTION>` | An option. |
| `<TEXTAREA>` | Multi-line text control. |
| `<LABEL>` | **A label bound to a control** — new. |
| `<FIELDSET>` | **Groups related controls** — new. |
| `<LEGEND>` | **Caption for a `FIELDSET`** — new. |

### Embedded content & links

| Tag | Definition |
|-----|------------|
| `<A>` | Anchor (hyperlink / target). |
| `<IMG>` | Embedded image. |
| `<OBJECT>` | **Generic embedded object** (image, applet, media, nested doc) — new, the standards-based replacement for `APPLET`. |
| `<PARAM>` | A parameter for an `OBJECT`/`APPLET`. |
| `<MAP>` | Client-side image map. |
| `<AREA>` | A region within a `MAP`. |

---

## Transitional (Loose) only — deprecated presentational tags

These exist in **Transitional/Frameset** but were **removed from Strict** (prefer CSS):

| Tag | Definition | Status |
|-----|------------|--------|
| `<FONT>` | Font face/size/colour. | Deprecated |
| `<BASEFONT>` | Base font for following text. | Deprecated |
| `<CENTER>` | Centre content (= `DIV align=center`). | Deprecated |
| `<U>` | Underline. | Deprecated |
| `<S>` | Strikethrough. | Deprecated |
| `<STRIKE>` | Strikethrough (older form). | Deprecated |
| `<DIR>` | Directory list. | Deprecated |
| `<MENU>` | Menu list. | Deprecated |
| `<ISINDEX>` | Searchable-document prompt. | Deprecated |
| `<APPLET>` | Java applet (superseded by `OBJECT`). | Deprecated |
| `<IFRAME>` | **Inline frame** (embeds another document). | Transitional |
| `<NOFRAMES>` | Fallback content for non-frame UAs. | Transitional |

---

## Frameset only

The **Frameset** DTD replaces `BODY` with a frame layout:

| Tag | Definition |
|-----|------------|
| `<FRAMESET>` | A set of frames (rows/columns) replacing the `BODY`. |
| `<FRAME>` | A single frame referencing another document. |
| `<NOFRAMES>` | Content shown when frames are unsupported. |
| `<IFRAME>` | Inline frame (also available in Transitional). |

## What changed from 3.2

**Added:** `<SPAN>`, `<Q>`, `<ABBR>`, `<ACRONYM>`, `<INS>`/`<DEL>`, `<BDO>`, the full accessible
table model (`THEAD`/`TBODY`/`TFOOT`/`COL`/`COLGROUP` + `summary`/`scope`/`headers`), richer forms
(`BUTTON`, `LABEL`, `FIELDSET`, `LEGEND`, `OPTGROUP`), `<OBJECT>`, frames
(`FRAMESET`/`FRAME`/`IFRAME`/`NOFRAMES`), and the `%i18n` attributes (`lang`, `dir`) across the
board. **Deprecated:** `FONT`, `BASEFONT`, `CENTER`, `U`, `S`, `STRIKE`, `DIR`, `MENU`,
`ISINDEX`, `APPLET` — pushed out of Strict in favour of CSS.

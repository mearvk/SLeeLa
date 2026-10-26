# HTML Living Standard (WHATWG) — element reference

Source: [`specifications/whatwg-html-living-standard.source.html`](../../specifications/whatwg-html-living-standard.source.html).
Element names are extracted from the source's authoritative element-definition markers
(`<dfn element><code>…</code></dfn>`). This is the modern element set; it is also the
authoritative list for **HTML5 (2014) / 5.1 / 5.2**, whose elements are a subset of this.

Elements are grouped by the spec's own content sections. **Obsolete** elements the spec still
documents (for parser/legacy reasons) are listed separately at the end.

## Document structure & metadata

| Tag | Definition |
|-----|------------|
| `<html>` | Document root. |
| `<head>` | Metadata container. |
| `<body>` | Document content. |
| `<title>` | Document title. |
| `<base>` | Base URL/target for the document. |
| `<link>` | External resource / relationship link. |
| `<meta>` | Metadata (charset, name/value, http-equiv, viewport…). |
| `<style>` | Embedded CSS. |

## Sections (semantic structure — the HTML5 landmark set)

| Tag | Definition |
|-----|------------|
| `<article>` | A self-contained, independently distributable composition. |
| `<section>` | A thematic grouping of content. |
| `<nav>` | A block of navigation links. |
| `<aside>` | Content tangential to the surrounding content (sidebars). |
| `<header>` | Introductory content / group of navigational aids. |
| `<footer>` | Footer for its nearest sectioning content. |
| `<main>` | The dominant content of the document. |
| `<search>` | A set of form controls for a search/filter operation. |
| `<h1>`–`<h6>` | Section headings, six levels. |
| `<hgroup>` | A heading plus associated subheading/tagline. |
| `<address>` | Contact information for its section. |

## Grouping content

| Tag | Definition |
|-----|------------|
| `<p>` | Paragraph. |
| `<hr>` | Thematic break. |
| `<pre>` | Preformatted text. |
| `<blockquote>` | Block quotation. |
| `<ol>` / `<ul>` | Ordered / unordered list. |
| `<menu>` | A list of commands (toolbar/menu). |
| `<li>` | List item. |
| `<dl>` / `<dt>` / `<dd>` | Description list, term, description. |
| `<figure>` | Self-contained figure (image/code/etc.). |
| `<figcaption>` | Caption for a `figure`. |
| `<div>` | Generic block container — "element of last resort." |

## Text-level semantics

| Tag | Definition |
|-----|------------|
| `<a>` | Hyperlink. |
| `<em>` | Stress emphasis. |
| `<strong>` | Strong importance. |
| `<small>` | Side comments / fine print. |
| `<s>` | Content no longer accurate/relevant. |
| `<cite>` | Title of a work. |
| `<q>` | Inline quotation. |
| `<dfn>` | Defining instance of a term. |
| `<abbr>` | Abbreviation (with `title` expansion). |
| `<ruby>` / `<rt>` / `<rp>` | Ruby annotation, its text, and parenthesis fallback. |
| `<data>` | Content with a machine-readable `value`. |
| `<time>` | A date/time in machine-readable form (`datetime`). |
| `<code>` | Computer code. |
| `<var>` | A variable. |
| `<samp>` | Sample output. |
| `<kbd>` | User input. |
| `<sub>` / `<sup>` | Subscript / superscript. |
| `<i>` | Alternate-voice/mood text (idiomatic, technical…). |
| `<b>` | Stylistically offset text (keywords…). |
| `<u>` | Non-textual annotation (e.g. misspelling, proper name in CJK). |
| `<mark>` | Highlighted/marked text for reference. |
| `<bdi>` | Bidirectional **isolation** of a text run. |
| `<bdo>` | Bidirectional **override** of text direction. |
| `<span>` | Generic inline container. |
| `<br>` | Line break. |
| `<wbr>` | Line-break **opportunity**. |

## Edits

| Tag | Definition |
|-----|------------|
| `<ins>` | Inserted (added) text. |
| `<del>` | Deleted (removed) text. |

## Embedded content

| Tag | Definition |
|-----|------------|
| `<img>` | Image (with `srcset`/`sizes` responsive support). |
| `<picture>` | Container choosing among `source`/`img` for art direction/responsiveness. |
| `<source>` | A media/image source alternative for `picture`/`audio`/`video`. |
| `<iframe>` | Nested browsing context (embedded document). |
| `<embed>` | Integration point for external (plugin) content. |
| `<object>` | Generic external resource / embedded object. |
| `<param>` | Parameter for an `object`. |
| `<video>` | Video player. |
| `<audio>` | Audio player. |
| `<track>` | Text track (captions/subtitles) for media. |
| `<map>` | Client-side image map. |
| `<area>` | A region within an image `map`. |
| `<canvas>` | Scriptable bitmap drawing surface (2D/WebGL). |

> The spec also integrates **MathML** (`<math>`) and **SVG** (`<svg>`) as foreign content; they
> are not HTML elements proper but are allowed in the HTML syntax.

## Tabular data

| Tag | Definition |
|-----|------------|
| `<table>` | A table of data. |
| `<caption>` | Table caption. |
| `<colgroup>` / `<col>` | Column group / column properties. |
| `<thead>` / `<tbody>` / `<tfoot>` | Row groups: header / body / footer. |
| `<tr>` | Table row. |
| `<th>` | Header cell (`scope`, `headers`, `abbr`). |
| `<td>` | Data cell. |

## Forms

| Tag | Definition |
|-----|------------|
| `<form>` | A form. |
| `<label>` | A caption bound to a control. |
| `<input>` | A control — many `type`s (text, email, url, number, range, date, color, search…). |
| `<button>` | A button. |
| `<select>` | A list-box / drop-down control. |
| `<datalist>` | A list of predefined options for autocompletion. |
| `<optgroup>` | A group of options. |
| `<option>` | An option. |
| `<selectedcontent>` | Reflects the selected option's content (customisable `select`). |
| `<textarea>` | Multi-line text control. |
| `<output>` | The result of a calculation/user action. |
| `<progress>` | A progress bar. |
| `<meter>` | A scalar gauge within a known range. |
| `<fieldset>` | Groups related controls. |
| `<legend>` | Caption for a `fieldset`. |

## Interactive elements

| Tag | Definition |
|-----|------------|
| `<details>` | A disclosure widget (expand/collapse). |
| `<summary>` | The visible summary/heading of a `details`. |
| `<dialog>` | A dialog box / modal. |

## Scripting

| Tag | Definition |
|-----|------------|
| `<script>` | Embedded or external script. |
| `<noscript>` | Fallback when scripting is unavailable. |
| `<template>` | Inert content for scripted instantiation. |
| `<slot>` | A placeholder in a shadow tree (Web Components). |

## Obsolete / non-conforming (still parsed, documented in the spec)

The Living Standard documents these so parsers handle legacy pages, but they **must not** be used:

`<acronym>`, `<applet>`, `<basefont>`, `<bgsound>`, `<big>`, `<blink>`, `<center>`, `<dir>`,
`<font>`, `<frame>`, `<frameset>`, `<isindex>`, `<keygen>`, `<listing>`, `<marquee>`,
`<menuitem>`, `<multicol>`, `<nextid>`, `<nobr>`, `<noembed>`, `<noframes>`, `<param>` (legacy on
`applet`), `<plaintext>`, `<rb>`, `<rtc>`, `<spacer>`, `<strike>`, `<tt>`, `<xmp>`.

## What changed at HTML5 and after

**Added at HTML5 (2014):** the semantic sections (`article`, `section`, `nav`, `aside`,
`header`, `footer`, `main`, `figure`/`figcaption`), media (`audio`, `video`, `source`, `track`),
`canvas`, `embed`, form additions (`datalist`, `output`, `progress`, `meter` + many `input`
types), text-level (`mark`, `time`, `bdi`, `wbr`, `data`, `ruby`/`rt`/`rp`), interactive
(`details`/`summary`), and scripting (`template`). **Added since (Living Standard / 5.1+):**
`dialog`, `picture`, `slot`, `search`, `selectedcontent`. **Removed vs HTML4:** frames
(`frame`/`frameset`/`noframes`), and all the presentational tags (`font`, `center`, `big`,
`strike`, `tt`, `u`-as-underline meaning repurposed, `basefont`, `applet` → `object`/`embed`).

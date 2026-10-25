# Tag evolution across versions — added / removed / deprecated

A cross-version delta, derived from the per-version references in this directory (which are in
turn derived from the archived DTDs / RFC / source). Use it to see when an element entered the
language and when it was deprecated or dropped.

## Timeline of first appearance

| Era | Elements first appearing |
|-----|--------------------------|
| **1991 (HTML Tags)** | `title` `nextid` `base` `isindex` `p` `h1`–`h6` `a` `ul` `menu` `dir` `li` `dl` `dt` `dd` `xmp` `listing` `plaintext` `address` `hp1`/`hp2` |
| **2.0 (1995)** | `html` `head` `body` `link` `meta` `hr` `br` `blockquote` `pre` `ol` `img` `form` `input` `select` `option` `textarea` + named phrase/font (`em` `strong` `code` `samp` `kbd` `var` `cite` `tt` `b` `i`) |
| **3.2 (1997)** | `div` `center` `style` `script`(reserved) `font` `basefont` `u` `strike` `big` `small` `sub` `sup` `dfn` `applet` `param` `map` `area` `table` `caption` `tr` `th` `td` |
| **4.0 / 4.01 (1998–99)** | `span` `q` `abbr` `acronym` `ins` `del` `bdo` `object` `button` `label` `fieldset` `legend` `optgroup` `thead` `tbody` `tfoot` `col` `colgroup` `frameset` `frame` `iframe` `noframes` `noscript` |
| **XHTML 1.0 (2000)** | *(no new elements — HTML 4.01 vocabulary, XML syntax)* |
| **XHTML 1.1 (2001)** | `ruby` `rb` `rt` `rp` `rbc` `rtc` (Ruby Annotation module) |
| **HTML5 (2014)** | `article` `section` `nav` `aside` `header` `footer` `main`¹ `figure` `figcaption` `audio` `video` `source` `track` `canvas` `embed` `datalist` `output` `progress` `meter` `mark` `time` `bdi` `wbr` `data` `ruby`/`rt`/`rp`² `details` `summary` `template` `hgroup` |
| **After 5.0 (Living Standard / 5.1+)** | `dialog` `picture` `slot` `search` `selectedcontent` `menu`³ |

¹ `main` was added shortly after the 2014 REC. ² Ruby came to HTML5 from XHTML 1.1 (`rbc`/`rtc`
dropped; `rt`/`rp` kept, `rb` later re-added as obsolete). ³ `menu` existed in HTML 2.0 as a list
type, was deprecated in 4.01, then redefined in the Living Standard as a command/toolbar list.

## Deprecated / removed

| Element | Deprecated | Removed from conforming HTML | Replacement |
|---------|-----------|------------------------------|-------------|
| `font` | HTML 4.01 (Transitional) | HTML5 | CSS |
| `basefont` | HTML 4.01 | HTML5 | CSS |
| `center` | HTML 4.01 | HTML5 | CSS `text-align`/`margin` |
| `u` (underline meaning) | HTML 4.01 | re-purposed in HTML5 | CSS / `<u>` for annotation |
| `s` / `strike` | HTML 4.01 | `strike` removed; `<s>` kept (new meaning) | `<s>` / `<del>` |
| `dir` | HTML 4.01 | HTML5 | `<ul>` |
| `menu` (old list) | HTML 4.01 | redefined in HTML5 | `<ul>` / new `<menu>` |
| `isindex` | HTML 4.01 | HTML5 | `<input>` in a `<form>` |
| `applet` | HTML 4.01 | HTML5 | `<object>` / `<embed>` |
| `frame` / `frameset` / `noframes` | — | HTML5 | `<iframe>` / server-side composition |
| `acronym` | — | HTML5 | `<abbr>` |
| `big` | — | HTML5 | CSS `font-size` |
| `tt` | — | HTML5 | `<code>`/`<kbd>`/`<samp>` or CSS |
| `xmp` / `listing` / `plaintext` | HTML 3.2 era | HTML5 (parsed, non-conforming) | `<pre>` + `<code>` |
| `nextid` | obsolescent from the start | HTML5 | — |
| `keygen` | — | removed from Living Standard (was in HTML5) | Web Crypto |
| `bgsound` `blink` `marquee` `multicol` `nobr` `spacer` | never standard | documented obsolete | CSS |

## Stable core (present and valid from introduction through today)

These have been continuously valid since they appeared and remain conforming in the Living
Standard: `html` `head` `body` `title` `base` `meta` `link` `p` `h1`–`h6` `hr` `br` `blockquote`
`pre` `a` `img` `ol` `ul` `li` `dl` `dt` `dd` `em` `strong` `code` `samp` `kbd` `var` `cite`
`sub` `sup` `b` `i` `table` `caption` `tr` `th` `td` `form` `input` `select` `option` `textarea`
`div` (3.2+) `span` (4.0+) and the full table/`form` model from 4.01.

## Count of elements by version (approximate, conforming)

| Version | Elements | Notes |
|---------|---------:|-------|
| HTML Tags 1991 | ~20 | informal, no DTD |
| HTML 2.0 | ~49 | first formal spec |
| HTML 3.2 | ~70 | +tables, +presentational |
| HTML 4.01 Strict | 77 (DTD decls 55 + grouped) | +`span`/`q`/edits/full tables/forms |
| XHTML 1.0 Strict | 77 | = HTML 4.01, XML syntax |
| XHTML 1.1 | ~83 | XHTML 1.0 Strict + 6 Ruby |
| Living Standard | ~110 conforming (+~30 obsolete documented) | modern set |

# GROFF — our buddy for the SLeeLa man pages

# SLeeLa
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

**groff** (GNU *roff*) is the typesetting system we use to render and check the
SLeeLa manual pages — right now [`man/man1/sleela.1`](man/man1/sleela.1). This
document is the working reference for *how we use groff in this repo*: what it
is, the macros and escapes our pages rely on, how to render and lint them, and
the conventions we keep so a page stays portable and warning-free.

> Scope note: this is a practical, repo-facing guide, not a full groff manual.
> For the complete system see the upstream groff documentation and the
> `groff(1)`, `groff_man(7)`, and `man(7)` pages on a system that ships them.

---

## 1. What groff is

groff is the GNU implementation of the classic Unix **roff** text-processing
pipeline. You write a plain-text source sprinkled with **requests** (lines
starting with `.`) and **escapes** (sequences starting with `\`); groff formats
it into a device output — a terminal page, PostScript, PDF, or HTML.

The relevant pieces of the pipeline:

| Program | Role |
|---------|------|
| `groff`  | the front-end driver; runs preprocessors + `troff` + an output device |
| `troff`  | the core formatter (typesetter) |
| `nroff`  | a wrapper that formats for terminals/fixed-width devices |
| `tbl`    | preprocessor for tables |
| `eqn`    | preprocessor for equations |
| `pic`    | preprocessor for diagrams |
| `col`    | post-filter that cleans reverse line feeds for plain text |

A **macro package** sits on top of raw troff requests. Man pages use the **`man`**
macro package (`-man`, a.k.a. `groff_man`): `.TH`, `.SH`, `.TP`, `.B`, etc.

### In this sandbox

```
GNU groff version 1.22.4
present: groff, troff, nroff, tbl, eqn, pic, col
absent:  man, mandb        <- so we validate with groff directly, not `man`
```

Because `man`/`mandb` are not installed here, we **render and lint man pages
with `groff` directly** (see §4). On a developer machine with `man`, the page
also works the normal way (`man sleela`).

---

## 2. The man-page macros we actually use

Our `sleela.1` uses the standard `man` macros. The ones in play:

| Macro | Meaning |
|-------|---------|
| `.TH name section date source manual` | title header (page name, section, etc.) |
| `.SH title` | section heading (NAME, SYNOPSIS, DESCRIPTION, …) |
| `.SS title` | subsection heading |
| `.TP` | tagged paragraph — a term on one line, its definition indented below |
| `.B` / `.I` / `.R` | bold / italic / roman (whole following line) |
| `.BR` / `.IR` / `.RB` … | alternating fonts (e.g. `.BR cmd (1)` → **cmd**(1)) |
| `.RS` / `.RE` | start / end a relative indent (must be balanced) |
| `.EX` / `.EE` | begin / end an example block (monospace, no fill) |
| `.PP` | paragraph break |
| `.\"` | a comment (ignored by the formatter) |

### Inline font escapes

Inside a line we switch fonts with `\f`:

- `\fB` bold, `\fI` italic, `\fR` roman, and `\fP` "previous font".
- Example: `\fIfile.sleela\fR` prints *file.sleela* then returns to roman.

### Character escapes and a portability rule we keep

- `\-` is a real **ASCII hyphen/minus** (use it for option dashes like
  `\-\-mm`, so the glyph is copy-pasteable). A bare `-` can render as a typographic
  hyphen.
- `\(en` is an en dash (we use it for ranges like `1.0 \(en 2.2`).
- **Avoid device-specific special glyphs in the ASCII path.** We hit this with
  the trademark sign: `\(tm` warns `can't find special character 'tm'` under
  `-Tascii`. Our rule: **write `(TM)` literally** instead of `\(tm` so the page
  is clean on every device. (This is exactly the fix applied to `sleela.1`.)

---

## 3. Anatomy of our `sleela.1`

```
.\" comment: who owns this page and how to rebuild the binary
.TH SLEELA 1 "2026-10-24" "SLeeLa 0.3.30-dev" "SLeeLa Manual"
.SH NAME
sleela \- SLeeLa compiler and runtime front end (Sleelvac(TM))
.SH SYNOPSIS
.B sleela
.RI [ global-options ]
.I command
.RI [ args ...]
.SH DESCRIPTION
...
.SH COMMANDS
.SS Source and artifact commands
.TP
.B sleela check \fIfile.sleela\fR
Parse, resolve referenced library units, and check semantics.
...
.SH EXAMPLES
.RS
.EX
sleela check lib/philosophers/philosophers-demo.sleela
.EE
.RE
.SH SEE ALSO
.BR nordshrift (1),
.BR sleela-syntax (7)
```

Section order follows the man-page convention: **NAME, SYNOPSIS, DESCRIPTION,
OPTIONS/COMMANDS, ENVIRONMENT, EXIT STATUS, EXAMPLES, FILES, SEE ALSO, AUTHORS,
BUGS**. The page content is kept **in lockstep with the binary's own usage**
(`impl/frontend/driver.cpp`); the binary is the authority.

---

## 4. Render and lint (how we check a page)

### Plain-text preview (what a terminal reader sees)

```sh
groff -man -Tascii man/man1/sleela.1 | col -b | less
# UTF-8 output (keeps en dashes etc.):
groff -man -Tutf8  man/man1/sleela.1 | less -R
```

`-man` selects the man macro package; `-T<device>` picks the output
(`ascii`, `utf8`, `ps`, `pdf`, `html`). `col -b` strips backspace overstrike so
bold/underline don't leave artifacts in a captured file.

### Lint — treat warnings as defects

This is the command we use as a gate; **clean output (no stderr) = good**:

```sh
groff -man -Tascii -ww -z man/man1/sleela.1
# -ww : all warnings; -z : parse/format but suppress the typeset output
```

A convenient repo one-liner (prints nothing when the page is clean):

```sh
groff -man -Tascii man/man1/sleela.1 2>&1 >/dev/null
```

### Common warnings and what they mean

| Warning | Cause | Fix |
|---------|-------|-----|
| `can't find special character 'tm'` | `\(tm` under an ASCII device | write `(TM)` literally |
| `can't break line` | a `.TP` tag / `.B` line too long to wrap | shorten the tag; move detail into the body |
| `.RS` / `.RE` imbalance | an indent opened and not closed | balance every `.RS` with an `.RE` |
| tables look wrong | missing `tbl` preprocessor | add `'\" t` as the first line, or run `groff -t` |

If you use tables/equations/diagrams, enable the preprocessor: `groff -t` (tbl),
`-e` (eqn), `-p` (pic), or let `groff` auto-run them from a leading
`'\" tep` comment on line 1.

### PostScript / PDF (for a printable page)

```sh
groff -man -Tps  man/man1/sleela.1 > sleela.1.ps
groff -man -Tpdf man/man1/sleela.1 > sleela.1.pdf   # groff 1.22.3+
```

> These need the device description files (`DESC`) for `ps`/`pdf` installed with
> groff. On a minimal install they may be missing — `groff -Tpdf` then fails
> with `invalid device 'pdf'` (as in this sandbox). If so, render PostScript and
> convert, or just stick to the `ascii`/`utf8` previews above:
>
> ```sh
> groff -man -Tps man/man1/sleela.1 | ps2pdf - sleela.1.pdf   # needs ghostscript
> ```

---

## 5. Installing the page (so `man sleela` works)

On a machine that ships `man`/`mandb` (not this sandbox):

```sh
# user-local
install -Dm644 man/man1/sleela.1 "${XDG_DATA_HOME:-$HOME/.local/share}/man/man1/sleela.1"
mandb "${XDG_DATA_HOME:-$HOME/.local/share}/man" 2>/dev/null || true
man sleela

# system-wide
sudo install -Dm644 man/man1/sleela.1 /usr/local/share/man/man1/sleela.1
sudo mandb 2>/dev/null || true
```

`man` applies its own macro package and preprocessors automatically; our only
job is to keep the source clean under `groff -man` (§4).

---

## 6. Our conventions (keep a page portable and clean)

- **One logical thing per line of markup.** Requests start at column 1 with `.`.
- **Dashes:** `\-` for literal option dashes; `\(en` for numeric ranges.
- **No ASCII-hostile special glyphs:** write `(TM)` not `\(tm`; prefer plain
  text over `\(aq`/`\(oq` fancy quotes in the ASCII path.
- **Balance `.RS`/`.RE` and `.EX`/`.EE`.**
- **Keep `.TP` tags short** so lines wrap; push detail into the paragraph body.
- **Section order** per the man convention (§3).
- **Source is the authority:** the page mirrors the `sleela` binary's usage; when
  they disagree, fix the page to match the binary.
- **Gate on warnings:** a page must render clean under
  `groff -man -Tascii … 2>&1 >/dev/null` before it is committed.

---

## 7. Why groff (and not Markdown) for the man page

Markdown is our prose format (READMEs, tutorials, `PHILOSOPHERS.*.md`, this
file). The **man page** is different: it is the artifact a user reaches through
`man sleela` at a terminal, and that channel speaks roff. groff lets us author
once in the canonical man format and render it everywhere — terminal, PostScript,
PDF, HTML — while linting for portability. So groff is the buddy that turns our
command-line truth into the page a user actually pulls up.

---

## 8. Quick reference

```sh
# preview (terminal)
groff -man -Tutf8 man/man1/sleela.1 | less -R

# lint (must be silent)
groff -man -Tascii man/man1/sleela.1 2>&1 >/dev/null

# all-warnings, no output
groff -man -Tascii -ww -z man/man1/sleela.1

# printable
groff -man -Tpdf man/man1/sleela.1 > sleela.1.pdf
```

See also: [`man/README.md`](man/README.md) and the page itself,
[`man/man1/sleela.1`](man/man1/sleela.1).

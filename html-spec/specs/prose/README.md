# Human-readable Recommendation prose

This directory holds the **human-readable prose** of W3C Recommendations — the part that
explains element semantics, not just the machine-readable grammar. It complements
[`../dtd/`](../dtd), which holds the normative DTD grammars.

Until now the archive had only the DTDs for HTML 4.x / XHTML (see
[`../../STATE.md`](../../STATE.md) gap #5: *"Prose missing for 3.2 / 4.x / XHTML"*). That gap
was blocked solely because `w3.org/TR` is unreachable from this `INTEGRATIONS_ONLY`
environment. These two prose snapshots were recovered through the one working egress — the
GitHub API — from repositories that mirror the W3C `/TR` trees.

| Spec | Location | Form | Completeness |
|------|----------|------|--------------|
| **HTML 4.01** (W3C REC, 24 Dec 1999) | [`html-4.01/`](./html-4.01) | multi-page chapter tree (`cover.html` → chapters) | ✅ **complete** — every canonical chapter + figures |
| **XHTML 1.0 2E** (W3C REC, 2000 rev. 2002) | [`xhtml-1.0/Overview.html`](./xhtml-1.0/Overview.html) | single-page, full | ✅ **complete** (55 sections, ends cleanly) |
| **XHTML 1.1 2E** (W3C REC, 23 Nov 2010) | [`xhtml-1.1/`](./xhtml-1.1) | single-page `xhtml11.html` + chapter files | ✅ **complete** prose (DTD/schema already under `../dtd`) |

## HTML 4.01 — complete

The mirror (`learnwebcoding/lwc`, folder `html401_Dec99`) carries the authentic published W3C
Recommendation — the files bear the genuine `$Id: cover.src,v 1.117 1999/12/24 ... ijacobs`
RCS stamp and the "W3C Recommendation 24 December 1999" banner. **Every canonical chapter is
present**, including the ones a first (partial) mirror lacked:

- `about.html`, `cover.html`, `conform.html`, `charset.html`, `types.html`, `references.html`
- `intro/` — `intro`, `sgmltut` (the SGML tutorial)
- `struct/` — `global`, `text`, **`lists`**, `links`, `objects`, `tables`, `dirlang`
- `present/` — `frames`, `graphics`, `styles`
- `interact/` — **`forms`**, `scripts`
- `sgml/` — the SGML-reference appendix (`intro`, `dtd`, `loosedtd`, `framesetdtd`, `entities`, `sgmldecl`)
- `appendix/` — **`changes`**, `notes`; `index/` — `elements`, `attributes`, `list`
- `images/` — the figures the prose references (table/cell diagrams, form examples, color swatches)
- `style/default.css`

The chapters the [structural-elements appraisal](../../APPRAISAL-structural-elements.md) cites —
`struct/tables.html`, `struct/dirlang.html`, `struct/global.html`, `present/styles.html` — are
all here, so every prose citation in that appraisal can be checked against the source.

The DTD/entity/catalog files bundled in the mirror alongside the prose were **removed** from this
copy: the archive already holds authoritative DTDs under [`../dtd/html-4.01/`](../dtd/html-4.01),
and keeping a second copy with different provenance would be confusing.

## XHTML 1.1 — complete prose (grammar already present)

The archive already had the XHTML 1.1 **grammar** (driver DTD + the 47-file Modularization set,
self-resolving offline — see [`../dtd/xhtml-1.1/`](../dtd/xhtml-1.1)). What was missing was the
**prose**, now added from the same `learnwebcoding/lwc` mirror (folder `xhtml11_SecondEd_Nov10`):
`xhtml11.html` (the full single-page REC, 121 KB), plus `Overview.html` (TOC), `introduction`,
`conformance`, `doctype`, `changes`, `references`, `acknowledgements`, and the `xhtml11_dtd` /
`xhtml11_schema` reference chapters. Header: *"XHTML™ 1.1 - Module-based XHTML - Second Edition,
W3C Recommendation 23 November 2010."* The DTD/schema binary trees were **not** re-copied here
(the grammar lives under `../dtd/xhtml-1.1/`); the `.pdf`/`.ps` renderings were also skipped.

## Provenance

| Artifact | Repo | Commit | Notes |
|----------|------|--------|-------|
| `html-4.01/**` | `learnwebcoding/lwc` | `f8a8bdc0a741f1f4cf93298ef1836fc96e9d4349` | `html/zz_html_w3c_html_recommendations/html401_Dec99/`; DTD/ent/cat duplicates stripped. (Supersedes an earlier partial mirror from `logesh-works/JavaScript-Tutorial`.) |
| `xhtml-1.0/Overview.html` | `manzamb/SemanticWebinAction` | `eaa271eb0932cbcb648fcd4ee73632087af54e8f` | the `xhtml1-20020801/Overview.html` single-page REC; 71428 bytes |
| `xhtml-1.1/**` | `learnwebcoding/lwc` | `f8a8bdc0a741f1f4cf93298ef1836fc96e9d4349` | `.../xhtml11_SecondEd_Nov10/`; prose only (DTD/SCHEMA/pdf/ps skipped) |

Canonical upstream (all blocked here): <https://www.w3.org/TR/html401/>,
<https://www.w3.org/TR/xhtml1/>, <https://www.w3.org/TR/xhtml11/>. Nothing was edited or
fabricated; files are byte-for-byte as mirrored. See [`../PROVENANCE.txt`](../PROVENANCE.txt)
for the full record.

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
| **HTML 4.01** (W3C REC, 1999) | [`html-4.01/`](./html-4.01) | multi-page chapter tree (`cover.html` → chapters) | **partial** — core chapters present (see note) |
| **XHTML 1.0 2E** (W3C REC, 2000 rev. 2002) | [`xhtml-1.0/Overview.html`](./xhtml-1.0/Overview.html) | single-page, full | **complete** (55 sections, ends cleanly) |

## HTML 4.01 — what is and isn't here

The mirror (`logesh-works/JavaScript-Tutorial` at `TR/html401/`) carries the authentic W3C
prose — the files bear the genuine `$Id: Overview.html,v 1.2 2018/03/27 ... ijacobs`
RCS stamp and "Changed by: Ian B. Jacobs" editorial markers. It includes:

- `cover.html`, `conform.html`, `charset.html`, `types.html`, `references.html`
- `struct/` — `global`, `text`, `links`, `objects`, **`tables`**, **`dirlang`**
- `present/` — `frames`, `graphics`, `styles`
- `interact/` — `scripts`
- `sgml/` — the SGML-reference appendix chapters (`intro`, `dtd`, `loosedtd`, `framesetdtd`, `entities`)
- `index/` — `elements`, `attributes`, `list`; `appendix/notes.html`; `style/default.css`

It is **partial** — these canonical chapters were **not** in the mirror and are still absent:
`about.html`, `intro/` (`intro.html`, `sgmltut.html`), `struct/lists.html`,
`interact/forms.html`, and `appendix/changes.html`. The complete REC lives at
<https://www.w3.org/TR/html401/> (blocked here). Importantly, the chapters that the
[structural-elements appraisal](../../APPRAISAL-structural-elements.md) relies on —
`struct/tables.html`, `struct/dirlang.html`, `struct/global.html`, `present/styles.html` — **are**
present, so the appraisal's prose citations can now be checked against the source.

The DTD/entity files bundled in that mirror alongside the prose were **removed** from this copy:
the archive already holds authoritative DTDs under [`../dtd/html-4.01/`](../dtd/html-4.01), and
keeping a second copy with different provenance would be confusing.

## Provenance

| Artifact | Repo | Commit | Notes |
|----------|------|--------|-------|
| `html-4.01/**` | `logesh-works/JavaScript-Tutorial` | `735f03cc0a9441bc5e5b28d8a0d7145ee1450ef8` | `TR/html401/` subtree; DTD/ent duplicates stripped |
| `xhtml-1.0/Overview.html` | `manzamb/SemanticWebinAction` | `eaa271eb0932cbcb648fcd4ee73632087af54e8f` | the `xhtml1-20020801/Overview.html` single-page REC; 71428 bytes |

Canonical upstream (blocked here): <https://www.w3.org/TR/html401/> and
<https://www.w3.org/TR/xhtml1/>. Nothing was edited or fabricated; files are byte-for-byte as
mirrored. See [`../PROVENANCE.txt`](../PROVENANCE.txt) for the full record.

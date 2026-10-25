# State of the HTML Spec Archive — Coherence Report

_Assessment date: 2026-10-25. This report audits what is actually in [`specs/`](./specs),
judges whether each artifact is coherent and complete, and names concretely what is wrong._

## Verdict in one line

**Original assessment:** the archive was accurate and honestly labeled, but _not_ "perfect"
or fully self-contained — one file was mislabeled, several DTDs weren't standalone, and two of
the ten versions have no original source.

> **UPDATE (fixes applied — see the "Fixes applied" section at the bottom):** issues #1, #2,
> and #3 are now **resolved** — the mislabeled file was renamed, the XHTML 1.1 grammar is
> complete and validates offline, and the whole DTD bundle is self-resolving via SGML + XML
> catalogs. The remaining gaps (#4 prose, #5 the 1991 note / 2014 REC text) are **blocked**
> by the network sandbox, not by anything fixable here. The per-spec notes below are the
> original audit; the final section records what changed.

---

## Per-spec state

### 1. HTML Tags (1991) — ❌ no source
- **State:** Write-up only. No original document archived.
- **Problem:** The 1991 CERN note is not hosted in any reachable GitHub repo, and w3.org is
  network-blocked from the build environment. Nothing was fabricated.
- **Coherent?** The *write-up* is coherent; the *archive entry* is incomplete by necessity.

### 2. HTML 2.0 — ✅ coherent & complete
- **File:** `specs/rfc1866-html-2.0.txt`
- **State:** The **full verbatim RFC 1866 text**. Verified it ends cleanly on `[Page 77]` of
  77 — no truncation. This is the single most complete original in the archive.
- **Problem:** None of substance. (It is fixed-width ASCII with form-feed page breaks and
  running headers/footers, i.e. the raw RFC presentation, not reflowed prose — that is the
  authentic artifact, not a defect.)
- **Coherent?** Yes. Gold standard.

### 3. HTML 3.2 — ⚠️ partial (grammar only)
- **File:** `specs/dtd/html-3.2/HTML32.dtd` (45 `<!ELEMENT>`, 44 `<!ENTITY>`, 35 `<!ATTLIST>`)
- **State:** The normative DTD, intact. **Mostly self-contained** (only 2 external references,
  for the ISO Latin-1 character entities).
- **Problem:** This is the **machine-readable grammar only** — the human-readable prose of the
  Recommendation (the part that explains semantics) is **not** archived.
- **Coherent?** Internally coherent; incomplete as "the spec."

### 4. HTML 4.0 / 4.01 — ⚠️ partial, and NOT standalone
- **Files:** `specs/dtd/html-4.0/{strict,loose,frameset}.dtd`,
  `specs/dtd/html-4.01/{strict,loose,frameset}.dtd` + `HTML{lat1,special,symbol}.ent`
- **State:** All three flavors for both versions, plus the three 4.01 entity sets. The
  strict/loose DTDs are substantial (55 / 65 elements).
- **Problems:**
  1. **`frameset.dtd` is a 1 KB stub, not a full grammar** (0 `<!ELEMENT>`). It works by
     pulling in the Transitional DTD via the SGML *public identifier*
     `-//W3C//DTD HTML 4.01 Transitional//EN` — it does **not** reference a local file. So a
     plain SGML parser pointed at `frameset.dtd` **cannot resolve it** without an SGML catalog
     mapping that public ID to the local `loose.dtd`. (This is normal for the W3C DTD, but it
     means the folder is not "drop-in parseable" as-is.)
  2. **Prose not archived** — same gap as 3.2.
- **Coherent?** The files are authentic and internally consistent, but the set is **not a
  self-resolving bundle**.

### 5. XHTML 1.0 — ⚠️ partial, external entity refs unresolved
- **Files:** `specs/dtd/xhtml-1.0/xhtml1-{strict,transitional,frameset}.dtd`
- **State:** All three DTDs, full size (strict = 77 elements).
- **Problems:**
  1. Each DTD declares the Latin-1 / symbol / special **entity sets via external identifiers**
     (6 external refs in strict). Those `.ent` files are **not bundled under `xhtml-1.0/`**
     (we only have copies under `html-4.01/`), so again this is **not self-resolving** without
     a catalog.
  2. **Prose not archived.**
- **Coherent?** Authentic, but depends on files not co-located.

### 6. XHTML 1.1 — ❌ grammar present but effectively unusable standalone
- **Files:** `specs/dtd/xhtml-1.1/xhtml11.dtd` + `xhtml11-model-1.mod`
- **State:** `xhtml11.dtd` is the **driver DTD** — it has **0 `<!ELEMENT>` declarations of its
  own**. The entire vocabulary lives in **26 external modules** (`xhtml-struct.mod`,
  `xhtml-text.mod`, `xhtml-form.mod`, `xhtml-table.mod`, `xhtml-ruby.mod`, … ).
- **Problem (the biggest gap in the DTD set):** We bundled only **2 of the ~27 files** this DTD
  needs (`xhtml11.dtd` + `xhtml11-model-1.mod`). The other **~25 modules are missing**, so the
  XHTML 1.1 grammar here **cannot actually be assembled or validated against**. It documents
  the modular *structure* but is not a working grammar.
- **Coherent?** No — it is a dangling reference to files that aren't present.

### 7. HTML5 (2014) — ⚠️ no dedicated source
- **State:** No file specifically for the 2014 Recommendation. Covered indirectly by the W3C
  5.x Bikeshed source (see item 9) and the WHATWG source (item 10).
- **Problem:** There is no snapshot of the actual 2014 REC text.
- **Coherent?** Write-up is fine; original not archived.

### 8. HTML 5.1 (2016) — ⚠️ no dedicated source
- **State:** Same as HTML5 — no 5.1-specific file; only the single evolving Bikeshed source.
- **Problem:** The README/write-ups imply "5.1 source ✅", but there is **no 5.1 snapshot** —
  only the one `w3c-html5x` file, which (see item 9) isn't even 5.1 or 5.2.

### 9. HTML 5.2 (2017) — ❗ **MISLABELED FILE** (most important issue)
- **File:** `specs/w3c-html52-single-page.bs`
- **State:** It **is** the authentic W3C HTML WG Bikeshed source — but its metadata header says:
  `Title: HTML 5.3`, `Level: 5.3`, `Status: ED` (Editor's Draft), `TR: .../TR/html53/`.
- **Problem:** **The filename claims 5.2, but the content is the HTML 5.3 _Editor's Draft_**
  (the never-finalized next-version working copy), **not** the published HTML 5.2 Recommendation.
  So the "5.2 source" and "5.1 source" claims in the README are **inaccurate** — this one file
  is a 5.3-ED and does not faithfully represent 5.0, 5.1, or 5.2 as published.
- **Coherent?** The file is genuine, but it is **misfiled and mis-described.** Needs renaming to
  something like `w3c-html53-ED-single-page.bs` and the README corrected.

### 10. HTML Living Standard — ✅ coherent & complete
- **File:** `specs/whatwg-html-living-standard.source.html` (8.0 MB, 164,839 lines)
- **State:** The complete `source` file. Verified it ends cleanly with `</body></html>` — no
  truncation. The authentic single-source-of-truth for the current standard.
- **Problem:** It is the **source** (Wattsi/annotated input), not the rendered multipage spec —
  expected and fine, but worth noting it needs the WHATWG build pipeline to render.
- **Coherent?** Yes.

---

## Summary table

| # | Spec | Artifact | Complete? | Self-contained? | Correctly labeled? |
|---|------|----------|-----------|-----------------|--------------------|
| 1 | HTML Tags 1991 | — | ❌ none | — | — |
| 2 | HTML 2.0 | RFC text | ✅ full | ✅ | ✅ |
| 3 | HTML 3.2 | DTD | ⚠️ grammar only | ✅ (≈) | ✅ |
| 4 | HTML 4.0/4.01 | DTDs + ents | ⚠️ grammar only | ❌ needs catalog | ✅ |
| 5 | XHTML 1.0 | DTDs | ⚠️ grammar only | ❌ ext. entities | ✅ |
| 6 | XHTML 1.1 | driver DTD | ❌ ~25 modules missing | ❌ | ✅ |
| 7 | HTML5 2014 | — | ❌ none | — | — |
| 8 | HTML 5.1 | (shared file) | ❌ none | — | ❌ overclaimed |
| 9 | HTML 5.2 | Bikeshed | ⚠️ wrong version | n/a | ❗ **mislabeled (is 5.3-ED)** |
| 10 | Living Standard | source | ✅ full | ✅ | ✅ |

## What is "wrong" — concise list

1. **Mislabeled 5.2 file** — it is actually the **HTML 5.3 Editor's Draft**, not 5.2. (fixable)
2. **README overclaims 5.1 / 5.2 sources** — both point at that single 5.3-ED file. (fixable)
3. **XHTML 1.1 is only 2 of ~27 files** — the grammar can't be assembled. (fetchable from the
   same `w3c/markup-validator` repo)
4. **HTML 4.01 frameset + XHTML 1.0 DTDs aren't self-resolving** — they reference external
   public IDs / entity files not co-located; no SGML catalog is provided. (fixable by copying
   the `.ent` files and adding a catalog)
5. **Prose missing for 3.2 / 4.x / XHTML** — only machine-readable DTDs are present.
   (blocked: prose lives at w3.org, unreachable here)
6. **No original for HTML Tags 1991 and the 2014 HTML5 REC.** (1991 unreachable anywhere; 2014
   REC text is at w3.org)

## What is genuinely good

- **HTML 2.0 and the Living Standard are complete and verified** (clean start and end, no
  truncation).
- Every DTD retrieved is **authentic upstream content** with correct W3C headers.
- Provenance is **fully recorded** (repos + commit SHAs in `PROVENANCE.txt`); nothing fabricated.

## Recommended fixes (in priority order)

1. Rename `w3c-html52-single-page.bs` → `w3c-html53-ED-single-page.bs` and correct the README
   (relabel items 7–9 as "W3C 5.x Bikeshed source = 5.3 Editor's Draft; 5.0/5.1/5.2 published
   RECs not snapshotted").
2. Fetch the missing ~25 XHTML modular `.mod` files from `w3c/markup-validator` so XHTML 1.1 is
   assemblable.
3. Co-locate the `.ent` entity files under `xhtml-1.0/` (and `html-4.0/`) and add an SGML
   `catalog` so the DTDs resolve standalone.
4. (Open-network only) Add the human-readable prose RECs and the 1991 Tags note.

---

## Fixes applied (2026-10-25)

Fixes #1–#3 from the list above have been carried out and verified:

### ✅ #1 — Mislabeled file renamed
`w3c-html52-single-page.bs` → **`w3c-html53-ED-single-page.bs`**. The content is the W3C
**HTML 5.3 Editor's Draft** (its own metadata says `Title: HTML 5.3`, `Status: ED`), now named
accordingly. The README no longer claims it is a 5.1/5.2 snapshot.

### ✅ #2 — XHTML 1.1 grammar completed
Added the full **XHTML Modularization 1.1 library (47 files)** under
`specs/dtd/xhtml-1.1/modules/`. `xhtml11.dtd` now resolves its entire ~26-module closure.
**Verified:** a sample XHTML 1.1 document validates with `xmllint` offline, 0 unresolved refs.

### ✅ #3 — DTD bundle is now self-resolving offline
- Added **`specs/dtd/catalog`** (SGML Open) and **`specs/dtd/catalog.xml`** (XML catalog)
  mapping every PUBLIC id and the external `w3.org` SYSTEM URLs to local files.
- Co-located the needed entity files: `html-3.2/ISOlat1.ent`, `html-4.0/HTML*.ent`,
  `xhtml-1.0/xhtml-*.ent`, and `xhtml-1.1/xhtml-datatypes-1.mod`.
- **Verified offline** with `xmllint --valid --catalogs`:

  | Flavor | Result |
  |--------|--------|
  | XHTML 1.1 (26-module closure) | ✅ VALID |
  | XHTML 1.0 Strict | ✅ VALID |
  | XHTML 1.0 Transitional | ✅ VALID |
  | XHTML 1.0 Frameset | ✅ VALID |

  (HTML 3.2/4.x are SGML, not XML; no `onsgmls` is installed here to live-test them, but all
  their referenced entity files are now present locally and catalog-mapped.)

### Still blocked (not fixable from this sandbox)
- **#4** — Human-readable **prose** of the 3.2 / 4.x / XHTML RECs (lives at the blocked w3.org).
- **#5** — The **1991 HTML Tags** note (in no reachable repo) and the **2014 HTML5 REC** text.

### Updated scorecard

| # | Spec | Was | Now |
|---|------|-----|-----|
| 6 | XHTML 1.1 | ❌ ~25 modules missing | ✅ complete + validates offline |
| 4 | HTML 4.0/4.01 | ❌ needs catalog | ✅ entities co-located + catalogs |
| 5 | XHTML 1.0 | ❌ ext. entities | ✅ entities co-located + validates |
| 9 | HTML 5.2 "file" | ❗ mislabeled | ✅ renamed to 5.3-ED, honestly labeled |
| 3 | HTML 3.2 | ⚠️ grammar only | ✅ self-resolving (prose still absent) |

---

## Fixes applied (2026-10-25, second session) — originals recovered via GitHub mirrors

The earlier audit assumed gaps #5 and most of #4 were permanently out of reach because
`w3.org`/`ietf.org` are network-blocked. This pass **re-verified the block is real** (shell
`curl` _and_ the agent web-fetch path both still get `403` from w3.org, rfc-editor.org, and
ietf.org) — and then recovered three of the missing **originals** anyway, through the one
egress that has always worked here: the **GitHub API**, pulling from repositories that mirror
the `w3.org` trees byte-for-byte. Same methodology and the same no-fabrication bar as every
earlier pass. Full record in [`specs/PROVENANCE.txt`](./specs/PROVENANCE.txt).

### ✅ #5a — The 1991 "HTML Tags" note is now archived — the oldest gap is **closed**
`specs/html-tags-1991/Tags.html` (9568 bytes, sha256 `96abc473…b264e`) — the first-ever HTML
document, from `andrewmaa/dsa`, a byte-for-byte mirror of the CERN `info.cern.ch` original
that w3.org serves. The authentic note: `<TITLE>`, `<NEXTID 22>`, `info.cern.ch` anchor URLs,
`<XMP>`, `<HP1>`/`<HP2>`, `<DL>`/`<DT>`/`<DD>`, `<UL>`/`<MENU>`/`<DIR>`. Item 1 in the archive
is no longer "❌ no source."

### ✅ #4a — XHTML 1.0 (2E) **prose** REC archived — complete
`specs/prose/xhtml-1.0/Overview.html` (71428 bytes) — the full single-page
_"XHTML 1.0 … (Second Edition), W3C Recommendation 26 January 2000, revised 1 August 2002,"_
55 sections, ends cleanly. From `manzamb/SemanticWebinAction`. This is the human-readable
prose, not just the DTD the archive already had.

### ⚠️ #4b — HTML 4.01 **prose** REC archived — partial (core chapters)
`specs/prose/html-4.01/**` (25 files) — authentic W3C prose (`$Id: …ijacobs` RCS stamp,
"Changed by: Ian B. Jacobs"). Has the core chapters incl. `struct/tables`, `struct/dirlang`,
`struct/global`, `present/styles` — exactly the sections the structural-elements appraisal
cites. **Partial**: the mirror lacks `about.html`, `intro/*`, `struct/lists.html`,
`interact/forms.html`, `appendix/changes.html`. The complete REC is at the blocked w3.org.

### Still genuinely not obtained (searched GitHub thoroughly; nothing authoritative)
- **HTML 3.2 prose** — only the DTD (archived) + Wikipedia-style derivatives exist on GitHub;
  no faithful full-prose mirror found. Not archiving a derivative (no-fabrication bar).
- **HTML5 (2014) published REC text** — the only reachable "html5" snapshot is a WHATWG
  `x-Hixie` **draft** fixture, not the published 28-Oct-2014 Recommendation. Not archived
  (would be mislabeled — the exact failure this report was created to catch).
- **HTML 5.1 / 5.2 published RECs** — unchanged (only the 5.3 Editor's Draft, honestly labeled).

### Updated scorecard (second session)

| # | Spec | After 1st session | After 2nd session |
|---|------|-------------------|-------------------|
| 1 | HTML Tags 1991 | ❌ no source | ✅ **original note archived** |
| 4 | HTML 4.0/4.01 | ✅ DTD self-resolving; prose absent | ✅ DTD + ⚠️ **partial prose added** |
| 5 | XHTML 1.0 | ✅ DTD validates; prose absent | ✅ DTD + ✅ **full prose added** |
| 7 | HTML5 2014 | ❌ none | ❌ none (no authoritative snapshot on GitHub) |
| 3 | HTML 3.2 | ✅ grammar; prose absent | ✅ grammar; prose still absent |

# HTML Specifications Archive (1991 → present)

A chronological archive of the international specifications for HTML — the web's markup
language — from its 1991 origin through the current WHATWG Living Standard.

Each version has a written-up Markdown summary in [`md/`](./md). Where the actual
specification source is obtainable, it is included verbatim in [`specs/`](./specs).

## Timeline

| # | Version | Year | Publisher | Write-up | Source included |
|---|---------|------|-----------|----------|-----------------|
| 1 | HTML Tags (first HTML) | 1991–92 | CERN (Tim Berners-Lee) | [md](./md/01-html-tags-1991.md) | ❌ (unobtainable) |
| 2 | HTML 2.0 (RFC 1866) | 1995 | IETF | [md](./md/02-html-2.0-1995.md) | ✅ **full prose** (`rfc1866-html-2.0.txt`) |
| 3 | HTML 3.2 "Wilbur" | 1997 | W3C | [md](./md/03-html-3.2-1997.md) | ✅ DTD (`dtd/html-3.2/`) |
| 4 | HTML 4.0 / 4.01 "Cougar" | 1997 / 1999 | W3C | [md](./md/04-html-4.0-4.01-1997-1999.md) | ✅ DTDs + entities (`dtd/html-4.0/`, `dtd/html-4.01/`) |
| 5 | XHTML 1.0 | 2000 | W3C | [md](./md/05-xhtml-1.0-2000.md) | ✅ DTDs (`dtd/xhtml-1.0/`) |
| 6 | XHTML 1.1 | 2001 | W3C | [md](./md/06-xhtml-1.1-2001.md) | ✅ full DTD + 47-file module set (`dtd/xhtml-1.1/`), validates offline |
| 7 | HTML5 | 2014 | W3C / WHATWG | [md](./md/07-html5-2014.md) | ❌ 2014 REC text not snapshotted (prose at w3.org) |
| 8 | HTML 5.1 | 2016 | W3C | [md](./md/08-html-5.1-2016.md) | ❌ no 5.1 snapshot (see note below) |
| 9 | HTML 5.2 | 2017 | W3C | [md](./md/09-html-5.2-2017.md) | ⚠️ only a W3C **5.3 Editor's Draft** source is archived (`w3c-html53-ED-single-page.bs`), not 5.2 |
| 10 | HTML Living Standard | 2011→now | WHATWG | [md](./md/10-html-living-standard.md) | ✅ |

## Appraisals

- [`STATE.md`](./STATE.md) — coherence audit of every archived spec (completeness, labeling).
- [`APPRAISAL-structural-elements.md`](./APPRAISAL-structural-elements.md) — structural-integrity
  appraisal of `div` / `span` / `table` vs. international-technology expectations, a safeguarding
  checklist for page structure, and the "can divs resize dynamically?" analysis.

## Folder layout

```
html-spec/
├── README.md              ← this index
├── md/                    ← written-up Markdown summary, one per version
│   ├── 01-html-tags-1991.md
│   ├── ... (chronological) ...
│   └── 10-html-living-standard.md
└── specs/                 ← actual retrieved specification sources
    ├── rfc1866-html-2.0.txt                       (HTML 2.0 — full verbatim IETF RFC text)
    ├── dtd/                                       (normative W3C grammars — self-resolving offline)
    │   ├── catalog, catalog.xml                   (SGML + XML catalogs; see dtd/README.md)
    │   ├── html-3.2/HTML32.dtd + ISOlat1.ent
    │   ├── html-4.0/{strict,loose,frameset}.dtd + HTML{lat1,symbol,special}.ent
    │   ├── html-4.01/{strict,loose,frameset}.dtd + HTML{lat1,symbol,special}.ent
    │   ├── xhtml-1.0/xhtml1-{strict,transitional,frameset}.dtd + xhtml-{lat1,symbol,special}.ent
    │   └── xhtml-1.1/{xhtml11.dtd,xhtml11-model-1.mod} + modules/ (47-file Modularization set)
    ├── whatwg-html-living-standard.source.html   (8.0 MB — the full Living Standard source)
    ├── w3c-html53-ED-single-page.bs              (W3C HTML 5.3 Editor's Draft Bikeshed source)
    ├── whatwg-html-README.md
    └── PROVENANCE.txt                            (exact repos + commit SHAs)
```

## A note on provenance (important — please read)

This archive was assembled inside a restricted build environment with **`INTEGRATIONS_ONLY`
network access**: the shell, package managers, and the web-fetch path all receive
`403 Forbidden` from `w3.org`, `ietf.org`, `rfc-editor.org`, `whatwg.org`, and the public
CDNs. The **only** working egress was the GitHub API.

Consequences (after an exhaustive mirror search):

- **Sources that are included** (`specs/`) were pulled directly from official upstream or
  faithful mirror GitHub repositories at specific commits recorded in
  [`specs/PROVENANCE.txt`](./specs/PROVENANCE.txt). These are byte-for-byte upstream source:
  - **HTML 2.0** — the complete verbatim IETF **RFC 1866** text (from `mcpherrinm/rfc-mirror`).
  - **HTML 3.2 / 4.0 / 4.01 / XHTML 1.0 / 1.1** — the **normative W3C DTDs and entity sets**
    (from `w3c/markup-validator`, which bundles them). These are the authoritative,
    machine-readable grammars each Recommendation is built on.
  - The XHTML 1.0/1.1 grammars are **complete and self-resolving** — they validate offline
    against the bundled files via `specs/dtd/catalog.xml` (verified with `xmllint`).
  - **HTML Living Standard** — the full 8 MB `source` (from `whatwg/html`).
  - **W3C HTML 5.x track** — a Bikeshed source (from `w3c/html`). ⚠️ Note: that file is the
    **HTML 5.3 Editor's Draft** (its own metadata says so), archived as
    `w3c-html53-ED-single-page.bs`. It is **not** a snapshot of the published 5.0 / 5.1 / 5.2
    Recommendations — those prose RECs are not archived (they live at w3.org, blocked here).
- **What is still NOT included:**
  - The **1991 "HTML Tags"** note — not in any reachable GitHub repo, and the W3C History
    tree is network-blocked here. Its `md/` write-up links the canonical w3.org URL.
  - The published **HTML5 (2014) / 5.1 / 5.2 Recommendation prose** — only the 5.3 Editor's
    Draft Bikeshed source was reachable.
  - The full **human-readable prose** of the HTML 3.2 / 4.x / XHTML Recommendations — only
    the normative DTDs were reachable (and are now complete + validatable); the prose lives
    at the `w3.org/TR` URLs linked in each write-up.

No specification text has been fabricated or presented as if it were the original source.
The `md/` files are clearly descriptive write-ups; only files under `specs/` are verbatim
upstream material.

## To complete the archive from an open-network machine

The missing historical sources can be downloaded directly:

- HTML Tags (1991): <https://www.w3.org/History/19921103-hypertext/hypertext/WWW/MarkUp/Tags.html>
- HTML 2.0: <https://www.rfc-editor.org/rfc/rfc1866.txt>
- HTML 3.2: <https://www.w3.org/TR/REC-html32>
- HTML 4.01: <https://www.w3.org/TR/html401/>
- XHTML 1.0: <https://www.w3.org/TR/xhtml1/>
- XHTML 1.1: <https://www.w3.org/TR/xhtml11/>

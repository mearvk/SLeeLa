# HTML Specifications Archive (1991 → present)

A chronological archive of the international specifications for HTML — the web's markup
language — from its 1991 origin through the current WHATWG Living Standard.

Each version has a written-up Markdown summary in [`md/`](./md). Where the actual
specification source is obtainable, it is included verbatim in [`specs/`](./specs).

## Timeline

| # | Version | Year | Publisher | Write-up | Source included |
|---|---------|------|-----------|----------|-----------------|
| 1 | HTML Tags (first HTML) | 1991–92 | CERN (Tim Berners-Lee) | [md](./md/01-html-tags-1991.md) | ✅ **original note** (`html-tags-1991/Tags.html`) |
| 2 | HTML 2.0 (RFC 1866) | 1995 | IETF | [md](./md/02-html-2.0-1995.md) | ✅ **full prose** (`rfc1866-html-2.0.txt`) |
| 3 | HTML 3.2 "Wilbur" | 1997 | W3C | [md](./md/03-html-3.2-1997.md) | ✅ DTD (`dtd/html-3.2/`) · ❌ prose |
| 4 | HTML 4.0 / 4.01 "Cougar" | 1997 / 1999 | W3C | [md](./md/04-html-4.0-4.01-1997-1999.md) | ✅ DTDs + entities (`dtd/html-4.0/`, `dtd/html-4.01/`) · ✅ **full prose** (`prose/html-4.01/`) |
| 5 | XHTML 1.0 | 2000 | W3C | [md](./md/05-xhtml-1.0-2000.md) | ✅ DTDs (`dtd/xhtml-1.0/`) · ✅ **full prose** (`prose/xhtml-1.0/`) |
| 6 | XHTML 1.1 | 2001 | W3C | [md](./md/06-xhtml-1.1-2001.md) | ✅ full DTD + 47-file module set (`dtd/xhtml-1.1/`), validates offline · ✅ **full prose** (`prose/xhtml-1.1/`) |
| 7 | HTML5 | 2014 | W3C / WHATWG | [md](./md/07-html5-2014.md) | ❌ 2014 REC text not snapshotted (prose at w3.org) |
| 8 | HTML 5.1 | 2016 | W3C | [md](./md/08-html-5.1-2016.md) | ❌ no 5.1 snapshot (see note below) |
| 9 | HTML 5.2 | 2017 | W3C | [md](./md/09-html-5.2-2017.md) | ⚠️ only a W3C **5.3 Editor's Draft** source is archived (`w3c-html53-ED-single-page.bs`), not 5.2 |
| 10 | HTML Living Standard | 2011→now | WHATWG | [md](./md/10-html-living-standard.md) | ✅ |

## Tag references

- [`md/tags/`](./md/tags) — a **per-version catalogue of every element (tag) with a one-line
  definition**, grouped by role, derived directly from the archived DTDs / RFC / source. See
  [`md/tags/README.md`](./md/tags/README.md) for the index and
  [`md/tags/CHANGES.md`](./md/tags/CHANGES.md) for the added/removed/deprecated deltas across
  versions.

## Worked examples

- [`examples/`](./examples) — one **well-formed demo document per version** showing each
  version's elements in their usual use cases. Every example has a Title and a Date and a
  **"performs as expected in a modern browser"** verdict (&#x2713; Yes / &#x2717; No). No CSS —
  just the markup and the status glyph. See [`examples/README.md`](./examples/README.md).

## CSS archive

- [`css/`](./css) — a parallel archive of **Cascading Style Sheets** from CSS1 (1996) to the
  modular CSS of today: version subfolders, each with the retrieved **spec source** and a
  **feature catalogue** (all selectors, properties, at-rules, functions, values and units of
  that level). File **signatures** (size + SHA-256) and provenance in
  [`css/SIGNATURES.md`](./css/SIGNATURES.md); index in [`css/README.md`](./css/README.md).

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
│   ├── 10-html-living-standard.md
│   └── tags/              ← per-version element (tag) catalogue + definitions
│       ├── README.md      (index)   CHANGES.md (cross-version deltas)
│       └── NN-<version>.tags.md ...
└── specs/                 ← actual retrieved specification sources
    ├── rfc1866-html-2.0.txt                       (HTML 2.0 — full verbatim IETF RFC text)
    ├── html-tags-1991/Tags.html                   (the 1991 CERN "HTML Tags" note — the first HTML doc)
    ├── prose/                                     (human-readable Recommendation PROSE — see prose/README.md)
    │   ├── html-4.01/                             (HTML 4.01 REC — complete chapter tree + figures)
    │   ├── xhtml-1.0/Overview.html                (XHTML 1.0 2E REC — full single-page prose)
    │   └── xhtml-1.1/                             (XHTML 1.1 2E REC — full prose; grammar is in dtd/)
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
  - **HTML Tags (1991)** — the original CERN note (`html-tags-1991/Tags.html`), recovered from
    a GitHub mirror of the `info.cern.ch` tree. _(Added 2026-10-25 — was previously unobtainable.)_
  - **HTML 4.01 prose** — the **complete** chapter tree (`prose/html-4.01/`, every canonical
    chapter + figures), **XHTML 1.0 (2E) prose** (`prose/xhtml-1.0/`, full single-page), and
    **XHTML 1.1 (2E) prose** (`prose/xhtml-1.1/`, full), all from GitHub mirrors of the
    `w3.org/TR` trees. _(Added 2026-10-25 — see [`prose/README.md`](./specs/prose/README.md).)_
- **What is still NOT included:**
  - The published **HTML5 (2014) / 5.1 / 5.2 Recommendation prose** — only the 5.3 Editor's
    Draft Bikeshed source was reachable. (No authoritative 2014-REC snapshot exists on GitHub;
    the "html5" candidates found are a WHATWG `x-Hixie` draft fixture and a 4 KB Hugo stub —
    neither the published REC.)
  - The **human-readable prose** of the **HTML 3.2** Recommendation — only its normative DTD
    is reachable; no faithful full-prose mirror was found on GitHub.

No specification text has been fabricated or presented as if it were the original source.
The `md/` files are clearly descriptive write-ups; only files under `specs/` are verbatim
upstream material.

## To complete the remaining gaps from an open-network machine

Nearly all of the historical originals are now archived locally (see the table above). Only
three gaps remain, and each can be downloaded directly where network access to w3.org is
available:

- HTML 3.2 **prose**: <https://www.w3.org/TR/REC-html32> &nbsp;(DTD already archived; prose still missing)
- HTML5 **2014 published REC**: <https://www.w3.org/TR/2014/REC-html5-20141028/>
- HTML 5.1 / 5.2 **published RECs**: <https://www.w3.org/TR/html51/> · <https://www.w3.org/TR/html52/>

Already archived locally (no longer need fetching): the 1991 Tags note, RFC 1866 (HTML 2.0),
the HTML 3.2/4.x/XHTML DTDs, the **complete HTML 4.01 prose**, XHTML 1.0 prose, XHTML 1.1 prose,
and the Living Standard.

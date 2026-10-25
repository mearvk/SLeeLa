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
| 6 | XHTML 1.1 | 2001 | W3C | [md](./md/06-xhtml-1.1-2001.md) | ✅ DTD + module (`dtd/xhtml-1.1/`) |
| 7 | HTML5 | 2014 | W3C / WHATWG | [md](./md/07-html5-2014.md) | ⚠️ via 5.x source |
| 8 | HTML 5.1 | 2016 | W3C | [md](./md/08-html-5.1-2016.md) | ✅ |
| 9 | HTML 5.2 | 2017 | W3C | [md](./md/09-html-5.2-2017.md) | ✅ |
| 10 | HTML Living Standard | 2011→now | WHATWG | [md](./md/10-html-living-standard.md) | ✅ |

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
    ├── dtd/                                       (normative W3C grammars)
    │   ├── html-3.2/HTML32.dtd
    │   ├── html-4.0/{strict,loose,frameset}.dtd
    │   ├── html-4.01/{strict,loose,frameset}.dtd + HTML{lat1,special,symbol}.ent
    │   ├── xhtml-1.0/xhtml1-{strict,transitional,frameset}.dtd
    │   └── xhtml-1.1/{xhtml11.dtd,xhtml11-model-1.mod}
    ├── whatwg-html-living-standard.source.html   (8.0 MB — the full Living Standard source)
    ├── w3c-html52-single-page.bs                 (W3C HTML 5.x Recommendation source)
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
  - **HTML Living Standard** — the full 8 MB `source` (from `whatwg/html`).
  - **W3C HTML 5.x** — the Bikeshed source (from `w3c/html`).
- **What is still NOT included:**
  - The **1991 "HTML Tags"** note — not in any reachable GitHub repo, and the W3C History
    tree is network-blocked here. Its `md/` write-up links the canonical w3.org URL.
  - The full **human-readable prose** of the HTML 3.2 / 4.x / XHTML Recommendations — only
    the normative DTDs were reachable; the prose lives at the `w3.org/TR` URLs linked in
    each write-up.

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

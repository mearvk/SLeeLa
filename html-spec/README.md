# HTML Specifications Archive (1991 → present)

A chronological archive of the international specifications for HTML — the web's markup
language — from its 1991 origin through the current WHATWG Living Standard.

Each version has a written-up Markdown summary in [`md/`](./md). Where the actual
specification source is obtainable, it is included verbatim in [`specs/`](./specs).

## Timeline

| # | Version | Year | Publisher | Write-up | Source included |
|---|---------|------|-----------|----------|-----------------|
| 1 | HTML Tags (first HTML) | 1991–92 | CERN (Tim Berners-Lee) | [md](./md/01-html-tags-1991.md) | ❌ |
| 2 | HTML 2.0 (RFC 1866) | 1995 | IETF | [md](./md/02-html-2.0-1995.md) | ❌ |
| 3 | HTML 3.2 "Wilbur" | 1997 | W3C | [md](./md/03-html-3.2-1997.md) | ❌ |
| 4 | HTML 4.0 / 4.01 "Cougar" | 1997 / 1999 | W3C | [md](./md/04-html-4.0-4.01-1997-1999.md) | ❌ |
| 5 | XHTML 1.0 | 2000 | W3C | [md](./md/05-xhtml-1.0-2000.md) | ❌ |
| 6 | XHTML 1.1 | 2001 | W3C | [md](./md/06-xhtml-1.1-2001.md) | ❌ |
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

Consequences:

- **Sources that are included** (`specs/`) were pulled directly from the official
  upstream GitHub repositories — [`whatwg/html`](https://github.com/whatwg/html) and
  [`w3c/html`](https://github.com/w3c/html) — at specific commits recorded in
  [`specs/PROVENANCE.txt`](./specs/PROVENANCE.txt). These are byte-for-byte upstream source.
- **Sources that are NOT included** — the 1991 "HTML Tags" note, HTML 2.0 (an IETF RFC),
  HTML 3.2, HTML 4.0/4.01, and XHTML 1.0/1.1 — are **not hosted in a retrievable GitHub
  repository** and could not be fetched from this environment. Their `md/` write-ups are
  summaries compiled from established, well-documented history, and each links to its
  canonical `w3.org` / `rfc-editor.org` URL so the authoritative text can be retrieved from
  a machine with open internet access.

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

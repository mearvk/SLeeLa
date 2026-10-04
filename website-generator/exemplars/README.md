# Website Generator — Exemplars

Reference HTML/CSS/JS/JSON captured from real, well-built sites, kept here as
study material for the SLeeLa Website Generator (design + sign model). An
exemplar is *input to learn from* — the generator does not ship or redistribute
it as output.

## Intended exemplars

| Source | Folder | Status |
|--------|--------|--------|
| American Airlines (`https://www.aa.com/`) | `aa_com/` | **Not captured** — see note below. |
| Piedmont Airlines (`https://piedmont-airlines.com/`) | `piedmont/` | **Not captured** — see note below. |

## Why these two are not captured here

Both sites could not be retrieved from the build sandbox:

1. The sandbox network is **`INTEGRATIONS_ONLY`** — all direct outbound HTTP(S)
   egress (shell `curl`, the headless browser) is blocked by the proxy
   (`CONNECT tunnel failed, 403`), including to any site.
2. The one tool with external reach (`web_fetch`) reaches the sites but receives
   **HTTP 403 Forbidden** from their own WAF / bot protection — even for
   `robots.txt`. These enterprise airline sites block non-interactive clients at
   the edge by design.

No placeholder or invented markup is checked in under those sources' names: an
exemplar must be the *actual* captured asset to be useful (and honest).

## How to populate an exemplar (from a machine with browser/network access)

Save the rendered page and its first-party assets, then drop them in the
matching folder here:

```
# In a normal browser: File > Save Page As > "Web Page, Complete"
#   -> yields index.html + an _files/ directory of css/js/json/images
# Or with a CLI on an unrestricted network:
wget -E -H -k -p -nd -P aa_com/      https://www.aa.com/
wget -E -H -k -p -nd -P piedmont/    https://piedmont-airlines.com/
```

Suggested per-exemplar layout:

```
exemplars/<name>/
  index.html        the page markup
  css/*.css         stylesheets
  js/*.js           scripts
  data/*.json       any JSON the page loads (config, content, i18n)
  SOURCE.md         the URL, capture date, and a note on what to study
```

## What to study in an exemplar

For the SLeeLa generator's **design + sign** model, extract:

- **Design** — the palette (brand/base/accent), type scale, spacing rhythm,
  corner radius, and the "energy" of the look (flat vs. gradient vs. motion).
- **Signs** — how the page decomposes into semantic blocks (nav, hero,
  feature grid, cards, CTA, footer) that map to `website/signs.sleela`.
- **JSON** — any content/config the site loads at runtime, as a model for a
  data-driven `.site` description.

These observations feed `website/design.sleela` and `website/signs.sleela`; the
raw assets are never emitted verbatim by the generator.

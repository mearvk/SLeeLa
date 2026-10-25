# W3C HTML / XHTML DTD bundle

The normative machine-readable grammars for the pre-HTML5 specifications, made
**self-resolving offline** via catalogs. Source: `w3c/markup-validator`
(commit `724a15b`) — see `../PROVENANCE.txt`.

## Contents

```
dtd/
├── catalog            SGML Open catalog (for onsgmls and other SGML tools)
├── catalog.xml        XML catalog (for xmllint and other XML tools)
├── html-3.2/          HTML32.dtd + ISOlat1.ent
├── html-4.0/          strict/loose/frameset.dtd + HTML{lat1,symbol,special}.ent
├── html-4.01/         strict/loose/frameset.dtd + HTML{lat1,symbol,special}.ent
├── xhtml-1.0/         xhtml1-{strict,transitional,frameset}.dtd + xhtml-{lat1,symbol,special}.ent
└── xhtml-1.1/         xhtml11.dtd + xhtml11-model-1.mod + xhtml-datatypes-1.mod
    └── modules/       the ~45 XHTML Modularization 1.1 .mod / .ent files
```

## Validating a document offline

For XHTML (XML) documents, with `xmllint`:

```sh
XML_CATALOG_FILES=/path/to/specs/dtd/catalog.xml \
  xmllint --noout --valid --catalogs yourdoc.xhtml
```

For classic HTML (SGML) documents, with `onsgmls` (from OpenSP, if installed):

```sh
SGML_CATALOG_FILES=/path/to/specs/dtd/catalog onsgmls -s -c /path/to/specs/dtd/catalog yourdoc.html
```

## Verified

All four XHTML flavors validate offline against these local files with no
network access:

| Flavor | Status |
|--------|--------|
| XHTML 1.1 (full 26-module closure) | ✅ valid |
| XHTML 1.0 Strict | ✅ valid |
| XHTML 1.0 Transitional | ✅ valid |
| XHTML 1.0 Frameset | ✅ valid |

The HTML 3.2 / 4.0 / 4.01 DTDs are SGML (not XML); `xmllint` cannot validate
against them, but every entity file they reference is now present locally and
mapped in both catalogs. Validate them with an SGML tool such as OpenSP `onsgmls`.

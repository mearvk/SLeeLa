# SleelaDoc API — HTML documentation for the SLeeLa standard library

This folder is a **generated, static HTML documentation site** for every source
unit under [`/lib`](../lib). Open it in any web browser — no server needed.

## How to read it

1. Open **`index.html`** — the home page, with a card for every package.
2. Read **`glossary.html`** first if you are new: it defines the reading level
   these pages are written for (**Student 2** — a capable adult learner returning
   to college-level study to read the books and learn what is true) and the core
   SLeeLa terms (class, struct, field, method, signature, facade, the Metadata
   Class record, SleelaDoc, package).
3. Click a package to open **`package/<name>.html`**, which lists every class and
   struct in that package with its fields (the values it stores) and methods (the
   actions it performs), each with a plain explanation taken from the source.

Hierarchy: **Home → Packages → Package → Class → Members (fields, methods)**.

See **`authorship.html`** for authorship and attribution metadata and
the source-syntax information represented by the generated corpus. There is no
single syntax version for the entire site: each source unit carries its own
`#sleela X.Y` pragma, shown as a pill on its package page. The compiler's
maximum accepted syntax (currently 2.9) is a separate toolchain contract and
does not rewrite individual source-unit pragmas. Attribution appears in the
footer of every page.

## Style

Clean house style in **blue, light blue, light grey, and white with black text**,
with a sticky package navigation sidebar and on-page filtering.

## Regenerating

The site is produced from the live `/lib` tree by a single generator:

```sh
python3 tools/generate-sleeladoc-api.py            # writes /sleeladoc-api
python3 tools/generate-sleeladoc-api.py --out DIR  # choose another output dir
```

The explanations are extracted from each source file's header
(`<< ... Definition: ... >>`) and the `<` / `<<` / `//` comments that precede
each field and method. The eleven standard Metadata Class accessors are
summarised rather than listed on every page, to keep the pages clean.

See also `manifesto/SLEELADOC.md` (the SleelaDoc convention), the
`lib/sleeladoc/` extractor package, and `manifesto/SLEELADOC.WIKI.md` (the
compiler's `--wiki` doc output).

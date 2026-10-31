# SleelaDoc API — SLeeLa standard library (syntax 2.9 snapshot)

This folder is a **generated, static HTML documentation site** for every source
unit under [`/lib`](../../../lib), captured at the **syntax 2.9** edition. Open
it in any web browser — no server needed.

This 2.9 snapshot is the first to include the **`philosophy`** package (the
philosophy tool handles and the composed base Tree with its `@mix` directives)
and reflects the 2.9 language edition — which adds **`@mix` consequent fields**
(multiple same-named fields understood as one indexed group; see
`../../../SLEELA.syntax` section 28). Each unit still shows its own `#sleela`
level as a pill on its package page.

## How to read it

1. Open **`index.html`** — the home page, with a card for every package.
2. Read **`glossary.html`** first if you are new: it defines the reading level
   and the core SLeeLa terms (class, struct, field, method, signature, facade,
   the Metadata Class record, SleelaDoc, package).
3. Click a package to open **`package/<name>.html`**, which lists every class and
   struct in that package with its fields and methods, each with a plain
   explanation taken from the source.

Hierarchy: **Home → Packages → Package → Class → Members (fields, methods)**.

See **`authorship.html`** for the authorship and attribution claims.

## Style

Clean house style in **blue, light blue, light grey, and white with black text**,
with a sticky package navigation sidebar and on-page filtering.

## Regenerating

The site is produced from the live `/lib` tree by a single generator:

```sh
python3 tools/generate-sleeladoc-api.py --out api/sleela/2.9   # this snapshot
python3 tools/generate-sleeladoc-api.py                        # the latest /sleeladoc-api
```

The explanations are extracted from each source file's header
(`<< ... Definition: ... >>`) and the `<` / `<<` / `//` comments that precede
each field and method. The eleven standard Metadata Class accessors are
summarised rather than listed on every page, to keep the pages clean.

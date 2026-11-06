# SleelaDoc API — SLeeLa standard library (syntax 2.9 snapshot)

This folder holds the **generated, static HTML documentation site** for every
source unit under [`/lib`](../../../lib), captured at the **syntax 2.9** edition.
Open any version's `index.html` in a web browser — no server needed.

## Versioned snapshots

The 2.9 edition is captured as a sequence of immutable, dated snapshots. Each
regeneration of the 2.9 doc set is placed in its own `version-NNN/` subfolder so
earlier snapshots are preserved and diffable, rather than overwritten in place:

| Snapshot | Contents |
| --- | --- |
| [`version-001/`](version-001/) | The initial 2.9 doc set. |
| [`version-002/`](version-002/) | Adds the expanded `lib/ui` component-quality API — `SLComponentQuality` gains **visibility** (`show`/`hide`), **margin** (`margin`/`setMargins`), and **icon** (`icon`/`iconFromAsset`/`clearIcon`) axes, and `SLLayoutDriver.applyQuality` realizes each via a real `ui*` bridge call (`uiWidgetVisible`, `uiWidgetMargin`, `uiWidgetSetIcon`). |

Each new regeneration adds the next folder in the sequence (`version-003/`, and
so on); the highest-numbered folder is the most recent 2.9 snapshot. Every
snapshot is self-contained (its own `index.html`, `packages.html`,
`package/*.html`, `glossary.html`, `authorship.html`, and `style.css`).

This 2.9 edition includes the **`philosophy`** package and its module
organization: structured event/data contracts, deterministic helper logic,
and a non-clinical, consent-first psychiatry-friend interface. The SLeeLa
handles are documented together with their source comments and the composed
base Tree with its `@mix` directives. It reflects the 2.9 language edition —
which adds **`@mix` consequent fields**
(multiple same-named fields understood as one indexed group; see
`../../../SLEELA.syntax` section 28). Each unit still shows its own `#sleela`
level as a pill on its package page.

## How to read it

1. Open a snapshot's **`index.html`** — the home page, with a card for every
   package.
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

The site is produced from the live `/lib` tree by a single generator. When the
library changes, capture a NEW snapshot into the next `version-NNN/` folder
(never overwrite an existing one):

```sh
python3 tools/generate-sleeladoc-api.py --out api/sleela/2.9/version-002  # this snapshot
python3 tools/generate-sleeladoc-api.py --out api/sleela/2.9/version-003  # the next one
python3 tools/generate-sleeladoc-api.py                                   # the latest /sleeladoc-api
```

The explanations are extracted from each source file's header
(`<< ... Definition: ... >>`) and the `<` / `<<` / `//` comments that precede
each field and method. The eleven standard Metadata Class accessors are
summarised rather than listed on every page, to keep the pages clean.

# SleelaDoc — the compiler's built-in documentation outputs (`--wiki` + originated twin)

Beyond the [`sleeladoc` extractor](../lib/sleeladoc/README.md), which renders the
**authored intent** of a unit, the **compiler itself** produces documentation
for an **author-marked** document as a *consequence of compilation*. These are
the sanctioned, zero-dependency API-doc outputs: you already run the compiler,
so you already have them.

There are two such outputs, both gated on the `author;` / `authorial` document
marker (syntax 2.4+):

| Output | Flag | What it is | Implemented in |
| --- | --- | --- | --- |
| **Originated source twin** | (automatic for author-marked) | a regenerated `.sleela` source in which every field and method is located and originated and carries a `∴` brander | `impl/frontend/originate.{h,cpp}` |
| **Wikitext emanation** | `--wiki` / `--wiki-dir=<dir>` | a structured MediaWiki page — infobox, a sortable wikitable of every class and member, and a declaration-order "Movement" section | `impl/frontend/wikigen.{h,cpp}` |

## The author marker

Both outputs require the document to be **author-marked**. Add the `author;`
(or `authorial`) marker near the top of the file, alongside `material;`:

```sleela
#sleela 2.5
author;
material;
<< ... >>
class MyUnit { ... }
```

Without the marker, the compiler emits only the runnable artifact; the
originated twin and the wiki page are not produced.

## Running it

```sh
# from the repository root, with the SHA-256 manifest selected
SLEELA_SHA256_MANIFEST="$(pwd)/security/sha256-manifest.json" \
  ./impl/build/sleela compile <file.sleela> \
    --wiki --wiki-dir=out -o out/<file>.sleela
```

Typical console output:

```
sleelvac: <file>.sleela -> out/<file>.sleela (runnable Sleela Core artifact)
sleelvac: originated source -> <file>.originated.sleela
sleelvac: wikitext emanated -> out/<file>.wiki
```

Flags:

- `--wiki` — enable the wikitext emanation (opt-in).
- `--wiki-dir=<dir>` — write the `.wiki` page into `<dir>` (implies `--wiki`);
  empty/omitted means the source directory.
- `--originate-dir=<dir>` — redirect the originated twin (default: the source
  directory).

> **Note.** The originated twin is written next to the source by default, so it
> can appear as a new `*.originated.sleela` file in your working tree. Point
> `--originate-dir` at a build/output directory to keep the source tree clean —
> and remember that files written under `lib/**` participate in the SHA-256
> manifest gate.

## What the wiki page contains

The emanation is **factual and structured** — a catalog of the *declared
surface*, not prose. For each unit it writes:

- an `{{Infobox software}}` with the document name, release/compile dates, the
  declared `#sleela` syntax version, and a size summary (classes, fields,
  methods);
- a **Time** section stamping the source edit date and the UTC compile instant
  on two temporal axes;
- a **Structure** section: a sortable `wikitable` listing every field and method
  with its **declaration-order id** and its **SHA-256 brander** (the same
  identity the originated twin carries). The brander preimage is
  `<doc>|<member>|<paramTypes>|<editDate>`;
- a **Movement** section laying the members out in declaration order (the tempo
  of the work) against the temporal axis.

The output scales with the size of the program: a larger work produces a larger
corpus. It is MediaWiki markup intended for a **self-hosted** wiki — it is not a
submission to any public encyclopedia, and auto-generated pages should not be
submitted to one without that project's approval.

## The originated twin

The `*.originated.sleela` document is the auditable twin of the runnable
artifact: a clean, regenerated source in which every member is located and
originated and carries the `∴` brander (the house analogue of Java's `@`). The
brander records the member's declaration-order id, its SHA-256 hash, the
document name, the member name, the parameter signature, and the edit date (see
`SLEELA.syntax` §18).

## Choosing between the three

SLeeLa offers three complementary documentation facilities:

| Facility | Renders | Form | Best for |
| --- | --- | --- | --- |
| [`sleeladoc` extractor](../lib/sleeladoc/README.md) | authored **intent** (`@doc` prose + tags) | Markdown / HTML | intent-level API pages, like `javadoc` |
| **`--wiki` emanation** (this doc) | the **declared surface** (every class/member + brander) | MediaWiki | a branded, member-by-member catalog on a self-hosted wiki |
| **originated twin** | the **located, branded source** | `.sleela` | an auditable, brander-stamped copy of the artifact |

Use `sleeladoc` when you want to publish the *meaning* a developer wrote; use
`--wiki` when you want a *structured inventory* of the surface as a byproduct of
every compile; use the originated twin for provenance and audit.

## Cross-references

- The convention and extractor: [`SLEELADOC.md`](SLEELADOC.md).
- The extractor package: [`lib/sleeladoc/README.md`](../lib/sleeladoc/README.md).
- The native implementations: `impl/frontend/wikigen.{h,cpp}`,
  `impl/frontend/originate.{h,cpp}`, and the CLI wiring in
  `impl/frontend/driver.cpp`.

---
inclusion: always
---

# Author field, `∴` branders, and the originated Source Document (syntax 2.4)

The authoritative design is
[`markdown/AUTHOR-FIELD-AND-BRANDER.design.md`](../../markdown/AUTHOR-FIELD-AND-BRANDER.design.md).
Status: **shipped.** Syntax 2.4 is implemented and is no longer the ceiling —
`maxSupportedSyntax()` is now `2.5` (the `material` document marker, section 19;
see `material-marker.md`). The `author`/`authorial` marker is live in the
lexer/parser/semantic analyzer and the originated/wiki emitters, and
`SLEELA.syntax` §18 is normative for it.

## The decision in one paragraph

Syntax **2.4** adds a modifier-less document marker **`author`** (synonym
**`authorial`**) — a single word, treated as a static class of the document
itself, meaning the document is **handled by professional council**. Its effect:
the document may then declare **duplicate fields and methods**, and each member
gets a **unique id** (declaration order, from 1) and a **hash brander**. The
standard compile produces **two** outputs: the usual compiled artifact, and a
regenerated **originated Source Document** in which every field/method is
preceded by a **`∴`** brander (the house analogue of Java's `@`) recording its
id, hash, document, member name, signature, and edit date. Clean and united.

## Fixed points (confirmed by the owner)

- `author` and `authorial` are **equivalent**; single word, no modifier/type.
- The brander glyph is **`∴`** (U+2234), used in place of `@`.
- The **hash is the brander**, over: document name, method name, the `int`
  parameters, with only **`String` and `int`** parameters considered, plus the
  **document edit date**.
- Duplicate members are legal **only when the document supposes `author`**.
- The originated Source Document is produced **as part of the standard
  compile**, written to the **source directory** or a **compile-time flag**
  location.
- Ordering of duplicates is a **table/map** (array-list or hashmap) of
  id-slots; the VM applies a **frequency-of-use adjustment** to pick slot N+1,
  N+2, … when a slot is busy; **direct call** of a specific definition is
  allowed when the document supposes `author` (surface syntax deferred).

## Reasoned defaults chosen where the brief left latitude (confirm/correct)

- **Hash algorithm = SHA-256** (already vendored; auditable). "MGEM" is **not**
  a defined algorithm in this repo — do not invent one; if MGEM is intended it
  needs a full spec (inputs, mixing, width, test vectors) first.
- **Ordering** = open-addressing forward probe from slot 1 with a frequency
  band B=1 ("busy → next slot"). The owner's example traces
  (`3→1,2,3`, `6→1,3,2,6,1`, `5→1,4,2,1`) were **not reproducible as a formula**
  and are read as illustrative live traces, not a table-construction rule; if
  they are literal tables, get the generating rule before coding dispatch.

## When implementing (slice order, each buildable + gated at `#sleela 2.4`)

1. Spec §18 + bump `maxSupportedSyntax()` to `{2,4}` and follow
   `spec-version-maintenance.md` (version.h, banners, README, wiki, version
   tests).
2. Lexer/parser: `author`/`authorial` top-level marker → AST document flag.
3. Semantic: relax duplicate-member rejection under the flag; assign ids;
   restrict the allowance to `String`/`int` signatures.
4. Hash + ordering map; thread a use-count into VM dispatch.
5. Originated emitter (`∴` branders; `--originate-dir`; output stays
   compilable).
6. Dispatch probe + reserve the id/hash direct-call brander.

## The third output: wikitext emanation (implemented)

A major author-marked work can also emanate a **MediaWiki wikitext document** as
a third compile output (alongside the artifact and the originated source). It is
opt-in: `sleela compile <src> -o <art> --wiki` writes `<src>.wiki` next to the
source; `--wiki-dir=<dir>` redirects it. The page is factual/structured and
scales with program size: an infobox, a Time section (edit date + UTC compile
stamp — "time as a second"), a Structure section (sortable wikitables of every
member with id/signature/slot/SHA-256 brander + the duplicate-dispatch ordering
table), a Movement section (members in declaration order — the "tempo"), and
Provenance + `[[Category:SLeeLa]]`. Implemented in
`impl/frontend/wikigen.{h,cpp}` (UTC stamp via `ledger::utc_iso8601`), wired in
`driver.cpp`, specified in `SLEELA.syntax` §18.5.

Honesty boundary kept: the emanation asserts nothing beyond the document's own
declared structure/timestamps, and it is for a **self-hosted** wiki — not a
submission to any public encyclopedia (which governs notability, independent
sourcing, and mass auto-generated pages). We did NOT invent a literal
"Universities" subject the owner mentioned — that remains an unnamed knob to be
specified before it appears in output.

Remember the integrity gate: any `lib/**` or `impl/**` edit requires
regenerating `security/sha256-manifest.json` and a passing `make check-manifest`
before commit.

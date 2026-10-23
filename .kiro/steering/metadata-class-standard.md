---
inclusion: always
---

# The Metadata Class standard (metapattern)

A **metadata class** carries the provenance, standing, and intent of a unit of
work — not its runtime behavior. The normative definition lives in
[`SLEELA.syntax` §15](../../SLEELA.syntax). This steering file is the working
rule for day-to-day authoring.

## When to include the metadata block

- **Worker class** (tutorials, examples, single-purpose wrappers): a metadata
  block is encouraged as a mark of craft; the baseline identity attributes are
  enough.
- **Executive+ class** (library-family classes, registry entries, anything that
  represents a standard, product, or jurisdictional artifact): the **full
  record is expected** — baseline identity *plus* legality and
  probation-of-intent. An executive-grade class without it is incomplete even
  if it compiles.

The pattern sets a pace in the collective intellect: the higher the standing of
the code, the more complete its self-description.

## The record (eleven catalogued attributes)

Each field has a matching `get`/`set` pair. Keep the comment header verbatim so
the block is greppable across the tree.

```sleela
  // ---- Descriptive metadata (version 1.11 nested-friendly accessors) ----
  // Eleven catalogued attributes with getter/setter pairs, so a GUI, loader,
  // or catalog can read and (where appropriate) override each record.
  // Values are faithful to the subject's real origin where publicly known.
  String metaVersion = "1.0.0";
  int metaId = 0;
  String metaAuthor = "...";
  String metaDate = "YYYY-MM-DD";
  String metaCopyright = "...";
  String metaTrademark = "none";
  String metaCorporation = "...";
  String metaOrigin = "International";   // National | <nation> | International | Galactic
  String metaBirthDate = "...";
  int metaScienceRating = 10;            // maturity/robustness on a 1..10 scale
  String metaNationalSchedule = "...";   // a real, citable instrument or "none"
```

## Framing for a mode of business

- **Legality** → `metaNationalSchedule` names the governing standard, spec,
  treaty, or statute (e.g. `"RFC 1952"`, `"ITU-T G.711"`). It must be a real,
  citable instrument or `"none"` — never invented to look legitimate.
- **Fiduciary** → `metaCopyright` + `metaTrademark` + `metaCorporation` state
  who owns and stewards, and under what terms.
- **Probation of intent (jurisdictional scope)** → `metaOrigin` on a widening
  scale: `"National"` (name the nation where known) → `"International"`
  (standards-body / multi-national) → `"Galactic"` (scope beyond terrestrial
  jurisdiction; top of the pace-setting scale).
- **Admin / Executive** → expressed by the authoring tier and the act of
  keeping the record current, not by a separate token.

## The one hard rule

Values are **faithful to the subject's real origin where publicly known**.
Unknown values use `"none"` / `"unknown"`, never a guess presented as fact.
Honesty of the record is what lets the higher classes of Sleela rely on it.

## Reference implementations

- `lib/codecs/SLCodec*.sleela` (20 classes) — the originating examples.
- `lib/compression/SLCompression*.sleela` (15 classes).
- Tutorial: `tutorial/11-metadata-class.md`.

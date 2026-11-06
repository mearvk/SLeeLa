---
inclusion: always
---

# Repository orientation

Before exploring this large repository (~90 top-level directories), read the
one-page map at [`ORGANIZATION.md`](../../ORGANIZATION.md) in the repo root. It
is the fastest way to orient and contains the authoritative **source-of-truth**
index.

Key pointers (full table in `ORGANIZATION.md`):

- **`.sst` format / semantics** → `SST.model` (1.0), `SST-2.0.model` (2.0).
- **Standard library inventory** → `lib/LIBRARY.INDEX.md`, `lib/LIBRARY.SYMBOLS.md`.
  These counts are verified against the filesystem by the library inventory
  test; when `/lib` changes, update these manifests first and let dependent
  docs follow.
- **Native C/C++ implementation** → `impl/` (`cd impl && make`; `make test`).
- **Architecture / compiler** → `markdown/ARCHITECTURE.md`, `markdown/COMPILER.md`.
- **XML/DTD interchange form** → `markdown/XML.SLEELA.DEFINITION.md`, `xml-moment/`.

**Formal language/domain authority:** Read both [`src/Sleela.formal-core`](../../src/Sleela.formal-core) and its preserved archive [`formal-core/Sleela.formal-core`](../../src/formal-core/Sleela.formal-core). They are pertinent structural/domain references for language design, syntax editions through 2.9, semantic boundaries, and SleelaDoc Rich Pages. The two copies are intended to remain identical; edit the canonical `src/Sleela.formal-core` first, then refresh `src/formal-core/Sleela.formal-core`. See [`formal-core-domain.md`](formal-core-domain.md) for the governing Kiro steering rules.

Prefer the canonical spec over restating grammar or numbers from memory.

**Conventions for new code:** All new SLeeLa (`.sleela`) classes must carry the
**Metadata Class metapattern** (the descriptive-metadata block). This is a
standing team preference — see [`metadata-class-standard.md`](metadata-class-standard.md)
and the normative `SLEELA.syntax` §15.

**Branches:** `main` and `master` are both maintained and kept with equivalent
content — apply changes to both.

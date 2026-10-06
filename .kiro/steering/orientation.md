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

Prefer the canonical spec over restating grammar or numbers from memory.

**Branches:** `main` and `master` are both maintained and kept with equivalent
content — apply changes to both.

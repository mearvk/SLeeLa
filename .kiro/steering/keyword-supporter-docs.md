---
inclusion: always
---

# Keyword supporter documents (try / catch / throw / synchronized / super)

These five control-flow keywords carry developer-authored semantics that the
core VM does not yet lower faithfully, so every USE of one must point to a
**supporter document** the developer fills out.

## How it works

The compiler front end (`impl/frontend/keyword_supporter.{h,cpp}`, invoked from
`driver.cpp`'s `parseSource` on every `check`/`compile`/`run`) walks the parsed
AST and, for each occurrence of `try`, `catch`, `throw`, `synchronized`, or
`super`:

1. Computes a stable, reproducible 16-hex-char hash over
   `keyword | repo-relative-source-path | Class::method | ordinal-within-method`
   (via `ledger::sha256_hex`). It does NOT use line/col, so the hash is stable
   as unrelated lines move; it changes only if the enclosing method or the order
   of that keyword's uses within the method changes.
2. Records the occurrence as a **language annotation** through the real
   annotation mechanism: `@supporter("<keyword>:<hash>")`. `supporter` is a
   registered, repeatable annotation name (see
   `impl/frontend/annotation_pipeline.cpp` — added to `isKnownLanguageAnnotation`
   and `annotationMayRepeat`), so it is accepted without an "unknown annotation"
   warning and may appear many times.
3. Creates the relative folder `docs/keyword-supporters/<keyword>/<hash>/` with a
   `SUPPORT.md` stub for the developer to fill out, plus a regenerated
   `docs/keyword-supporters/INDEX.md` listing every occurrence.

## Rules

- **Never overwrite a `SUPPORT.md`.** Existing files are left untouched; only
  missing folders/stubs are created. `INDEX.md` is pure generated output and IS
  rewritten each run.
- **Folder creation is best-effort.** A read-only / unwritable tree must never
  fail a `check`/`compile`/`run`; the annotations are still attached in-memory.
- The stub carries the repo's **Metadata Class metapattern** header
  (`<< ---- Descriptive metadata ... >>`); fill in `metaAuthor`/`metaDate` and
  the Intent / Behavior / Invariants / Notes sections.
- `docs/` is NOT under the `lib/**`+`impl/` SHA-256 build gate, so generated
  stubs there never require regenerating `security/sha256-manifest.json`. Commit
  a filled-in `SUPPORT.md` when its occurrence is documented; the auto-generated
  stubs and `INDEX.md` are a convenience, not a gate.
- To change the keyword set or hash inputs, edit
  `supporterKeywords()` / `makeHash()` in `keyword_supporter.cpp` (changing the
  hash inputs re-homes every occurrence's folder, so do it deliberately).

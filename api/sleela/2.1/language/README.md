# SLeeLa Language API and Version Contracts

This directory documents the language-facing contracts that affect API consumers. The normative grammar remains [`../../SLEELA.syntax`](../../SLEELA.syntax); compiler behavior and the runtime implementation are authoritative over explanatory documentation.

## Versioned contracts

- [Syntax version matrix](SYNTAX-VERSION-MATRIX.md) — features and compatibility boundaries.
- [Document markers](DOCUMENT-MARKERS.md) — the `author`/`authorial`, `material`, `memory`, and `determine` document-scope markers in one catalog.
- [System namespace API](system-namespace/API.md) — the `ran::` system-reference namespace.
- [Verified system references](system-namespace/REFERENCES.md) — reference inventory grounded in the current syntax 1.9 fixtures.

## Version 1.9: GC hints

Syntax 1.9 introduces the contextual assignment right-hand-side forms `x = gc N;` and `x = mem N;`. See [GC-hint API](../runtime/gc/GC-HINT-API.md), [semantics](../runtime/gc/SEMANTICS.md), and [test matrix](../runtime/gc/TEST-MATRIX.md).

The feature is a request to the runtime, not a promise that every allocation is reclaimed immediately. Reachable objects must remain valid. A language-level hint is distinct from the native collector API under `impl/` and from the tracked allocator under `api/native/memory/`.

## Documentation status

The matrix marks evidence and implementation boundaries explicitly. Do not promote a planned API to implemented status based only on a design document, class name, or placeholder declaration.

# SLeeLa Language API and Version Contracts

This directory documents the language-facing contracts that affect API
consumers. The normative grammar remains
[`SLEELA.syntax`](../../../../SLEELA.syntax); compiler behavior and regression tests
establish the implemented boundary.

## Versioned contracts

- [Syntax version matrix](SYNTAX-VERSION-MATRIX.md) — syntax editions,
  feature history, and implementation boundaries.
- [Document markers](DOCUMENT-MARKERS.md) — the complete document-scope marker
  family, including the compiler-active `author` marker and declarative markers.
- [System namespace API](system-namespace/API.md) — the `ran::` system-reference
  namespace.
- [Verified system references](system-namespace/REFERENCES.md) — reference
  inventory and its fixture evidence.

## Current syntax edition: 2.9

The compiler accepts source syntax from **1.0 through 2.9** and defaults to
2.9. The normative specification documents the **1.3–2.9** language surface.
A file's `#sleela X.Y` pragma selects its source-syntax edition; the compiler
release number is a separate version and does not silently change that pragma.

Syntax 2.9 adds `@mix` consequent fields (see §28 of
[`SLEELA.syntax`](../../../../SLEELA.syntax)). Repeated same-named instance fields
must carry `@mix`; their declaration order supplies indexes unless explicit
indexes are provided. The `prestige` marker described in §27 remains a future
proposal and is **not** an implemented 2.9 feature.

## Earlier versioned feature: syntax 1.9 GC hints

Syntax 1.9 introduces the contextual assignment right-hand-side forms
`x = gc N;` and `x = mem N;`. See the [GC-hint API](../runtime/gc/GC-HINT-API.md),
[semantics](../runtime/gc/SEMANTICS.md), and
[test matrix](../runtime/gc/TEST-MATRIX.md).

The feature is a request to the runtime, not a promise that every allocation is
reclaimed immediately. Reachable objects must remain valid. A language-level
hint is distinct from the native collector API under `impl/` and from the
tracked allocator under `api/native/memory/`.

## Documentation status

The matrix marks evidence and implementation boundaries explicitly. Do not
promote a planned API to implemented status based only on a design document,
class name, or placeholder declaration.

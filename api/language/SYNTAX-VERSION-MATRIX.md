# SLeeLa Syntax Version Matrix

The version declared in a source file (for example, `#sleela 2.4`) is the
**language syntax version**, not the Sleelvac compiler release number. The
normative grammar is [`SLEELA.syntax`](../../SLEELA.syntax); the native
version gate is declared in [`impl/frontend/version.h`](../../impl/frontend/version.h)
and implemented in `impl/frontend/version.cpp`.

| Syntax version | Documented feature | Evidence / reference | Status note |
|---|---|---|---|
| 1.8 | Inferred local declarations with `let`; constructor arguments on `new Type(args)` | `SLEELA.syntax` §12 | Constructor overload resolution and `this(...)` / base delegation are not verified by the specification and must be rejected explicitly. |
| 1.9 | Contextual GC-hint assignment: `x = gc N;` and `x = mem N;` | [GC-hint API](../runtime/gc/GC-HINT-API.md), syntax 1.9 fixtures | Keep range, context, liveness, and stress tests aligned with implementation. |
| 1.10 | Class inheritance and document-level `extends to` grouping | `SLEELA.syntax`, [EXTENDS.md](../../markdown/EXTENDS.md) | Keep the class and document forms distinct. |
| 1.11 | One level of nested classes | `SLEELA.syntax` §14 | Maximum lexical nested-class depth is two including the enclosing class; deeper types should be top-level declarations referenced by fields. |
| 2.2 | Generic type signatures, parameterized classes, and wildcards | `SLEELA.syntax` §16 | Do not infer full Java generic semantics from signature parsing; follow the restrictions in §16. |
| 2.3 | Guide-chain expression `new T(...) :: Guide :: Guide ...` | `SLEELA.syntax` §17 and Guide-chain fixtures | Verify parser, semantic analysis, lowering, and runtime behavior together. |
| 2.4 | `author` / `authorial` document marker and originated-source branding | `SLEELA.syntax` §18 | Confirm the complete output/branding pipeline before relying on generated artifacts. |

## Recursion and versioning

Self-recursive and mutually recursive method calls use the ordinary method-call,
parameter, and return syntax. They are **not a separately versioned syntax
feature**. Correctness depends on declaration/name resolution, per-invocation
call state, return propagation, reference identity, and runtime resource
handling. See [`SLEELA.syntax`](../../SLEELA.syntax) §3.3.1 and the collection
verification plan in [`SLList-Heterogeneous-Design.md`](../../lib/collections/SLList-Heterogeneous-Design.md).

The syntax-version gate currently declares support for `1.0..2.4` and defaults
to `2.4`; features introduced in later minors still need their individual
version gates. This range describes accepted pragma values, not proof that every
feature has complete end-to-end coverage.

## Compatibility rules

- Use the version pragma and compiler gate as the source of truth for whether a
  feature is legal in a file.
- A newer compiler does not automatically make a newer syntax feature legal
  under an older file pragma.
- Tests expected to fail are part of the contract: invalid forms must fail
  explicitly rather than being silently clamped or reinterpreted.
- The matrix is a navigation aid, not a replacement for the full grammar or
  implementation tests.

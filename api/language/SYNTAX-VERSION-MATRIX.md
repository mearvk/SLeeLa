# SLeeLa Syntax Version Matrix

The version declared in a source file (for example, `#sleela 1.9`) is the **language syntax version**, not the Sleelvac compiler release number. Consult [`SLEELA.syntax`](../../SLEELA.syntax) and `impl/frontend/version.{h,cpp}` for the normative grammar and accepted version gate.

| Syntax version | Documented feature | Evidence / reference | Status note |
|---|---|---|---|
| 1.8 | Inferred local declarations with `let`; constructor arguments on `new Type(args)` | `SLEELA.syntax`, compiler semantic analysis | The specification explicitly excludes constructor overload resolution and `this(...)` / base delegation from verified support. |
| 1.9 | Contextual GC hint assignment: `x = gc N;` and `x = mem N;` | [GC-hint API](../runtime/gc/GC-HINT-API.md), [syntax 1.9 fixtures](../../tests/sleela-syntax-1.9/README.md) | Range, version-gate, contextual-name, liveness and stress fixtures exist. |
| 1.10 | Two meanings of `extends`: class inheritance and document-level grouping | `SLEELA.syntax`, `markdown/EXTENDS.md` | Keep the class and document forms distinct. |
| 1.11 | One level of nested classes | `SLEELA.syntax`, section 14 | Maximum lexical nested-class depth is two including the enclosing class; deeper types should be declared at top level and referenced. |

## Compatibility rules

- Use the version pragma and compiler gate as the source of truth for whether a feature is legal in a file.
- A newer compiler does not automatically make a newer syntax feature legal under an older file pragma.
- Tests that are expected to fail are part of the contract: invalid forms must fail explicitly rather than silently being clamped or reinterpreted.
- This matrix is a concise navigation aid, not a replacement for the full grammar or implementation tests.

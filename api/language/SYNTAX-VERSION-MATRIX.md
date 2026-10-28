# SLeeLa Syntax Version Matrix

The version declared in a source file (for example, `#sleela 1.9`) is the **language syntax version**, not the Sleelvac compiler release number. Consult [`SLEELA.syntax`](../../SLEELA.syntax) and `impl/frontend/version.{h,cpp}` for the normative grammar and accepted version gate.

| Syntax version | Documented feature | Evidence / reference | Status note |
|---|---|---|---|
| 1.8 | Inferred local declarations with `let`; constructor arguments on `new Type(args)` | `SLEELA.syntax`, compiler semantic analysis | The specification explicitly excludes constructor overload resolution and `this(...)` / base delegation from verified support. |
| 1.9 | Contextual GC hint assignment: `x = gc N;` and `x = mem N;` | [GC-hint API](../runtime/gc/GC-HINT-API.md), [syntax 1.9 fixtures](../../tests/sleela-syntax-1.9/README.md) | Range, version-gate, contextual-name, liveness and stress fixtures exist. |
| 1.10 | Two meanings of `extends`: class inheritance and document-level grouping | `SLEELA.syntax`, `markdown/EXTENDS.md` | Keep the class and document forms distinct. |
| 1.11 | One level of nested classes | `SLEELA.syntax`, section 14 | Maximum lexical nested-class depth is two including the enclosing class; deeper types should be declared at top level and referenced. |
| 2.2 | Generic type signatures (type parameters on classes/methods, parameterized types, wildcards) | `SLEELA.syntax`, section 16 | The 1.3..1.11 surface is carried forward unchanged. |
| 2.3 | Philosophers Guide-chain expression `new T(...) :: Guide :: Guide ...` | `SLEELA.syntax`, section 17 | First-class construct executed by the lexer/parser/semantic analyzer/compiler and native Core. |
| 2.4 | `author` / `authorial` document marker | `SLEELA.syntax`, section 18 | Legalises duplicate members (each gets a declaration-order id + SHA-256 brander); emits the originated source with `∴` branders. Contextual word, not a reserved keyword. |
| 2.5 | `material` document marker (current ceiling) | `SLEELA.syntax`, section 19 | Declarative only — no effect on parsing/typing/codegen. Marks membership in the set of material documents (state / progress / securing of change). |
| 2.5 | `memory` document marker | `SLEELA.syntax`, section 20 | Declarative-only sibling of `material`. |
| 2.5 | Wide signed integer primitives: `long`, `int64`, `int128` | `SLEELA.syntax`, section 21 | `long`/`int64` are genuine 64-bit signed integers (runtime stores `int64_t`); they type-check like `int`. `int128` is a **true 128-bit** signed integer: the VM carries a dedicated `SL_INT128`/`__int128` value and arithmetic/comparisons/printing all compute in 128 bits (e.g. `9000000000 * 9000000000` = `81000000000000000000`). |
| 2.8 (planned) | `object` field marker; code **Ready for Public Exchange** | `docs/TECH-NOTE-2.8-OBJECT-MARKER-AND-PUBLIC-EXCHANGE.md` | **Forward-facing / not yet implemented.** 2.8 and past are intended for public exchange. `object` marks work forward-facing into the United States and the Times, and into International Progress (Contrary / Secondary — indicated). Follows the `material` marker pattern when built. |

## Compatibility rules

- Use the version pragma and compiler gate as the source of truth for whether a feature is legal in a file.
- A newer compiler does not automatically make a newer syntax feature legal under an older file pragma.
- Tests that are expected to fail are part of the contract: invalid forms must fail explicitly rather than silently being clamped or reinterpreted.
- This matrix is a concise navigation aid, not a replacement for the full grammar or implementation tests.

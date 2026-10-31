# SLeeLa Syntax Version Matrix

The version declared in a source file (for example, `#sleela 2.9`) is the
**language syntax version**, not the Sleelvac compiler release number. The
normative grammar is [`SLEELA.syntax`](../../SLEELA.syntax); the compiler gate
is implemented in `impl/frontend/version.{h,cpp}`.

The compiler currently accepts source syntax **1.0 through 2.9** and defaults
to its maximum supported version. The specification's documented normative
surface is **1.3 through 2.9**. A newer compiler does not silently upgrade an
older source file: each feature remains subject to its own syntax-version gate.

| Syntax version | Documented feature | Normative reference | Status / boundary |
|---|---|---|---|
| 1.8 | Inferred local declarations with `let`; constructor arguments on `new Type(args)` | `SLEELA.syntax`, §12 | Constructor overload resolution and `this(...)` / base delegation are not verified and are rejected. |
| 1.9 | Contextual GC-hint assignment: `x = gc N;` and `x = mem N;` | `SLEELA.syntax`, §13; [GC-hint API](../runtime/gc/GC-HINT-API.md) | Runtime cleanup requests, not guarantees of immediate reclamation; reachable objects remain valid. |
| 1.10 | Two meanings of `extends`: class inheritance and document-level grouping | `SLEELA.syntax`, §14; [EXTENDS](../../../../markdown/EXTENDS.md) | Keep the class and document forms distinct. |
| 1.11 | One level of nested classes | `SLEELA.syntax`, §14 | Maximum lexical nested-class depth is two including the enclosing class. |
| 2.2 | Generic type signatures | `SLEELA.syntax`, §16 | Type parameters on classes/methods, parameterized types, and wildcards; earlier 1.3–1.11 surface is carried forward. |
| 2.3 | Philosophers Guide-chain expression `new T(...) :: Guide :: Guide ...` | `SLEELA.syntax`, §17 | Normative language construct; consult implementation and tests for current execution coverage. |
| 2.4 | `author` / `authorial` document marker | `SLEELA.syntax`, §18 | Compiler-active: permits duplicate members in marked documents and emits originated-source branding. |
| 2.5 | `material` and `memory` document markers | `SLEELA.syntax`, §§19–20 | Declarative document claims; no parsing, typing, dispatch, or code-generation effect. |
| 2.5 | Wide signed integer primitives: `long`, `int64`, `int128` | `SLEELA.syntax`, §21 | Signed-width behavior is described in the normative specification; do not infer arbitrary-precision arithmetic from the larger named widths. |
| 2.6 | Keyword supporter documents | `SLEELA.syntax`, §22 | Links occurrences of selected control-flow keywords to supporting documentation; additive annotation behavior. |
| 2.6 | `determine`, `object`, and `field` document markers | `SLEELA.syntax`, §§23, 26 | Declarative markers. `object` has the additional 2.8 reification standing described in §26.6. |
| 2.7 | Base-2 signed integer series: `int32` through `int16777216` | `SLEELA.syntax`, §24 | Named signed widths; actual arithmetic capacity remains bounded by the implemented native value slots. |
| 2.8 | Base-2 unsigned integer series: `uint32` through `uint16777216` | `SLEELA.syntax`, §25 | Positive-only unsigned widths; operands must satisfy the specified unsigned typing rules. |
| 2.8 | Reified standing of the `object` document marker | `SLEELA.syntax`, §26.6 | The marker is gated from 2.6; its catalogue/reification standing is specified from 2.8. |
| 2.9 | `@mix` consequent fields | `SLEELA.syntax`, §28 | Repeated same-named instance fields marked `@mix` form an indexed group. Automatic indexes follow declaration order; explicit index lists must match the number of fields. |
| 2.9 (future proposal) | `prestige` document marker | `SLEELA.syntax`, §27 | **Not implemented.** Its appearance in a 2.9 specification does not make it an accepted compiler feature. |

## Document-marker catalog

The document-scope marker family is summarized in
[DOCUMENT-MARKERS.md](DOCUMENT-MARKERS.md). The `author` marker is the
compiler-active exception; the other catalogued markers are declarative and
must not be described as changing runtime behavior.

## Compatibility rules

- Use the source file's `#sleela X.Y` pragma and the compiler's feature gate to
  determine whether a construct is legal.
- The compiler's maximum accepted version is not proof that every proposed
  feature described under that version is implemented.
- Keep planned or future proposals explicitly separate from implemented syntax.
- Tests that are expected to fail are part of the contract: invalid forms must
  fail explicitly rather than being silently clamped or reinterpreted.
- This matrix is a navigation aid, not a replacement for the full grammar,
  implementation, or regression tests.

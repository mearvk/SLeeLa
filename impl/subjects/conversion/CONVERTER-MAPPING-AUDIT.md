# Java-to-SLeeLa converter mapping audit

Date: 2026-11-14  
Scope: C++ converter (`sleelactl/convert`), SLeeLa-native converter
(`lib/java/converter`), and their shared subject
(`impl/subjects/conversion/conversion.model`). This audit does not authorize
changes to the compiler, VM, or general runtime.

## Status vocabulary

- **Implemented and shared** — both converters consume the same rule and have
  evidence that exercises it.
- **Implemented, duplicated** — behavior exists, but the implementations or
  tables can drift.
- **Implemented, converter-specific** — one converter has a lowering or
  resolver the other does not.
- **Partial** — a restricted subset is implemented.
- **Designed only** — documented, but not demonstrated by executable tests.
- **Missing** — no implementation or evidence found in this pass.

## 1. Mapping and semantic-law audit

| Area | Current finding | Status | Required next step |
|---|---|---|---|
| Primitive type inventory | The model lists all eight Java primitives, but the C++ translator also hard-codes operand classes and numeric ranks. | Implemented, duplicated | Make type class/rank/promotion decisions model-driven. |
| `float` | C++ has a dedicated `JavaFloat.f32` path for arithmetic, comparisons, casts and increments. The model does not declare `promote float`, float binary-operation rows, or float cast rows. | Implemented, converter-specific | Express the supported float laws in the model and have both converters consume them. |
| Numeric promotion | The model has byte/short/char/int/long/double promotion rows, but no float promotion row. Mixed numeric promotion is partly hard-coded in C++. | Partial | Cover JLS binary numeric promotion, including float and all widening paths. |
| Casts and widening | The model has int/long/double casts, but the C++ translator special-cases float and some coercions. | Implemented, duplicated | Audit every primitive source/target pair and make the model authoritative. |
| Compound assignment | The model lists int/long/double/text/boolean cases; float is currently handled by C++ special paths rather than a float row. | Partial | Verify narrowing, promotion, and one-time lvalue evaluation for every supported operator. |
| Boxing/unboxing | The model lists a subset of wrapper classes; C++ rejects boxed types in declarations/casts and treats some wrappers as primitive operands. | Partial | Define wrapper coverage and fail-closed rules consistently. |
| Library result types | The shared model has extensive `ret` and `field` rows. | Implemented, shared data | Verify provenance/version against JDK 28, overload-sensitive results, and generic substitutions. |
| Built-in call qualification | The model has `qualify` rows, but C++ still has a separate `builtin_names.h` table. | Implemented, duplicated | Remove the second authority or mechanically verify exact set equality. |
| String conversion | `tostr` and `chartext` rows exist; native and C++ implementations read different subsets. | Partial parity | Test all text-conversion call sites and null/reference behavior in both converters. |

Important observation: the C++ README says the translator has no Java semantic
rules of its own, but `classOf`, `rankOf`, float lowering, and portions of
coercion/promotion currently encode behavior in C++. The documentation should
not claim full model authority until those paths are data-driven or explicitly
documented as converter mechanics.

## 2. Identifier and converter parity

The collision audit records 13 problematic Java identifiers. The native
converter already globally renames `require`, `usualgc`, `usualmem`,
`int64`, and `int128`; the C++ reserved-name set did not. This pass adds
those five names to the C++ set and adds a differential regression input at
`sleelactl/convert/tests/java/ReservedIdentifiers.java`.

Remaining identifier work:

- Move rename policy into shared model data rather than keeping
  `kSleelaReserved` and `JConvRules.isRenamedIdent` as independent lists.
- Preserve context: `next` is unsafe as a variable but must remain intact for
  calls such as `it.next()`.
- Rename `connect` and `send` only when they are declarations in the
  converted program; do not rename calls to library APIs.
- Preserve the entry-point name `main` where the target execution contract
  requires it, while addressing the measured collision for other method uses.
- Compare the C++ built-in qualification list with the model's `qualify`
  rows. The C++ `builtin_names.h` remains a second copy.
- Add cross-converter fixtures for declarations, locals, fields, parameters,
  qualified calls, library calls, nested types, and entry points.

The C++ converter fails closed per compilation unit. The SLeeLa-native
converter intentionally emits per-member throwing stubs. These are different
products and must not share a single “converted successfully” status. Reports
must distinguish complete conversion, partial/stubbed output, and refusal.

## 3. Generic types and overload resolution

### C++ converter

- The symbol table is project-wide and stores methods, fields, inheritance,
  interfaces, parameter types, return types, static status and varargs.
- Class type variables and nested generic arguments are still erased to unknown (`?`).
  Method-level type variables are retained as signature metadata for the direct
  unbounded forms `T`, `T[]`, and `T...`.
- `pickMethod` now evaluates fixed-arity applicability before variable arity,
  uses identity, model-driven primitive widening, known reference-subtype and
  recursive reference-array conversions, walks inherited class/interface
  methods, collapses overridden signatures, and selects among supported
  most-specific candidates. It fails
  closed when supported candidates remain incomparable. This is a meaningful
  partial JLS implementation, not full Java overload resolution.
- Direct unbounded method-variable inference now binds `T`, `T[]`, and `T...`
  from known argument types and substitutes the selected parameter/return
  signature. Repeated variables currently require the same inferred type; null-only
  inference is rejected. Bounds, nested generic arguments, class-generic
  substitution, target-type inference, boxing/unboxing, unchecked conversions,
  wildcard capture, intersection constraints, and full JLS specificity remain
  incomplete. A method type variable inside a generic argument is not inferred.
- Lambda arguments are deferred until a selected method supplies a known
  functional-interface target. The target must resolve to a collected or
  seeded interface with exactly one distinct abstract instance-method signature
  across its full inherited interface graph (diamond paths and duplicate
  redeclarations collapse; Object methods are excluded). Method references now
  select collected static, unbound-instance, bound-instance, `this::`, and
  constructor overloads against the SAM parameter and return types, including
  argument adaptation and static/instance disambiguation. Library method
  references without collected signatures, generic SAM substitution, and
  intersection-target resolution remain incomplete.

### SLeeLa-native converter

- It tracks type variables for erasure and recognizes many generic syntax
  shapes, but its per-type method table is keyed by method name. Overloads
  with differing return types collapse to an unknown return type.
- It now loads the shared primitive `widen` rows into `JConvModel`, exposes
  `canWiden` through `JConvRules`, and keeps matching defaults when the model
  file is unavailable. `JConvExpr.promote` and `JConvExpr.merge` now use that query for `char` to
  `int` unary/binary promotion and the supported int/char/long widening paths
  to `long`/`double`. Cast decisions and some primitive-kind distinctions are
  still represented by native kind rules. Its member table also
  erases overloads by name and does not implement generic invocation inference.
- It does not currently provide a project-wide generic type substitution and
  overload-resolution phase comparable to the C++ symbol table.
- The expression pass can use model return rows, but those rows do not by
  themselves resolve generic substitutions or overloaded signatures.

### Required shared behavior

1. Preserve declared type variables and their bounds in the project symbol
   index, even if emitted SLeeLa uses erasure.
2. Instantiate generic parameter and return types at each call site.
3. Implement Java invocation applicability phases and most-specific selection.
4. Account for primitive widening, reference widening, boxing/unboxing,
   varargs, and generic inference in the correct order.
5. Detect ambiguous and unresolved invocations; refuse them with source-located
   diagnostics rather than selecting the first candidate.
6. Pass the resolved functional-interface target to lambda and method-reference
   lowering.

## 4. Imports and cross-package symbols

The C++ parser now retains explicit type imports and explicit/wildcard static
imports. Conflicting explicit type imports fail with `import-ambiguous`;
unqualified static method calls are resolved only after ordinary in-scope
lookup, and multiple applicable static-import owners fail with
`static-import-ambiguous`. Imported static owners are reduced to the model's
simple owner name. This is not yet a complete Java name-resolution engine:
wildcard type-import package indexing, module readability,
package-private accessibility, and nested type imports remain open. The
SLeeLa-native converter already records package and import tokens for class
literals, but does not yet share the C++ project symbol resolver.

## 5. Lambda and method-reference target typing

Existing work is significant: the C++ converter collects functional-interface
signatures and defers lambda arguments until a target parameter is selected.
The SLeeLa-native converter has a defunctionalized lambda representation and
supports selected lambda/method-reference forms.

Remaining work is to make target typing a consequence of the same resolved
method signature in both converters. Cover generic functional-interface substitutions, intersection targets,
primitive-specialized functional interfaces, library method-reference
signatures, and overloaded method references where generic SAM types are
involved. Preserve receiver evaluation exactly once and at reference
creation time. Do not count an unknown-target stub as successful conversion.

## Acceptance checks for this pass

- The same Java fixture is accepted or refused by both converters for the same
  semantic reason, except where an explicitly documented mode differs.
- When accepted, both outputs have the same observable behavior as the JDK.
- Re-running conversion is deterministic and does not leave stale output.
- An ambiguous overload or unresolved target is diagnosed; no first-match
  guess is emitted.
- Reports count fully equivalent, partial/stubbed, refused, and failed files
  separately.
- JDK 28 corpus results are measured independently for each converter. Older
  JDK 25 measurements are not evidence of JDK 28 source coverage.

## Immediate work queue

1. Complete the primitive-law audit and move float/promotion/cast decisions into
   the shared model.
2. Consolidate identifier rename and built-in qualification data, retaining
   context-sensitive rename categories.
3. Complete C++ invocation applicability and specificity beyond the current
   supported subset: boxing/unboxing, bounded/class/nested generic inference,
   wildcard capture, and the remaining JLS ambiguity rules. Add focused
   differential fixtures before claiming broader coverage.
4. Extend `JConvExpr` model use from char-to-int unary promotion to the full
   supported numeric-promotion/cast set, then bring the SLeeLa-native signature
   index toward overload-aware resolution and add generic SAM substitution.
   The emitter and reports now stage, compare, and publish per-file outputs;
   native source/model/output provenance digests and a whole-run output
   transaction are still missing.

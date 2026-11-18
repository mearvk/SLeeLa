# Java-to-SLeeLa converter mapping audit

Date: 2026-11-18  
Scope: C++ converter (`sleelactl/convert`), SLeeLa-native converter
(`lib/java/converter`), and their shared subject
(`impl/subjects/conversion/conversion.model`). This audit does not authorize
changes to the compiler, VM, or general runtime.

## Status vocabulary

- **Implemented and shared** — both converters consume the same rule and have
  evidence that exercises it.
- **Implemented, pending verification** — both implementations now read the shared
  data, but the changed fixtures or full differential suite have not yet run.
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
| Primitive type inventory | `type` rows provide target spelling, primitive class and numeric rank; both C++ and native type classifiers now read the model, with `promote` rows supplying unary promotion. | Implemented, pending verification | Run the C++ self-check and cross-converter primitive fixtures. |
| `float` | Model 1.5.0 declares unary promotion, arithmetic/comparison and unary operator templates, primitive casts, increment/decrement, and text conversion. C++ consumes the rows; native expression rewriting now tracks `f`, rounds operands/results, and reads the float text/cast rows. | Implemented, pending verification | Differentially test mixed numeric promotion, literal conversion, casts, compounds and text formatting; native compound float assignment remains partial. |
| Numeric promotion | `promote` and `type` rows now drive C++ operand classes/ranks; native type classification reads the same rows and its merge path checks model widening edges, including float. | Implemented, pending verification | Exercise every numeric promotion pair against a real JDK. |
| Casts and widening | Model 1.5.0 supplies all numeric primitive source/target cast pairs; C++ implicit coercion requires a `widen` row and applies the matching `cast` template. Native explicit cast rewriting now selects templates by source and target kind. | Implemented, pending verification | Differentially verify all 7×7 numeric casts, including NaN, infinities, saturation and narrowing. |
| Compound assignment | Float `compound` rows are now declared and consumed by C++; native float assignment initializers and simple assignments round to binary32, but native compound-float lowering is not yet implemented. | Partial | Add native compound-float lowering and test one-time lvalue evaluation and narrowing. |
| Boxing/unboxing | The model lists a subset of wrapper classes; C++ rejects boxed types in declarations/casts and treats some wrappers as primitive operands. | Partial | Define wrapper coverage and fail-closed rules consistently. |
| Library result types | The shared model has extensive `ret` and `field` rows. | Implemented, shared data | Verify provenance/version against JDK 28, overload-sensitive results, and generic substitutions. |
| Built-in call qualification | Both converters now read `qualify` rows from the model; the duplicate C++ header is removed. The model includes `close`, `connect` and `send`. Receiver-independent `connect`/`send` member renaming remains a separate unresolved case. | Partial | Verify ordinary built-in qualification and implement member-context rewriting for converted source declarations/calls. |
| String conversion | C++ reads primitive `tostr` rows including float; native `JConvModel` reads generic `tostr` rows and the expression pass uses `tostrFloat` for binary32 values. | Implemented, pending verification | Differentially test primitive, reference, null, char and boxed text conversion. |

Implementation update (2026-11-18): the C++ translator's `classOf`, `rankOf`,
float lowering, primitive coercion, identifier replacement and built-in
qualification now query the model. The native converter's `JConvModel` reads
type, promotion, operator, cast, widening, rename and qualification records;
`JConvRewriter` and `JConvExpr` consume the applicable rows. This is an
implementation change, not a test result: the updated native and C++ differential
suites have not yet been run. The trusted build and important-file SHA-256 manifests
also need regeneration from a full checkout before the verified build gate is used.

## 2. Identifier and converter parity

The collision audit's replacement policy is now represented by `rename` rows
in the shared model. Both converters consume the same `all` mappings; the
context-only `next` rule is marked `value` so `next()` invocations remain
ordinary Java method calls. C++ built-in qualification also reads `qualify`
rows directly; the duplicate generated header has been retired.

Remaining identifier work:

- Confirm the `all` versus `value` context policy on declarations, locals,
  fields, parameters and every use site in both converters.
- Confirm `close`'s special `closeResource` mapping remains distinct from the
  generic trailing-underscore replacements.
- Preserve the entry-point name `main` where the target execution contract
  requires it.
- Add/extend cross-converter fixtures for declarations, locals, fields,
  parameters, qualified calls, library calls, nested types and entry points.

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
- `pickMethod` now evaluates strict fixed-arity, loose fixed-arity (boxing/unboxing), and variable-arity applicability in Java invocation-phase order,
  uses identity, model-driven primitive widening, known reference-subtype and
  recursive reference-array conversions, walks inherited class/interface
  methods, collapses overridden signatures, and selects among supported
  most-specific candidates. It fails
  closed when supported candidates remain incomparable. This is a meaningful
  partial JLS implementation, not full Java overload resolution.
- Method-variable inference binds direct `T`, `T[]`, and `T...` forms, plus nested invariant forms such as `Holder<T>` and `List<? extends T>`. Scalar primitive arguments are boxed into model-declared wrappers. Repeated reference constraints merge when one known class is a subtype of the other. Simple declared upper bounds are checked after inference; unbounded, `? extends`, and `? super` generic arguments receive conservative compatibility checks. Class-generic substitution follows parameterized superclass and interface paths for method signatures, and resolves inherited generic fields. Repeated reference constraints can use a known common superclass; unrelated types without a known common class fall back conservatively to `Object`. Interface-intersection least-upper-bound inference, parameterized least-containing invocations, null-only inference, intersection bounds, target-type inference, wrapper inheritance, unchecked conversions, wildcard capture with inference variables in lower bounds, and full JLS specificity remain incomplete.
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
- `JConvModel` now loads shared `type`, `promote`, `binop`, `unop`, `cast`,
  `widen`, `rename`, `qualify` and `tostr` rows. `JConvRewriter` uses the model
  for primitive classification and promotion; `JConvExpr` distinguishes
  binary32 from binary64, applies model float-rounding templates to arithmetic,
  assignments, casts and text conversion, and selects explicit cast rows by
  source and target. Native float compound-assignment lowering remains partial.
  Its member table still erases overloads by name and does not implement
  generic invocation inference.
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

1. Shared primitive type/rank/promotion, float, cast, identifier replacement and
   built-in qualification implementation is now in model 1.5.0; add differential
   coverage and run the C++ self-check before calling it verified.
2. Complete native compound-float assignment and test the `next` value-only
   mapping, `closeResource` special case and built-in qualification parity.
3. Complete C++ invocation applicability and specificity beyond the current
   supported subset: generic inheritance, target typing, intersection-bound and
   wildcard-capture constraints, wrapper inheritance, and the remaining JLS
   ambiguity rules. Add focused differential fixtures before claiming broader coverage.
4. Extend the SLeeLa-native signature index toward overload-aware resolution,
   add generic SAM substitution, and implement the same direct generic method
   inference subset as C++.
5. Complete package-aware symbol identity for colliding simple type names,
   wildcard type-import indexing, module/accessibility checks, and static
   nested-type imports. Add whole-run staging to C++ and SHA-256
   source/model/converter/output provenance to the SLeeLa-native converter.

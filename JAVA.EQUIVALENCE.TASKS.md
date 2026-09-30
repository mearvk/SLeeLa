# JAVA.EQUIVALENCE.TASKS.md

## Java Source -> SLeeLa Equivalence Task Ledger

Scope: known Java source represented congruently at the SLeeLa source/API level. Not required: JVM, Java bytecode execution, SLVM, or JVM runtime internals.

The official Java SE specification page currently identifies Java SE 27 as released in September 2026; the project's Java 28 target is therefore a forward compatibility target, not a released Java specification. citeturn0search0

### Eight source-equivalence pieces

| # | Piece | Required capability | Status |
|---|---|---|---|
| 1 | Lexical | Java-compatible tokens, identifiers, literals, operators, annotations, generic punctuation, source metadata | Foundation expanded |
| 2 | Type system | Primitive/reference/array/null types; generics; wildcards; bounds; conversions; inference | Type model created |
| 3 | Declarations | Class/interface/enum/record/annotation; fields/methods/constructors/nested types | Metadata + model expanded |
| 4 | Expressions | Names, access, calls, creation, arrays, operators, casts, patterns, lambdas, method references, conditionals | Expression inventory types created |
| 5 | Statements | Blocks, declarations, control flow, switch/yield, exceptions, synchronized, try-with-resources | Statement inventory types created |
| 6 | Semantic constraints | Resolution, access, conversions, overload/override, generics, definite assignment, exceptions | Rule taxonomy created |
| 7 | API counterparts | Exact Java package/type/member/signature/annotation counterparts | Existing JDK API envelopes; dependency closure next |
| 8 | Equivalence testing | Java-to-SLeeLa inventory, normalized comparison, deterministic diagnostics | First deterministic driver created |

### Next major piece: Java Source Equivalence Driver

Implemented:
- tests/java_source_equivalence.py
- paired Java/SLeeLa fixture under tests/java_equivalence/
- tests/Makefile target java-source-equivalence

The driver reports structural, semantic, and functional-source states separately. It does not claim behavioral equivalence from API envelopes.

### Remaining major implementation queue

1. Expand Java expression AST and parser. **Completed in 0.3.6-dev: expression node taxonomy, operator lexing, precedence parsing, assignments, conditionals, casts, array access, postfix increment/decrement, method references, and constructor arguments.**
2. Expand Java statement AST and parser. **Foundation completed in 0.3.7-dev:** blocks, if/while/do/for, switch, break/continue/return/throw/assert/yield, synchronized, and try/catch/finally. **Remaining:** enhanced-for, local-variable declarations, try-with-resources, switch rules/guards, and full statement semantic constraints.
3. Implement Java type/conversion semantics. **Foundation completed in 0.3.8-dev:** conversion categories/contexts, identity, widening/narrowing primitive conversion, boxing/unboxing, reference conversion classification, and unary/binary numeric promotion.
4. Implement overload and override resolution. **Foundation completed in 0.3.9-dev:** strict/loose/variable-arity phases, applicability classification, most-specific selection foundation, override-equivalence, and basic return compatibility.
5. Implement definite-assignment and reachability rules. **Foundation added in 0.3.12-dev:** source-flow facts, use-before-assignment diagnostics, final reassignment checks, branch merging, loop/abrupt-completion reachability, switch/try/finally flow scaffolding, and deterministic C++/Python qualification suite. **Foundation added in 0.3.12-dev:** source-flow facts, use-before-assignment diagnostics, final reassignment checks, branch merging, loop/abrupt-completion reachability, switch/try/finally flow scaffolding, and deterministic C++/Python qualification suite.
6. Implement checked-exception analysis. **Foundation added in 0.3.13-dev:** exception hierarchy/subtyping, checked-vs-unchecked classification, catch coverage, redundant catch detection, throws-clause coverage, and overriding throws compatibility.
7. Implement dependency-driven Java API counterpart closure. **Foundation added in 0.3.14-dev:** source-reference discovery, Java-qualified dependency-to-envelope mapping, missing-counterpart diagnostics, and deterministic isolated-fixture qualification.
8. **Completed in 0.3.15-dev:** expand negative and constraint fixtures with deterministic expected-diagnostic corpus and qualification runner.
9. **Foundation completed in 0.3.16-dev:** normalized declaration/signature comparison beyond the inventory driver.
10. Add Linux, Windows 10+, and macOS qualification records.


### Overload and override refinement

The 0.3.10-dev refinement expands the 0.3.9-dev foundation toward the Java source rules for method invocation and inheritance:

- generic-method type inference foundation;
- lambda and method-reference pertinence classification;
- a dedicated most-specific comparison model;
- generic type-variable inference constraints;
- generic/subsignature-aware metadata hooks;
- covariant reference return compatibility;
- checked-exception restriction checks for overrides;
- access-level, static, final, and private override restrictions;
- variable-arity applicability;
- a refinement test corpus.

These are source-semantic qualification foundations. Full JLS coverage remains a continuing qualification task, especially for inference constraints, functional-interface target typing, intersection types, bridge/subsignature behavior, and complete interface method inheritance.

Oracle's JLS describes applicability phases, pertinence of implicitly typed lambdas/inexact method references, most-specific selection, and generic inference in the method-invocation rules. It separately defines subsignatures, overriding restrictions, return-type substitutability, and inherited/default-method conflicts. citeturn0search2turn0search8


### 0.3.11-dev overload/override refinement

The next program refinement extends overload and override semantics with:

- maximally-specific candidate selection;
- concrete-versus-abstract/default tie handling;
- preferred covariant return handling for equivalent signatures;
- invocation-type inference after generic method selection;
- target-type compatibility checks for inferred invocation results;
- explicit interface/default-method inheritance conflict detection;
- richer method metadata for abstract/default/interface methods and erased signatures.

This follows the Java SE 27 specification's separation of applicability, most-specific selection, invocation type inference, and interface inheritance rules. citeturn0search0turn0search12

This remains a source-level semantic model rather than a JVM implementation.

### Non-goals for this qualification

Do not add JVM execution requirements to this ledger. Java source congruence is the target; JVM/SLVM interoperability is a separate future project.

**MEARVK LLC — 2026**


### 0.3.12-dev — definite assignment and reachability foundation

Added `impl/frontend/java_flow.h/.cpp`, `tests/java_flow_semantics.cpp`, and `tests/java_flow_suite.py`, plus the `java-flow` Makefile target. The model distinguishes definitely assigned and definitely unassigned facts, reports reads before assignment, checks repeated final assignment, merges conditional paths, tracks abrupt completion, and provides structured flow scaffolding for loops, switch, and try/finally. This is a source-semantic foundation; complete JLS Chapter 16 coverage remains a qualification task.

Oracle's Java Language Specification Chapter 16 defines definite assignment around all possible execution paths and gives dedicated rules for boolean operators and conditional expressions, statements, loops, switch, abrupt completion, and try/finally. citeturn0search1turn0search13


### 0.3.12-dev — definite assignment and reachability foundation

Added `impl/frontend/java_flow.h/.cpp`, `tests/java_flow_semantics.cpp`, and `tests/java_flow_suite.py`, plus the `java-flow` Makefile target. The model distinguishes definitely assigned and definitely unassigned facts, reports reads before assignment, checks repeated final assignment, merges conditional paths, tracks abrupt completion, and provides structured flow scaffolding for loops, switch, and try/finally. This is a source-semantic foundation; complete JLS Chapter 16 coverage remains a qualification task.


### 0.3.13-dev — checked-exception foundation

Added `impl/frontend/java_exceptions.h/.cpp`, `tests/java_exceptions.cpp`, and `tests/java_exceptions_suite.py`, with the `java-exceptions` Makefile target. The model is source-level: it tracks exception type hierarchy, identifies checked exceptions, verifies catch-or-declare coverage, detects shadowed/redundant catches, and checks that overriding methods do not introduce incompatible checked exceptions. Full JLS Chapter 11 propagation rules—including expression/statement-specific exception sets, try/catch/finally propagation, multi-catch, precise rethrow, resource initialization/close exceptions, and generic checked-exception inference—remain to be completed.

Oracle's Java Language Specification defines checked-exception compile-time checking, catch-or-specify requirements, and restrictions on checked exceptions in overriding declarations. citeturn1search0turn1search13


### 0.3.14-dev — Java API dependency closure foundation

Added `lib/java/tools/java_api_dependency_closure.py` and `tests/java_api_dependency_suite.py`, with the `java-api-dependencies` Makefile target. The auditor scans SLeeLa source for Java-qualified API references and verifies that each referenced type maps to a corresponding `/lib/java` envelope without modifying generated or handwritten API sources.

This closes the first dependency-driven layer of API qualification. Full closure still requires member-level dependency extraction, nested/inner types, generic bounds and annotations, inherited API members, module/package metadata, and a dependency graph suitable for complete transitive closure.

The Java statement model also remains an active companion area: the JLS defines enhanced `for`, `try`-with-resources, switch rules/guards, and detailed reachability/normal-completion constraints. citeturn0search0turn0search2


### 0.3.15-dev — negative and constraint corpus

Added `tests/java_constraints/` with deterministic negative/constraint fixtures for definite assignment, final reassignment, unreachable statements, checked exceptions, redundant catches, override throws restrictions, overload ambiguity, interface default conflicts, missing Java API counterparts, and forbidden conversions.

Added `tests/java_constraints_suite.py` and the `java-constraints` Makefile target. The suite validates the fixture-to-diagnostic contract, executes the existing flow/exception/overload qualification implementations, and verifies a deliberate missing Java API counterpart is rejected by the dependency-closure auditor.

The corpus is source-level qualification data. It does not add JVM or SLVM execution requirements.

Oracle's Java SE 27 JLS defines compile-time constraint areas including definite assignment, exception checking, method overriding/inheritance, and related source-language rules. citeturn0search2turn0search8

### 0.3.16-dev — normalized AST/signature comparison foundation

Added `tests/java_signature_comparison.py` and a deterministic qualification suite with paired Java/SLeeLa fixtures. The comparator normalizes declaration kinds, names, modifiers, generic parameter shape, parameter types, return types, throws types, and fields, then reports missing/extra normalized structures.

This is the first comparison layer beyond the inventory driver's marker-based comparison. It remains source/API congruence tooling and does not claim JVM descriptor or runtime equivalence.

Oracle's Java SE 27 specification defines Java source syntax and declaration/signature rules; generic signatures and type structure remain distinct from runtime behavior. citeturn0search0turn0search4

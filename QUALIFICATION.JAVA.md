# QUALIFICATION.JAVA.md

## Java Authorship → SLeeLa Source/API Qualification

**Status:** Active compatibility qualification plan  
**Target:** Java SE / JDK 28 source and API compatibility boundary  
**SLeeLa Syntax:** 1.6  
**Java Authorship Transition Gate:** 1.3-dev

## Scope

This qualification establishes **source-level and API-level congruence** between Java authorship and its SLeeLa counterpart.

It does **not** require SLeeLa to implement a JVM, execute Java bytecode, provide an SLVM, or reproduce JVM runtime internals.

Java keywords, modifiers, annotations, and Java API symbols used by this work are **Java compatibility vocabulary**. They are not part of the native or advanced SLeeLa symbol inventory. See `JAVA.COMPATIBILITY.SCOPE.md`.

The Java Language Specification defines the relevant Java source-level lexical, type, declaration, class, interface, annotation, module, and related rules. citeturn0search0

## Qualification gates

### Q0 — Java source/API inventory
- Pin the exact Java specification/API release used for the qualification.
- Inventory Java packages, types, members, nested types, annotations, and status classifications.
- Inventory corresponding SLeeLa declarations under `lib/java`.
- Keep Java compatibility counts separate from native SLeeLa symbol counts.

### Q1 — Lexical compatibility
- Recognize the Java vocabulary required for compatibility.
- Preserve identifiers, qualified names, literals, operators, separators, annotations, and generic syntax.
- Explicitly classify compatibility tokens as compatibility vocabulary in compiler/tooling documentation.
- Do not add compatibility tokens to the native SLeeLa symbol inventory merely because the lexer recognizes them.

### Q2 — Declaration compatibility
- Preserve class, interface, enum, record, and annotation-interface identity.
- Preserve Java modifiers and their constraints.
- Preserve superclass, interfaces, type parameters, fields, methods, constructors, parameters, thrown types, and qualified names.
- Preserve source/API status such as standard, preview, incubator, and internal.

### Q3 — Generic/signature compatibility
- Preserve type parameters and bounds.
- Preserve parameterized, nested, wildcard, extends, super, generic method, generic constructor, intersection, and array types.
- Preserve type-use annotations in signatures.
- Produce deterministic normalized signatures.

### Q4 — Annotation compatibility
- Preserve marker, normal, single-element, and repeated annotations.
- Preserve declaration and type-use locations, including type parameters, parameters, fields, methods, constructors, record components, packages, and modules.
- Preserve annotation element values and defaults.
- Preserve Java Target and Retention semantics as source/API metadata where claimed.
- Keep Java annotation interfaces distinct from native SLeeLa annotation vocabulary.
Java defines annotation interfaces as a specialized interface form and defines declaration/type-use annotation contexts separately. citeturn0search12turn0search7

### Q5 — Java type/member constraints
- Validate access modifiers and mutually exclusive modifiers.
- Validate inheritance/interface relationships.
- Validate overriding/overloading/hiding constraints that are claimed as compatible.
- Validate constructor identity.
- Validate record, enum, sealed/non-sealed, and annotation-interface restrictions.
- Validate varargs, throws declarations, covariant return declarations, and generic bounds where supported.
- Preserve unsupported Java constraints as explicit qualification gaps rather than silently treating them as native SLeeLa semantics.

### Q6 — Java API counterpart completeness
For every claimed Java API counterpart under `lib/java`:
- package and qualified type name
- type kind
- modifiers
- superclass
- interfaces
- type parameters
- fields/constants
- constructors
- methods
- parameter types/names where available
- return types
- thrown types
- nested types
- annotations
- documented API status

API existence and API signature compatibility are separate gates.

### Q7 — Native/compatibility symbol separation
- Maintain a machine-readable distinction between native SLeeLa symbols and Java compatibility symbols.
- Native SLeeLa symbol indexes must not count Java compatibility keywords or Java API counterpart names as native symbols.
- IDE completion and documentation should label Java compatibility entries.
- Class-count documents may report Java counterpart counts separately.
- New Java compatibility additions must not silently change the reported native SLeeLa symbol count.

### Q8 — Source-to-source congruence
For every qualified construct:
1. Java source fixture.
2. Corresponding SLeeLa source fixture.
3. Lexer/token expectation.
4. AST/declaration metadata expectation.
5. Semantic constraint expectation.
6. API/signature comparison where applicable.
7. Deterministic PASS/FAIL result.

### Q9 — Negative/constraint corpus
Maintain invalid fixtures proving that claimed Java constraints are enforced, including:
- illegal modifier combinations
- invalid sealed/non-sealed relationships
- invalid constructor identity
- invalid annotation targets
- invalid generic bounds
- invalid overriding/signature combinations
- unsupported constructs with explicit diagnostics

### Q11 — Eight-layer source equivalence model
- Q1 Lexical, Q2 Type System, Q3 Declarations, Q4 Expressions, Q5 Statements, Q6 Semantic Constraints, Q7 API Counterparts, and Q8 Source Equivalence Testing are represented by explicit frontend equivalence types in impl/frontend/java_equivalence.h.
- The source-equivalence driver must report structural, semantic, and functional-source states separately.
- Functional-source status must remain INCOMPLETE unless the corresponding source/API semantics have actually been implemented and checked.

### Q10 — Platform and reproducibility
Frontend/API qualification should run independently of JVM availability.

Where native SLeeLa compilation is supported, qualify Linux, Windows 10+, and macOS.

Every qualification record should identify:
- SLeeLa commit
- SLeeLa version
- syntax version
- Java specification/API version
- OS and architecture
- compiler/toolchain
- test-suite revision
- pass/fail/skipped counts
- unsupported features
- normalized comparison output

## Deliberately outside this qualification

The following are **not required to establish Java/SLeeLa source/API congruence**:
- JVM implementation
- Java bytecode interpreter
- SLVM
- JVM stack/frame model
- JVM garbage collector
- JVM class-loader implementation
- JVM monitor implementation
- JNI runtime compatibility
- JVM TI implementation
- JDWP implementation
- Java bytecode verification/execution

A future Java interoperability project may address those independently.

## Completion gates

- [ ] Q0 Java source/API inventory
- [ ] Q1 Lexical compatibility
- [ ] Q2 Declaration compatibility
- [ ] Q3 Generic/signature compatibility
- [ ] Q4 Annotation compatibility
- [ ] Q5 Java type/member constraints
- [ ] Q6 Java API counterpart completeness
- [ ] Q7 Native/compatibility symbol separation
- [ ] Q8 Source-to-source congruence
- [ ] Q9 Negative/constraint corpus
- [ ] Q10 Platform/reproducibility
- [ ] Q11 Eight-layer equivalence model
- [ ] Native SLeeLa symbol inventory remains independent
- [ ] Java compatibility inventory is separately reportable

## Current position

SLeeLa has substantial lexical/declaration/generic/annotation infrastructure. The immediate objective is to make the distinction between the **native SLeeLa language** and the **Java compatibility surface** explicit in source, documentation, tests, symbol accounting, and developer tooling.

## Final rule

**Java compatibility vocabulary exists so SLeeLa can faithfully represent Java authorship. It does not redefine what SLeeLa is.**

**A Java keyword is not automatically a SLeeLa-native keyword. A Java annotation is not automatically a native SLeeLa annotation. A Java API class is not automatically a native SLeeLa class.**
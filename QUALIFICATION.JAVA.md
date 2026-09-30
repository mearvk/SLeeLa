# QUALIFICATION.JAVA.md

## Java Authorship → SLeeLa Runtime Qualification

**Status:** Transition qualification plan  
**Target:** Java SE / JDK 28 compatibility boundary  
**SLeeLa Syntax:** 1.6  
**Java Authorship Transition Gate:** 1.2-dev

JDK 28 is currently a draft/early-access specification. Every qualification run must pin the exact JDK 28 specification/build used. The JDK 28 specification set covers the JLS, JVMS, API, JDK tools, JAR, JNI, serialization, JDWP, JPDA, RMI, security algorithms, Javadoc, and JVM TI. citeturn0search0

## Qualification rule

The transition is not qualified merely because Java-shaped source compiles. Java authorship must survive the lexer, parser, AST, semantic compiler, runtime, class-file boundary, libraries, and tooling. Behavioral equivalence must be tested independently from API-envelope existence.

## Qualification gates

### Q0 — Source inventory
- Pin the JDK version/build.
- Inventory Java packages, types, members, nested types, and preview/incubator APIs.
- Inventory the corresponding SLeeLa source under lib/java.
- Produce deterministic missing-type and duplicate/conflict reports.

### Q1 — Lexical preservation
- Preserve identifiers, qualified names, literals, operators, separators, and required source comments.
- Support annotations, generic syntax, wildcards, arrays, records, sealed declarations, and enabled preview syntax.
- Maintain deterministic tokenization.

### Q2 — Declaration metadata
- Preserve class, interface, enum, record, annotation, and value-type identity.
- Preserve public/protected/private/static/final/abstract/native/synchronized/volatile/transient/strictfp/sealed/non-sealed/default modifiers.
- Preserve superclass, interfaces, type parameters, fields, methods, constructors, parameters, thrown types, synthetic/bridge metadata, and qualified names.
- Preserve API status: standard, preview, incubator, and internal.
- Preserve declaration and type-use annotations.

### Q3 — Generic signatures
- Preserve type parameters and multiple bounds.
- Preserve parameterized, nested, wildcard, extends, super, generic method, generic constructor, and generic-array types.
- Preserve type-use annotations within signatures.
- Generate and compare deterministic normalized signatures.
- Reconcile source signatures with class-file Signature metadata. The JVM class-file format defines Signature as a standard metadata attribute. citeturn0search2

### Q4 — Annotation qualification
- Preserve marker, normal, single-element, and repeated annotations.
- Preserve declaration, type-use, type-parameter, parameter, field, method, constructor, record-component, package, and module annotations.
- Preserve primitive, String, class, enum, annotation, and array-valued elements.
- Preserve default annotation values.
- Implement and test Retention and Target semantics.
- Distinguish runtime-visible and runtime-invisible annotations.
- Preserve annotation defaults and type-annotation metadata.
The JVM specification defines runtime-visible/invisible annotation, parameter-annotation, type-annotation, and AnnotationDefault attributes. citeturn0search2

### Q5 — Java type/member semantics
- Qualify overload resolution, overriding, hiding, access control, static/instance dispatch, constructors, initialization order, final fields, interface defaults, covariant returns, bridge methods, varargs, enums, records, nested classes, sealed hierarchies, and supported preview/value-class behavior.
- Pin preview features to the exact JDK build because JDK 28 includes evolving preview work. citeturn0search3

### Q6 — Expressions and execution
- Qualify primitive operations, conversions, assignment, control flow, pattern matching, switch, exceptions, try/catch/finally, try-with-resources, assertions, lambdas, method references, functional interfaces, local/anonymous classes, concurrency, memory visibility, initialization, and termination.
- Compare observable behavior against a reference Java execution.

### Q7 — Exceptions and errors
- Preserve exception type, cause, suppressed exceptions, stack information, checked throws declarations, catch compatibility, finally behavior, initialization errors, linkage errors, class-loading errors, and reflection errors.

### Q8 — Object identity and class metadata
- Implement object identity, null, arrays, class identity, Class metadata, identity/hash behavior, lifecycle, initialization, synchronization association where applicable, and supported identity/value-object semantics.
- Reflection-visible Class metadata must be derived from the actual runtime definition rather than a source envelope. Java Class objects expose characteristics originating from the class definition and loading environment. citeturn0search9

### Q9 — Reflection
- Qualify classes, constructors, methods, fields, modifiers, annotations, generic signatures, parameters, records, sealed classes, modules, accessibility, invocation, and arrays.
- Reflection results must agree with the qualified Java reference.

### Q10 — Class-file boundary
- Implement parsing, generation, validation, and deterministic round-tripping.
- Cover CAFEBABE, versions, constant pool, access flags, classes, interfaces, fields, methods, descriptors, Code, StackMapTable, Signature, Exceptions, BootstrapMethods, annotations, type annotations, parameter annotations, records, permitted subclasses, modules, nests, synthetic/deprecated metadata, and source/debug metadata.
- Preserve or explicitly reject unsupported attributes deterministically.
- Integrate structural validation and verification.
The JVMS class-file is structured around the constant pool, flags, class/interface references, fields, methods, and attributes. citeturn0search2
- Include compatibility coverage for the JDK 28 java.lang.classfile API, which provides class-file parsing, generation, and transformation models. citeturn0search7

### Q11 — Loading, linking, initialization
- Qualify defining/initiating loader identity, loading, verification, preparation, resolution, initialization, circularity, linkage errors, loader constraints, and supported hidden classes.

### Q12 — Modules and services
- Preserve module declarations, requires, exports, opens, uses, provides, versions, annotations, layers, services, and access checks.
- Qualify module graphs and service resolution.

### Q13 — JNI/native boundary
- Qualify native declarations, JNI names/signatures, primitive/reference conversion, object references, local/global/weak references, exceptions, thread attachment, native library loading, and lifecycle.
- Run native qualification on every supported operating system.

### Q14 — Serialization
- Qualify Serializable, Externalizable, serialVersionUID, serial fields, custom read/write methods, object graphs, cycles, enums, records, class evolution, compatibility failures, and security-sensitive paths.
- Test both compatible and incompatible class evolution. Java serialization explicitly treats class identity and compatible evolution as part of its contract. citeturn0search4turn0search8

### Q15 — Tooling and debugging
- Qualify JAR, Javadoc, diagnostics, source mapping, line/local-variable metadata, JDWP, JPDA, JVM TI, debugger-visible metadata, and generated-source metadata.

### Q16 — Java SE API compatibility
- Compare every claimed JDK API type against the pinned API inventory.
- Verify package/type identity, modifiers, constructors, methods, fields, generic signatures, exceptions, annotations, nested types, inherited members, and preview/incubator status.
- Keep API-envelope tests separate from behavioral-equivalence tests.

## Evidence required for every feature

Each qualified feature must have:
1. A SLeeLa fixture.
2. An equivalent Java fixture.
3. Expected lexer/parser or AST metadata.
4. Expected runtime behavior.
5. Expected failure behavior where applicable.
6. Normalized comparison output.
7. CI coverage.

## CI qualification matrix

- Lexer corpus
- Parser corpus
- Declaration metadata
- Generic signatures
- Annotation semantics
- Java semantic fixtures
- Exception/error behavior
- Reflection
- Class-file round trips and validation
- Loading/linking/initialization
- Modules/services
- JNI on supported platforms
- Serialization
- JDK API inventory
- API signature synchronization
- Strict behavioral equivalence

Strict behavioral-equivalence CI must fail when an implementation is only a declaration contract or API envelope.

## Platform qualification

Where runtime/native integration is involved, qualify Linux, Windows 10+, and macOS. Frontend and class-file tests should run independently of GUI availability.

## Required qualification record

Every run records:
- SLeeLa commit and branch
- SLeeLa version
- syntax version
- exact JDK version/build
- operating system and architecture
- compiler/toolchain
- enabled preview features
- test-suite revision
- pass/fail/skipped counts
- unsupported features
- generated artifacts
- normalized comparison report

## Completion gates

- [ ] Q0 Source inventory
- [ ] Q1 Lexical preservation
- [ ] Q2 Declaration metadata
- [ ] Q3 Generic signatures
- [ ] Q4 Annotation qualification
- [ ] Q5 Type/member semantics
- [ ] Q6 Expression/execution semantics
- [ ] Q7 Exceptions/errors
- [ ] Q8 Object identity/class metadata
- [ ] Q9 Reflection
- [ ] Q10 Class-file boundary
- [ ] Q11 Loading/linking/initialization
- [ ] Q12 Modules/services
- [ ] Q13 JNI
- [ ] Q14 Serialization
- [ ] Q15 Tooling/debugging
- [ ] Q16 Java SE API compatibility
- [ ] [Cross-platform qualification](#platform-qualification)
- [ ] Strict behavioral-equivalence CI
- [ ] Reproducible qualification report

## Current position

SLeeLa currently has substantial Q0–Q4 infrastructure: JDK API inventory/synchronization, lexer/parser preservation, declaration metadata, generic-signature preservation, and source-level annotation preservation. The next major work is to turn the remaining semantic and runtime boundaries into executable implementations and conformance tests.

## Final qualification rule

**Source representation is qualified when Java authorship can be reconstructed.**

**Runtime compatibility is qualified only when observable Java behavior is reproduced.**

**The complete Java → SLeeLa transition is qualified only when source, semantics, runtime metadata, class files, loading, reflection, modules, native integration, serialization, tooling, and API behavior all have executable evidence.**
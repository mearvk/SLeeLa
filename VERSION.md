# SLeeLa Version

## Current Development Version

**SLeeLa:** 0.3.21-dev
**Sleela-Complete:** 0.3.2-dev  
**Nordshrift Complete:** 2.7.1-dev  
**Native Foundation:** 0.3.2-dev  
**Sleela Language Syntax:** 1.6 (supported range 1.3 .. 1.6)  
**Compiler Compatibility Gate:** 2.8-dev  
**Java Authorship Transition Gate:** 1.19-dev
**Edition:** SLeeLa Complete / Native Foundation  
**Status:** Active Development  
**Repository:** mearkv/SLeeLa

## 0.3.9 Development Increment

This increment advances Java source-equivalence semantics with overload and override resolution foundations: applicability phases, conversion-aware candidate filtering, most-specific selection, override-equivalence, and basic return compatibility.

### Java compatibility boundary

- Java compatibility keywords and modifiers are explicitly treated as authorship-preservation vocabulary.
- Java annotations are explicitly treated as compatibility metadata rather than native SLeeLa annotations.
- Java API counterparts under `lib/java` are explicitly separated from native SLeeLa symbols.
- Java compatibility counts are required to remain separate from native SLeeLa symbol counts.
- Added `JAVA.COMPATIBILITY.SCOPE.md`.
- Added a Java compatibility boundary fixture.
- Updated `QUALIFICATION.JAVA.md` to focus on source/API congruence rather than JVM/SLVM execution.
- Clarified that Java compatibility does not require an SLVM.

The Java Language Specification provides the source-level reference for Java lexical, type, declaration, class, interface, annotation, and related constructs. citeturn0search0

**SLeeLa — MEARVK LLC — 2026**

## 0.3.11 Development Increment

This increment refines Java overload and override semantics beyond the 0.3.9 foundation. It adds generic-method inference foundations, lambda/method-reference pertinence, dedicated most-specific comparison, covariant reference return checks, checked-exception restrictions, access/static/final/private override restrictions, and variable-arity applicability.

This remains a source-level Java congruence implementation; it is not a JVM or SLVM implementation. Full Java overload and override qualification remains subject to the JLS rules for generic inference, functional target typing, subsignatures, interface/default inheritance, and related corner cases.

**Java Authorship Transition Gate:** 1.12-dev


## 0.3.11 Development Increment

This increment extends the Java overload/override foundation with maximally-specific selection, generic invocation-type inference, target-type compatibility, concrete/abstract/default tie handling, covariant return preference, and interface/default-method conflict detection.

**Java Authorship Transition Gate:** 1.12-dev


## 0.3.12 Development Increment

This increment adds the Java source-level definite-assignment and reachability foundation: flow facts, use-before-assignment diagnostics, final reassignment checks, branch joins, abrupt completion, loop/switch/try-finally scaffolding, and a deterministic test suite. Full JLS Chapter 16 qualification remains in progress.

**Java Authorship Transition Gate:** 1.12-dev


## 0.3.13 Development Increment

Java checked-exception source semantics foundation added: exception hierarchy/subtyping, checked-vs-unchecked classification, catch-or-declare coverage, redundant catch detection, and overriding throws compatibility. Full Java exception propagation remains in qualification.

**Java Authorship Transition Gate:** 1.12-dev


## 0.3.14 Development Increment

Java API dependency-closure foundation added: Java-qualified source-reference discovery, counterpart path mapping, missing-type diagnostics, and deterministic isolated-fixture testing.

**Java Authorship Transition Gate:** 1.12-dev


## 0.3.15 Development Increment

Java negative/constraint qualification corpus added: ten deterministic source fixtures with explicit expected diagnostic families, a manifest, a qualification runner, and a `java-constraints` Makefile target. The suite connects the corpus to the existing flow, checked-exception, overload/override, and Java API dependency-closure foundations.

**Java Authorship Transition Gate:** 1.13-dev

## 0.3.16 Development Increment

Normalized Java/SLeeLa declaration and signature comparison added. The qualification tool compares normalized declarations, methods, constructors, generic parameter shape, parameter/return types, throws types, modifiers, and fields using a deterministic paired-source fixture.

**Java Authorship Transition Gate:** 1.14-dev


## 0.3.17 Development Increment

Normalized Java/SLeeLa declaration comparison was deepened from the 0.3.16 foundation. The comparison normal form now distinguishes constructors from methods, records declaration ownership/path metadata, inheritance clauses (extends, implements, permits), annotations, parameter names/types/varargs metadata, and a versioned normalization schema. The qualification suite now includes a deliberate constructor-parameter mismatch to prove that mismatches are detected rather than merely accepting paired text.

**Java Authorship Transition Gate:** 1.15-dev


## 0.3.18 Development Increment

Added the Java source-equivalence platform/reproducibility qualification system for Q10. Linux, Windows 10+, and macOS now have explicit qualification records covering architecture families, toolchain expectations, required Python tooling, and deterministic qualification commands. The runner records observed host evidence and distinguishes READY from NOT_EXECUTED rather than claiming untested remote platforms have passed.

**Java Authorship Transition Gate:** 1.16-dev


## 0.3.19 Development Increment

Added the unified Java qualification manifest, which coordinates the existing source-equivalence, expression, normalized signature, constraint, flow, exception, API-dependency, and platform qualification layers. Each child result remains visible, while the aggregate record provides deterministic counts, environment metadata, and an overall status.

**Java Authorship Transition Gate:** 1.17-dev


## 0.3.20 Development Increment

The next semantic qualification layer hardens Java control-flow and checked-exception analysis. The source-semantic work remains independent of JVM or SLVM execution and is being extended toward complete Java definite-assignment, abrupt-completion, and exception-flow joins.

**Java Authorship Transition Gate:** 1.18-dev


## 0.3.21 Development Increment

Java flow qualification advanced toward branch-sensitive source semantics. The flow model now exposes true/false branch facts, handles boolean short-circuit and negation structure, validates labeled and unlabeled break/continue targets, and uses loop exit facts when determining normal completion.

This remains source/API qualification only; JVM, bytecode, SLVM, and runtime equivalence are outside this increment.

**Java Authorship Transition Gate:** 1.19-dev

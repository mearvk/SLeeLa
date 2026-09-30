# Java Authorship Transition

SLeeLa now preserves Java authorship metadata through the lexer, parser, AST, and compiler boundary instead of reducing Java declarations to a generic envelope.

## Current capabilities

- class, interface, enum, and record type identity
- Java access and implementation modifiers
- sealed, non-sealed, strictfp, native, synchronized, volatile, transient, final, abstract, and default metadata
- superclass and implemented/super interfaces
- qualified reference types and array dimensions
- explicit constructor identity
- declared thrown exception types
- JDK 28 source/API status metadata
- preview, incubator, and internal classification fields
- Java compatibility manifest counters
- compiler-time metadata validation

## Transition boundary

The compiler validates Java metadata before ordinary SLeeLa lowering. Metadata preservation is not treated as behavioral equivalence. Runtime bindings must implement the observable semantics separately.

The remaining runtime layers include object identity, class metadata, reflection, class loading/linking, exceptions, synchronization and memory visibility, modules, JNI, serialization, Java class-file parsing/verification/transformation, tooling/debug interfaces, and reference-vs-SLeeLa behavioral tests.

## Class-file boundary

JDK 28 defines Java class-file major version 72, with the preview minor-version convention. SLeeLa therefore needs a dedicated class-file compatibility layer in addition to source parsing.

## Version

SLeeLa: 0.3.2-dev

SLeeLa syntax: 1.4

Java Authorship Transition Gate: 1.0-dev

These versions describe the implemented source/metadata capability, not completion of Java behavioral compatibility.

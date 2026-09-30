# SLeeLa Version

## Current Development Version

**SLeeLa:** 0.3.3-dev  
**Sleela-Complete:** 0.3.2-dev  
**Nordshrift Complete:** 2.7.1-dev  
**Native Foundation:** 0.3.2-dev  
**Sleela Language Syntax:** 1.5 (supported range 1.3 .. 1.5)  
**Compiler Compatibility Gate:** 2.8-dev  
**Java Authorship Transition Gate:** 1.1-dev  
**Edition:** SLeeLa Complete / Native Foundation  
**Status:** Active Development  
**Repository:** mearkv/SLeeLa

## 0.3.2 Development Increment

This increment expands the SLeeLa front end for Java-authorship transition. The lexer and parser now preserve Java type kind, modifiers, inheritance/interfaces, constructors, thrown types, array type identity, source/API status metadata, preview/internal/incubator markers, and member metadata instead of discarding these distinctions.

The compiler now validates this Java transition metadata before lowering. This is a representation and semantic-boundary capability; it does not claim behavioral equivalence until the corresponding SLeeLa runtime binding and conformance test exist.

### Java transition foundations

- Java-aware lexer vocabulary: class/interface/enum/record and Java modifiers.
- Qualified reference types and array dimensions.
- extends, implements, and throws metadata.
- Explicit constructor identity.
- Java type/member modifier preservation.
- JDK 28 source/API status metadata.
- Preview, incubator, internal, and standard API classification fields.
- Java compatibility manifest counters.
- Compile-time Java metadata validation.
- Backward-compatible SLeeLa syntax 1.4.

### Remaining Java transition gates

- complete Java language semantics
- generic type/signature preservation
- annotations and annotation values at type/member/parameter level
- complete exception semantics
- object identity and class metadata
- reflection and class loading
- synchronization and Java memory semantics
- Java class-file parsing/linking/verification
- modules and module layers
- JNI/native boundary
- serialization
- JDK tool/debug interfaces
- behavioral runtime bindings and reference conformance

JDK 28 specifications are draft/early-access material, so the compatibility layer records version/status metadata rather than treating preview material as permanently standardized.

---

**SLeeLa — MEARVK LLC — 2026**
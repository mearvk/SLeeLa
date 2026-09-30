# SLeeLa Version

## Current Development Version

**SLeeLa:** 0.3.8-dev  
**Sleela-Complete:** 0.3.2-dev  
**Nordshrift Complete:** 2.7.1-dev  
**Native Foundation:** 0.3.2-dev  
**Sleela Language Syntax:** 1.6 (supported range 1.3 .. 1.6)  
**Compiler Compatibility Gate:** 2.8-dev  
**Java Authorship Transition Gate:** 1.7-dev  
**Edition:** SLeeLa Complete / Native Foundation  
**Status:** Active Development  
**Repository:** mearkv/SLeeLa

## 0.3.8 Development Increment

This increment advances Java source-equivalence semantics with explicit conversion categories and contexts, primitive widening/narrowing, boxing/unboxing classification, reference conversion classification, and unary/binary numeric promotion.

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
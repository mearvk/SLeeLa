# Trophy Edition — Java Source Support

## Purpose

The **Trophy Edition** is reserved for the later Java-authorship phase of SLeeLa.

Trophy will inherit the Java source-level support developed for the Original SLeeLa implementation. The objective is to allow SLeeLa to represent Java-authored source congruently through compatible syntax, symbols, declarations, types, expressions, statements, constraints, API counterparts, and semantic analysis.

This is a **source-language and source/API congruence target**. It does not require a JVM, Java bytecode execution, or an SLVM.

## Deferred Work

Trophy Java support is intentionally scheduled for work **after the Original SLeeLa has established the Java code-support foundation**.

The later Trophy implementation should incorporate:

1. Java compatibility vocabulary, kept separate from native SLeeLa symbols.
2. Java declarations, modifiers, annotations, generic signatures, and type-use metadata.
3. Java expression and statement coverage.
4. Java conversion semantics.
5. Java overload and override resolution.
6. Definite-assignment and definite-unassignment analysis.
7. Reachability analysis.
8. Checked-exception analysis.
9. Java API counterpart coverage and dependency closure.
10. Negative and constraint fixtures.
11. Source-equivalence comparison and qualification tooling.

## Definite Assignment and Reachability

Trophy is specifically reserved to consume the completed Original SLeeLa implementation of Java flow semantics rather than creating a parallel, incompatible implementation.

The Java Language Specification defines definite assignment as a compile-time property: local variables and blank final fields must be definitely assigned before value access, and blank final variables must be definitely unassigned before assignment. The rules are structural and cover expressions and statements including conditional operators, blocks, if, switch, loops, break, continue, return, throw, synchronized, and try. citeturn0search1turn0search2

Trophy should therefore consume the Original SLeeLa flow-analysis model once that model is complete, including:

- definitely-assigned facts;
- definitely-unassigned facts;
- normal-completion paths;
- abrupt-completion paths;
- reachability;
- loop back-edges;
- break and continue targets;
- return, throw, and yield exits;
- try/catch/finally flow;
- conditional-expression flow;
- switch flow;
- final-field assignment constraints.

## Symbol Boundary

Trophy must preserve the established three-domain boundary:

### Native SLeeLa

Actual SLeeLa language symbols and semantics.

### Java Compatibility Vocabulary

Java keywords, modifiers, annotations, and source-level constructs retained so Java-authored source can be represented. These are **not native SLeeLa symbols**.

### Java API Counterparts

SLeeLa declarations representing Java API types and members. These are compatibility counterparts, not claims that SLeeLa is a JVM implementation.

Recognition of Java vocabulary does not transfer ownership of that vocabulary to the native SLeeLa language.

## Sequencing Rule

The implementation sequence is:

**Original SLeeLa Java support → complete Java source-equivalence semantics → qualify flow/constraint behavior → Trophy Edition integration.**

Trophy should not become a competing implementation of Java semantics while the Original SLeeLa foundation is still being completed.

## Scope Boundary

Trophy Java support does **not** make the following prerequisites:

- JVM execution;
- Java bytecode generation;
- Java bytecode verification;
- JVM garbage collection;
- JVM class loading;
- JNI runtime compatibility;
- JVM debugging protocols;
- SLVM execution.

Those are separate interoperability/runtime projects and are outside this edition's source-congruence requirement.

## Qualification Reference

The authoritative Java language reference for this work is the current Java SE 27 Java Language Specification. Oracle's specification identifies Chapter 16 as the source for definite-assignment analysis and its statement/expression rules. citeturn0search2turn0search5

**SLeeLa — MEARVK LLC — 2026**

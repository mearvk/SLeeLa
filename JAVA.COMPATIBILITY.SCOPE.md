# JAVA.COMPATIBILITY.SCOPE.md

## Purpose

SLeeLa provides a **Java Compatibility Surface** so Java-authored APIs and source constructs can have congruent SLeeLa declarations.

This compatibility surface is **not the SLeeLa language itself**.

Java keywords, Java modifiers, Java annotation syntax, Java package names, and Java API symbols that appear in SLeeLa source for this purpose must not be interpreted by SLeeLa developers as the base SLeeLa symbol vocabulary or as the advanced SLeeLa symbol vocabulary.

## Three distinct symbol domains

### 1. SLeeLa Native Symbols

These are the symbols that define SLeeLa itself: native syntax, types, operations, contextual built-ins, protocols, libraries, and advanced language facilities documented by the SLeeLa project.

**This is the language SLeeLa developers should learn as SLeeLa.**

### 2. Java Compatibility Symbols

These are symbols retained or introduced so that a Java-authored declaration can be represented congruently in SLeeLa.

Examples include: class, interface, enum, record, extends, implements, throws, public, protected, private, static, final, abstract, native, synchronized, volatile, transient, strictfp, sealed, non-sealed, default, Java annotation forms beginning with @, Java-compatible generic/signature notation, and qualified Java API namespaces represented in lib/java.

Their presence establishes a **Java compatibility meaning**. It does not promote the corresponding Java vocabulary into the native SLeeLa symbol set.

### 3. Java API Counterparts

The source declarations under lib/java represent Java API authorship. A file such as lib/java/java/lang/Object.sleela is a SLeeLa representation of the Java API type java.lang.Object.

It is not a statement that Object, its Java members, or the Java API namespace constitute native SLeeLa vocabulary.

## Explicit non-confusion rule

When reviewing SLeeLa source, documentation, examples, IDE completion, symbol indexes, or language references:

> **Java compatibility vocabulary MUST be labeled as Java compatibility vocabulary and MUST NOT be counted as native SLeeLa symbols.**

A Java-compatible keyword or annotation can therefore be lexically recognized, parsed, stored in the AST, semantically validated, emitted in a Java compatibility manifest, represented in lib/java, and tested for Java congruence without becoming part of the base or advanced SLeeLa symbol inventory.

## Annotation boundary

Java annotations are retained so SLeeLa can represent Java authorship. They do not define the native SLeeLa annotation vocabulary.

For example, @Deprecated in a Java-compatibility declaration means that the declaration preserves the Java annotation authored as java.lang.Deprecated. It does not mean that Deprecated is a native SLeeLa language keyword or native SLeeLa concept.

The Java specification distinguishes annotation interfaces, declaration contexts, and type-use contexts; SLeeLa preserves those distinctions for compatibility qualification. citeturn0search0turn0search12

## Keyword boundary

The SLeeLa lexer may recognize Java-compatible keywords because the parser must preserve Java authorship.

**Recognition is not ownership.**

The implementation should therefore treat these as separate concepts:

- native SLeeLa symbol → belongs to the SLeeLa language
- Java compatibility token → recognized to preserve Java authorship
- Java API symbol → represented to provide an equivalent Java API declaration

This distinction must remain visible in compiler documentation and symbol tooling.

## Qualification rule

Java compatibility qualification asks whether a Java declaration can be represented; whether its names, kinds, modifiers, relationships, signatures, annotations, and constraints are preserved; whether the corresponding SLeeLa declaration can be compiled and semantically checked; whether the corresponding API symbol exists; and whether unsupported constructs are identified explicitly.

It does **not** ask whether SLeeLa implements the JVM. SLeeLa does not need an SLVM merely to establish source/API congruence.

## Symbol inventory rule

Future symbol-count documents should maintain separate counts for:
- Native SLeeLa symbols
- Java compatibility keywords/tokens
- Java compatibility annotations
- Java API counterpart types
- Java API counterpart members
- Other imported/foreign compatibility vocabularies

A Java compatibility addition must not silently inflate the reported native SLeeLa symbol count.

## Developer-facing language

Use these terms consistently:
- **SLeeLa native symbol**
- **Java compatibility symbol**
- **Java API counterpart**
- **Java authorship metadata**
- **Java compatibility surface**

Avoid describing a Java keyword as simply a “SLeeLa keyword” unless the context explicitly says **Java compatibility keyword**.

## Relationship to the JLS

The Java Language Specification defines Java lexical grammar, keywords, types, declarations, classes, interfaces, annotation interfaces, annotations, modules, and related source-level rules. citeturn0search0

SLeeLa uses those rules as a compatibility reference where Java congruence is claimed. They do not replace or redefine the SLeeLa language specification.

## Completion criterion

The Java compatibility surface is complete when the claimed Java source/API inventory has corresponding SLeeLa declarations and constraints with clear qualification evidence.

The SLeeLa language remains independently defined by its native symbol set and native semantics.

---

**SLeeLa — MEARVK LLC — 2026**

## 0.3.21 Development Upgrade — Java Flow Qualification Closure

This upgrade turns the next semantic layer into an explicit implementation program. The Java/SLeeLa source-equivalence model now treats control-flow qualification as a first-class closure task rather than a collection of isolated checks.

### Affected SLeeLa source

- `impl/frontend/java_flow.h` / `java_flow.cpp` — definite-assignment, definite-unassignment, reachability, abrupt-completion, branch and loop joins.
- `impl/frontend/java_exceptions.h` / `java_exceptions.cpp` — checked-exception hierarchy, catch coverage, redundant/invalid catches, and override `throws` compatibility.
- `impl/frontend/java_equivalence.h` / `java_equivalence.cpp` — Java source/API normalization and semantic descriptors consumed by the qualification layer.

### Affected Java-facing qualification documents

- `QUALIFICATION.JAVA.md` — Q5/Q8/Q9/Q10 scope and completion requirements.
- `JAVA.EQUIVALENCE.TASKS.md` — semantic closure milestones.
- `JAVA.COMPATIBILITY.SCOPE.md` — Java compatibility vocabulary remains separate from native SLeeLa vocabulary.
- `JAVA.AUTHORSHIP.TRANSITION.md` — Java-authored source remains the compatibility target; JVM/SLVM execution remains outside this gate.
- `tests/JAVA.QUALIFICATION.MANIFEST.md` — unified evidence model.

### Required semantic coverage

The next executable fixtures must cover:

1. boolean-path-sensitive `&&`, `||`, `!`, and `?:` assignment facts;
2. constant boolean expressions;
3. `break`/`continue` target validation and loop exit joins;
4. `while`, `do`, and `for` normal-completion rules;
5. traditional and modern `switch` completion;
6. `try`, `catch`, `finally`, and abrupt completion joins;
7. constructor and blank-final field definite-unassignment rules;
8. lambda/capture flow boundaries;
9. labeled control-flow targets;
10. checked-exception propagation through the same statement structure.

The Java Language Specification defines definite assignment in terms of every possible execution path and gives special treatment to `&&`, `||`, `!`, `?:`, and boolean constants. It also defines the consequences of abrupt completion for flow analysis. citeturn0search7turn0search2

### Qualification evidence rule

A source construct is not considered qualified merely because the parser accepts it. Qualification requires the source fixture, normalized representation, semantic rule, positive/negative result, and inclusion in the unified manifest. JVM, bytecode, SLVM, and runtime implementation remain outside this program upgrade.

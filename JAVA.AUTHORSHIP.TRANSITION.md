# Java Authorship Transition

SLeeLa preserves Java authorship metadata through the lexer, parser, AST, compiler boundary, and Java compatibility library.

## Important vocabulary boundary

The Java compatibility surface is **not the native SLeeLa language**.

Java keywords, Java modifiers, Java annotation syntax, Java annotation interfaces, and Java API names are recognized or represented so that Java-authored source can have a congruent SLeeLa counterpart.

They must not be presented to SLeeLa developers as the base or advanced SLeeLa symbol set.

See `JAVA.COMPATIBILITY.SCOPE.md` for the authoritative boundary.

## Current capabilities

- class, interface, enum, record, and annotation-oriented type identity
- Java access and implementation modifiers
- sealed, non-sealed, strictfp, native, synchronized, volatile, transient, final, abstract, and default metadata
- superclass and implemented/super interfaces
- qualified reference types and array dimensions
- explicit constructor identity
- declared thrown exception types
- generic type/signature metadata
- declaration and type-use annotation metadata
- JDK source/API status metadata
- preview, incubator, and internal classification fields
- Java compatibility manifest counters
- compiler-time metadata validation

## Compatibility boundary

The compiler validates Java compatibility metadata before ordinary SLeeLa lowering.

Metadata preservation is not behavioral equivalence, and Java compatibility vocabulary is not native SLeeLa vocabulary.

This project does **not** require an SLVM or JVM implementation merely to establish Java/SLeeLa source and API congruence.

## Qualification

`QUALIFICATION.JAVA.md` defines the source/API qualification gates.

The key evidence is:

`Java source → SLeeLa counterpart → preserved metadata/signatures/constraints → qualification result`

rather than:

`Java source → JVM bytecode → SLeeLa virtual machine`

## Version

SLeeLa: 0.3.4-dev

SLeeLa syntax: 1.6

Java Authorship Transition Gate: 1.3-dev

These versions describe the implemented compatibility-surface capability, not JVM or Java-bytecode execution compatibility.

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

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

This upgrade makes Java control-flow qualification a first-class closure task. Qualification now requires a source fixture, normalized representation, semantic rule, positive or negative result, and unified-manifest evidence.

Required next coverage: boolean-path-sensitive `&&`, `||`, `!`, and `?:`; constant boolean expressions; break/continue joins and labels; while/do/for completion; switch completion; try/catch/finally abrupt paths; constructor and blank-final definite-unassignment; lambda capture boundaries; and checked-exception propagation.

The work remains source/API qualification and does not imply JVM, bytecode, SLVM, or runtime equivalence.

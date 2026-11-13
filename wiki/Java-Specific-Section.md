# Java Specific Section

Everything specific to Java source is specified apart from the SLeeLa language, in the Java Specific
Section of the overall specification: [`lib/java/JAVA.SPEC.md`](../lib/java/JAVA.SPEC.md).

- **The Java tier** is SLeeLa library code under `lib/java` plus the converter
  (`sleelactl run java-convert`). The SLeeLa compiler, VM and runtime stay language-neutral; the
  runtime loads the tier when a program imports `java.jdk-28`.
- **Converted today:** integer, float and char semantics; String concatenation, `valueOf`, `join` and
  `format` with a literal format; `switch` (statement, arrow and expression); enums; varargs; assignment
  and `++`/`--` as values; try-with-resources; arrays of any rank; anonymous classes, lambdas and method
  references; nested and inner classes; `instanceof` patterns; and `synchronized`/`volatile` under
  `--single-thread`; records; Java field defaults and initialization order (field initializers and
  instance initializer blocks after `super(...)`).
- **Measured 2026-11-13:** 32 of 32 differential checks pass (22 programs identical to OpenJDK 25.0.2,
  9 refusals, a stale-output check; in CI); the JDK 25 `java.base` subset converts 420 of 884 files.
  Full status: [`JAVA.CONVERSION.STATUS.md`](../JAVA.CONVERSION.STATUS.md).
- **Open:** operands of unknown type (no generics inference), lambdas whose target interface is unknown,
  nested capture chains, local classes, switch type patterns, `static { }` blocks, `Object` methods on a
  type that does not declare them, several nested types sharing one simple name in a file, `native`.

The principles and the full ledger of what is converted and what is left: [`CONVERSION.PRINCIPLES.md`](../lib/java/CONVERSION.PRINCIPLES.md).

See the section for the lowering of each construct, the proof tests and how to extend the tier.

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
  `--single-thread`. Records are implemented but not yet tested.
- **Open:** operands of unknown type (no generics inference), lambdas whose target interface is unknown,
  nested capture chains, local classes, switch type patterns, `native`.

See the section for the lowering of each construct, the proof tests and how to extend the tier.

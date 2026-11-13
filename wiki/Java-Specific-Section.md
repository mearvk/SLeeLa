# Java Specific Section

Everything specific to Java source is specified apart from the SLeeLa language, in the Java Specific
Section of the overall specification: [`lib/java/JAVA.SPEC.md`](../lib/java/JAVA.SPEC.md).

- **The Java tier** is SLeeLa library code under `lib/java` plus the converter
  (`sleelactl run java-convert`). The SLeeLa compiler, VM and runtime stay language-neutral; the
  runtime loads the tier when a program imports `java.jdk-28`.
- **Converted today:** integer, float and char semantics; String concatenation, `valueOf`, `join`
  and `format` with a literal format; `switch` (statement, arrow and expression); enums; varargs;
  assignment and `++`/`--` as values; try-with-resources; arrays of any rank; and
  `synchronized`/`volatile` under `--single-thread`.
- **Open:** lambdas, anonymous and inner classes, nested types, patterns, records, operands of
  unknown type, `native`.

See the section for the lowering of each construct, the proof tests and how to extend the tier.

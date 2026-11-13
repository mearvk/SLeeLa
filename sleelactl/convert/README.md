# javaconv: Java -> SLeeLa converter (sleelactl `java-convert`)

Converts the Java source of a presumed JDK version into **functionally
equivalent** SLeeLa source. Standing rules: `.kiro/steering/java-to-sleela-conversion.md`.

```
Java source --java_lex.c--> tokens --java_to_sleela.cpp--> SLeeLa 3.0 document(s)
                                          ^
                          conv_model.c <--+-- impl/subjects/conversion/conversion.model
```

| File | Language | Role |
|---|---|---|
| `java_lex.{h,c}` | C99 | JLS ch. 3 tokenizer: literals decoded (hex/octal/binary, `_`, `L`, exponents, escapes, text blocks) |
| `conv_model.{h,c}` | C99 | loads and indexes the conversion subject model |
| `java_to_sleela.{h,cpp}` | C++17 | typed recursive-descent translator; two passes (signatures, then bodies) |
| `javaconv_main.cpp` | C++17 | CLI: directory walk, symbol collection over the whole tree, report |

Converted constructs and the rules for extending the converter are specified in the Java Specific
Section, [`lib/java/JAVA.SPEC.md`](../../lib/java/JAVA.SPEC.md). Java improvements go in the converter and in
`lib/java`, never in the compiler, VM or runtime.

## The conversion subject

Like `math` and `physics`, `conversion` is a subject with a model:
`impl/subjects/conversion/conversion.model`. It holds the Java types, boxing,
promotion, operator templates for `int`/`long`/`double`/`boolean`/`String`,
narrowing casts, compound-assignment and `++`/`--` rules, string conversion, and
library result types. The translator contains no Java rules of its own; the int
laws are executed by `lib/java/JavaOperands.sleela` (verified against a JDK by
`examples/java/differential/OperandSemantics`).

## Use

```sh
bin/sleelactl build java-convert
bin/sleelactl run   java-convert --jdk-version 28            # sleelactl/jdk/28/java -> sleelactl/jdk/28/sleela
bin/sleelactl run   java-convert --jdk DIR --out DIR [--jdk-version N] [--split|--bundle]
bin/sleelactl run   java-convert --file Foo.java --out DIR   # one runnable bundled document
bin/sleelactl run   java-convert --self-check
```

Exit 0: everything converted. Exit 1: some files failed closed (see
`CONVERSION-REPORT.md/.json` in the output). Exit 2: usage/IO.

## Fail closed

Every construct the model does not cover is refused with
`file:line:col [construct] reason`. A file with any failed member writes no output.
The refusal rows are the `unsupported` records of the model.

## Verification

`tests/javaconv_differential_test.sh` (also `make -C sleelactl/convert test`):

- `tests/java/*.java` run on a real JDK and, converted, on the SLeeLa VM; outputs must be
  identical (Basics, Control, Operands (191 lines of int/long/double/narrowing results),
  Shapes (interfaces, inheritance, ctors, statics, casts), Strings, Flow (labels, `finally` on
  return/break/continue, array initializers)).
- `tests/reject/*.java` must be refused with the stated construct and write nothing.

## Status (honest)

Converted and proven against a JDK (`tests/javaconv_differential_test.sh`, 18 programs plus 4 refusals):
integer, float, char and String semantics; `String.valueOf/join/format` (literal format); native `switch`
(fallthrough, arrow cases, switch expressions); assignment and `++`/`--` as values; enums; varargs;
try-with-resources; `synchronized`/`volatile` under `--single-thread`; arrays of any rank; anonymous
classes, lambdas and method references (captured variables as fields); nested and inner classes;
`instanceof` patterns. Records are implemented but not yet covered by a test.

Not converted (refused with a located diagnostic): operands of unknown type (no generics inference, the
largest blocker on real JDK code), lambdas whose target interface is unknown, nested capture chains,
`outer.new Inner()`, local classes, switch type patterns, `native`. The list with reasons is section 5
of [`lib/java/JAVA.SPEC.md`](../../lib/java/JAVA.SPEC.md).

The JDK as a whole is not converted: the earlier whole-tree measurement (JDK 25 `java.base`, 345 of 884
files) predates these lowerings and has not been repeated. Nothing is claimed beyond what the report of
a run and the differential tests show.

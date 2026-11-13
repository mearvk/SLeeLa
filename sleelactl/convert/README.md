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
  Shapes (interfaces, inheritance, ctors, statics, casts), Strings).
- `tests/reject/*.java` must be refused with the stated construct and write nothing.

## Status (honest)

Run over JDK 25 `java.base` `java/lang`, `java/util`, `java/math`, `java/text` (884 files,
2026-11-12): **344 converted, 540 failed closed**. The main blockers, in order, are nested/inner
types, operands whose type cannot be determined without generics inference, `synchronized`,
assignment/`++` used as a value, `float`, lambdas, `native`, varargs. Nothing is claimed beyond
what the report and the differential test show; the JDK is not yet fully converted.

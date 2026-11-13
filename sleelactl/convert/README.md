# javaconv: Java -> SLeeLa converter (sleelactl `java-convert`)

Converts the Java source of a presumed JDK version into **functionally
equivalent** SLeeLa source. Standing rules: `.kiro/steering/java-to-sleela-conversion.md`.

```
Java source --java_lex.c--> tokens --java_to_sleela.cpp--> SLeeLa 3.1 document(s)
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
`CONVERSION-REPORT.md/.json` in the output). Exit 2: usage/IO (including a file that cannot be written).

A re-run into the same output directory first removes the files the previous run wrote (listed in
`<out>/.javaconv-emitted`), so a file that now fails closed leaves no stale conversion behind. Only files
in that list are deleted.

## Fail closed

Every construct the model does not cover is refused with
`file:line:col [construct] reason`. A file with any failed member writes no output.
The refusal rows are the `unsupported` records of the model.

## Verification

`tests/javaconv_differential_test.sh` (also `make -C sleelactl/convert test`):

- `tests/java/*.java` (21 programs) run on a real JDK and, converted, on the SLeeLa VM; outputs must be
  identical: Anon, Basics, Control, Enums, Floats, Flow, Formats, Functional, Grid, Init, Nested,
  ObjectText, Operands (191 lines of int/long/double/narrowing results), Records, Refs, Resources, Shapes, Strings,
  Switches, Sync, Varargs. A `// flags:` line passes converter options (`Sync` uses `--single-thread`). Options: `--single-thread`, `--unchecked-sizes` (omit the `NegativeArraySizeException` wrapper), `--bundle`/`--split`.
- `tests/reject/*.java` (9 programs) must be refused with the construct named on their `// expect:` line
  and write nothing.
- stale output: a file converted on one run and failing closed on the next leaves no `.sleela` behind.

## Status (honest)

Converted and proven against a JDK (`tests/javaconv_differential_test.sh`, 21 programs plus 9 refusals and a
stale-output check, 31 of 31 passing on 2026-11-13; in CI as `.github/workflows/java-convert.yml`):
integer, float, char and String semantics; `String.valueOf/join/format` (literal format); native `switch`
(fallthrough, arrow cases, switch expressions); assignment and `++`/`--` as values; enums; varargs;
try-with-resources; `synchronized`/`volatile` under `--single-thread`; arrays of any rank; anonymous
classes, lambdas and method references (captured variables as fields); nested and inner classes;
`instanceof` patterns; records; field default values; Java initialization order (field initializers and
instance initializer blocks after `super(...)`); members inherited from library superclasses; qualified
nested type names (`Format.Field` is `Format__Field`); library objects shown as text through `toString()`;
`char` arguments of `StringBuilder.append`/`insert` as text.

Not converted (refused with a located diagnostic): operands of unknown type (no generics inference, the
largest blocker on real JDK code), lambdas whose target interface is unknown, nested capture chains,
`outer.new Inner()`, local classes, switch type patterns, `static { }` blocks, `Object` methods on a type
that does not declare them, `native`. The list with reasons is section 5
of [`lib/java/JAVA.SPEC.md`](../../lib/java/JAVA.SPEC.md).

**Whole-tree measurement, 2026-11-13** (JDK 25.0.2 `src.zip`, `java.base` packages `java/lang`, `java/util`,
`java/math`, `java/text` with subpackages, 884 files, the same set as the 2026-11-12 baseline of 344):
**420 of 884 files (496 SLeeLa files); 426 with `--single-thread`**. Runtime about 8 s. The files that fail closed, by their first refusal:
`nested-ambiguous` 131 (one simple name used by several nested types in a file, for example `Entry`),
`nested` 67, `untyped` 57, `volatile` 32, `object-method` 31, `syntax` 28, `init-block` 25, `model` 18,
`synchronized` 16, `boxed` 13, `native` 12. The whole status, with the history of this number: the root
`JAVA.CONVERSION.STATUS.md`. "Converted" means the converter accepted the file. It does not mean the file
passes `sleela check`: a split-layout file names sibling nested types (`Format__Field`) that a standalone check
does not link, and calls on library types are only as good as their `lib/java` units. That check has not been
measured over the tree.

Earlier note, kept for the record: the earlier whole-tree measurement (JDK 25 `java.base`, 345 of 884
files) predates these lowerings and has not been repeated. Nothing is claimed beyond what the report of
a run and the differential tests show.

---
inclusion: always
---

# IMPORTANT: Java -> SLeeLa conversion (sleelactl `java-convert`)

**Standing rule (2026-11-13).** Java improvements do NOT go into the SLeeLa compiler (`impl/frontend`),
VM (`impl/core`) or runtime. They go into the Java tier: SLeeLa library code under `lib/java` and
lowerings in the converter (`sleelactl/convert`, `conversion.model`). They are specified in the Java
Specific Section, `lib/java/JAVA.SPEC.md`, not in `SLEELA.syntax`, and from there into the
documentation. Syntax 3.1/3.2 (sections 44 to 46) stays as the language features the tier uses.

Standing, always-on. Registered 2026-11-12. Changes to this area must keep these rules.

## The goal

`sleelactl` takes the Java source of a **presumed JDK version** and converts it
into a **SLeeLa version of that JDK that is functionally equivalent**. Equivalence
of behaviour is the goal, never textual resemblance. A conversion that is
"close" is a defect.

## Where things live

| What | Path |
|---|---|
| Presumed JDK source (input, not committed) | `sleelactl/jdk/<N>/java/` (see `sleelactl/jdk/README.md`) |
| Converted SLeeLa JDK (output) | `sleelactl/jdk/<N>/sleela/` + `CONVERSION-REPORT.md/.json` |
| Converter: C lexer + model loader, C++ translator | `sleelactl/convert/` |
| **Conversion subject model** (the laws; like math, physics) | `impl/subjects/conversion/conversion.model` |
| Executable form of the integer laws | `lib/java/JavaOperands.sleela` |
| Reflected result types (model section 10b, generated; hand rows win) | `impl/subjects/conversion/generate_ret_rows.py` (`--check` to verify) |
| Second consumer of the model (SLeeLa program, per-member stubs) | `lib/java/converter` (`JConvModel.sleela`) |
| Registry entry | `bin/sleelactl.d/registry.conf` (`java-convert`, nature `tool`) |
| Differential test | `tests/javaconv_differential_test.sh` |

Run: `sleelactl build java-convert`, then
`sleelactl run java-convert --jdk-version 28` (or `--jdk DIR --out DIR`).

## Rules

1. **The model is the single source of Java semantics.** Operator widths,
   promotion, casts, compound assignment, string conversion and library result
   types are rows in `conversion.model`. The C/C++ holds NO Java rules of its own.
   To change behaviour, change the model (and its test), not the translator.
2. **Fail closed.** If the model does not cover a construct, the converter stops
   with a located diagnostic (`file:line:col [construct] reason`). It never
   guesses, never emits a near-miss, never silently drops behaviour. A file with
   any failed member produces **no output** (no partly converted type).
3. **Equivalence is proven against a real JDK**, not asserted. Every supported
   construct needs a program in `sleelactl/convert/tests/java/` whose output is
   byte-identical between `java` and the converted program on the SLeeLa VM.
   Every refusal needs a program in `tests/reject/` (`// expect: <construct>`).
4. **Never claim more than the report shows.** The numbers in
   `CONVERSION-REPORT.md` (converted vs failed closed, by construct) are the
   status. Do not say "the JDK is converted" while files fail closed.
5. **No new SLeeLa syntax, no housing class** (SLEELA.syntax 43.1, 38.4,
   `lib/java/jdk-28/LOWERING.md` principle). A Java construct SLeeLa has no form
   for is lowered to existing constructs or refused; it is never given new syntax.
6. **Metadata Class metapattern** is emitted on every converted class (interfaces
   hold no state and carry none). Author/legal values the converter cannot know
   are `unknown`/`none`, never invented.
7. **Clean-room / licence.** JDK source is GPLv2+Classpath; do not commit the
   input tree. Converted output is derivative of that source: keep its notices
   when publishing it.
8. Commit straight to the default branch of `mearvk/SLeeLa` (standing preference).

## Known SLeeLa limits the converter must respect (do not paper over)

- Converted output declares `#sleela 3.2` (SLEELA.syntax sections 44 to 46). It uses the
  array initializer, multi-dimensional creation `new T[a][b]` (jagged and nested initializers
  included), labeled `break`/`continue`, and `return`/`break`/`continue` that run `finally`.
  The converter emits a char literal as its integer code. Because 3.2 shows a value of static
  type `char` as text, a Java `(int)`/`(long)` cast of a char is emitted as a real cast, and a
  `char`/`double` shown as text still goes through the `tostr` rows (`Double.toString`, JavaText).
- The open syntax blockers are the register in SLEELA.syntax 44.6. When SLeeLa gains a
  form, update 44.6, the model `unsupported` rows, the translator and a differential
  test together. `String` is still a keyword type: `String.valueOf/format/join` are
  refused (`string-static`).
- A class holds at most 64 instance fields; the 13 metadata fields count.
- A user program does not auto-link sibling files: a runnable conversion is a
  single bundled document (`--bundle`, default for `--file`); the library layout
  is one type per file (`--split`, default for `--jdk`).
- Refused today (see JAVA.SPEC.md section 5): `==` on Strings/boxed values, `native`, operands of unknown
  type (no generics inference), lambdas whose target interface is unknown, lambdas or anonymous classes
  nested in other generated classes, `outer.new Inner()`, local classes, `case Type t ->` patterns,
  non-literal `String.format`, `static { }` blocks, `Object.equals/hashCode/getClass` on a user type that does
  not declare them. Converted since the first version: `float`, `synchronized`/`volatile`
  (only with `--single-thread`), `++`/assignment as values, nested/inner/anonymous classes, lambdas and
  method references, enums, varargs, switch expressions, try-with-resources, `instanceof` patterns.
  Records (test `Records`); field defaults, Java initialization order and instance initializer blocks (test `Init`).
- Fields: the converter writes the Java default value on every field without an initializer and moves
  instance field initializers into `javaFieldInit_<Class>()`, called after `super(...)`. Keep both: SLeeLa
  fields start as `null` and SLeeLa runs initializers at allocation, before any constructor.
- Output directories: a re-run deletes only the files listed in `<out>/.javaconv-emitted` (what the
  previous run wrote), so a file that fails closed never leaves an old conversion behind.
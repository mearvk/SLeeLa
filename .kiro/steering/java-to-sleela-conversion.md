---
inclusion: always
---

# IMPORTANT: Java -> SLeeLa conversion (sleelactl `java-convert`)

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

- No array literal, one-dimensional `new T[n]` only (initializers are lowered to
  element stores; multi-dimensional creation is refused).
- No `char` literal (`'a'` becomes its code, typed `char`); a `char`/`double`
  shown as text goes through the `tostr` rows (`Double.toString`, JavaText).
- `String` is a keyword type: `String.valueOf/format/join` are refused (`string-static`).
- `return`/`break`/`continue` out of a try that has a `finally` is refused
  (SLEELA.syntax 43.5: the VM would skip the finally).
- A class holds at most 64 instance fields; the 13 metadata fields count.
- A user program does not auto-link sibling files: a runnable conversion is a
  single bundled document (`--bundle`, default for `--file`); the library layout
  is one type per file (`--split`, default for `--jdk`).
- `==` on Strings/boxed values, `float` arithmetic, `synchronized`, `volatile`,
  lambdas, method references, labeled jumps, `++`/`--` or assignment used as a
  value, nested/inner/anonymous types, enums, varargs, try-with-resources,
  switch expressions are refused today. See the `unsupported` rows of the model.

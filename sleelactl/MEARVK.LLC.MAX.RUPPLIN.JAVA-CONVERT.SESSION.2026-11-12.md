# MEARVK.LLC — Max.Rupplin — `sleelactl` Java -> SLeeLa converter Session Record

**Date:** 2026-11-12
**Repository:** `mearvk/SLeeLa`
**Branch:** `master`
**Subject:** a conversion model (like math, physics) and C/C++ in `/sleelactl` that converts a presumed JDK's Java source into a functionally equivalent SLeeLa JDK

## What was built

| Piece | Path |
|---|---|
| Conversion subject model (the laws) | `impl/subjects/conversion/conversion.model` |
| C lexer, model loader | `sleelactl/convert/java_lex.c`, `conv_model.c` |
| C++ translator, CLI | `sleelactl/convert/java_to_sleela.cpp`, `javaconv_main.cpp` |
| Presumed-JDK directory convention | `sleelactl/jdk/<N>/java` (input) -> `sleelactl/jdk/<N>/sleela` (output) |
| sleelactl registration | `java-convert` (nature `tool`) in `bin/sleelactl.d/registry.conf` |
| Important Kiro setting | `.kiro/steering/java-to-sleela-conversion.md` (`inclusion: always`) |
| Differential test | `tests/javaconv_differential_test.sh` |

## Design decisions

- The model, not the code, owns Java semantics. The int laws execute through the existing `lib/java/JavaOperands.sleela`.
- Fail closed: unsupported constructs are refused with `file:line:col [construct] reason`; a file with any failed member writes nothing.
- Equivalence is demonstrated against a real JDK (outputs byte-identical), not asserted.

## Verified

- Differential: 5 programs identical to the JDK (Operands covers 191 lines of int/long/double/narrowing behaviour), 10 refusal cases pass.
- `bin/sleelactl check-registry --strict`, `tests/sleelactl_cli_test.sh`, `tests/sleelactl_systemctl_scope_test.sh`, `make check-manifest`: pass.
- Run over JDK 25 `java.base` (`java/lang`, `java/util`, `java/math`, `java/text`; 884 files): 344 converted, 540 failed closed (see `sleelactl/convert/README.md`).

## Found along the way (SLeeLa limits the converter works around or refuses)

- `working-binaries/sleela` is not the source compiler; build `impl/build/sleela` to check 3.0 output.
- A user program does not auto-link sibling files, so runnable output is one bundled document.
- `T.length` on arrays is `arrayLength(a)`; no array literal; no `char` literal; `String` is a keyword type; a double concatenates as `3` unless routed through `Double.toString`.
- The VM skips `finally` on return/break/continue out of the try (SLEELA.syntax 43.5).

## Not done

Nested/inner/anonymous types, generics inference, lambdas, `synchronized`, `float`, varargs, enums, assignment/`++` as values. The full JDK is therefore NOT yet converted.

*MEARVK LLC — 2026.*

## Addendum: syntax 3.1 (same day)

The converter's blockers were taken into the language. SLEELA.syntax is now **3.1** (section 44; `#sleela 3.01` is accepted and means 3.1):

| Added | Where |
|---|---|
| character literal `'a'` (integer UTF-16 code unit) | lexer `Tok::Char`, parser, gated |
| array initializer `{..}`, `new T[]{..}` | parser `parseArrayLiteral`, `NewExpr::init`, semantic type check, `emitNew` (`OP_NEWARRAY` + `OP_DUP`/`OP_ARRSET`/`OP_POP`) |
| labeled statements, `break label`, `continue label` | `LabeledStmt`; the label was previously parsed and silently ignored |
| `return`/`break`/`continue` run enclosing `finally` | compiler `regions_` + `unwindTo` (pops each handler frame, emits its finally inline) |
| fix: `continue` inside a `switch` inside a loop | the switch context no longer receives the continue |

Verified: `impl/tests/syntax31/run_syntax31_tests.sh` (8), `impl/tests/version/run_version_tests.sh`, spark/oop/lineage/exceptions suites, `make test-sleela`, nordshrift tests and the cross-version fixture (now 3.2 is the above-ceiling case), 17 `examples/java/differential` programs, and the converter differential (`Flow.java` now converts labels, finally-on-return and array initializers and matches the JDK).

Decisions to review: "3.01" is read as minor 1 (= 3.1) because minors are integers (`1.10` vs `1.1`); `working-binaries/` was NOT refreshed and still reports 1.0-3.0 (`impl/build/sleela` is the 3.1 binary); the char literal is an integer, not a distinct value kind. Still open (SLEELA.syntax 44.6): `String.m(..)` statics, `new T[a][b]`, varargs, lambdas, inner/anonymous classes, enums, switch expressions, try-with-resources, `float`.

## Addendum: syntax 3.2, the char and array specifications (same day)

SLEELA.syntax is now **3.2**: section 45 (char) and section 46 (array). Governing code: `impl/core/sleela_core.{h,c}` (three append-only opcodes: `OP_CHARSTR` 192, `OP_ADDCHAR` 193, `OP_NEWARRAYND` 194; ISA now 195 opcodes), `impl/frontend/{lexer,ast,parser,semantic,compiler,version}`. Library: `lib/char/` (`SLChar`) and `lib/array/` (`SLIntArray`, `SLDoubleArray`, `SLRefArray`, `SLArray2D`), each with an `SLPackage` facade; `lib/vm/InstructionSet.sleela` brought up to date (it had stopped at opcode 184, so `test-osls-isa` had been failing).

Verified with a freshly built binary: `impl/tests/syntax32` (11), syntax31, version, spark/oop/lineage/exceptions, nordshrift (6 targets), `test-sleela`, `test-osls-isa`, `verify-vm-source-coverage`, library inventory (123 packages, 18120 sources), 17 Java differential programs, the converter differential (14).

Process lesson recorded: the build is integrity-gated, so the SHA-256 manifest must be regenerated BEFORE `make`; a stale manifest makes the build stop silently and leaves the old binary in place (two earlier test runs this session used a stale binary until I caught it).

Not done (SLEELA.syntax 45.7, 46.9): Unicode tables beyond Latin-1, array covariance, `clone()`, varargs, `String` static members. (Superseded: the converter later adopted multi-dimensional creation and the typed char; see the addenda below.)

## Addendum: the Java tier (2026-11-13)

Per the standing rule, further Java improvements were made only in `lib/java` and `sleelactl/convert`
(no change to `impl/frontend` or `impl/core`) and specified in `lib/java/JAVA.SPEC.md`: native switch and
switch expressions, assignment/`++` values, enums, varargs, `float` (`JavaFloat`), `String.valueOf/join/format`
(`JavaFormat`), try-with-resources, `--single-thread`, anonymous classes, lambdas, method references,
nested/inner types, `instanceof` patterns, and a record lowering (untested). Differential suite: 18 programs and
4 refusals, all matching the JDK.

## Addendum: review pass (2026-11-13)

A review of `sleelactl java-convert` with new probe programs (javac + java vs converted + `impl/build/sleela run`)
found five places where the converter emitted a program that ran but did not behave like Java, or whose
failure surfaced only in the SLeeLa checker. All were fixed in `sleelactl/convert` and the model, per the standing
rule (no change to `impl/frontend` or `impl/core`).

| Found | Java | Converted program did | Fix |
|---|---|---|---|
| Field without an initializer | `0` / `false` / `0.0` / `null` | printed `null`; `si += 5` failed with "shift operators require integer operands"; an unset instance `String` read as `""` | the default value of the type is written on every field (JLS 4.12.5) |
| Field initializer order | runs after `super(...)` (JLS 12.5) | ran at allocation, so a superclass constructor saw subclass fields already set (`init v=5` instead of `v=0`) and side effects were reordered | initializers collected into `javaFieldInit_<Class>()`, called after `super(...)`; `this(...)` delegates leave it to the delegate |
| Instance initializer block `{ }` | converted | refused as `[nested]` (wrong reason) | converted, in source order with the field initializers; `static { }` refused as `[init-block]` |
| Members inherited from a library superclass | `e.getMessage() + e.code` is String concatenation | refused `[untyped]` | calls are typed up the declared superclass chain through the model `ret` rows |
| `a.equals(b)` on a user class that does not declare it | `Object.equals` | converted, then the checker failed (`has no method 'equals'`) | refused at conversion, located, `[object-method]`; records declare the methods they generate |
| Re-run into the same output directory | — | a file that now fails closed kept its earlier `.sleela` | the run removes what the previous run wrote (`<out>/.javaconv-emitted`); write errors exit 2 |

Tests added: `tests/java/Init.java` (defaults, ordering, blocks, inherited members), `tests/java/Records.java`
(records were implemented but untested), `tests/reject/ObjectMethod.java`, and a stale-output check in
`tests/javaconv_differential_test.sh`.

Verified: `tests/javaconv_differential_test.sh` pass=26 fail=0 (20 programs identical to OpenJDK 25.0.2, 5 refusals,
stale-output check); `javaconv --self-check`; `cd impl && make check-manifest` OK (no gated file changed).
Skipped per `validation-cadence.md`: the long suites, and the whole-JDK measurement (no presumed JDK tree in the sandbox).

Still open from the review: `Object.equals`/`hashCode` on enums and plain classes (identity semantics),
`static { }` blocks, an uncaught exception prints `uncaught language exception` instead of Java's
`Exception in thread "main" ...` text, `System.out.println(e)` of a library exception shows the handle
(`RuntimeException#0`), and `String.split` is missing from the String method set.

## Addendum: status re-measured, a hang fixed (2026-11-13, later)

Correction to the review addendum above: the sandbox does have JDK sources (`$JAVA_HOME/lib/src.zip` of OpenJDK
25.0.2), so the whole-tree run was repeated on the same 884-file `java.base` subset as the 2026-11-12 baseline.

| Measurement | Converted | Note |
|---|---|---|
| 2026-11-12 baseline | 344 of 884 | first converter |
| converter at `323cc04f` (before the review) | 424 of 884 | the 2026-11-13 lowerings |
| converter at `8f617ba2` (the review commit) | did not finish | hung on `java/text/{DateFormat,MessageFormat,NumberFormat}.java` |
| this commit | **423 of 884** (499 SLeeLa files); 429 with `--single-thread` | about 8 s |

The hang: `parseType` kept only the last segment of a qualified type, so `NumberFormat.Field extends Format.Field`
resolved `Format.Field` to `NumberFormat__Field` and the class extended itself. The new `[object-method]` superclass
walk (added in the review) had no bound and looped on it. Fixed at the root: `Outer.Inner` in a type now names the
hoisted `Outer__Inner`, and both new walks are bounded like the existing ones (depth 32, self-reference stops).
Measured cost of the root fix alone: 420 to 433 files.

A second gap found by checking converted output: an unqualified `getClass()` inside a class (as `java.text.Format`
does) bypassed the `[object-method]` check, which only saw receiver calls. Both paths now share one helper.

Compared with `323cc04f`: 9 files newly convert, 10 are newly refused, all `[object-method]`. Those 10 previously
produced output that SLeeLa's checker rejects (`call to unknown method 'getClass'`, checked on a probe), so the
count is lower but more honest.

Files that fail closed, by first refusal: `nested-ambiguous` 133, `nested` 68, `untyped` 57, `object-method` 33,
`volatile` 32, `init-block` 29, `syntax` 28, `model` 18, `synchronized` 16, `native` 12. "Converted" is not
"checks": a split-layout file names sibling nested types (`Format__Field`) that a standalone `sleela check` does not
link (spot-checked on 4 files); a tree-wide check was not measured.

Tests added: `tests/reject/QualifiedNested.java` (fails with `[nested-ambiguous]` under the old name resolution),
`tests/reject/BareObjectMethod.java`. Suite: 28 of 28. Model revision 1.2.0 (the three refusal rows; no law changed).
Documents updated with these numbers: `sleelactl/convert/README.md`, `sleelactl/jdk/README.md`, `lib/java/JAVA.SPEC.md`,
both `CONVERSION.PRINCIPLES.md`, `lib/java/STATUS.md`, `impl/subjects/conversion/README.md`,
`wiki/Java-Specific-Section.md`, `.kiro/steering/java-to-sleela-conversion.md`.

## Addendum: model 1.3.0, CI, overall status page (2026-11-13, later)

- Library objects shown as text printed the VM handle (`StringBuilder#0`, `RuntimeException#1`,
  `ArrayList#2`) although `JAVA.SPEC.md` 4.4 already claimed `toString()`. Now the model row
  `tostr reference` routes a reference of a known type through `JavaFormat.objectText` (new method in
  `lib/java/JavaFormat.sleela`; manifest regenerated). The first version also routed values of unknown
  type and broke `Resources` (`e.getSuppressed().length` is an int at run time): unknown-typed values keep
  the plain text. A user object without `toString()` is refused `[object-text]` (Java prints an identity hash).
- `sb.append('x')` appended `120`: model section 9c `chartext` rows pass a char as text to
  `StringBuilder`/`StringBuffer` `append`/`insert`. The model loader gained the kind.
- `Integer i = 42` converted and then did not compile (pre-existing): refused `[boxed]` until a boxing lowering exists.
- CI: `.github/workflows/java-convert.yml`. CI also showed that `verify-security` had failed on `master` since
  `8e39e535` (stale `sleela_terminal.c` entry; fixed in `c91a4ecf`) and that a Native Drive failure on `main`
  was a runner `nm` timeout (passed on re-run).
- Suite 31 of 31 (21 programs, 9 refusals, stale-output check). Whole tree 420 of 884 (426 with `--single-thread`).
- The overall status page is the root `JAVA.CONVERSION.STATUS.md`.

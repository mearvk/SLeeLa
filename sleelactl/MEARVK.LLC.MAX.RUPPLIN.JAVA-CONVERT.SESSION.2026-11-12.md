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

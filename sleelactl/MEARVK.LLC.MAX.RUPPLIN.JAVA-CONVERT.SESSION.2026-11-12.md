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

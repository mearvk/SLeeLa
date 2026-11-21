# conversion subject (Java -> SLeeLa)

`conversion.model` is the declarative model of the laws under which Java source
becomes functionally equivalent SLeeLa source: types, boxing, numeric promotion,
operator lowering, narrowing casts, compound assignment, string conversion, library
result types, and the constructs refused (fail closed).

It is consumed by `sleelactl/convert` (C lexer + model loader, C++ translator),
registered in sleelactl as `java-convert`, and by `lib/java/converter` (a SLeeLa
program; `JConvModel.sleela` reads type/rank/promotion, operator, cast, widening,
rename, qualification, `ret`, `field` and `tostr` rows; its output stays at syntax 3.1). Unlike math/physics it is not compiled

> **Caveat (2026-11-20):** the edition statement above is the original wording and is kept. Syntax **3.2** is the current edition (`SLEELA.syntax` 44.1; `impl/frontend/version.h` ceiling 3.2; `api/sleela/3.2/LANGUAGE-API.md`). `#sleela 3.01` means 3.1 and does not mean 3.2. Sections 44, 45 and 46 are not one edition: 44 is 3.1, 45 and 46 are 3.2. The C++ converter output is `#sleela 3.2`. See `sleelactl/MEARVK.LLC.MAX.RUPPLIN.JAVA-CONVERT.SESSION.2026-11-12.md`, last addendum.
into the `sleela` runtime: it governs a tool, and its executable half is
`lib/java/JavaOperands.sleela`. Rules: `.kiro/steering/java-to-sleela-conversion.md`.

## Revisions

| Revision | Change |
|---|---|
| 1.7.0 (2026-11-20) | `binop double %` and `compound double %` added; `binop float %` changed: both use `JavaOperands.remDouble` (JLS 15.17.3), because the VM `%` traps on a zero divisor where Java gives NaN. Refusal rows added: `unboxing-cast`, `print-stream`, `string-nul`, `class-literal`, `intersection-cast`, `string-surrogate` (65 rows); `switch-pattern` narrowed (type patterns are converted). Test `FloatRemainder` checks 17 remainder cases against the JDK. |
| 1.6.0 (2026-11-20) | Section 12 is the registry of refusal kinds: 38 rows added for kinds the translator emitted with their reason only in C++ (`library-stub`, `library-member`, `library-type`, `member-access`, `type-ambiguous`, `nested-ambiguous`, `generic-*`, `overload-*`, `native`, `volatile`, ...). Rows `float`, `enum`, `lambda`, `assign-expr` are marked superseded (not emitted). The translator reports a refusal kind without a row as `[model]`; the report shows each kind's model reason. Header scope narrowed (see below); `chartext` added to the record grammar. No law changed. |
| 1.5.0 (2026-11-18) | Shared type/rank/promotion laws; `promote float`; binary32 arithmetic, comparisons, unary operations, increment/decrement and text rows; all 49 numeric primitive cast pairs; context-aware `rename` records and built-in qualification consumed by both converters. C++ self-check expanded and `builtin_names.h` retired. New C++ `Floats` cases added; not yet built or differentially verified at the time (verified 2026-11-20: `Floats` passes, 24 lines identical to the JDK). |
| 1.3.0 (2026-11-13) | Law rows: `tostr reference` (string conversion of an object through `JavaFormat.objectText`), section 9c `chartext` (library parameters that take a char as text: `StringBuilder`/`StringBuffer` `append`, `insert`). Refusal rows `boxed`, `object-text`. |
| 1.2.0 (2026-11-13) | Refusal rows `init-block` (static initializer blocks), `member-annotation`, `object-method` (`equals`/`hashCode`/`getClass` on a user type that does not declare them). No law changed. |
| 1.1.0 (2026-11-13) | Section **10b**: 8,922 `ret` rows reflected from the public API of OpenJDK 25.0.2 (`lib/java/jdk-25/api/jdk-signatures.jsonl`, `jdk-api.jsonl`) by `generate_ret_rows.py`, for the procedural core of `java.base` (`java.lang`, `java.util`, `java.io`, `java.math`, `java.time`, `java.text`, `java.net`, `java.nio`, ...; the list is `SCOPE` in the script). Hand rows win. |
| 1.0.0 | Types, boxing, promotion, operators, casts, compound assignment, string conversion, 45 hand `ret` / `field` rows, refusals |

## Section 10b: reflected result types

A row is written only when it is unambiguous: every overload of the member,
declared or inherited (most derived declaration first), on every JDK type with
that simple name, agrees on a model type (a primitive, `String`, or a top-level
type by simple name); overloads that always return the type of their first
argument are written `@arg0`. Type variables, arrays and nested types are left
out, so such a result stays unknown and both converters keep failing closed on
it (`sleelactl`) or leave it as written (`lib/java/converter`). A `void`
overload takes no part in the agreement, because a call whose value is used
cannot be the `void` one.

```sh
python3 impl/subjects/conversion/generate_ret_rows.py           # rewrite section 10b
python3 impl/subjects/conversion/generate_ret_rows.py --check   # exit 1 when out of date
```

## What the model covers, and what it does not (1.6.0)

The model holds the expression-level laws and the refusal registry. The declaration and name-resolution
laws are implemented in the C++ translator and specified in `lib/java/JAVA.SPEC.md`: name resolution and
access (JLS 6), the implicit modifiers of interface, record and enum members (JLS 8.9.3, 8.10, 9.4),
overload resolution (JLS 15.12), generic inference, the enum and record lowerings, and initialization order.
Earlier wording said the converter holds no Java semantic knowledge of its own. That was true only of the
expression laws.

Library members have two typing sources. Section 10 and 10b give result types. The `lib/java/jdk-28` units
themselves are read for member existence and arity, stub bodies, placeholder static fields (a non-final
static in a "java declarations" section), and abstract interface methods (dispatch, not stubs). Unit headers
list only `extends`, so a member that an interface unit does not show is not refused
(`JAVA.SPEC.md` 5.1).

## Section 12: the refusal registry

Every refusal kind the translator emits is an `unsupported` row. A diagnostic keeps its located, specific
message, and the row is the general reason, shown per kind in `CONVERSION-REPORT.md`. A refusal kind
without a row is reported as `[model]` and still fails closed. `tests/javaconv_differential_test.sh` checks
this (`model-registry`: the `string-identity` row is removed from a copy of the model, and the refusal must
come out as `[model]`). `syntax` and `model` are errors, not refusals, and have no row.

## Current measurement (2026-11-20, model 1.6.0, Corretto 25.0.5.1)

`tests/javaconv_differential_test.sh`: 116 of 116 (1.6.0); 138 of 138 with 1.7.0, and all of `java.base` 864 of 3344. `java.base` subset (`lang`, `util`, `math`, `text`; 884 files):
262 converted. All of `java.base` (3344 files): 862 converted. The 1.6.0 change left both runs file-for-file
identical to 1.5.0, with identical refusal counts and no `[model]` refusals. The measurements below are older.

Measured with 1.3.0 (2026-11-13): `tests/javaconv_differential_test.sh` 32 of 32 (22 programs, 9 refusals,
stale-output check); the JDK 25.0.2 `java.base` subset of 884 files converts 420 (see `JAVA.CONVERSION.STATUS.md`).

Measured with 1.1.0 (2026-11-13): `tests/javaconv_differential_test.sh` 14 of 14
pass, unchanged from 1.0.0. On a 17-file JDK sample `java-convert` gets further
in two files (their first refusal moves from `[untyped]` to `[native]`); it still
converts 2 of 17, because it refuses a file at its first unsupported construct.

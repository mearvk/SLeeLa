# conversion subject (Java -> SLeeLa)

`conversion.model` is the declarative model of the laws under which Java source
becomes functionally equivalent SLeeLa source: types, boxing, numeric promotion,
operator lowering, narrowing casts, compound assignment, string conversion, library
result types, and the constructs refused (fail closed).

It is consumed by `sleelactl/convert` (C lexer + model loader, C++ translator),
registered in sleelactl as `java-convert`, and by `lib/java/converter` (a SLeeLa
program; `JConvModel.sleela` reads the `ret`, `field`, `binop int` shift,
division and remainder rows, the `cast long int` row (the 32-bit wrap) and the
`tostr` rows; its output stays at syntax 3.1). Unlike math/physics it is not compiled
into the `sleela` runtime: it governs a tool, and its executable half is
`lib/java/JavaOperands.sleela`. Rules: `.kiro/steering/java-to-sleela-conversion.md`.

## Revisions

| Revision | Change |
|---|---|
| 1.0.0 | Types, boxing, promotion, operators, casts, compound assignment, string conversion, 45 hand `ret` / `field` rows, refusals |
| 1.1.0 (2026-11-13) | Section **10b**: 8,922 `ret` rows reflected from the public API of OpenJDK 25.0.2 (`lib/java/jdk-25/api/jdk-signatures.jsonl`, `jdk-api.jsonl`) by `generate_ret_rows.py`, for the procedural core of `java.base` (`java.lang`, `java.util`, `java.io`, `java.math`, `java.time`, `java.text`, `java.net`, `java.nio`, ...; the list is `SCOPE` in the script). Hand rows win. |

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

Measured with 1.1.0 (2026-11-13): `tests/javaconv_differential_test.sh` 14 of 14
pass, unchanged from 1.0.0. On a 17-file JDK sample `java-convert` gets further
in two files (their first refusal moves from `[untyped]` to `[native]`); it still
converts 2 of 17, because it refuses a file at its first unsupported construct.

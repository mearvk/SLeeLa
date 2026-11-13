# Java Specific Section of the Overall SLeeLa Specification

Status: normative for the Java tier. Revision 1.0, 2026-11-13.
Owner: `lib/java` (this directory) and the converter `sleelactl/convert`.

## 1. Scope and the standing rule

The SLeeLa language is specified in `SLEELA.syntax`. Everything that exists **only because a
program is written in Java** is specified here, in the Java tier, and not in the language
specification.

**Standing rule.** A Java improvement is made in the Java tier:

- as SLeeLa library code under `lib/java` (support classes such as `JavaOperands`, `JavaFloat`,
  `JavaFormat`, the `jdk-25` / `jdk-28` unit trees), loaded by the runtime when a program says
  `import java.jdk-28;`, and
- as a lowering in the converter (`sleelactl run java-convert`, source in `sleelactl/convert`,
  laws in `impl/subjects/conversion/conversion.model`).

It is **not** made in the SLeeLa compiler (`impl/frontend`), the VM (`impl/core`) or the runtime.
Where the tier needs something the language does not have, the converter lowers the Java construct
to constructs the language already has (the principle of `lib/java/jdk-28/LOWERING.md`); it does
not add syntax or opcodes.

Relation to the language specification. Sections 44, 45 and 46 of `SLEELA.syntax` (syntax 3.1 and
3.2: character literal, array initializer, labeled statements, `finally` on abrupt exit, `a.length`,
multi-dimensional arrays, char as text) are language features that the Java tier **uses**. They were
added to the language before this rule and stay there. Nothing new is added to the language by the
Java tier. `SLEELA.syntax` section 43 (Java support, `spark`) and 38.4 (the Java container) are
unchanged.

## 2. The Java tier

| Part | Where | Role |
|---|---|---|
| Java type units | `lib/java/jdk-25`, `lib/java/jdk-28` | one SLeeLa file per public JDK type |
| Operand laws | `lib/java/JavaOperands.sleela` | 32-bit `int`, narrowing, shifts, division corners |
| Floating point | `lib/java/JavaFloat.sleela` | binary32 (`float`) held in a double and rounded after every operation |
| Text formatting | `lib/java/JavaFormat.sleela` | `String.format` conversions, `String.join` |
| Text units | `lib/java/jdk-25/implementations/.../JavaText`, `JavaStringMethods` | UTF-16 text, String methods |
| Support classes | `lib/java/support` | lambdas table, beans, properties, bit helpers |
| Conversion laws | `impl/subjects/conversion/conversion.model` | types, operators, casts, string conversion, result types |
| Converter | `sleelactl/convert` (`java-convert`) | Java source to SLeeLa source |
| Second converter | `lib/java/converter` | a converter written in SLeeLa; same laws, same model |

Loading. A program that imports `java.jdk-N` has the tier's classes resolved by name from `/lib`.
Converted output carries `import java.jdk-N;` and `material;` and declares `#sleela 3.2`.
The tier is not built into `sleela`: removing `lib/java` removes Java support and leaves the
language intact.

## 3. The conversion contract

1. **Functional equivalence** is the goal. A converted program, run on the SLeeLa VM, produces the
   same observable behavior (output, exceptions, results) as the Java program on a JDK.
2. **Fail closed.** A construct the tier cannot convert equivalently is refused with
   `file:line:col [construct] reason`. A file with any refused member writes no output.
3. **Proof by difference.** Every supported construct has a program in
   `sleelactl/convert/tests/java/` whose output must be byte-identical between `java` and the
   converted program (`tests/javaconv_differential_test.sh`). Every refusal has a program in
   `tests/reject/`.
4. **No claim beyond the report.** `CONVERSION-REPORT.md` of a run is the status of that run.

## 4. Lowering catalogue

Each entry is a Java construct, the SLeeLa it becomes, and the proof test.

### 4.1 Integer arithmetic (JLS 4.2, 5, 15.17 to 15.24)
`int` operations wrap at 32 bits through `JavaOperands` (`addInt`, `mulInt`, `shlInt`, ...);
`byte`, `short`, `char` narrow on store and cast; integer division by zero throws
`ArithmeticException`; `long` is native; `double` is native. Rows: `binop`, `unop`, `cast`,
`compound`, `incr` of the model. Test: `Operands`.

### 4.2 Floating point
- `double`: native. Shown as text with `Double.toString` (`tostr double`).
- `float`: held in a double and rounded to binary32 after each `+ - * / %`, each int/long to float
  conversion and each `(float)` cast, by `JavaFloat.f32` (the rounding of `java.lang.Float`:
  24-bit significand, round half even, overflow to infinity, gradual underflow). Correct because a
  double has more than `2*24+2` significant bits. Float literals are the binary32 value nearest
  the decimal, written with enough digits to read back. A float is shown as text with
  `JavaFloat.text`. A float is a `double` in every SLeeLa declaration. Test: `Floats`.
- `++`/`--` of a float used as a value is refused (the old value cannot be recovered exactly).

### 4.3 Characters
A char literal is its integer code (typed `char`). `(int)` and `(long)` casts of a char are real
casts, because a value of static type `char` is shown as text. A char shown as text in a Java
String context goes through `tostr char`. Tests: `Strings`, `Operands`.

### 4.4 Strings
- `+` with a String converts the other operand as JLS 5.1.11: objects through `toString()`
  (`null` is `"null"`), doubles through `Double.toString`, floats through `JavaFloat.text`.
- `String.valueOf(x)` is `"" + x` with the same conversions.
- `String.join(sep, a, b, ...)` and `String.join(sep, String[])` are
  `JavaFormat.joinArray(sep, String[])`.
- `String.format(literal, args...)` with a **literal** format string is expanded at conversion
  time into a concatenation of `JavaFormat` calls, one per conversion, so every argument keeps its
  Java type. Supported: `%d %x %X %o %f %s %c %b %n %%` with flags `- 0 + , (space)`, a width and a
  precision. `%f` rounds half up on the shortest decimal digits of the double, as Java does. A
  non-literal format, argument indexes (`%1$s`), `%e %g %S %h %t` and a type mismatch between
  conversion and argument are refused. `==` between two Strings is refused (identity in Java, value
  in SLeeLa). Test: `Formats`, `Strings`.

### 4.5 Control flow
- `switch` statement: a native SLeeLa switch. Fallthrough, `default` anywhere, `continue` inside a
  switch in a loop and `break` follow Java. Old-style stacked labels and arrow cases (`case A, B ->`)
  are both accepted; an arrow case gets an implicit `break`.
- `switch` expression: hoisted. A temporary is declared, a native switch assigns it in each arm
  (`->` expression, `->` block with `yield`, or old-style with `yield`), and the temporary is the
  value. Allowed where the expression is the whole of a declaration initializer, an assignment
  right side, a `return` value or an expression statement; elsewhere refused. A `yield` inside a
  nested loop or switch is refused. Type patterns and `case null` are refused.
- Enum labels: a bare constant name in `case` is resolved in the selector's enum type.
- Labeled `break`/`continue`, `return`/`break`/`continue` through `finally`: language features of
  syntax 3.1; used as they are.
- `try`-with-resources (JLS 14.20.3): each resource is declared, then its own `try`; an exception
  from the body is kept as primary and one from `close` is added with `addSuppressed`; resources
  close in reverse order; the statement's own `catch` and `finally` wrap the whole. `return`,
  `break` and `continue` out of the body close the resources. Test: `Resources`, `Flow`, `Switches`.
- `assert` is omitted (disabled by default in Java).

### 4.6 Expressions
- Assignment used as a value (`a = b = 9`, `(x = 7) + 1`) and prefix/postfix `++`/`--` used as a
  value are converted with the Java 32-bit wrap; the old value of a postfix operation is recovered
  by the inverse step. The operand must be free of side effects (it is evaluated twice) or the
  construct is refused (`[side-effecting-lvalue]`). Test: `Switches`.
- Method invocation conversion: arguments are widened to `double` parameters, and the trailing
  arguments of a variable-arity call are packed into an array (4.7).

### 4.7 Classes
- `enum` (JLS 8.9): a class with two hidden fields (`enumName`, `enumOrdinal`), a static instance
  per constant, a constant-specific body becomes a generated subclass `E__C`, a static array and the
  members `values()` (a copy), `valueOf(String)` (throws `IllegalArgumentException`), `name()`,
  `ordinal()`, `toString()`, `compareTo`. An enum with a constructor gets the two extra leading
  parameters. Refused: `this(...)` in an enum constructor, enums nested in a class. Test: `Enums`.
- Variable arity `T... xs`: the parameter is an array; a call to a method or constructor whose
  signature is known (same file, or a collected tree) packs the extra arguments into
  `new T[]{...}`. A call to a varargs method of a type whose source was not collected is not packed.
  Test: `Varargs`.
- Name mapping. `close()` is a SLeeLa built-in; the tier names the Java method `closeResource()`
  in declarations and calls. Java members named like a SLeeLa reserved word or built-in
  (`record`, `native`, `struct`, `join`, `spawn`, `sleep`, ...) are refused; local variables with
  those names are renamed with a trailing `_`.
- `public static void main(String[] args)` becomes `static void main()` with
  `String[] args = new String[0];`.
- Interfaces carry no metadata fields; every class carries the Metadata Class record.
- Limits: at most 64 instance fields per class (13 are metadata).

### 4.8 Arrays
One-dimensional arrays, multi-dimensional creation `new T[a][b]`, jagged `new T[a][]`, nested
initializers, `a.length` (as `arrayLength(a)`), `char[]` and `float[]` (as `double[]`). Language
limit: 4096 live arrays, and each row of a grid is one. Test: `Grid`.

### 4.9 Concurrency
`synchronized` (method and block) and `volatile` order accesses between threads. The tier converts
a program as **single-threaded**: with `--single-thread` they become plain (a `synchronized` block is
its body), without it they are refused (`[synchronized]`, `[volatile]`). Equivalence for a program
that really uses several threads is not claimed. Test: `Sync`.

### 4.10 Not convertible
`native` methods (no Java body). Everything else refused is listed in section 5.

## 5. Open items (refused today)

| Construct | Why | Plan |
|---|---|---|
| Lambdas, method references | need a generated class per lambda with captured locals as fields, a captured outer `this`, and the target interface's single method | lower to a generated class implementing the target type; needs the target type known |
| Anonymous classes, inner (non-static) classes | the same capture machinery (outer instance field) | the same generated-class lowering |
| Nested and static nested classes | converter hoists only top-level types | hoist to `Outer__Inner` with name resolution |
| `instanceof T x` patterns, record patterns, `case Type t ->` | binding scope | lower to a cast declaration after the test where the binding is used |
| Records, sealed hierarchies | records are classes with generated members | records via generated class |
| Operands of unknown type | no generics inference | collect generic signatures and substitute |
| `String.format` with a non-literal format, `%e`, argument indexes | runtime format parsing | a runtime formatter in `JavaFormat` |
| `native` | no body | none |

## 6. How to extend the tier

1. Write the Java program that exercises the construct in `sleelactl/convert/tests/java/`.
2. Implement the lowering in the converter and, if it needs runtime support, in `lib/java` as SLeeLa
   code. Do not touch `impl/frontend` or `impl/core`.
3. Run `bash tests/javaconv_differential_test.sh`; the output must be identical to the JDK.
4. Update this document (section 4 and 5), `sleelactl/convert/README.md` and the steering note
   `.kiro/steering/java-to-sleela-conversion.md`.
5. If a library class under `lib` was added: regenerate `lib/LIBRARY.SYMBOLS.md`, the inventory test
   counts, `lib/LIBRARY.INDEX.md` and `security/sha256-manifest.json`.

## 7. Verification

`bash tests/javaconv_differential_test.sh` runs 14 programs on a JDK and on the converted SLeeLa:
Basics, Control, Enums, Flow, Floats, Formats, Grid, Operands, Resources, Shapes, Strings,
Switches, Sync, Varargs, and 4 refusal programs under `tests/reject` that must be refused (a lambda,
String identity comparison, a `synchronized` block without `--single-thread`, an operand of unknown type).
`bash examples/java/differential/run-differential.sh` is the older hand-converted suite.

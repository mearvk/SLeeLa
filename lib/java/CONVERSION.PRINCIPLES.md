# Java to SLeeLa conversion: principles, what is done, what is left

Revision 1.0, 2026-11-13. Part of the Java tier; the normative text is
[`JAVA.SPEC.md`](JAVA.SPEC.md). This page is the summary and the ledger.

## 1. Principles

| # | Principle | Meaning in practice |
|---|---|---|
| 1 | **Functional equivalence** | A converted program prints and returns what the Java program does on a JDK. Textual likeness is not the goal. |
| 2 | **Java improvements live in the Java tier** | Library code in `lib/java` and lowerings in the converter (`sleelactl/convert`). Not in the SLeeLa compiler, VM or runtime. The language features syntax 3.1 and 3.2 added before this rule stay where they are and are used as they are. |
| 3 | **Fail closed** | A construct that cannot be converted equivalently is refused with `file:line:col [construct] reason`. A file with any refused member writes nothing. No near-misses. |
| 4 | **The model owns the laws** | Types, operators, casts, string conversion and library result types are rows of `impl/subjects/conversion/conversion.model`. The converter holds structure (how a statement is rewritten), not arithmetic. |
| 5 | **Lower to what exists** | No new syntax or opcode. A Java construct without a SLeeLa form becomes a short sequence of existing forms (generated classes, temporaries, library calls), as `jdk-28/LOWERING.md` prescribes. |
| 6 | **Proof by difference** | Every supported construct has a Java program whose output must be byte-identical between a JDK and the converted program on the SLeeLa VM. Every refusal has a program that must be refused. |
| 7 | **Honest status** | The report of a run and the passing tests are the status. Nothing is claimed beyond them. The whole JDK is not converted. |
| 8 | **Preserve evaluation order and Java widths** | `int` wraps at 32 bits, `char` is 16 bits, `float` rounds to 24 bits after every operation, division by zero throws, operands evaluate left to right. |
| 9 | **Name mapping is explicit** | Names that collide with SLeeLa built-ins are renamed consistently (`close` is `closeResource`); members with reserved names are refused; locals are renamed with a trailing `_`. |
| 10 | **Policies are flags, not defaults** | Anything that weakens equivalence is off unless asked: `--single-thread` makes `synchronized` and `volatile` plain. |
| 11 | **Metadata Class on every class** | Converted classes carry the descriptive record; unknown values are `unknown`/`none`, never invented. Interfaces hold no state and carry none. |

## 2. What is done

Every row has a proof program under `sleelactl/convert/tests/java/` (run
`bash tests/javaconv_differential_test.sh`) unless the last column says otherwise.

### 2.1 Values and operators

| Java construct | How it is converted | Where | Proof |
|---|---|---|---|
| `int` arithmetic, shifts, `~`, unary `-` | 32-bit wrap through `JavaOperands` (`addInt`, `mulInt`, `shlInt`, ...) | model `binop`/`unop` rows, `lib/java/JavaOperands.sleela` | `Operands` |
| Division and remainder | `divInt`/`remInt`/`divLong`/`remLong`; zero divisor throws `ArithmeticException`; `MIN / -1` corner | `JavaOperands` | `Operands` |
| `long`, `double` arithmetic | native | model rows | `Operands`, `Basics` |
| `byte`, `short`, `char` | narrowing on store and cast (`toByte`, `toShort`, `toChar`) | model `cast` rows | `Operands` |
| `float` | held in a double, rounded to binary32 after every `+ - * / %`, conversion and cast; literals are the nearest float | `lib/java/JavaFloat.sleela` (uses `java.lang.Float` rounding) | `Floats` |
| Casts (`(int) d`, `(long) d`, `(char) i`, ...) | Java saturation and truncation rules (`d2i`, `d2l`, `d2b`, ...) | model `cast` rows | `Operands` |
| Compound assignment `x op= y` | `x = (T)(x op y)` with the implicit narrowing | converter + model `compound` | `Operands` |
| Char literals and `char` as a number | integer code typed `char`; `(int)`/`(long)` of a char is a real cast because syntax 3.2 shows a char-typed value as text | converter | `Strings`, `Operands` |
| Widening to `double` parameters and variables | explicit `(double)`, so later `/` does not divide as integers | converter `coerce`, `adaptArgs` | `Varargs`, `Basics` |

### 2.2 Strings and text

| Java construct | How it is converted | Where | Proof |
|---|---|---|---|
| String `+` with any operand | JLS 5.1.11: `double` through `Double.toString`, `float` through `JavaFloat.text`, `char` through `JavaText`, objects through `toString()` (null is `"null"`) | model `tostr` rows, converter `strOf` | `Strings`, `Enums` |
| Object shown as text | generated `static String javaText(Name x)` on every class with a `toString` | converter | `Nested`, `Enums` |
| `String.valueOf(x)` | `"" + x` with the same conversions | converter | `Formats` |
| `String.join(sep, ...)` | `JavaFormat.joinArray` | `lib/java/JavaFormat.sleela` | `Formats` |
| `String.format` with a literal format | expanded at conversion time into `JavaFormat` calls, one per conversion; `%d %x %X %o %f %s %c %b %n %%`, flags `- 0 + , space`, width, precision; `%f` half-up on the shortest digits | `JavaFormat` | `Formats` |
| `==` between Strings | refused (identity in Java, value in SLeeLa) | converter | `reject/StringIdentity` |
| Text blocks, escapes, `\u` | decoded by the C lexer | `java_lex.c` | escapes: `Strings`; text blocks: no test |

### 2.3 Control flow

| Java construct | How it is converted | Where | Proof |
|---|---|---|---|
| `if`, `while`, `do`, `for`, enhanced `for`, `break`, `continue` | native | converter | `Control`, `Basics` |
| `switch` statement (fallthrough, `default` anywhere, stacked labels) | native SLeeLa `switch` | converter | `Switches` |
| Arrow cases `case A, B ->` | case plus implicit `break` | converter | `Switches` |
| `switch` expression with `yield` | hoisted temporary assigned in each arm | converter | `Switches`, `Enums` |
| Enum labels in `case` | bare name resolved in the selector's enum | converter | `Enums` |
| Labeled `break`/`continue` | language feature, syntax 3.1 | SLEELA.syntax 44.4 | `Flow` |
| `return`/`break`/`continue` through `finally` | language feature, syntax 3.1 (the `finally` runs) | SLEELA.syntax 44.5 | `Flow`, `Resources` |
| `try`/`catch`/`finally`, multi-catch | typed catch; a multi-catch replicates the handler | converter | `Control`, `Flow` (multi-catch: no test) |
| try-with-resources | one `try` per resource, reverse close, suppressed exceptions kept | converter | `Resources` |
| `assert` | omitted (disabled by default in Java) | converter | no test |

### 2.4 Expressions

| Java construct | How it is converted | Where | Proof |
|---|---|---|---|
| Assignment used as a value | `(x = y)` with the assigned value | converter | `Switches` |
| Prefix `++`/`--` as a value | new value, with the 32-bit wrap | converter | `Switches` |
| Postfix `++`/`--` as a value | old value recovered by the inverse step | converter | `Switches` |
| Ternary `?:`, `instanceof`, casts to classes | native | converter | `Basics`, `Shapes` |
| `x instanceof T v` | `v` declared before the statement, assigned when the test succeeds | converter | `Refs` |
| Method invocation conversion | argument widening, varargs packing | converter `adaptArgs` | `Varargs` |

### 2.5 Arrays

| Java construct | How it is converted | Where | Proof |
|---|---|---|---|
| `new T[n]`, defaults | native, typed fill (0, 0.0, false, null) | VM | `Basics` |
| `{a, b, c}` and `new T[]{...}` | language feature, syntax 3.1 | SLEELA.syntax 44.3 | `Basics`, `Grid` |
| `new T[a][b]`, jagged `new T[a][]`, nested initializers | language feature, syntax 3.2 | SLEELA.syntax 46.3 | `Grid` |
| `a.length` | `arrayLength(a)` | model `call` row | `Grid` |
| `char[]`, `float[]`, `long[]`, `boolean[]` | element defaults and narrowing as above | converter | `Grid`, `Floats` |

### 2.6 Classes and types

| Java construct | How it is converted | Where | Proof |
|---|---|---|---|
| Classes, fields, methods, constructors, `this`, `super(...)`, inheritance, virtual calls | native SLeeLa classes | converter | `Shapes` |
| Interfaces, abstract classes | native; interfaces carry no metadata | converter | `Shapes`, `Anon` |
| `static` members and static fields | native (initialised before `main`) | converter | `Shapes` |
| `public static void main(String[] args)` | `static void main()` with `String[] args = new String[0]` | converter | all |
| Enums (fields, constructors, methods, constant bodies, `values`, `valueOf`, `ordinal`, `name`, `compareTo`) | a class with hidden name/ordinal, one static instance per constant, constant bodies as generated subclasses | converter | `Enums` |
| Varargs `T... xs` | parameter is `T[]`; call sites with a known signature pack the extra arguments | converter | `Varargs` |
| Nested static classes, interfaces, enums | hoisted to `Outer__Name`; static members of the outer visible | converter | `Nested` |
| Inner (non-static) classes | field `outer_`, constructors take the enclosing object, `Outer.this` | converter | `Nested` |
| Anonymous classes | generated class; captured locals as `cap_` fields, enclosing object as `outer_` | converter | `Anon` |
| Lambdas | generated class implementing the target's one abstract method; `this` is the enclosing object | converter | `Anon`, `Functional` |
| Method references (`Type::static`, `Type::instance`, `Type::new`, `this::m`, `local::m`) | the lambda they stand for | converter | `Refs` |
| JDK functional interfaces (Runnable, Supplier, Consumer, Function, Predicate, Comparator, Bi/Unary/Binary forms) | seeded as known, generic parameters untyped | converter | `Functional` |
| Records | field per component, canonical (or compact) constructor, accessors, `toString`, `equals`/`hashCode` for simple components | converter | `Records` |
| Field defaults and initialization order | default value on every field; field initializers and instance `{ }` blocks after `super(...)` | converter | `Init` |
| `close()` | mapped to `closeResource()` in declarations and calls | converter `mapMember` | `Resources` |
| Reserved or built-in member names | refused; locals renamed | converter | `Varargs` (a method named `join` was caught); no refusal test |
| Packages, imports | dropped; library members resolve through `import java.jdk-N;` | converter | all |

### 2.7 Concurrency and policy

| Java construct | How it is converted | Where | Proof |
|---|---|---|---|
| `synchronized` method and block, `volatile` | plain, only with `--single-thread`; refused without it | converter option | `Sync`, `reject/SyncBlock` |

### 2.8 Machinery

| Piece | What it does | Where |
|---|---|---|
| C lexer | JLS ch. 3 tokens: numbers (hex, octal, binary, `_`, suffixes), escapes, text blocks | `sleelactl/convert/java_lex.c` |
| Model loader | reads the conversion subject model | `conv_model.c`, `impl/subjects/conversion/conversion.model` |
| Typed translator (C++) | two passes; pass 1 collects signatures of the whole tree so calls across files are typed | `java_to_sleela.cpp` |
| CLI | `--jdk`, `--jdk-version`, `--file`, `--bundle`/`--split`, `--single-thread`, `--self-check`; writes `CONVERSION-REPORT.md/.json` | `javaconv_main.cpp` |
| sleelactl registration | `sleelactl build java-convert`, `sleelactl run java-convert ...` | `bin/sleelactl.d/registry.conf` |
| Presumed-JDK directories | `sleelactl/jdk/<N>/java` in, `sleelactl/jdk/<N>/sleela` out | `sleelactl/jdk/README.md` |
| Support library | `JavaOperands`, `JavaFloat`, `JavaFormat`, `JavaText`, `jdk-25`/`jdk-28` units | `lib/java` |
| Differential test | 20 programs on a JDK and on the VM, plus 5 refusals and a stale-output check | `tests/javaconv_differential_test.sh` |
| Language features the tier uses | char literal, array initializer, labels, `finally` on abrupt exit (3.1); `a.length`, multi-dimensional arrays, char as text (3.2) | `SLEELA.syntax` sections 44 to 46 |

## 3. What is left

| # | Item | Why it is open | How it would be done | Size |
|---|---|---|---|---|
| 1 | **Operands of unknown type (generics)** | Generic parameters are untyped, so arithmetic and some calls on them are refused. This is the largest blocker on real JDK code and on most lambdas over collections. | Collect generic signatures; substitute type arguments through calls and assignments; infer lambda parameter types from the target. | large |
| 2 | **Lambda targets that are JDK interfaces not collected** | The abstract method is unknown unless the interface is one of the seeded ones. | Convert the JDK tree so its interfaces are collected, or extend the seeded table. | medium |
| 3 | **Nested capture chains** | A lambda or anonymous class inside another generated class is refused. | Capture through the enclosing generated class. | medium |
| 4 | **`outer.new Inner()` and local classes** | Rare forms not parsed. | Qualified inner creation; hoist local classes like nested ones. | small to medium |
| 5 | **Switch type patterns, record patterns, sealed exhaustiveness** | Binding scope in a switch arm. | Per-arm cast declaration; exhaustiveness is not checked at run time. | medium |
| 6 | **Records: the harder members** | `equals`/`hashCode` omitted for `double`, `float` and reference components (test `Records` covers the rest). | Lower `Double.compare` and `Objects.equals`. | small |
| 7 | **`String.format` beyond literals** | Non-literal formats, `%e %g %S %h %t`, argument indexes. | A runtime formatter in `JavaFormat`. | medium |
| 8 | **Static initializer blocks** | `static { }` is refused; instance `{ }` blocks are converted (test `Init`). | A guarded static init method called on first use (JLS 12.4). | small |
| 9 | **Real concurrency** | `synchronized`/`volatile` are plain under `--single-thread`; a truly multi-threaded program is not claimed equivalent. | Monitors and a memory model on the VM, or keep the policy. | large |
| 10 | **`native` methods** | No Java body to convert. | Hand-written bindings only. | not convertible |
| 11 | **Library behavior** | Converted code calls `lib/java` units; many `jdk-28` units are signature stubs that return null. Equivalence of a call depends on that unit being implemented. | Implement units; the differential suite is the oracle. | very large |
| 12 | **Whole-JDK measurement** | The earlier count (JDK 25 `java.base` subset, 345 of 884 files) predates almost all of the table in section 2 and has not been repeated. | Re-run on a presumed JDK tree, publish the report. | small |
| 13 | **Reflection and class literals** | `getClass()`, `X.class`, `int.class`, annotation processing. | No lowering designed. | open |
| 14 | **`float`/`double` corner cases** | Postfix `++` of a float used as a value; `-0.0` sign; hex float literals. | Case by case. | small |
| 15 | **Two converters** | `sleelactl/convert` (C/C++) and `lib/java/converter` (SLeeLa) share the model but not the lowerings. | Align them or retire one. | medium |
| 16 | **CI and binaries** | The differential test is not in CI; `working-binaries/` still reports syntax 3.0. | Add a CI job (needs a JDK and a source build); refresh the binaries. | small |
| 17 | **Name-mapping interplay with unconverted `lib/java` units** | A converted class that implements a library interface declaring `close()` does not match `closeResource()`. | Rename in the library units too, or map both. | small |

## 4. How to keep this page true

When a construct moves from section 3 to section 2: add the proof program under
`sleelactl/convert/tests/java/`, update `JAVA.SPEC.md` (sections 4 and 5), the converter
`README.md`, and move the row here. If a row cannot name a passing proof, it stays in section 3.

# Java Specific Section of the Overall SLeeLa Specification

Status: normative for the Java tier. Revision 1.0, 2026-11-13.
Owner: `lib/java` (this directory) and the converter `sleelactl/convert`.

The summary ledger (principles, everything converted, everything left) is
[`CONVERSION.PRINCIPLES.md`](CONVERSION.PRINCIPLES.md).

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
3.1 (also written 3.01): character literal, array initializer, labeled statements, `finally` on abrupt exit, `a.length`,
multi-dimensional arrays, char as text) are language features that the Java tier **uses**. They were
added to the language before this rule and stay there. Nothing new is added to the language by the
Java tier. `SLEELA.syntax` section 43 (Java support, `spark`) and 38.4 (the Java container) are
unchanged, except two language and VM changes made the same day at the owner's request, before
or alongside this rule: 43.7 (implemented interfaces are runtime supertypes; a method call on an
`Any` value dispatches at run time) and 37.5 (`MIN / -1` without SIGFPE; inside a `try`, integer
division by zero, an array index out of range and a failed cast are thrown, as
`ArithmeticException`, `ArrayIndexOutOfBoundsException`, `ClassCastException` through
`lib/java/support/SLJavaFaults`). The tier uses them; they are general language behavior.

## 1.1 The Java handler module (compiler boundary)

The few places where the SLeeLa compiler must act on a Java-authored or Java-referencing document
are **not** compiler logic. They are the **Java handler**, `impl/frontend/java/java_handler.cpp`,
loaded through the neutral interface `impl/frontend/language_handler.h`:

| Hook | Java behaviour provided |
|---|---|
| `documentMarkers` | the `spark` Preonem marker, minimum `#sleela 3.0` (SLEELA.syntax 43) |
| `resolveReference` | `java.lang.Throwable` resolves to the `/lib` class `Throwable`; a local class cannot impersonate it |
| `eraseSignatureType` | type variables and package-qualified types erase to `Any`; `java.lang.String` to `String` |
| `stringMethodLibrary` | `JavaStringMethods` provides methods on a String receiver |
| `faultFunction` / `libraryNeeds` | `SLJavaFaults` builds the exception a VM fault throws inside a try; the driver seeds it for a Java-style document that uses try |
| `validateProgram` | Java compatibility metadata checks |
| `filterLibraryCandidates` | tier selection among library units with the same simple name: real source beats reference files, `java/jdk-28` beats `docs/api`, `import java.jdk-28;` selects the tier, JDK 28 clashes (`java.util.List` vs `java.awt.List`) rank by longest import root, module, package, depth |
| `runHost` | `sleela run` of `.java`, `.jar`, `--class-path`/`-cp`/`--class` on an external JVM (`java/java_host.cpp`, linked into `sleela` and `sleelvac` only) |

The compiler core knows only the interface. Removing the handler from `FE_SRC` in `impl/Makefile`
removes Java support and changes nothing else. A Java improvement that needs a compiler hook adds a
neutral hook to `LanguageHandler` and implements it in the handler, never a Java branch in
`parser.cpp`, `semantic.cpp`, `compiler.cpp`, the VM or the runtime. Review, inventory and the
staged plan (the `javaString*` builtins and `OP_JSTR_*` VM string kind are the remaining open item): `impl/frontend/COMPILER-JAVA-REVIEW.md`.

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
Converted output carries `import java.jdk-N;` and `material;` and declares `#sleela 3.1`.
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
  An object of a library type, `Object` or an interface goes through the model row
  `tostr reference`, `JavaFormat.objectText(x)`, which calls `toString()` at run time (until
  2026-11-13 such objects printed the VM handle, `StringBuilder#0`). A value of unknown type (an
  erased generic, an untyped call result) may be a primitive at run time and keeps the plain SLeeLa
  text, so an *object* held in an erased generic still shows its handle (open, section 5). An object
  of a class of this tree with no `toString()` in its chain is refused (`[object-text]`): Java prints
  an identity hash. Tests: `ObjectText`, `reject/IdentityText`.
- A `char` passed to a library member that takes it as text (model rows `chartext`:
  `StringBuilder`/`StringBuffer` `append` and `insert`) is converted through the `tostr char` row;
  a char is an integer at run time, so `append('x')` would append `120`. Members that take a char as
  a code (`indexOf`, `Character.isDigit`) are not listed. Test: `ObjectText`.
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
- A call to a method whose name is a SLeeLa built-in (`read`, `write`, `accept`, `lock`, `send`, `join`, ...) is qualified with
  its owner class or `this.` (list: `sleelactl/convert/builtin_names.h`). Test: `Bounds` (a method named `read`).
- Name mapping. `close()` is a SLeeLa built-in; the tier names the Java method `closeResource()`
  in declarations and calls. Java members named like a SLeeLa reserved word or built-in
  (`record`, `native`, `struct`, `join`, `spawn`, `sleep`, ...) are refused; local variables with
  those names are renamed with a trailing `_`.
- `public static void main(String[] args)` becomes `static void main()` with
  `String[] args = new String[0];`.
- Nested and inner types. A nested class, interface, enum or record is hoisted to a top-level type
  named `Outer__Name` (a name used by several nested types is refused). A static nested type may use
  the static members of its enclosing class. An inner (non-static) class holds the enclosing object in
  the field `outer_`; its constructors take it as a leading parameter, `new Inner(...)` inside the
  outer class passes `this`, and `Outer.this` is `outer_`. A type written with its qualifier,
  `Outer.Inner`, names `Outer__Inner` (package segments in lower case are dropped), so
  `NumberFormat.Field extends Format.Field` extends `Format__Field`, not itself. Not converted:
  `outer.new Inner()`, local classes declared in a method. Tests: `Nested`, `reject/QualifiedNested`.
- Anonymous classes, lambdas and method references. Each becomes a generated top-level class
  `Top__N`: the captured local variables become constructor-initialised fields `cap_x` (Java requires
  them effectively final, so copying is exact), the enclosing object becomes `outer_`, unqualified
  names resolve first in the class, then to captured locals, then to the enclosing class. A lambda
  class implements the one abstract method of its target interface; in a lambda `this` is the
  enclosing object. The target type comes from a declaration, assignment, `return`, field
  initializer, or the parameter of a method whose signature is known (a lambda argument is resolved
  after the call is resolved). The common JDK functional interfaces (Runnable, Supplier, Consumer,
  Function, Predicate, Comparator and the Bi/Unary/Binary forms) are known with generic parameters
  untyped. A method reference `Type::static`, `Type::instance`, `Type::new`, `this::m` and
  `local::m` is the lambda it stands for. Refused: a lambda or anonymous class inside another one,
  a bound method reference on an expression that is not a local, a target interface whose source
  was not collected. Tests: `Anon`, `Functional`, `Refs`.
- Pattern matching. `x instanceof T v` declares `v` before the statement and tests
  `(x instanceof T) && ((v = (T) x) != null)`; an impure subject is held in a temporary. `case Type t ->`
  patterns and record patterns are refused. Test: `Refs`.
- Records (JLS 8.10). A record is a class with one field per component, the canonical constructor
  (a compact constructor runs its body first), an accessor per component, `toString()` as
  `Name[a=1, b=2]`, `equals` when every component is a primitive (not float/double) or a String, and
  `hashCode` for int-like, long, boolean and String components. Test: `Records`.
- Field defaults (JLS 4.12.5). A field without an initializer is written with the default value of
  its type (`0`, `0.0`, `false`, `null`), because a SLeeLa field starts as `null`: an unset `int` would
  print `null` and fail the integer laws, and an unset instance `String` would read as `""`. Test: `Init`.
- Initialization order (JLS 12.5, 8.6). Instance field initializers and instance initializer blocks
  run in source order inside the constructor, after `super(...)` and before the rest of the body; a
  constructor that delegates with `this(...)` leaves them to the delegate. They are collected into a
  generated method `javaFieldInit_<Class>()` called at that point (an implicit default constructor is
  written when the class has none). SLeeLa alone would run them at allocation, before every
  constructor, so a superclass constructor would see subclass fields already set. An initializer that
  needs extra statements (a temporary) stays on the field. Enums, records and anonymous classes keep
  field initializers on the field. Refused: `static { }` blocks (`[init-block]`). Test: `Init`.
- Inherited library members. A call on a user class that extends a library type (`class MyEx extends
  Exception`) is typed through the superclass chain and the model `ret` rows, so `e.getMessage()` has
  the type `String`. Test: `Init`.
- `java.lang.Object` methods. `equals`, `hashCode`, `getClass`, `clone`, `wait`/`notify` called on a
  user type that does not declare them (and has no library ancestor) are refused (`[object-method]`),
  with a receiver (`a.equals(b)`) or without one (`getClass()` inside the class): SLeeLa has no such
  method, and the call would otherwise only fail later in the checker. Records declare the
  `equals`/`hashCode` they generate. Tests: `reject/ObjectMethod`, `reject/BareObjectMethod`.
- Object text. An object of a class of this tree shown as text calls `toString()` through a
  generated static helper `Owner.javaText(x)` (`null` is `"null"`); other objects through
  `JavaFormat.objectText` (4.4).
- Boxed types. Boxed values are held as primitives; a boxed type written as a declared, parameter,
  return or cast type (`Integer i = 42`, `(Long) o`) is refused (`[boxed]`) because the declaration
  would name the `lib/java` class, to which a primitive is not assignable. Boxed type arguments
  (`List<Integer>`) are erased and converted. Test: `reject/BoxedDecl`.
- Interfaces carry no metadata fields; every class carries the Metadata Class record.
- Limits: at most 64 instance fields per class (13 are metadata).

### 4.8 Arrays
One-dimensional arrays, multi-dimensional creation `new T[a][b]`, jagged `new T[a][]`, nested
initializers, `a.length` (as `arrayLength(a)`), `char[]` and `float[]` (as `double[]`). Language
limit: 4096 live arrays, and each row of a grid is one. Test: `Grid`.

Run-time checks. An out-of-range index (read, write, compound store) is thrown by the VM inside a `try` as
`ArrayIndexOutOfBoundsException("Index i out of bounds for length n")` (SLEELA.syntax 37.5, `lib/java/support/SLJavaFaults`);
the converter adds nothing. A negative array size is wrapped as `JavaChecks.size(n)` (non-constant sizes only) and throws
`NegativeArraySizeException` with the size as its message; `--unchecked-sizes` omits the wrapper. Test: `Bounds`.
Open: null dereference and stack overflow remain fatal.

### 4.9 Concurrency
`synchronized` (method and block) and `volatile` order accesses between threads. The tier converts
a program as **single-threaded**: with `--single-thread` they become plain (a `synchronized` block is
its body), without it they are refused (`[synchronized]`, `[volatile]`). Equivalence for a program
that really uses several threads is not claimed. Test: `Sync`.

### 4.10 Not convertible
`native` methods (no Java body). Everything else refused is listed in section 5.

## 5. Open items (refused today)

This table is the status of `sleelactl java-convert`. The second converter, `lib/java/converter`
(per-member stubs, not whole-file refusal), lowers lambdas and method references to `(id, env)`
(0.6.0) and inner classes with an `slOuter` field (0.7.0); see its README.

| Construct | Why | Plan |
|---|---|---|
| Operands of unknown type | no generics inference: generic parameters are untyped, so arithmetic on them is refused (this is what blocks most JDK lambdas and collections code) | collect generic signatures and substitute type arguments |
| A lambda or method reference whose target is a JDK interface not in the collected tree and not in the seeded list | the abstract method is unknown | collect the JDK interfaces (convert the JDK tree) |
| Lambda or anonymous class inside another generated class | nested capture chains | capture through the enclosing generated class |
| `outer.new Inner()`, local classes | rare forms | hoist local classes like nested ones |
| `case Type t ->` patterns, record patterns, sealed exhaustiveness | binding scope in a switch | per-arm cast declaration |
| Records: `equals`/`hashCode` for double/float/reference components | Java's rules for those | `Double.compare` and `Objects.equals` lowering |
| Boxed declared types (`Integer i`) | boxed values are primitives in SLeeLa; the declaration names a class | declare as `Any`, unbox at each use, keep `null` |
| An object held in an erased generic shown as text | its run-time kind (object or primitive) is unknown at conversion | a run-time text helper that recognises primitives |
| A nested user exception in `Throwable.toString()` | prints `Outer__Inner`, Java `Outer$Inner` | a binary-name table for hoisted types |
| `static { }` initializer blocks | class initialization runs on first use (JLS 12.4) | a guarded static init method called on first access |
| `Object.equals`/`hashCode`/`getClass` on a type that does not declare them (enums included) | SLeeLa has no root object methods | identity `equals`, an identity hash, a runtime class name |
| `String.format` with a non-literal format, `%e`, argument indexes | runtime format parsing | a runtime formatter in `JavaFormat` |
| `synchronized`/`volatile` in a multi-threaded program | memory model | real monitors, or keep the single-thread policy |
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

`bash tests/javaconv_differential_test.sh` runs 21 programs on a JDK and on the converted SLeeLa:
Anon, Basics, Control, Enums, Flow, Floats, Formats, Functional, Grid, Init, Nested, ObjectText, Operands, Records,
Refs, Resources, Shapes, Strings, Switches, Sync, Varargs, and 9 refusal programs under `tests/reject` that must be refused (a lambda
argument to an unknown method, String identity comparison, a `synchronized` block without `--single-thread`, an operand of
unknown type, an `Object` method a user type does not declare, called with and without a receiver, a qualified nested
superclass that shares its simple name with the class, a boxed declared type, and a user object without `toString()` shown
as text). The workflow `.github/workflows/java-convert.yml` runs the suite in CI on every change to the converter, the
model, the Java tier or the compiler/VM. A last check converts a file, makes it fail closed, converts
again and requires that the earlier output is gone (the converter keeps the list of what it wrote in `.javaconv-emitted`).
`bash examples/java/differential/run-differential.sh` is the older hand-converted suite.

Whole-tree status (2026-11-13, JDK 25.0.2 `java.base`: `java/lang`, `java/util`, `java/math`, `java/text`,
884 files): 420 of 884 files (496 SLeeLa files); 426 with `--single-thread`; first refusals led by `nested-ambiguous` (131 files),
`nested` (67) and `untyped` (57). Full status: `JAVA.CONVERSION.STATUS.md`. The report of the run is the status; see `sleelactl/convert/README.md`.

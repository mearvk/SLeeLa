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

> **Caveat (2026-11-20):** every "syntax 3.1" statement in this document is the original wording and is kept. Syntax **3.2** is the current edition (`SLEELA.syntax` 44.1; compiler ceiling 3.2). `#sleela 3.01` means 3.1, not 3.2. Section 44 (char literal, array initializer, labels, `finally` on abrupt exit) is 3.1. Sections 45 and 46 (typed `char` text, `a.length`, `new T[a][b]`) are 3.2: under `#sleela 3.1` or `3.01` `a.length` is rejected ("requires #sleela 3.2"). The C++ converter in `sleelactl/convert` emits `#sleela 3.2`. See `sleelactl/MEARVK.LLC.MAX.RUPPLIN.JAVA-CONVERT.SESSION.2026-11-12.md`, last addendum.

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
Converted output carries `import java.jdk-N;` and `material;` and declares `#sleela 3.2` (multi-dimensional creation and nested initializers require 3.2, SLEELA.syntax 46.3).
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
5. **One registry of refusal kinds.** Every `[construct]` the converter emits is an `unsupported` row of
   `impl/subjects/conversion/conversion.model` (section 12, 59 rows since model 1.6.0). The diagnostic keeps
   its located, specific message, and the row gives the general reason shown in `CONVERSION-REPORT.md`. A
   kind without a row is reported `[model]` and still fails closed (check `model-registry`). New refusals
   add their row in the same change.
6. **Where the laws live.** Expression-level laws are rows of the conversion model. Declaration and
   name-resolution laws (access, implicit modifiers, overload resolution, generic inference, the enum and
   record lowerings) are implemented in the C++ translator and specified in this document.

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
  its owner class or `this.` (list: `qualify` rows in `impl/subjects/conversion/conversion.model`). Test: `Bounds` (a method named `read`).
- Name mapping is declared by the shared `rename` rows in
  `impl/subjects/conversion/conversion.model`. Java `close()` maps to
  `closeResource()`; the `next` replacement is value-context only, so
  `it.next()` remains a method call. Other reserved identifier replacements
  use their model-provided spelling. The C++ converter still fails closed for
  reserved member declarations it cannot safely rewrite; the SLeeLa-native
  converter applies the shared member policy. Built-in method calls are
  qualified by the shared `qualify` rows.
- `public static void main(String[] args)` becomes `static void main()` with
  `String[] args = new String[0];`.
- Nested and inner types. A nested class, interface, enum or record is hoisted to a top-level type
  named `Outer__Name` (a name used by several nested types is refused). A static nested type may use
  the static members of its enclosing class. An inner (non-static) class holds the enclosing object in
  the field `outer_`; its constructors take it as a leading parameter, `new Inner(...)` inside the
  outer class passes `this`, and `Outer.this` is `outer_`. A type written with its qualifier,
  `Outer.Inner`, names `Outer__Inner` (package segments in lower case are dropped), so
  `NumberFormat.Field extends Format.Field` extends `Format__Field`, not itself. Not converted: `outer.new Inner()` and local classes declared in executable blocks (`[local-type]`); these need an enclosing-instance expression or a separate lexical scope/capture environment. Tests: `Nested`, `reject/QualifiedNested`, `reject/LocalClass`.
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
  `local::m` is the lambda it stands for. Refused: a lambda or anonymous class inside another generated class (capture-chain lowering is not yet implemented),
  a bound method reference on an expression that is not a local, and a target interface whose SAM signature
  is not known. Tests: `Anon`, `Functional`, `Refs`, `reject/LambdaUse`, `reject/NestedLambda`.
- Pattern matching. `x instanceof T v` declares `v` before the statement and tests
  `(x instanceof T) && ((v = (T) x) != null)`; an impure subject is held in a temporary. Test: `Refs`.
- Pattern switches (JLS 14.11, 14.30; 2026-11-20). A switch with type-pattern labels, statement or expression,
  is lowered to an `instanceof` chain in source order inside a loop that runs once, so `break` and `yield` leave
  it. The selector is evaluated once. A null selector runs `case null`, or throws `NullPointerException` when there
  is none; no match without `default` throws `MatchException`. Unnamed bindings (`case T _`), `case null, default`
  and constant labels beside patterns are converted. Refused (`[switch-pattern]`): `when` guards, record
  patterns, `:` pattern groups, boxed or array type patterns, a case label after `default`, and an unlabeled
  `continue` inside an arm (it would restart the wrapper loop). Tests: `PatternSwitch`, `reject/SwitchPattern`,
  `reject/SwitchRecordPattern`, `reject/SwitchPatternContinue`.
- Other lowerings added 2026-11-20 (model 1.7.0):
  - `for` loops with several declarators, initializers or updates (JLS 14.14.1): the inits become statements
    before the loop; several updates become a loop whose first pass skips them, so `continue` still runs them.
    A label stays on the loop. Test `ForLoops`.
  - floating remainder `%` and `%=` on `double` and `float` go through `JavaOperands.remDouble` (JLS 15.17.3),
    because the VM `%` traps on a zero divisor where Java gives NaN. Test `FloatRemainder`.
  - control characters other than newline, tab and carriage return are written `\u00XX` in string literals;
    NUL is refused (`[string-nul]`), because a SLeeLa string ends at it (the C lexer used to drop the rest of
    the literal silently). Tests `StringControl`, `reject/StringNul`.
  - a boxing cast to the operand's own wrapper (`(Integer) i`) is the identity on primitive storage and keeps
    the wrapper type for overload resolution. Test `BoxingCast`.
  - hexadecimal floating literals (JLS 3.10.2) are decoded exactly (`strtod`, `strtof` for `f`) and written with
    17 significant digits. A subnormal double, which has no SLeeLa literal spelling, is written
    `(m / 2^52) * 2^-1022`, exact in binary64. Test `HexFloats`.
  - class literals (JLS 15.8.2) become `Class.ofLiteral(binary name, kind)`, the `lib/java` form shared with the
    SLeeLa-native converter (rule 8e). The package of a `/lib` type is taken from an explicit import, or from
    the selected `lib/java/jdk-28` unit when that package is `java.lang` or wildcard-imported; otherwise the
    literal is refused (`[class-literal]`). Tests `ClassLiterals`, `reject/ClassLiteralPackage`.
  - annotations on statements, locals and parameters, with dotted names and array values, are dropped like
    member annotations (JLS 9.7: metadata). Test `Annotated`.
  - an element of an array of arrays may be an array expression or `null`. Test `ArrayOfArrays`.
  - nested enum constructors and `private` enum constructors are recognized. Test `EnumConstructors`.
  - new refusal kinds instead of `[syntax]`, `[model]` or unchecked output: `unboxing-cast` (`(int) obj`),
    `print-stream` (`System.out.print`, `System.err`), `intersection-cast` (`(A & B) expr`), `string-surrogate`
    (an unpaired surrogate), and `string-static` for `String.X` static fields.
  - recovery after a refused member skips exactly that member (braces inside an annotation value no longer end
    it) and clears a half-consumed `>>`, so the next member reports its own refusal rather than a syntax error.
    Checks `member-recovery` and `generic-recovery` in `tests/javaconv_differential_test.sh`.
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
  field initializers on the field. Refused: `static { }` blocks (`[init-block]`) because Java class initialization is lazy and ordered by active use; the converter does not yet synthesize a guarded class-initialization state machine. Tests: `Init`, `reject/StaticInitializer`.
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
- Diamond allocation `new C<>(...)`. A library class keeps erased type arguments (the declared type of the
  variable governs later typing). A generic class of this tree takes its arguments from a target type of the same
  class (declaration, assignment or return, JLS 15.9.3). Any other target, a diamond inside a larger expression and
  an anonymous `new C<>() {}` are refused (`[generic-constructor-diamond]`). Tests: `GenericConstructorDiamond`,
  `ObjectText`, `reject/DiamondSupertypeTarget`.
- Lower-bounded wildcards `? super T`. `T` is inferred from the other arguments or the target type, then the
  argument is checked against the inferred bound. Tests: `LowerBoundWildcardInference`, `GenericWildcardLowerBound`.
- String concatenation with a `char` shown as text. NUL and lone surrogates exist only as UTF-16 text and the VM
  joins two Strings only of the same kind, so such a chain is joined by `JavaText.cat` (either kind). Test: `Floats`.
- Overloaded methods. The converter selects the overload by JLS 15.12 (strict, loose, then variable-arity phases;
  most specific by 15.12.2.5, including `long <: double` and `Integer <: Number`). SLeeLa cannot make that
  selection itself (it does not tell `int` from `long`, stores `Integer` as `int`, and binds a call on a subclass
  to its own method of that name), so an overloaded user method is emitted under a name built from its declared
  parameter types (`pick__int`, `pick__long`, `choose__Object`). An override declares the same parameter types
  and gets the same name, so virtual dispatch is unchanged. Names that may override a library method (`toString`,
  `equals`, `hashCode`, functional-interface methods, modelled library members) and `main` keep their Java names.
  Constructors are not renamed. Tests: `OverloadWidening`, `BoxingOverload`, `BoxingPhaseRules`,
  `RemainingOverloadRules`, `WideningMostSpecific`, `reject/AmbiguousOverload`.
- Boxed types. Scalar wrapper declarations and parameters use primitive storage for non-null values; modelled boxing/unboxing and invocation widening are supported (`BoxingOverload`, `BoxingUnboxingWidening`). A wrapper default or explicit `null` assignment is refused (`[boxed-null]`) because a primitive slot cannot preserve Java null; wrapper arrays are refused (`[boxed]`) because reference-array identity and nullable elements are not represented. Boxed type arguments (`List<Integer>`) are erased. Tests: `BoxingUnboxingWidening`, `reject/BoxedDecl`, `reject/BoxedNull`, `reject/BoxedNullParameter`, `reject/BoxedNullField`.
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
`native` methods without a declared and verified Java-to-SLeeLa ABI binding. Everything else refused is listed in section 5.

## 5. Open items (refused today)

This table is the status of `sleelactl java-convert`. The second converter, `lib/java/converter`
(per-member stubs, not whole-file refusal), lowers lambdas and method references to `(id, env)`
(0.6.0) and inner classes with an `slOuter` field (0.7.0); see its README.

| Construct | Why | Plan |
|---|---|---|
| Operands of unknown type after supported generic inference | target-type inference, interface-intersection least-upper-bound constraints, wildcard capture, generic constructors, and unresolved class-generic contexts remain unsupported | implement the remaining JLS 15.12 constraint solver; keep unresolved calls fail-closed |
| A lambda or method reference whose target is a JDK interface not in the collected tree and not in the seeded list | the abstract method is unknown | collect the JDK interfaces (convert the JDK tree) |
| Lambda or anonymous class inside another generated class | nested capture chains | capture through the enclosing generated class |
| `outer.new Inner()`, local classes | enclosing-instance expressions and lexical capture scopes are not implemented | lower qualified inner allocation and hoist local declarations with a separate capture environment (`[local-type]`) |
| Record patterns, `when` guards, `:` pattern groups (`[switch-pattern]`; type patterns are converted since 2026-11-20) | deconstruction and guards in a switch | record accessor calls per pattern component; guards as chained conditions |
| Records: `equals`/`hashCode` for double/float/reference components | Java's rules for those | `Double.compare` and `Objects.equals` lowering |
| Nullable boxed values and boxed arrays | non-null scalar wrappers use primitive storage, which cannot preserve Java null or reference-array identity | introduce nullable wrapper storage and wrapper-array representation; current null paths fail closed (`[boxed-null]`) and arrays refuse (`[boxed]`) |
| A `char` or `float` held as Object or in an erased generic, shown as text | the VM stores them as an int and a double, so the run-time kind cannot tell them apart (boxing a char or float into Object is refused, `[boxed-reference]`, test `reject/BoxedCharObject`; Integer, Long, Short, Byte, Double and Boolean held as Object print as in Java since 2026-11-20, `JavaFormat.objectText`, test `BoxedObjectText`) | a tagged representation for char and float values held as Object |
| A nested user exception in `Throwable.toString()` | prints `Outer__Inner`, Java `Outer$Inner` | a binary-name table for hoisted types |
| `static { }` initializer blocks | class initialization runs on first use (JLS 12.4) | a guarded static init method called on first access |
| `Object.equals`/`hashCode`/`getClass` on a type that does not declare them (enums included) | the identity adapter exists as library groundwork but the converter does not yet bind generated object handles to it | wire VM heap identity, object handles and runtime class names into generated classes; keep wait/notify/clone refused |
| `String.format` with a non-literal format, `%e`, argument indexes | runtime format parsing | a runtime formatter in `JavaFormat` |
| `synchronized`/`volatile` in a multi-threaded program | memory model | real monitors, or keep the single-thread policy |
| A boxed value converted to a class supertype of its wrapper (`Integer` to `Number`, `Comparable`, `Serializable`) | boxed values use primitive storage; `Object` (the VM universal type) holds one, a SLeeLa class type such as `lib/java` `Number` cannot | a `Number` view over primitive storage (runtime `intValue`/`doubleValue` helpers); until then fails closed (`[boxed-reference]`, test `reject/BoxedToNumber`) |
| A concrete array passed to, or recovered from, a type-variable array `T[]` | `T[]` erases to `Any[]`; SLeeLa arrays are invariant and casts to array types are not available (SLEELA.syntax 46) | array element contracts in SLeeLa, then a checked array cast; `T...` with packed arguments is converted (packed as `Any[]`), the rest fails closed (`[generic-array]`, test `reject/GenericArrayIdentity`) |
| A type that is neither converted in the same run nor provided by a unit under `lib/java` (`[library-type]`) | the reference would not resolve when the output is linked; this includes types of units that failed closed | convert the unit that declares it, or add the type to `lib/java` |
| A call or constructor of a `lib/java/jdk-28` type that its unit does not declare with that arity (`[library-member]`) | the reference would not resolve when the output is linked | add the member to the unit |
| A call or constructor that the `lib/java/jdk-28` unit declares only in its "java signatures" stub section (`[library-stub]`) | the stub returns a default value, so the program would run but not behave like Java | write the member logic in the unit |
| A method call on an array (`a.clone()`, `a.equals(b)`; `[object-method]`, test `reject/ArrayClone`) | a SLeeLa array has no methods; the call does not link | an array copy lowering for `clone()` |
| A read of a static field that the `lib/java/jdk-28` unit declares only as a non-final placeholder in its "java declarations" section (`[library-stub]`, for example `BigInteger.ONE`, `StandardCharsets.UTF_8`, `Collections.EMPTY_SET`) | the placeholder holds `null` or `0`, not the JDK value | give the unit a `static final` field with the real value |
| Two project declarations with the same simple name, even in different packages (`[type-ambiguous]`) | converted documents link into one flat type namespace | package-qualified hoisted names |
| Two constructors that lower to the same SLeeLa parameter list, such as `float` and `double` (`[overload-erasure]`) | SLeeLa selects a constructor by its emitted parameter types | signature-named constructor lowering |
| `native` | no Java body and no declared Java-to-SLeeLa ABI binding | accept only when a modelled, verified native binding maps the exact owner/signature/ABI; otherwise fail closed (`[native]`) |

### 5.1 Resolution of references across a run (2026-11-20)

A run converts to a fixpoint. A unit that fails closed emits nothing, so the converter drops it from the
project index and converts the other units again; their references to the dropped types then resolve
like any non-project type, or fail closed (the diagnostic says `round N`). A non-project type in the
output must be a `lib/java` unit name (a file stem such as `Objects` or `Map.Entry`); a nested type of a
library type is written `Outer.Inner`, the spelling the SLeeLa loader resolves.

For a type with a `lib/java/jdk-28` unit, the converter reads the unit the loader selects (java.base first,
then the package order of `impl/frontend/java/java_handler.cpp`) and checks every static call, instance call,
constructor, `super(...)` and unqualified inherited call by name and arity, walking the unit `extends` chain:

- a real member converts. A Java static method that the unit implements as an instance method (the tier
  convention, for example `Arrays.fill`) is called as `new Arrays().fill(...)` when the unit has a
  no-argument constructor;
- a member found only in the stub section is refused `[library-stub]`;
- no member is refused `[library-member]`;
- a member the unit declares to return `Any` gets a cast back to the Java result type at a typed target.

Static fields of a `lib/java/jdk-28` type are checked the same way (2026-11-20, later). This applies to a qualified read
`Owner.NAME`, to a bare name that a single static import binds to `Owner.NAME` (JLS 7.5.3), and to a bare name
inherited from a library superclass (JLS 8.3), all walking the `extends` chain. In a unit's "java declarations"
section a reflected constant is `static final` with its JDK value. A non-final static there is a placeholder
(`static Any ONE = null;`, or `0.0` for an infinity), so reading it is refused `[library-stub]`. A field the
chain does not declare is refused `[library-member]`. The declared SLeeLa field type is lossy (a Java `char`
is stored as `int`, a `float` as `double`), so the read gets a type only when the field is typed as its own
unit class (an enum-style constant such as `TimeUnit.NANOSECONDS`); otherwise its type stays unknown and typed
operators refuse it (`[untyped]`). A single static import of a project field reads the declared static field
through the usual access check. An import that names a member type or a static method is not a field.
Tests: `LibraryStaticImport`, `reject/LibraryFieldPlaceholder`, `reject/InheritedLibraryPlaceholder`, and the
`project-static-import` project of the differential script.

The type parameters of a class are in scope in its member classes (JLS 6.3), so an inner class may use the `E`
of its enclosing generic class (test `InnerOuterTypeVar`). Enum constants are public (JLS 8.9.3) when read from
another package.

Members that Java makes implicitly public are recorded as public, so they are visible from another package:
interface methods other than `private` ones (JLS 9.4), record accessors, `toString`, `equals` and `hashCode`
(JLS 8.10.3), the implicit canonical constructor with the access of its record (JLS 8.10.4), and the generated enum
methods (JLS 8.9.3). Project `project-implicit-public` in the differential script.

Calls that reach a `/lib` member indirectly are checked like direct calls. This covers an instance call on a project
type that does not declare the method, and an unqualified call in a class. The converter walks the project superclasses and interfaces to their
non-project ancestors. A real member in any ancestor converts the call, and a stub refuses it (`[library-stub]`). A
missing member refuses it only on the superclass chain. The unit headers list only `extends`, so a member that an
interface unit does not show may come from an interface it extends. In an interface unit, a method without `default`
or `static` is abstract: calling it dispatches to the implementing class, so it is not a stub. In a nested class the
check applies only when an ancestor unit has a member of that name, because a bare name the class lacks resolves
outward (JLS 15.12.1). A single static import of a `/lib` method binds to its owner, gets the same check, and is
written `Owner.m(...)` or, for a static that the tier implements as an instance method, `new Owner().m(...)`. The JDK functional
interfaces the converter seeds to type lambdas (`Runnable`, `Comparator`, `Function`, ...) are library types for
these checks. Tests: `StaticImportLibrary`, `reject/StaticImportStub`, `reject/InterfaceDefaultStub`,
`reject/InheritedInterfaceStub`.

The unit names and the selected unit per name are recorded in `CONVERSION-PROVENANCE.json`
(`libraryUnits`). Parameter types are not compared, only arity. `tests/javaconv_check_tree.sh`
measures what still fails at link time.

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

Current (2026-11-20, later; model 1.7.0, Corretto 25.0.5.1): `bash tests/javaconv_differential_test.sh` passes
**138 of 138** checks: 65 programs, 48 refusal fixtures, 9 multi-file projects, 9 project-level refusals, and the
stale-output, provenance, repeat-run, transaction, model-registry, member-recovery and generic-recovery checks.
Earlier the same day (master `7126f6c6c`) it passed 116 of 116. It runs the programs of `sleelactl/convert/tests/java/` on a JDK and, converted, on the
SLeeLa VM, and requires byte-identical output. It requires the programs of `tests/reject/` to be refused with
their `// expect:` construct and no output. It converts and runs 9 multi-file projects (cross-file generics,
packages, imports, access, static imports, implicitly public members), checks 9 project-level refusals (access,
duplicate names, cascades), and checks stale output, provenance,
repeat-run mtimes, transactional publication and the model refusal registry (`model-registry`). The workflow
`.github/workflows/java-convert.yml` runs the suite in CI on every change to the converter, the model, the Java tier
or the compiler/VM.

Whole-tree status (2026-11-20, `java.base` of Corretto 25.0.5.1): the `lang`/`util`/`math`/`text` subset converts
262 of 884 files, and all of `java.base` 864 of 3,344 (862 before the model 1.7.0 work). The 881 emitted files
link with 109 semantic errors in total (`sleela run` of a stub with every emitted file). Full status: `docs/reference/JAVA.CONVERSION.STATUS.md`. The
report of a run is the status; see `sleelactl/convert/README.md`.

`bash examples/java/differential/run-differential.sh` is the older hand-converted suite.

Earlier (2026-11-13, JDK 25.0.2), kept as history: 22 programs and 9 refusals; 420 of 884 subset files (496 SLeeLa
files; 426 with `--single-thread`), first refusals led by `nested-ambiguous` (131 files), `nested` (67) and
`untyped` (57). That run predates the resolution and member checks that made later conversions stricter, so its
count is not comparable with the current one.

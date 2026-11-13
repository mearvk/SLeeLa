# Java to SLeeLa Conversion: Principles, What Is Done, What Is Left

Status: summary as of 2026-11-13. This file collects the Java to SLeeLa work in
one place. It adds no rule of its own. The normative texts are:

- `SLEELA.syntax`, sections 37.5, 38.4, 40, 41, 43, 44, 45 and 46 (the language);
- `lib/java/JAVA.SPEC.md` (the Java tier and `sleelactl java-convert`);
- `lib/java/converter/README.md` (the SLeeLa-written converter);
- `lib/java/jdk-28/LOWERING.md` and `lib/java/jdk-28/PURPOSE.md` (lowering designs, gap register);
- `impl/subjects/conversion/conversion.model` (the shared conversion laws);
- steering: `.kiro/steering/java-to-sleela-conversion.md`, `java-converter-lib.md`,
  `validation-cadence.md`.

Where this file and one of those disagree, the normative text wins.

## 1. Principles

1. **Functional equivalence.** A converted program run on the SLeeLa VM must produce the same
   observable behaviour (output, exceptions, results) as the Java program on a JDK.
2. **Proof by output.** Equivalence is shown by running both sides and comparing their output
   byte for byte. This covers the differential pairs, the `sleelactl` conversion tests and the
   converter samples. A claim is not made without a run.
3. **Fail loud, never wrong.** A construct that cannot be converted exactly is refused or stubbed,
   never approximated. `sleelactl java-convert` refuses the whole file. `lib/java/converter` writes
   a per-member stub that throws `UnsupportedOperationException` and keeps the Java body as a
   comment. When a lowering is unsure, the result is a check error, not a call to the wrong method.
4. **Lower, do not add syntax.** A Java construct with no SLeeLa form is lowered to forms SLeeLa
   already has (`LOWERING.md`). Java-only behaviour lives in the Java tier: `lib/java` library
   code and the converters (`JAVA.SPEC.md` section 1). General language gaps found on the way are
   fixed in the language, written in `SLEELA.syntax` first.
5. **One set of laws.** Operator, cast, string-conversion and result-type laws are rows of
   `conversion.model`. Both converters read the same rows, and a law is changed in the model, not
   in a converter.
6. **Output syntax stays fixed.** `lib/java/converter` writes `#sleela 3.1` (owner decision);
   `sleelactl java-convert` writes `#sleela 3.2`.
7. **Faithful library.** `lib/java/jdk-28` holds one SLeeLa unit per public JDK 28 type (4,721,
   1:1). A unit that computes something must compute what the JDK computes (for example a
   correctly rounded `Math.sqrt`).
8. **Every change is documented and gated.** Spec first, then a backup of edited compiler or VM
   files (`impl/backup/<topic>-<date>/`). After the edit: the regenerated
   `security/sha256-manifest.json`, the library inventory counts, and the regression tests.

## 2. What has been done

### 2.1 Language and VM (`SLEELA.syntax`, `impl/frontend`, `impl/core`)

| Item | What it gives Java conversion | Where | Proof |
|---|---|---|---|
| Java signature surface, static member access | `Type.member`, Java modifiers, generics erased to `Any` | 38 | spark / oop tests |
| Overload resolution | methods and constructors chosen by argument types | 40 | oop tests |
| Methods on String values | `s.length()`, `s.charAt(i)` bind to `JavaStringMethods` | 41 | conformance |
| Java object behaviour | subtypes, `super(args)`, `this(args)`, implicit `super()`, virtual dispatch, `super.m()`, bodiless methods, enhanced `for`, `L`/`d`/`f` and `_` literals, typed / multi `catch`, `finally`, `++`/`--` | 43.6 | `impl/tests/java_references/oop` |
| Exact `long` comparison | int64 compares, not through `double` | 43.4 | differential |
| JDK 28 tier rule | `import java.jdk-28;` is authoritative for every name with a JDK 28 file | 43.4 | conformance |
| Character literals, array initializers, labels, `return`/`break`/`continue` through `finally`, `continue` in a switch fixed | the 3.1 conversion blockers | 44 | `impl/tests/syntax31` |
| `char` type and char as text; `a.length`, `new T[a][b]` | the 3.2 char and array specifications | 45, 46 | `impl/tests/syntax32` |
| Narrowing on store and cast | `byte`, `short`, `char`, `int32` wrap at Java width | compiler `emitJavaNarrow` | `OperandSemantics` pair |
| Bodiless method of a series integer returns 0 | abstract / interface `int32 m();` default value | 43.6 rule 8 | oop tests |
| `new int32[n]` starts at 0 | Java `int[]` default | 44.3, `arrayFillKind` | converter sample |
| Interfaces are runtime supertypes | `(Shape) c`, `c instanceof Shape`, `catch (Iface e)` | 43.7; `slvm_add_struct_super`; artifact format 5 | `oop/any_dispatch` |
| Calls on an `Any` receiver | `x.m(args)` on an erased value dispatches at run time over the loaded classes (and `JavaStringMethods`) | 43.7 | `oop/any_dispatch`; JDK sample 553 to 522 errors |
| `MIN / -1` and `MIN % -1` | no SIGFPE; `MIN` and `0` as in Java | 37.5 | `oop/vm_faults` |
| VM faults inside a `try` | `/ 0`, `% 0`, array index, failed cast throw `ArithmeticException`, `ArrayIndexOutOfBoundsException`, `ClassCastException` (Java messages) through `SLJavaFaults` | 37.5; artifact format 6 | `oop/vm_faults` |

### 2.2 Java tier library (`lib/java`)

| Item | Role | Where | Proof |
|---|---|---|---|
| JDK 25 and JDK 28 unit trees | one SLeeLa unit per public JDK type; real implementations where written | `lib/java/jdk-25`, `lib/java/jdk-28` | `validate_jdk28.py` (4,721 : 4,721), conformance |
| `JavaOperands` | 32-bit `int` arithmetic, shifts, narrowing, `divInt` / `remInt` / `divLong`, JLS 5.1.3 conversions | `lib/java/JavaOperands.sleela` | `OperandSemantics` pair |
| `JavaFloat` | binary32 `float` held in a double, rounded after each operation | `lib/java/JavaFloat.sleela` | `Floats` conversion test |
| `JavaFormat` | `String.format` conversions, `String.join` | `lib/java/JavaFormat.sleela` | `Formats` conversion test |
| `JavaText`, `JavaStringMethods` | UTF-16 text and String methods | `lib/java/jdk-25/implementations` | conformance |
| `SLJavaLambda`, `SLJavaLambdaTable` | lambda values `(id, env)` answering every common functional-interface method | `lib/java/support` | `LambdaDemo` sample |
| `SLJavaFaults` | builds the Java exception for a VM fault inside a `try` | `lib/java/support/SLJavaFaults.sleela` | `oop/vm_faults` |
| `Math.sqrt` correctly rounded; `Math.abs` / `absExact` at `MIN_VALUE` | library results identical to the JDK | `lib/java/jdk-28` | `MathSqrt` pair, converter sample |
| Differential pairs | Java and SLeeLa side by side, output identical | `examples/java/differential` | 18 of 18 identical |

### 2.3 Conversion laws (`impl/subjects/conversion/conversion.model`)

| Item | Role | Proof |
|---|---|---|
| `binop`, `unop`, `cast`, `compound`, `incr` rows | Java operator and cast laws as templates | both converters |
| `tostr` rows | Java text of `double`, `float`, `char`, objects | converter samples |
| `ret` and `field` rows (model 1.1.0) | 8,922 reflected result types of the OpenJDK 25.0.2 API, for typing call chains | `generate_ret_rows.py --check` |

### 2.4 `sleelactl java-convert` (C/C++, `sleelactl/convert`, whole-file refusal)

| Area | Lowered | Proof (`sleelactl/convert/tests/java`) |
|---|---|---|
| Integer arithmetic, narrowing, division by zero | through `JavaOperands`; `long`, `double` native | `Operands`, `Basics` |
| `float` | through `JavaFloat` | `Floats` |
| Characters and strings | char codes, JLS 5.1.11 string conversion, `String.valueOf`, `join`, literal `format` | `Strings`, `Formats` |
| Control flow | switch (fallthrough, arrows), switch expressions (hoisted), labels, try-with-resources with suppressed exceptions | `Control`, `Flow`, `Switches`, `Resources` |
| Expressions | assignment and `++`/`--` used as values with the 32-bit wrap | `Switches` |
| Classes | enums (constant bodies as subclasses), varargs packing, name mapping (`close` to `closeResource`), `main(String[])` | `Enums`, `Varargs`, `Shapes` |
| Nested and inner types | hoisted to `Outer__Name`; inner classes hold `outer_` | `Nested` |
| Anonymous classes, lambdas, method references | generated classes `Top__N` with `cap_` fields and `outer_`; target type from the declaration, assignment, return, field or callee signature | `Anon`, `Functional`, `Refs` |
| `instanceof T v` | declared before the statement, assigned in the test | `Refs` |
| Records | canonical and compact constructors, accessors, `toString`, `equals`, `hashCode` | `Records` |
| Fields and initialization | Java default values; field initializers and instance `{ }` blocks after `super(...)` (JLS 12.5); library superclass members typed | `Init` |
| Stale output | a re-run removes what the previous run wrote, so a failed-closed file leaves nothing | suite `stale-output` check |
| Arrays | one and many dimensions, jagged, nested initializers, `a.length` | `Grid` |
| Concurrency | `synchronized` / `volatile` plain under `--single-thread`, else refused | `Sync` |
| Refusals | proven by programs that must be refused | `tests/reject` (4) |
| Qualified nested names | `Outer.Inner` in a type is `Outer__Inner`; a nested class no longer extends itself | `reject/QualifiedNested` |
| Whole suite | `tests/javaconv_differential_test.sh` | 20 programs + 7 refusals + stale-output check (28 of 28, 2026-11-13) |
| Whole-tree run | JDK 25.0.2 `java.base` (`java/lang`, `java/util`, `java/math`, `java/text`), 884 files | 423 converted (429 with `--single-thread`), 2026-11-13; 344 on 2026-11-12 |

### 2.5 `lib/java/converter` (written in SLeeLa, per-member stubs, output `#sleela 3.1`)

| Version | Lowered | Measured |
|---|---|---|
| 0.1.0 | walk, lex, rewrite, translate, fail-loud stubs, report | 17-file JDK sample: 47 stubs |
| 0.2.0 | 3.1 forms kept (array initializers, char literals, labels, `finally` jumps); `for` headers with commas; pattern `instanceof` in two shapes | 35 stubs |
| 0.3.0 | typed pass: `int` shifts through the model; `double` / `char` shown as text; `JCONV_BUNDLE=1` (one document per Java file) | check errors 903 to 553 bundled |
| 0.4.0 | laws read from `conversion.model` (`JConvModel`); call chains typed through `ret` / `field` rows | sample program 29 of 29 lines identical |
| 0.5.0 | expression tree (`JConvExpr`): one `toInt` wrap where an `int` value leaves int arithmetic; `divInt` / `remInt` | `ConvDemo` 31 of 31 identical |
| 0.6.0 | lambdas (0 to 2 parameters, typed or target-typed) and method references with a known target lowered to `(id, env)` (`JConvLambda`); functional interface types written as `Any` | `LambdaDemo` 17 of 17 identical; eight lambda-dense `java.util` files 31 to 15 stubs |
| 0.7.0 | inner classes reach the enclosing instance: `slOuter` field, `slOuterArg` parameter, `new Inner(this, ..)`, `Outer.this`, outer members prefixed (`JConvOuter`) | JDK sample: 26 stubs, 475 check errors, 0 syntax errors |

### 2.6 Process

| Item | Where |
|---|---|
| Backups of every edited compiler / VM file | `impl/backup/java-converter-2026-11-13`, `any-dispatch-2026-11-13`, `vm-faults-2026-11-13` |
| Execution gate | `security/sha256-manifest.json`, `make check-manifest` |
| Library inventory | `lib/LIBRARY.SYMBOLS.md`, `lib/LIBRARY.INDEX.md` revisions 1.08 to 1.14, `test-suites/test-library-inventory.sh` |
| Long suites run occasionally, fast checks always | `.kiro/steering/validation-cadence.md` |
| Standing converter decisions | `.kiro/steering/java-converter-lib.md`, `java-to-sleela-conversion.md` |

## 3. What is left

| Item | Where it shows | Owner area | Next step |
|---|---|---|---|
| Statics written as instance methods in jdk-28 units (`Float.isNaN`, `Objects.checkIndex`, `Arrays.copyOf`, `Character.charCount`) | largest check-error group on the JDK sample (94 of 475) | library (`lib/java/jdk-28`) | regenerate those units with `static` |
| Nested type files do not resolve (`Outer.Inner.sleela`) | separate-file conversions; a nested type resolves only inside its bundled document | loader (`impl/frontend/driver.cpp`) | resolve the dotted unit name |
| An `Any` value is not assignable to a typed variable or return without a cast | `method 'stream' returns Any, expected Stream` | language (javac-style checked cast) or converter (insert casts) | decide where the inserted cast lives |
| Operands of unknown type (no generics inference) | arithmetic on generic parameters refused or unknown | both converters | collect generic signatures, substitute type arguments |
| Lambdas with 3 or more parameters | stub reason 1 | `lib/java/converter`, `SLJavaLambdaTable.call` takes two arguments | widen `call` or pass an argument array |
| Lambdas that capture an array or take an array parameter | stub reason 1 | language: a cast to an array type does not compile | allow `(T[]) x` checked casts |
| Method references without a known target, or `Type::m` of another class | stub reason 2 | `lib/java/converter` | target typing from library signatures; static versus unbound from the model |
| Lambda ids are per document | several converted documents with lambdas in one program clash on `SLJavaLambdaTable` | `lib/java/converter` | one table per program, or ids folded with a document number |
| Anonymous and local classes | stub reasons 3, 14 in `lib/java/converter` | `lib/java/converter` (`sleelactl` lowers them) | the `JConvOuter` mechanism plus captured locals |
| `outer.new Inner()`, a static method of an inner class reaching an outer member | not lowered by either converter | both converters | pass the explicit outer reference |
| A lambda or anonymous class inside another generated class | refused by `sleelactl` | `sleelactl` | capture through the enclosing generated class |
| Switch expressions, multi-label and arrow cases | stub reason 5 in `lib/java/converter` | `lib/java/converter` (`sleelactl` lowers them) | hoist to a temporary as `sleelactl` does |
| `case Type t ->` patterns, record patterns, sealed exhaustiveness | refused / stubbed | both converters | per-arm cast declaration |
| Pattern `instanceof` outside the two lowered shapes | stub reason 6 | `lib/java/converter` | the `sleelactl` form (declare, then assign in the test) |
| `var` | stub reason 8 | `lib/java/converter` | infer from the initializer through the typed pass |
| Class literals `X.class`, `getClass().getName()` | stub reason 9; a user exception prints the library class name | language / library | a runtime class-name query |
| `enum` declarations in SLeeLa source (43.5); enums nested in a class, `this(...)` in an enum constructor | lowered by both converters except those two forms, refused by `sleelactl` | language / `sleelactl` | — |
| `static { }` blocks; `Object.equals`/`hashCode`/`getClass` on a user type (enums included) | refused by `sleelactl` (`init-block`, `object-method`) | `sleelactl` / library | guarded static init; identity `equals` and hash |
| One simple name for several nested types in a file (`nested-ambiguous`) | first refusal of 133 of the 461 JDK files that fail closed (2026-11-13), the largest group | `sleelactl` | resolve a bare nested name by scope (innermost enclosing type first) |
| Converted is not checked | a split-layout file names sibling nested types a standalone `sleela check` does not link | loader / `sleelactl` | measure `check` over the tree; bundle or resolve `Outer__Inner` files |
| `String` static members in `lib/java/converter` | stub reason 22 | `lib/java/converter` | the `JavaFormat` route `sleelactl` uses |
| `System.out.print` without a newline | stub reason 23 | language / library (`print` always adds a newline) | a no-newline print built-in |
| Files with non-ASCII bytes | skipped by `lib/java/converter` (12 of 15,020 JDK files) | VM `read` returns 4,095-byte plain String pieces | a byte-level read |
| Double `%` by zero | a fault in SLeeLa, `NaN` in Java | VM (37.5) | return `NaN` for the double case |
| Null dereference, stack overflow | still fatal, not `NullPointerException` / `StackOverflowError` | VM (37.5) | extend the fault kinds |
| Plain `int` is 64-bit | converted code is exact (through `JavaOperands`); hand-written SLeeLa is not Java | language decision | keep, or a Java-int mode |
| `synchronized`, `volatile`, Java memory model | converted single-threaded only | language / VM | real monitors |
| Stores into `int` array elements do not wrap in the VM | `a[i] = x + y` keeps a 64-bit value until read through a wrap | VM / converter | wrap on store into `int32[]` |
| `print` of a double shows `9`, Java `9.0` | handled by the converters (`Double.toString`), not by the language | converters | — |
| `java_equivalence` model not called by semantic analysis; `java_flow.cpp`, `java_exceptions.cpp` not linked | 43.5 | compiler | wire them or retire them |
| No Java bytecode path | `sleela run Foo.java` hands the file to a JVM | out of scope today | — |
| Streams | fused-loop lowering designed, not built | converters | `LOWERING.md` section 3 |
| `native` methods | no Java body | none | not convertible |

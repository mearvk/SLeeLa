# /lib/java/converter: Java source to SLeeLa source

> **Container.** Java container content (`SLEELA.syntax` section 38.4). Nothing
> here is part of the SLeeLa specification or adds syntax to SLeeLa.

Status: **0.7.0, best effort (2026-11-13).** Output stays at syntax **3.1** (also
written 3.01) by the owner's decision; syntax 3.1 sections 45 and 46 forms are not used
(`.kiro/steering/java-converter-lib.md`). The operand laws come from the shared
conversion model, `impl/subjects/conversion/conversion.model` (read by
`JConvModel`), the same data `sleelactl java-convert` reads. Output declares `#sleela 3.1` and
uses its forms (SLEELA.syntax section 44): array initializers, character
literals, labels, and jumps that run `finally`. Revise the rules here as the
language and the library close the gaps listed in section 6. Changes since
0.1.0 are in section 8.

**Relation to `sleelactl java-convert`.** A separate converter, written in C/C++
under `sleelactl/convert/` with its Java semantics in
`impl/subjects/conversion/conversion.model`, landed on `master` the same day. Its
always-on steering (`.kiro/steering/java-to-sleela-conversion.md`) requires
behavioural equivalence proven against a real JDK and fails closed **per file**.
This module is the SLeeLa-language, best-effort converter that was asked for in
`/lib/java/converter`: it fails loud **per member** (a stub that throws), so a
file with one unconvertible member still yields the rest of its type. It does
not claim equivalence. Its known near-misses are listed in section 6.6; refused
constructs are listed in section 4.

**Policy (decided 2026-11-13 by the repository owner): per-member stubs.**
`lib/java/converter` keeps the rest of a type when one member cannot be
converted, and marks that member with a stub that throws. It does not adopt the
whole-file refusal of `sleelactl java-convert`. Where both converters share a
law (the `binop`, `tostr` rows of `conversion.model`), this module applies the
same template, so the two agree on what the converted code computes.

## 1. What it is

A SLeeLa program that walks a Java source tree recursively and writes the
equivalent SLeeLa source files, one per Java type, under an output tree with the
same folders. Run on the JDK sources (the `<module>/<package>/<Type>.java` layout
of a JDK `src.zip`), it produces the JDK 28 type tree as converted SLeeLa:
`java.base/java/util/Optional.java` becomes `java.base/java/util/Optional.sleela`.

Every Java-to-SLeeLa decision with functional or declarative significance is
taken in this module, in one place: [`JConvRules.sleela`](JConvRules.sleela)
holds the table, and the other units apply it.

The output is **not** written into `lib/java/jdk-28`. That tier is curated
(API-derived, no Java source text) and the converter refuses to write there.
Converted files are derived from the Java source. Each one keeps the licence
comment of its source, because a conversion of OpenJDK code is a derivative work
under its licence (GPLv2 with the Classpath Exception).

## 2. Running it

```sh
# unpack a JDK source tree (any tree of .java files works)
mkdir -p /tmp/jdk-src && cd /tmp/jdk-src && unzip -q "$JAVA_HOME/lib/src.zip"

# convert java.util (public JDK 28 types only), from the repository root
JCONV_IN=/tmp/jdk-src JCONV_OUT=/tmp/jdk28-sleela JCONV_FILTER=java.base/java/util \
  bash lib/java/converter/run-converter.sh
```

| Variable | Meaning | Default |
|---|---|---|
| `JCONV_IN` | root of the Java source tree | required |
| `JCONV_OUT` | root of the SLeeLa output tree | `./jdk28-converted` |
| `JCONV_SCOPE` | `api`: only types listed in `lib/java/jdk-28/INDEX.tsv`; `all`: every `.java` file | `api` |
| `JCONV_FILTER` | only paths starting with this prefix | none |
| `JCONV_LIMIT` | stop after this many files | no limit |
| `JCONV_INT` | `int32` (Java 32-bit `int`) or `int` (64-bit SLeeLa `int`) | `int32` |
| `JCONV_COMMENTS` | `keep` or `drop` (the licence comment is always kept) | `keep` |
| `JCONV_MODEL` | the conversion model to read | `$SLEELA_HOME/impl/subjects/conversion/conversion.model` |
| `JCONV_BUNDLE` | `1`: nested types go into the document of their top-level type, so a converted program runs as one document; `0`: one file per type (`Outer.Inner.sleela`, the library layout) | `0` |

`sleela run` treats every argument as a source file, so the settings travel in
the environment. The script names only `JConvMain.sleela`; the loader finds the
other units in `/lib` by name. Naming every file makes each one resolve library
units again and duplicates classes (`lib/java/STATUS.md` section 6).

Output: one `.sleela` per type (nested types as `Outer.Inner.sleela`, as
`lib/java/jdk-28` names them), plus `CONVERSION-REPORT.tsv` (one line per Java
file, written as the run goes) and `CONVERSION-SUMMARY.md`.

Speed: about 3 seconds per Java file on the source-built VM (16 files, 13,552
output lines in 50 s). The 4,721 public JDK 28 types are therefore a run of
several hours. Use `JCONV_FILTER` to convert one package or module at a time.

## 3. The rules (all in `JConvRules` and applied by `JConvLexer` / `JConvRewriter` / `JConvTranslator`)

| # | Java | SLeeLa output | Why |
|---|---|---|---|
| 1 | Identifiers that are SLeeLa keywords or built-ins: `print`, `close`, `record`, `struct`, `require`, `sealed`, `yield`, `Sleela`, `Any`, `utf16`, `int64`, `int128` | trailing `_` (`close_`) at declaration and every use | `lib/java/jdk-28/PURPOSE.md` row 27. `close` is the Munction reach-handle built-in on every receiver; `print` is a statement keyword |
| 1b | Unqualified call to a method named like a VM built-in (`read`, `write`, `sleep`, `join`, `lock`, ...) | `this.read(...)`, or `Type.read(...)` for a static method | an unqualified call binds to the built-in (steering `sleela-authoring-gotchas.md`) |
| 1c | `$` in an identifier | `_S_` | not a SLeeLa identifier character |
| 2 | `int` | `int32` (or `int` with `JCONV_INT=int`) | `int32` wraps at 32 bits on store, cast, parameter and return, as Java does (`impl/frontend/compiler.cpp` `emitJavaNarrow`). Plain `int` is 64-bit |
| 2b | `byte`, `short`, `char`, `long`, `float`, `double`, `boolean` | kept | `byte`/`short`/`char` wrap at their width on store and cast |
| 2c | Generic type arguments (`List<String>`, `new HashMap<>()`, `Foo.<T>bar()`) | erased | SLEELA.syntax 38.1. Class type parameters are kept as data in `JAVA_TYPE_PARAMETERS`, method type parameters in `@generic("<T>")` (the jdk-28 convention), and type variables erase to `Any` |
| 2d | Fully qualified names `java.util.List` | `List` | the `import java.jdk-28;` tier resolves the simple name |
| 2e | `x instanceof String` | `(x instanceof string \|\| x instanceof utf16)` | a Java String is either SLeeLa string kind (only for a receiver without calls, so it is not evaluated twice) |
| 2g | `T x = expr;` and `return expr;` with `T` a primitive, `String` or a known class (not an array: the compiler rejects array casts) | `T x = (T) (expr);` | javac inserts the same checkcast. SLeeLa rejects an erased `Any` stored into a typed variable. **Not** for interfaces: a runtime cast to an interface fails in the VM today |
| 3 | `strictfp`, `sealed`, `non-sealed`, `permits` | dropped | no SLeeLa meaning (Java 17 made floating point strict; sealing is a compile-time check) |
| 3b | `protected` on an instance member | `@JavaProtected` | SLeeLa reserves `protected` for static members |
| 3c | `final` on locals and parameters | dropped | SLeeLa rejects it there; `final` fields are kept |
| 3d | `public`, `static` on a type | dropped | every converted type is a top-level SLeeLa class |
| 3e | Java annotations (`@Override`, `@Deprecated`, ...) | dropped | metadata only in Java, and they would clash with SLeeLa annotations |
| 3f | Interface fields | `static final` added | explicit constant |
| 4 | `class` / `interface` | `class` / `interface` (`extends`, `implements` kept) | |
| 4b | `enum E { A, B(1); ... }` | class `E` with `static final E A = new E("A", 0);`, fields `enumName` / `enumOrdinal`, `name()`, `ordinal()`, `toString()`, `compareTo`, `values()`, `valueOf(String)`; declared constructors get the name and ordinal first | SLeeLa has no `enum` declaration (SLEELA.syntax 43.5); this is the shape of the jdk-28 enums |
| 4c | `record R(int x, String y)` | class `R` with `final` fields, the canonical constructor, accessors, `toString` (`R[x=1, y=a]`), `equals`, `hashCode`. A compact constructor runs its body, then assigns the fields | the record `hashCode` algorithm is unspecified by Java |
| 4d | `@interface` | an empty `interface`, with the elements kept as a comment | annotations carry no SLeeLa behavior |
| 4e | Nested type `Outer.Inner` | its own file `Outer.Inner.sleela`, class `Inner` | the jdk-28 layout |
| 4f | `static { ... }` | `static int slStaticInitN_ = slStaticInitN();` and the block as that method | it runs at class initialization, in source order |
| 4g | Instance initializer `{ ... }` | `int slInstanceInitN_ = slInstanceInitN();` | it runs for each new instance before the constructor body |
| 5 | Literals: octal, hexadecimal and binary int literals | decimal, wrapped to the int range (`0xFFFFFFFF` is `(-1)`, `0x80000000L` stays positive) | Java reads them that way |
| 5b | `.5`, `5.`, `1e5`, `1.5f` | `0.5`, `5.0`, `1.0e5`, `1.5` | SLeeLa number forms |
| 5c | Escapes `\s`, octal `\12`, `\uXXXX`, surrogate pairs | decoded and written in SLeeLa form | SLeeLa knows fewer escapes |
| 5d | Text blocks `"""` | string literal, incidental indentation removed (JLS 3.10.6) | |
| 5e | `'a'` | `'a'` (SLeeLa 3.1 character literal, 44.2), or `"a"` next to a string in `+` | a SLeeLa character literal is its integer code, so `"x" + 'a'` would print `x97`; a surrogate stays its integer code |
| 5f | `//`, `/* */`, `/** */` | `<< ... >>` on their own line, inner `>>` / `<<` separated | the only multi-line comment form; `/* */` is not SLeeLa |
| 6 | `a.length` on an array | `arrayLength(a)` | SLeeLa arrays have no `length` member (not for `this.length`) |
| 6b | `int a[]` | `int32[] a` | |
| 6c | `{1, 2, 3}`, `{{1, 2}, {3}}` and `new int[]{...}` | kept (SLeeLa 3.1 array initializer, 44.3) | |
| 6d | `new int[n]` | kept: numeric and boolean arrays start at zero / `false` (44.3 fill kind; `int32` since the 2026-11-13 compiler fix) | |
| 6e | `int a = 1, b = 2;` | one declaration per variable | |
| 6f | `for (...) ;`, `while (...) ;` | `{ }` body | SLeeLa has no empty statement |
| 6i | `for (int i = 0, j = n; c; i++, j--) body` | `{ int i = 0; int j = n; for (; c; ) { slForN: { body } i++; j--; } }`, each `continue` of the loop becomes `break slForN;` | SLeeLa `for` takes one initializer and one update; the labeled block (44.4) keeps Java order on `continue`. A body that is itself `if` / `for` / `while` / `switch` / `try` without braces stays a stub (reason 11) |
| 6j | `if (o instanceof T v [&& rest]) S [else S2]` | `if (o instanceof T) { T v = (T) (o); [if (rest)] S } [else S2]` (`else` only without `&& rest`) | SLeeLa has no pattern variable. Only when `o` is a name chain without calls (it is evaluated twice) and `T` a class (a cast to an interface fails at run time) |
| 6k | `return o instanceof T v && rest;` | `if (!(o instanceof T)) { return false; } T v = (T) (o); return rest;` | the `equals` shape; same conditions |
| 6l | Labels, `break label`, `continue label`, `return` / `break` / `continue` through `finally` | kept | SLeeLa 3.1 (44.4, 44.5) |
| 6m | `x++` and `(a = b)` used as a value | kept | the language evaluates both (43.6 rule 12); the value wraps at 32 bits only when stored |
| 7a | `a << n`, `a >> n`, `a >>> n` with `a` of type `int`; `x <<= n` and friends | the function of the model's `binop int <<` / `>>` / `>>>` row (`JavaOperands.shlInt(a, n)`, `shrInt`, `ushrInt`); `x = JavaOperands.ushrInt(x, n)` | read from `conversion.model`: Java masks the count to 5 bits and works in 32 bits (`h ^ (h >>> 16)` in `HashMap.hash` was wrong before). `long` shifts are already exact and stay |
| 7b | a `double` or `float` shown as text (string concatenation, `println`) | the model's `tostr double` template (`Double.toString(x)`) | Java prints `2.0`, SLeeLa `2` |
| 7c | a `char` shown as text | the model's `tostr char` template (`JavaText.plainIfRepresentable(JavaText.codePointU(c))`) | Java prints `z`, SLeeLa `122` |
| 7e | `int` `+ - * & | ^ ~` and shift left whose value leaves int arithmetic (a comparison, `/`, `%`, shift right, string text, an argument, an index, a cast to `long` / `double`, a store into a non-int) | `JavaOperands.toInt(...)` around the whole chain (the model's `cast long int` row) | these operators agree with arithmetic modulo 2^32, so one wrap where the value is used gives the Java result; a chain stored into an `int` stays native (the store wraps). `if (count + 1 < 0)` and `long x = a * b` now compute as Java does |
| 7f | `int` `/` and `%` (and `/=`, `%=` on an int name) unless the divisor is a nonzero literal | the model's `binop int /` and `%` rows (`JavaOperands.divInt`, `remInt`) | exact, including `Integer.MIN_VALUE / -1`, and a catchable `ArithmeticException` on zero (the VM division by zero was fatal; since SLEELA.syntax 37.5 it is catchable inside a `try` too) |
| 7d | operand types for 7a to 7c | fields of the type and declarations of the member (parameters, locals, `for`, `catch`), array elements, casts, literals, and **call chains**: `Math.sqrt(x)`, `s.charAt(i)`, `s.length()`, `sb.reverse().toString()`, `Integer.MAX_VALUE`, own methods, through the model's `ret` and `field` rows (`@arg0`: the type of the first argument) | a name declared with two types in one member, an overloaded own method with two result types, or a member the model does not type makes the operand unknown, and an unknown operand is left as written: the pass never guesses |
| 6g | `assert` | dropped | Java runs with assertions disabled by default, so the statement is not evaluated |
| 6h | `System.out.println(x)` | `print(x)` | `System.out.print` (no newline) is a stub (reason 23): `print` always adds one |
| 8a | a lambda `x -> e`, `(a, b) -> e`, `(int a, int b) -> { ... }` (0 to 2 parameters) | `new SLJavaLambda(N, new Any[]{this, k, ...})` at the site; the body becomes `Any slLambdaN(Any[] slEnv, Any slA, Any slB)` in the same class, which declares each captured local from `slEnv` and each parameter from `slA` / `slB`; the document gets one generated `SLJavaLambdaTable` whose `call` forwards `N` to it (`JConvLambda`) | defunctionalization, `lib/java/jdk-28/LOWERING.md` section 2. Java captures only effectively final locals, so copying their values at the site is exact. An instance context stores `this` in `slEnv[0]` and the body is an instance method, so fields, methods and `this` read as written. An untyped parameter takes its type from the target: a declaration `Function<Integer, Integer> f = ...`, the method return type for `return x -> ...`, the declared parameter of a method of this file the lambda is passed to, or the result type argument of an enclosing lambda (`a -> b -> ...`); boxes become primitives (`Integer` is `int32`); else `Any`. Not lowered (stub reason 1): a switch arrow, more than two parameters, a captured local or parameter of array type (a cast to an array type does not compile) |
| 8b | a method reference | lowered like 8a: `System.out::println` (a `Consumer`); with a known target interface `this::m`, `name::m` (the receiver is evaluated once, at the reference), `Type::m` (static, or unbound for `String` and own instance methods) and `Type::new` | the arity and the parameter types come from the target interface. Without a target, or `Type::m` of another class (static or unbound cannot be told apart), stub reason 2 |
| 8d | an inner (non-static) class `class Inner` declared in a class | the field `Outer slOuter` (only in a base inner class) and a first constructor parameter `slOuterArg` (a default constructor is written when there is none); `this.slOuter = slOuterArg;` after an explicit `super(...)` of a non-inner superclass, else first; `this(...)` and `super(...)` of an inner superclass pass `slOuterArg` on. `new Inner(a)` becomes `new Inner(this, a)` in the outer class and `new Inner(this.slOuter, a)` in a sibling inner class; `Outer.this` becomes `this.slOuter`, `Inner.this` becomes `this`; an unqualified member of the outer class becomes `this.slOuter.x` (instance) or `Outer.x` (static) (`JConvOuter`) | JLS 8.1.3, 15.9.2, 15.8.4; `lib/java/jdk-28/LOWERING.md` section 2.4. A name is left alone when it is a local, a member of the inner class, or possibly inherited (JLS 6.4.1: the class and its supertypes come before the enclosing class): inherited names come from the model `ret` / `field` rows or a type of the file written earlier, and when the superclass is unknown no method name is rewritten, so a doubt gives a check error, never a call to the wrong method. Not lowered: `outer.new Inner()`, an anonymous or local class (reasons 3, 14), a static method of an inner class reaching an outer instance member |
| 8c | a variable, field, parameter, return type or cast of a functional interface type (`Function`, `Comparator`, `Runnable`, the 43 of `java.util.function`, ...) | `Any` | so `f.apply(x)` dispatches at run time (SLEELA.syntax 43.7) to `SLJavaLambda.apply` or to a program class that implements the interface. A class of the file with the same name is kept |

## 4. Fail-loud stubs (rule 5 of `JConvRules`)

A member that uses a construct SLeeLa cannot express yet is converted to a stub
that throws `UnsupportedOperationException("not converted to SLeeLa (<reason>): <Type>.<member>")`,
with `@throwable("java.lang.UnsupportedOperationException")`, and the original
Java body kept beside it as a comment for manual conversion. A gap therefore
fails loudly at the call and never returns a wrong answer (`lib/java/jdk-28/PURPOSE.md`).
A field with such an initializer keeps its declaration with the Java zero value
and is listed in the report. An initializer block with such a construct is not
run, and is listed in the report.

| Code | Reason |
|---:|---|
| 1 | lambda or switch arrow (lambdas lowered since 0.6, rule 8a; still a stub: switch arrows and the lambdas 8a lists) |
| 2 | method reference (lowered since 0.6 where rule 8b applies) |
| 3 | anonymous class |
| 4 | labeled statement (retired in 0.2) |
| 5 | switch expression or multi-label case |
| 6 | pattern matching `instanceof` outside the two lowered shapes (rules 6j, 6k) |
| 7 | try-with-resources |
| 8 | `var` |
| 9 | class literal `X.class` |
| 10 | qualified `this` or `super` (`Iface.super.m()`, `Outer.this` of a class that is not the enclosing class; `Outer.this` of the enclosing class is lowered since 0.7, rule 8d) |
| 11 | comma in a `for` header whose body is an unbraced compound statement |
| 12 | multi-dimensional array creation |
| 13 | `return`, `break` or `continue` inside a `try` that has `finally` (retired in 0.2: 44.5) |
| 14 | local class inside a method |
| 15 | hexadecimal floating point literal |
| 16 | unpaired surrogate in a string or char literal |
| 17 | `native` method |
| 18 | nested array initializer (retired in 0.2: 44.3) |
| 20 | enum constant with a class body (the body is dropped and reported) |
| 22 | static member of `java.lang.String` (`valueOf`, `format`, `join`, ...): `String` is a SLeeLa keyword and cannot be a static receiver, and lowering `valueOf` to `"" + x` would print a `double` or `char` in SLeeLa form |
| 23 | `System.out.print` without a newline |

## 5. Every converted file

Each output file carries, in order: a header naming the Java type, module and
source; the licence comment of the Java source; `#sleela 3.1`; `spark;` (Java
style declarative, SLEELA.syntax 43.2); `import java.jdk-28;`; the type's Javadoc;
the class with the Metadata Class block (author, copyright, trademark and
corporation are taken from the licence comment: Oracle / OpenJDK, GPLv2 with the
Classpath Exception, Java, Oracle Corporation; `metaScienceRating = 3` marks
unverified converted code); the housing fields `javaType`, `javaModule`,
`javaSource` (SLEELA.syntax 38.4); the members; the generated helpers; and a
closing comment with the member and stub counts. Interfaces get no Metadata block
(their fields would be constants).

## 6. Measured result and known gaps (2026-11-13, converter 0.2.0)

Sample: 17 files from the OpenJDK 25.0.2 `src.zip` (`Math`, `Record`, `Override`,
`Runnable`, `DayOfWeek`, `AbstractList`, `BitSet`, `HashMap`, `HexFormat`,
`Objects`, `Optional`, `OptionalInt`, `StringJoiner`, `StringTokenizer`, `UUID`,
`Adler32`, `CRC32`), `JCONV_SCOPE=all`.

| Measure | 0.1.0 | 0.2.0 | 0.3.0 | 0.3.0, `JCONV_BUNDLE=1` |
|---|---:|---:|---:|---:|
| Java files converted | 16 of 17 | 16 of 17 | 16 of 17 | 16 of 17 (`UUID.java` skipped: non-ASCII bytes) |
| SLeeLa files written | 36 | 36 | 36 | 16 (one document per Java file) |
| Types / members | 36 / 627 | 36 / 627 | 36 / 627 | 36 / 627 |
| Members stubbed (fail loud) | 47 (7.5%) | 35 (5.6%) | 35 (5.6%) | 35 (5.6%) |
| Files with a syntax error | 0 | 0 | 0 | 0 |
| `sleela check` errors in total | 892 | 902 | 903 | **553** |
| Typed-pass rewrites in the output (0.4.0 with model 1.1.0 / without the model file) | | | | 40 / 40 |
| 0.5.0 int wraps (`toInt`) / `divInt`, `remInt` calls in the output | | | | 88 / 7 (check numbers unchanged, 0 syntax errors) |
| Same, compiler with SLEELA.syntax 43.7 (calls on `Any`, interfaces at run time) | | | | **522** (11 files with an error) |
| 0.7.0, inner classes (`JConvOuter`, rule 8d): stubbed members / check errors | | | | **26** / **475** (11 files with an error, 0 syntax errors) |
| Files with no `sleela check` error | 8 of 36 | 8 of 36 | 8 of 36 | 5 of 16 |

**0.6.0, lambdas (2026-11-13).** The 17-file sample has no lambda outside a
member that is a stub for another reason, so its numbers do not move (522
check errors, 35 stubs). On the eight most lambda-dense files of OpenJDK
25.0.2 `java.util` (`Spliterator`, `Map`, `Comparator`, `PrimitiveIterator`,
`Formattable`, `DoubleSummaryStatistics`, `Optional`, `Currency`), with
`JCONV_BUNDLE=1`: **31 stubbed members before, 15 after**; 17 lambdas and 3
method references lowered. `samples/lambda/LambdaDemo.java` (typed and untyped
parameters, captures, `this`, block bodies, a curried lambda, a lambda passed
to a method, `System.out::println`, `Type::m`, `this::m`, `String::length`)
runs **all 17 lines identical** to OpenJDK 25.0.2
(`samples/LambdaDemo.java.expected.txt`, `samples/LambdaDemo.sleela.expected.txt`).

**0.7.0, inner classes (2026-11-13).** On the 17-file sample, bundled: stubs
35 to **26** (nine `Outer.this` members of `HashMap`, `AbstractList` and others
no longer need reason 10) and check errors 522 to **475**, 0 syntax errors.
`HashMap` alone writes 45 `slOuter` references. By message, the remaining
errors are mostly "instance method ... must be called on an instance" (94:
statics written as instance methods in the jdk-28 units), "use of undeclared
variable" (71: names of JDK-internal or not yet resolved types), and errors
that follow from those. Both samples still run identical.

0.2.0 removed 12 stubs (the ten `for` headers with commas and two pattern
`instanceof` members). 0.4.0 changes what the converted code computes, not
whether it compiles, so the check numbers are those of 0.3.0. On these 17 JDK
files the model's `ret` rows add no typed rewrite beyond names, literals and
casts (40 either way): the JDK code here seldom shifts or prints a library call
result directly. In ordinary programs they do (the sample program, below). Bundling removes 39% of the check errors, because a nested
type declared in the same document resolves by its simple name; the remaining
errors come from the library and the language, not from conversion slips: by
message (0.3.0), mostly "use of undeclared variable" (inner classes reaching the fields
of the enclosing instance, lowered since 0.7.0), "struct ... has no field" (inherited types that do
not resolve), and "instance method ... must be called on an instance" (statics
written as instance methods in the jdk-28 units).

**Behaviour, side by side with a real JDK.** `samples/demo/ConvDemo.java` (plain
procedural Java: int overflow, hex / octal / binary literals, a string hash loop,
an array initializer and a nested one, `.length`, `new int[n]`, multi-declaration,
char literals and a `switch` over them, narrowing casts, an enum, a static
initializer, a text block, the ternary, a `for` with commas and `continue`, a
labeled `continue` / `break`, `return` through `finally`, pattern `instanceof`
in `if` and `return`, int shifts inside expressions including `h ^ (h >>> 16)`,
`>>>=`, a `long` shift, `double` and `char` values in string concatenation and
`println`, results of `Math.sqrt`, `Math.abs`, `charAt`, `length()`, a
`StringBuilder` chain and an own `double` method in concatenation) converts with
no stub, as do overflow checks, a `long` from an `int` product, division and
remainder by zero caught as `ArithmeticException`, `Integer.MIN_VALUE / -1`, the
binary-search midpoint and `Math.abs(Integer.MIN_VALUE)`. With `JCONV_BUNDLE=1` it
runs as one document and prints **all 31 lines identical** to OpenJDK 25.0.2
(`samples/ConvDemo.java.expected.txt` against `samples/ConvDemo.sleela.expected.txt`).

Reproduce:

```sh
# from the repository root
JCONV_IN=lib/java/converter/samples JCONV_OUT=/tmp/conv-demo JCONV_SCOPE=all JCONV_BUNDLE=1 bash lib/java/converter/run-converter.sh
R=$(pwd); (cd /tmp/conv-demo/demo && SLEELA_HOME=$R SLEELA_LIB=$R/lib "$R/impl/build/sleela" run ConvDemo.sleela) | grep -v -E '^\[memory-manager\]|^verification|\[security\]|\[defender\]' > /tmp/conv-demo/sleela.out
diff lib/java/converter/samples/ConvDemo.java.expected.txt /tmp/conv-demo/sleela.out
```

A runnable conversion must be one document, which is what `JCONV_BUNDLE=1`
writes: several files given to `sleela run` duplicate the library units, and a
converted type in a separate file whose name is also a JDK 28 type (`Color`, as
`java.awt.Color`) loses to the JDK 28 tier (SLEELA.syntax 43.4). Declared in the
same document it wins.

The remaining check errors in the JDK sample are not conversion slips. They are gaps of the language and
library, in this order of weight:

1. **Nested and inner types do not resolve.** The loader does not load
   `Outer.Inner.sleela` (PURPOSE.md row 4). Since 0.7.0 an inner class reaches
   its enclosing instance (`this.slOuter.size`, `this.slOuter.modCount` in
   `HashMap.KeySet`, rule 8d); a nested type still resolves only inside its
   bundled document. Writing nested types
   inline (SLEELA.syntax 14, hoisted as `Outer__Inner`) was measured on
   2026-11-13 and is not a way out yet: only a plain nested class with fields
   and methods works; a nested class must be named `Outer.Inner` even inside
   `Outer`; a nested constructor that calls `super(...)`, a nested class that
   extends another, static members of a nested class (`Outer.S.K`),
   `new Outer.C[1]`, and a nested interface all fail.
2. **JDK-internal types are not in the tier.** `jdk.internal.access.JavaLangAccess`,
   `jdk.internal.util.Preconditions`, `ZipUtils` and the like are not public API
   and have no `lib/java/jdk-28` file. Converting with `JCONV_SCOPE=all` produces
   them, but they are mostly native or VM-intrinsic.
3. **jdk-28 library units expose some statics as instance methods** (`Float.isNaN`,
   `Objects.checkIndex`, `Arrays.copyOf`, `Character.charCount`: STATUS.md section 6).
4. **An `Any` value is not assignable to a typed variable or return type**
   without a cast (`method 'stream' returns Any, expected Stream`). Since
   2026-11-13 calls on an `Any` receiver dispatch at run time, and casts,
   `instanceof` and `catch` see implemented interfaces (SLEELA.syntax 43.7);
   a call whose method no loaded class declares is still an error.
5. **Files with non-ASCII bytes are skipped** (12 of the 15,020 files in the JDK
   `src.zip`): the VM `read` built-in returns 4,095-byte pieces of a plain String,
   a UTF-8 character can be split between two pieces, and the VM has no
   byte-level view of a plain String to repair it.
6. **Near-misses the token-level conversion cannot see (it has no expression
   types where the typed pass cannot see them):** an operand whose kind is
   unknown (a call the model does not type, an interface-typed receiver, a
   generic result) is left as written, so a `double` there still prints in
   SLeeLa form (`9`, not `9.0`); an `int` chain whose other operand is of unknown
   kind is wrapped (exact, sometimes unneeded). Stores into `int` array elements
   do not wrap in the VM, so `a[i] = x + y` keeps a 64-bit value until it is read
   through a wrap. `switch` over enum constants is not lowered.

Close a gap in the language, VM or library first. Then remove or relax the rule
in `JConvRules` that works around it, re-run the converter, and update this table.

## 7. Units

| Unit | Role |
|---|---|
| `JConvMain` | entry point: settings, walk, convert, summary |
| `JConvConfig` | settings from the environment; `INDEX.tsv` scope set and type kinds |
| `JConvWalker` | recursive directory traversal (`oslsListDirectory`, `osIsDir`) |
| `JConvSource` | loads a file in 4,095-byte pieces; code units and slices |
| `JConvLexer` | Java tokens; literal and comment forms rewritten to SLeeLa |
| `JConvRules` | the decision table (renames, types, modifiers, built-ins, stub reasons) |
| `JConvModel` | reads the shared conversion model (`ret`, `field`, `binop int` shifts / division / remainder, `cast long int`, `tostr`) |
| `JConvExpr` | the expression half of the typed pass: statements, precedence tree, wraps and calls (rules 7a to 7f) |
| `JConvOuter` | inner classes: registry, outer context, `slOuter` rewrites |
| `JConvLambda` | lambda and method reference lowering (rules 8a, 8b): sites, target typing, captures, body methods, the `SLJavaLambdaTable` of the document |
| `JConvRewriter` | token-level rewriting, the fail-loud scan, declaration and cast post-pass, helpers |
| `JConvTranslator` | files, types, members, enum / record lowering, nested types, stubs, metadata |
| `JConvEmitter` | layout and buffered output below the 65,536-byte String limit |
| `JConvReport` | `CONVERSION-REPORT.tsv` and `CONVERSION-SUMMARY.md` |
| `run-converter.sh` | runs `JConvMain` with the settings |
| `samples/` | `demo/ConvDemo.java`, its JDK output `ConvDemo.java.expected.txt` and its converted output `ConvDemo.sleela.expected.txt` (section 6) |

## 8. Changes

| Version | Change |
|---|---|
| 0.1.0 | First cut: walk, lex, rewrite, translate, stubs, report |
| 0.7.0 | `JConvOuter`: inner (non-static) classes get `Outer slOuter` and a first constructor parameter `slOuterArg`; `new Inner(..)` passes `this` (or `this.slOuter` from a sibling inner class); `Outer.this` becomes `this.slOuter`; an unqualified outer member becomes `this.slOuter.x` or `Outer.x` unless it is a local, an own member or possibly inherited (model `ret` / `field` rows). Stub reason 10 kept only for other qualified `this` / `super`. JDK sample, bundled: **26 stubs (was 35), 475 check errors (was 522), 0 syntax errors**; both samples still identical |
| 0.6.0 | `JConvLambda`: lambdas and method references lowered to `(id, env)` (rules 8a, 8b); functional interface types written as `Any` (8c), relying on SLEELA.syntax 43.7 (calls on `Any`). New sample `samples/lambda/LambdaDemo.java`, 17 of 17 lines identical. On eight lambda-dense `java.util` files the stubbed members fall from 31 to 15 |
| 0.5.0 | `JConvExpr`: an expression parser (JLS precedence) on the output tokens of each member; rules 7a to 7c moved onto the tree and rules 7e, 7f added (int wrap where the value leaves int arithmetic; `divInt` / `remInt`). In the JDK sample it writes 88 wraps and 7 division calls, with 0 syntax errors and the check numbers unchanged. The sample program gains overflow checks, `long` from an `int` product, division by zero caught, `MIN / -1`, the binary-search midpoint, and runs all 31 lines identical. Library fixes found by it: jdk-28 `Math.abs(int)` at `MIN_VALUE`, `Math.absExact(int)` now throws |
| 0.4.0 | Model-driven: `JConvModel` reads `conversion.model` (shift functions, `tostr` templates, 8,964 `ret` rows of model 1.1.0, `field` rows); the typed pass types call chains through them (rule 7d). Built-in defaults only when the file cannot be read (the summary says which). The sample program gains library and own-method calls and runs all 29 lines identical, which needed the jdk-28 `Math.sqrt` to be correctly rounded (fixed the same day) |
| 0.3.0 | Policy recorded: per-member stubs. Typed pass (rules 7a to 7d): `int` shifts through `JavaOperands`, `double` / `char` string conversion through `Double.toString` / `JavaText`, the same templates as the `binop` / `tostr` rows of `conversion.model`. `JCONV_BUNDLE=1`: one document per Java file, nested types inside (39% fewer check errors on the JDK sample; the sample program runs with all lines identical) |
| 0.2.0 | Output declares `#sleela 3.1` and uses its forms: array initializers and `new T[n]` kept (the `slArray_*` / `slZeroArray_*` helpers are gone), character literals kept, labels and jumps through `finally` kept (stub reasons 4, 13, 18 retired). New lowerings: `for` headers with commas (6i), pattern `instanceof` in two exact shapes (6j, 6k). No cast to array types (the compiler rejects them). Compiler fix alongside: `new int32[n]` starts at 0 (`impl/frontend/compiler.cpp` `arrayFillKind`) |


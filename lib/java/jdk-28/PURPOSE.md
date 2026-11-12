# /lib/java/jdk-28: Purpose — translating Java source to SLeeLa, based on JDK 28

## Purpose

`jdk-28/` exists so that **Java procedural source code can be translated into
SLeeLa that does the same thing**, running on the SLeeLa VM with no JVM.

- **Vocabulary.** One SLeeLa file per public JDK 28 type (1:1, same folders as
  the online API) gives a translated program every class, constructor, method,
  field and constant it can name, with the exact Java signature.
- **Behavior.** Where a type is implemented (see
  [`../JDK28-PROCEDURAL-IMPLEMENTATION.md`](../JDK28-PROCEDURAL-IMPLEMENTATION.md)
  and [`../COVERAGE.md`](../COVERAGE.md)) the file carries real SLeeLa logic.
  Everywhere else the body throws `UnsupportedOperationException` (or
  `AbstractMethodError` for abstract methods), so a gap fails loudly and never
  silently returns a wrong answer.
- **Target.** Same output for the same input. The yardstick is the differential
  tests in `examples/java/differential/` (real JDK vs SLeeLa, identical output)
  and the self-checking programs in `examples/java/`.
- **Not a goal.** Copying Java implementation source, or calling into a JVM.
  Each file names its JDK 28 API page as its Java Source; nothing is copied.

A translation is **faithful** when each Java construct below is either
(a) supported natively by SLeeLa, (b) rewritten by a stated rule, or
(c) rejected with a clear error. The tables list what must be addressed.

Status values: **Native** = SLeeLa does it as Java does. **Rule** = translate by
the stated rewrite. **Gap** = still to be addressed in the compiler/VM/library.
Statuses are taken from `JDK28-PROCEDURAL-IMPLEMENTATION.md` (2026-11-10) and
`SLEELA.syntax` sections 38–41; re-check them there as items land.

## 1. Language differences

| # | Area | Java (JDK 28) | SLeeLa | Status | What we need to address |
|---|---|---|---|---|---|
| 1 | Source version | No per-file marker | File declares `#sleela 3.0`; real `throw` and Java UTF-16 String ops are gated at 3.0 | Rule | Translator emits `#sleela 3.0` first in every file. |
| 2 | Package / file layout | `package a.b;` one public type per file | Library unit found by simple file name; `jdk-28/` is a reference tier, a real unit of the same name wins | Rule | Map `a.b.Type` to `jdk-28/<module>/a/b/Type.sleela`; resolve simple-name clashes with `import java.java.util;` style imports. |
| 3 | Imports | `import a.b.C;`, `import a.b.*;`, `import static` | Package-prefix `import`; ambiguous when a subpackage reuses a name | Gap | Loader needs exact-package import matching (deferred item 15). Static imports need a rewrite to `Class.member`. |
| 4 | Nested types | `Map.Entry`, `Character.UnicodeBlock` | Files named `Outer.Inner.sleela`; loader resolves by simple name so does not load them yet | Gap | Loader support for dotted nested-type files (item 14). |
| 5 | Static members | `Math.abs(x)`, `Integer.MAX_VALUE` | `Class.member` / `Class.method(...)` work (syntax §38.2). `Math` is real statics (`Math.abs(x)`, `Math.PI`); `StrictMath` maps each static call to the `Math` function of the same name; other hand-written classes still expose statics as instance methods (`new Integer(0).parseInt("42")`) | Native, partly Rule | A static `Math`/`StrictMath` reference is **mapped, not rewritten**: `Math.sin(x)` stays `Math.sin(x)`, and `StrictMath.sin(x)` is `Math.sin(x)`. Convert the remaining classes to real statics class by class; until then the translator emits the instance form for them. |
| 6 | Overloading | Resolved by parameter types | Resolved by argument count, assignability and exact-type score; ties go to source order (§40). Hand-written classes still use suffixed names (`absInt`, `toStringRadix`, `nextIntBound`) | Native, partly Rule | Convert suffixed names to true overloads; translator must map a Java overload to the right suffix meanwhile. Ranking differs from Java's most-specific rule in edge cases, so test them. |
| 7 | Constructors | Many per class; `this(...)`, `super(...)` | Many allowed at 3.0 (§40). `super("constructor")` is lowered to the root of the lineage (`SLEELA.syntax` §42.5); jdk-28 stubs carry it (`tools/jdk28_add_lineage.py`) | Rule | Java `super(args)` argument passing is still not evaluated; translator inlines inherited initialization. |
| 8 | Inheritance and virtual dispatch | An override is called through a base-typed reference | An override is not reached through a base-typed receiver or implicit-`this` call | Gap | Real virtual dispatch (item 3). Until then exceptions compose `detailMessage` at construction and collections do not consult user `equals`/`compareTo`. |
| 9 | Field initializers in base classes | Run for every subclass instance | Do not run for subclass instances | Gap | Initialize inherited state in each generated constructor (item 4), or fix in compiler. |
| 10 | Subtype assignment | `Throwable t = ioe;` | Not implicit; needs a cast | Gap | Implicit assignability (item 6). Translator inserts explicit upcasts: `(Throwable) ioe`. |
| 11 | Generics | Type parameters, bounds, wildcards, reified only by erasure | Signatures accept them (§38.1) and erase to `Any`; no type checking of arguments | Rule | Translator inserts checked casts on reads: `(int) list.get(0)`. Java compile-time generic errors are not reproduced. |
| 12 | Interfaces, default and static interface methods | Yes | Interface/abstract types present as signatures; abstract calls throw `AbstractMethodError` | Native, partly Gap | Verify default-method inheritance through dispatch (depends on row 8). |
| 13 | Exceptions: `throw` | `throw new X(...)` | Real `throw` at 3.0 | Native | None beyond row 1. |
| 14 | Exceptions: `catch` | Typed, multi-catch, `finally`, try-with-resources | Only `catch (Any e)`; `finally` not lowered | Gap | Typed and multiple catch clauses, `finally`, and try-with-resources (item 7). Interim: catch `Any`, test type with `instanceof`, rethrow. |
| 15 | Exception hierarchy | Real `extends` chain | 290 `java.*` Throwables and 208 others carry the real chain and documented constructors | Native | Keep `getMessage()` semantics (row 8). |
| 16 | Primitives | `byte short char int long float double boolean`, fixed width | VM integer is 64-bit; `byte/short/char/float` accepted as types on the int/double slots (§38.1) | Rule | Wrap `int` results to 32 bits where Java would overflow; carry byte/short/char as in-range `int`; round `float` to binary32 (`fround`). |
| 17 | Literals | `10L`, `1.5f`, `0x`, `0b`, `_` separators, char `'a'` | `L`/`f` suffixes being added | Gap | Confirm each literal form in the lexer; translator normalizes the rest. |
| 18 | Bitwise operators | `& \| ^ ~ << >> >>>` | Implemented arithmetically in the library; operators being added | Gap | Native operators with Java shift-count masking (`& 31` / `& 63`) and sign rules for `>>>`. |
| 19 | Integer arithmetic | Silent wrap on overflow, `/` truncates toward zero, `%` sign follows dividend | 64-bit VM integer | Rule | Wrap to 32 bits after `+ - *` on `int`; check division and modulo sign behavior; `/ 0` must throw `ArithmeticException`. |
| 20 | Floating point | IEEE 754 binary32/64, `NaN`, `-0.0`, `strictfp` default | Double slot; `Double`/`Float` give exact bit views and Java `toString` | Native, library | Use `Double.toString`/`parseDouble` from the library, never the legacy number formatter. |
| 21 | Strings | UTF-16, immutable, `+` concatenation, interned literals | Literals are legacy UTF-8; `javaString*` are VM UTF-16 strings; no UTF-16 ↔ legacy conversion; `+` cannot mix them; methods on String via `JavaStringMethods` (§41) | Gap | One VM operation for UTF-16 ↔ legacy conversion (item 8). Until then restrict to ASCII, or keep all text in one kind. Note 4,096 live UTF-16 handles cap. |
| 22 | `String` equality | `equals` by value, `==` by identity | `==` compares strings by value | Rule | Translator maps `a.equals(b)` to value compare; flag Java code that relies on `==` identity of strings (rare, usually a bug). |
| 23 | Arrays | Zero-initialized, bounds-checked, covariant | New `int[]`/`boolean[]` elements read as `null` | Gap | Zero/false-initialize on allocation (item 9); interim: library clears explicitly. Check `ArrayIndexOutOfBoundsException` messages. |
| 24 | Multi-dimensional arrays | `new int[3][4]`, ragged | Array-of-array handling to be confirmed | Gap | Test and, if needed, lower to nested allocation. |
| 25 | Enums | Constants with bodies, `values()`, `valueOf`, `switch` | All 161 JDK enums complete (constants, `name`, `ordinal`, `values`, `valueOf`, `compareTo`); constant-specific methods not done | Native, partly Gap | Constant-specific bodies (for example `DayOfWeek.plus`). User enums need the same generated shape. |
| 26 | Records | Canonical constructor, accessors, `equals/hashCode/toString` | 4 JDK records complete; Java-style `record` keyword exists in the language | Native, partly Gap | `record` is reserved in SLeeLa; user identifiers named `record` must be renamed. |
| 27 | Reserved words | `native`, `synchronized`, `volatile`, … are Java keywords | `record`, `close`, `native`, `enum`, `interface`, `struct`, `switch`, `instanceof`, `assert`, `super`, `this`, … reserved; `close` collides with a runtime built-in; `protected` is reserved for static members | Rule | Rename conflicting identifiers (trailing `_`, as in generated parameter names). Java protected instance members carry `@JavaProtected`. |
| 28 | Lambdas, method refs, functional interfaces | `x -> ...`, `String::length`, `Function`, `Comparator`, … | Not provided yet | Gap | Lambda / closure support and the `java.util.function` types (item 11). Blocks comparator sorting, `compute`/`merge`, `forEach`, Streams. |
| 29 | Streams, `Optional` | `java.util.stream` | `Optional*` done; Streams need lambdas | Gap | Depends on row 28. |
| 30 | Collections | Live views, fail-fast iterators | 14 collections done; `keySet/values/entrySet/subList` return snapshots | Gap | Live views (item 12); fail-fast `ConcurrentModificationException` check. |
| 31 | Equality and hashing in collections | Uses `equals`/`hashCode` of elements | `==`; objects by identity; hash matches Java for numbers, booleans, Strings | Gap | Consult user `equals`/`hashCode` (needs row 8). |
| 32 | Concurrency | Threads, `synchronized`, `volatile`, executors, `java.util.concurrent` | Atomics implemented on VM lock 31; no `Thread` semantics listed | Gap | Plan: `synchronized` to VM locks, `Thread`/`Runnable`, executors, memory-model statement. Out of scope for the first procedural pass. |
| 33 | Inner and anonymous classes | Capture outer `this` and effectively final locals | Not listed as supported | Gap | Lower to named classes with explicit outer reference and captured fields. |
| 34 | Initializer blocks, `static {}` | Yes | Not listed | Gap | Confirm, otherwise lower into constructors / a static init method called on first use. |
| 35 | Control flow | `if for while do switch break continue` labeled, enhanced `for`, switch expressions, pattern matching, `yield` | Core control flow present; enhanced `for`, labeled jumps, switch expressions and patterns to be confirmed | Gap | Verify each, lower the rest (for example enhanced `for` to an index or iterator loop). |
| 36 | `var`, text blocks, sealed types | Java 10+/15+/17+ | Not listed | Gap | Translator resolves `var` to a declared type; text blocks to a string literal; sealed is a compile-time check that can be dropped. |
| 37 | Annotations | Metadata only, no runtime effect by default | `@…` annotations exist with SLeeLa meanings (`@modium`, `@loopDelay`, …) | Rule | Strip or map Java annotations; avoid name clashes with SLeeLa annotations. |
| 38 | Comments | `//`, `/* */`, `/** */` | `//`, `#`, `< … >`, `<< … >>`; `/* */` is **not** supported; angle comments must begin a line; an apostrophe inside a comment is a lexical error | Rule | Translator rewrites `/* … */` to `<< … >>` and drops or rewrites apostrophes. |
| 39 | Memory | GC, finalizers, weak refs | VM-owned handles reclaimed by collection; `gc` hint | Rule | Drop `finalize`; map `System.gc()` to the VM hint. |
| 40 | Class size | No fixed limit that matters | At most 64 named instance fields per class or struct | Rule | Pack related fields into an array field with accessor methods, or split the class. |
| 41 | `Object` methods | `equals`, `hashCode`, `toString`, `getClass` | A value of any type is assignable to `Object` | Native, partly Gap | Provide defaults on every class; `getClass` needs reflection (see row 43). |
| 42 | Static/instance access | Instance method via class name is an error | Same: `instance method 'C::m' must be called on an instance` | Native | None. |
| 43 | Reflection, `ClassLoader`, `Proxy` | Yes | Out of scope | Gap | Decide whether to support; most procedural code does not need it. |
| 44 | I/O, networking, native | `java.io`, `java.nio`, `java.net`, JNI | SLeeLa has OS and pipe built-ins; most JDK I/O types are signatures only | Gap | Implement per package after the core language rows; JNI is out of scope. |
| 45 | Entry point | `public static void main(String[] args)` | `main` method required in the run unit (a library class alone reports "no 'main' method found") | Rule | Translator emits a `main` wrapper passing `args`. |
| 46 | Multi-file programs | javac compiles the whole tree | `run` resolves library units per file and duplicates them; use one main file and let the loader pull `/lib` units | Gap | Fix multi-file duplication (item 13). |

## 1a. Decided rules: stubs, `super`, and the lineage list

1. **`@throwable` on the method.** Until a member has SLeeLa logic, its stub
   carries `@throwable("<java exception>")` on the method or constructor itself
   (as the generated files already do) and returns the Java default value. The
   annotation names what Java would throw; a `throw` statement in the body is
   not used for stubs.
2. **`super(...)` goes to the backmost class.** For `A extends B extends C
   extends D`, a `super("constructor")` written in `A` means: set the reference
   to `D`, the root. `B` and `C` are not called by hand; they are implied by
   the SLeeLa language definition (the chain is walked by the language, not by
   translated code). The translator therefore emits one root-directed `super`
   per constructor and never a hand-written chain.
3. **`Sleela.home` is the implied lineage structure (assumed for now).**
   SLeeLa provides an implied static structure, `Sleela.home`, on every class.
   It is the bean setter for the lineage links. The translator never declares
   parent or child fields; they become available whenever a lineage keyword
   (`extends`, `super`) is used. The syntax definition creates the structure at
   the time of the static reference, at compile time and again at run time.
4. **Reading the links.** For `A extends B extends C extends D`:

   | Expression (in `A`) | Meaning |
   |---|---|
   | `Sleela.home.parent` | `B`, the immediate parent |
   | `Sleela.home.child` | The SLeeLa VM. The VM links to `A` and `A` links back to the VM, so the most-derived class is closed by the VM |
   | `Sleela.home.parents("-")` | One level up: `B` |
   | `Sleela.home.parents("--")` | Two levels up: `C` |
   | `Sleela.home.parents("---")` | Three levels up: `D`, the root. Each `-` is one level |
   | `Sleela.home.parents("...")` | Same as `"---"`: each `.` is one level, like `-` |

   From `B`, `Sleela.home.child` is `A`; from `D`, it is `C`. The list is
   therefore `VM <-> A <-> B <-> C <-> D`, walkable both ways from any member.
5. **Constructor rule.** `super("constructor")` in `A` is
   `Sleela.home.parents("---")`: it addresses the root `D` directly. `B` and
   `C` are filled in by the language definition, not by translated code.
6. **Decided answers (normative text: `SLEELA.syntax` section 42, implemented
   in the source compiler and `working-binaries/sleela`; tests in
   `impl/tests/lineage/`):**
   - `D`'s `Sleela.home.parent` is `Sleela.System`, the VM, so the lineage is
     a cycle of references: `VM -> A -> B -> C -> D -> VM`.
   - `parents("")` is `Sleela.System`. More dashes than ancestors clamp to the
     root: `parents("----")` matches `parents("---")` when there are three
     ancestors.
   - `Sleela` is reserved; `Sleela.VM`, `Sleela.System` and `Sleela.OS` are
     equivalent names for the VM reference. The VM hands it back as a
     HashMap-like value holding the VM reference and the nearest descendant. Its
     basic holder is `lib/java/SleelaHome.sleela`, public fields `vm` and
     `nearestDescendant`. A translated
     Java identifier named `Sleela` is renamed (row 27).
   - `SleelaHome` is included statically in every class, with no import. It is
     inert (`vm` and `nearestDescendant` are null) until the VM loads a file or
     files; loading makes it live. A translator therefore emits no setup code.
   - During VM boot, init and start the VM-end references (`child` of the
     most-derived class, `parent` of the root, `parents("")`) point to
     `lib/security/Security2.sleela`, which sits above the VM in colocated
     memory; they point to the VM once it is fully initialized
     (`SLEELA.syntax` section 42.6).
   - A link is set left to right: `B -> Sleela.home.parent`.

## 2. Value and library conventions the translator must follow today

1. **Static call.** `Math.abs(x)` and `StrictMath.abs(x)` map straight to the SLeeLa `Math.abs(x)` (no instance form). Any other class becomes `Class.method(x)` when it is converted, else `new Class().method(x)`.
2. **Overload.** Pick the suffixed method name that matches the Java overload (`absInt`, `absLong`, `toStringRadix`).
3. **Generics.** Erase to `Any` and cast reads: `(int) list.get(0)`.
4. **Upcasts.** Write them explicitly.
5. **Array elements of a user class.** Bind to a typed local before calling a method on it: `Foo f = foos[i]; f.run();`. Calling through the element inline loses the type at runtime.
6. **Int overflow.** Wrap to 32 bits wherever Java would.
7. **Imports.** Add `import java.java.lang;` / `import java.java.util;` when a simple name (`Character`, `Vector`, `List`, `Map`) also exists elsewhere in `/lib`.
8. **Unsupported member.** Calls to an unimplemented member throw `UnsupportedOperationException: not yet implemented in SLeeLa: <signature>`. Treat this as a work item, not an error in the program.

## 3. How a gap is closed

1. Pick a row marked **Gap**.
2. Make the compiler or VM change, with a normative note in `SLEELA.syntax`.
3. Remove the workaround from the generators in `../tools/implementation/` and `../tools/jdk28_implement_throwables.py`, and re-run them.
4. Add a `<Name>.java` / `<Name>.sleela` pair under `examples/java/differential/`, so that real Java output and SLeeLa output are compared line for line.
5. Regenerate `security/sha256-manifest.json`, the `lib/` inventory and `COVERAGE.md`.
6. Change the row's status here in the same commit.

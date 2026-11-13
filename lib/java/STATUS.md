# /lib/java status: the Java JDK in SLeeLa

**Date:** 2026-11-12 (every number below was re-measured on this date; see section 4)
**Branch:** `master`
**Reference JDK:** OpenJDK 25.0.2. Its reflected public API (4,801 types,
51,554 members, `jdk-25/api/jdk-api.jsonl`) is the yardstick for both trees
below. The file envelopes use the JDK 28 format.
**Premise:** `/lib/java` covers the Java procedural language in SLeeLa. Every
Java type gets its fields, constructors, methods, return values and imports
as SLeeLa source that runs on the SLeeLa VM, with no JVM involved. The method
signatures match the Java originals.

## 0. Two trees, one yardstick

`/lib/java` holds two SLeeLa trees. They are selected by import and they are
**not** equal in content, so no single "coverage" number describes `/lib/java`.

| Tree | Layout | Selected by | Used by |
|---|---|---|---|
| `jdk-25/` | `jdk-25/<package>/<Type>.sleela` (plus the API dump in `jdk-25/api/` and all generators in `jdk-25/tools/`) | `import java.jdk-25.<package>;` | 10 of the 26 runner programs: 4 conformance (`java.util`, `Character`) and 6 differential (`java.util.concurrent`, `java.time`, `java.math`, `StringTokenizer`, `UUID`) |
| `jdk-28/` | `jdk-28/<module>/<package>/<Type>.sleela`, one file per type, same folders as the online JDK 28 API | `import java.jdk-28;` | 10 of the 26 runner programs (all differential), and 5 of the 6 demos in `examples/java/jdk-28/` |

The remaining 6 runner programs (5 conformance, `ExtendedThrowables`) import
neither tier explicitly. Earlier versions of this document reported the `jdk-25/` numbers as if they
described the whole library. The `jdk-28/` tree, which new code imports, is
behind it. Both are reported here.

---

## 1. Headline numbers (measured 2026-11-12)

Regenerate: `python3 lib/java/jdk-25/tools/jdk_api_coverage.py --tier jdk-25 --markdown lib/java/COVERAGE.md`
and `... --tier jdk-28 --markdown lib/java/COVERAGE-JDK28.md`.

| Measure | `jdk-25/` tree | `jdk-28/` tree |
|---|---:|---:|
| Public JDK types in the yardstick | 4,801 | 4,801 |
| Types with a SLeeLa file | **4,801 (100%)** | **4,656 (97.0%)** |
| Types with SLeeLa members | 3,390 (70.6%) | 1,687 (35.1%) |
| Types with hand-written behavior | 920 | 849 |
| Public members (constructors, methods, fields, constants) | 51,554 | 51,554 |
| Members covered (present with their Java meaning) | **23,432 (45.5%)** | **12,591 (24.4%)** |
| Members with working behavior | **5,165 (10.0%)** | **4,123 (8.0%)** |
| Exact-signature methods awaiting logic | 24,101 | 33,021 |
| of which abstract (`AbstractMethodError`) | 9,675 | not separated (0) |

Reading the table:

- **The `jdk-28/` tree is the one that matters going forward and is the
  weaker one**: about 21 points less coverage and 1,042 fewer working members
  than `jdk-25/`.
- **Why `jdk-28/` covers less.** Its behavior gap is 1,042 members, but its
  coverage gap is 10,841. Most of the difference is declaration content the
  `jdk-25/` generators wrote (constants with real values, fields, complete
  enums and records, abstract methods, probed accessor contracts) that has not
  been carried into the `jdk-28/` files.
- **4,656 versus 4,801 types.** 145 API types have a `jdk-25/` file but no
  `jdk-28/` file: 119 are nested types (`Outer.Inner`, which the loader
  cannot resolve), 120 sit in `java.desktop`, and the rest include
  `jdk.unsupported`, `StableValue`, `PEMRecord` and `DEREncodable`. In the
  other direction `jdk-28/` has 65 files for types that are new in JDK 28
  (for example `LazyConstant`, `IdentityException`, `BinaryEncodable`); the
  yardstick does not contain them, so they are not counted above.
- **Behavior is under 10% in both trees.** A signature that throws
  `UnsupportedOperationException` is a declaration, not a feature.

### 1a. Progress charts (from `COVERAGE.md` and `COVERAGE-JDK28.md`)

Bars are 20 cells wide (each cell = 5%).

| Measure | `jdk-25/` | `jdk-28/` |
|---|---|---|
| Types with a file | `████████████████████` 100.0% | `███████████████████░` 97.0% |
| Types with SLeeLa members | `██████████████░░░░░░` 70.6% | `███████░░░░░░░░░░░░░` 35.1% |
| Members covered | `█████████░░░░░░░░░░░` 45.5% | `█████░░░░░░░░░░░░░░░` 24.4% |
| Members with working behavior | `██░░░░░░░░░░░░░░░░░░` 10.0% | `██░░░░░░░░░░░░░░░░░░` 8.0% |

**Twelve largest modules, `jdk-25/` tree**

| Module | Members | Covered | Behavior |
|---|---:|---|---|
| java.desktop | 21,096 | `███████░░░░░░░░░░░░░` 35.7% | `█░░░░░░░░░░░░░░░░░░░` 5.6% |
| java.base | 17,496 | `█████████░░░░░░░░░░░` 46.1% | `████░░░░░░░░░░░░░░░░` 18.3% |
| java.xml | 1,714 | `█████████████░░░░░░░` 65.9% | `█░░░░░░░░░░░░░░░░░░░` 6.2% |
| java.management | 1,573 | `█████████░░░░░░░░░░░` 45.8% | `██░░░░░░░░░░░░░░░░░░` 10.6% |
| java.sql | 1,442 | `█████████████████░░░` 83.6% | `█░░░░░░░░░░░░░░░░░░░` 5.1% |
| jdk.xml.dom | 974 | `████████████████████` 99.9% | `░░░░░░░░░░░░░░░░░░░░` 0.1% |
| jdk.compiler | 920 | `█████████████░░░░░░░` 65.8% | `░░░░░░░░░░░░░░░░░░░░` 0.0% |
| java.compiler | 821 | `███████████░░░░░░░░░` 54.8% | `░░░░░░░░░░░░░░░░░░░░` 1.0% |
| java.naming | 690 | `███████████░░░░░░░░░` 52.6% | `████░░░░░░░░░░░░░░░░` 19.7% |
| jdk.jdi | 575 | `███████████████████░` 95.3% | `██░░░░░░░░░░░░░░░░░░` 8.3% |
| java.sql.rowset | 476 | `███████░░░░░░░░░░░░░` 32.8% | `█░░░░░░░░░░░░░░░░░░░` 2.9% |
| java.xml.crypto | 325 | `█████████████░░░░░░░` 66.5% | `██░░░░░░░░░░░░░░░░░░` 8.6% |

**Twelve largest modules, `jdk-28/` tree**

| Module | Members | Covered | Behavior |
|---|---:|---|---|
| java.desktop | 21,096 | `█████░░░░░░░░░░░░░░░` 26.7% | `█░░░░░░░░░░░░░░░░░░░` 5.3% |
| java.base | 17,496 | `██████░░░░░░░░░░░░░░` 29.4% | `███░░░░░░░░░░░░░░░░░` 15.9% |
| java.xml | 1,714 | `███░░░░░░░░░░░░░░░░░` 14.9% | `░░░░░░░░░░░░░░░░░░░░` 1.8% |
| java.management | 1,573 | `███░░░░░░░░░░░░░░░░░` 13.3% | `█░░░░░░░░░░░░░░░░░░░` 5.9% |
| java.sql | 1,442 | `███░░░░░░░░░░░░░░░░░` 13.9% | `░░░░░░░░░░░░░░░░░░░░` 0.3% |
| jdk.xml.dom | 974 | `█░░░░░░░░░░░░░░░░░░░` 5.2% | `░░░░░░░░░░░░░░░░░░░░` 0.0% |
| jdk.compiler | 920 | `████░░░░░░░░░░░░░░░░` 21.5% | `░░░░░░░░░░░░░░░░░░░░` 0.0% |
| java.compiler | 821 | `████░░░░░░░░░░░░░░░░` 19.4% | `░░░░░░░░░░░░░░░░░░░░` 0.0% |
| java.naming | 690 | `████░░░░░░░░░░░░░░░░` 19.9% | `██░░░░░░░░░░░░░░░░░░` 9.3% |
| jdk.jdi | 575 | `█░░░░░░░░░░░░░░░░░░░` 4.3% | `░░░░░░░░░░░░░░░░░░░░` 0.0% |
| java.sql.rowset | 476 | `██░░░░░░░░░░░░░░░░░░` 8.0% | `░░░░░░░░░░░░░░░░░░░░` 1.1% |
| java.xml.crypto | 325 | `█████░░░░░░░░░░░░░░░` 23.4% | `░░░░░░░░░░░░░░░░░░░░` 1.2% |

**Coverage trajectory of the `jdk-25/` tree** (history; the `jdk-28/` tree has
one measurement, today's)

| Milestone | Covered | Behavior |
|---|---:|---:|
| Throwable hierarchy + numerics + collections | 3.0% | n/a |
| Full API reflected; constants, fields, enums, records | 38.6% | n/a |
| `java.time` core | 39.9% | n/a |
| Exact signatures + static access | 41.5% | n/a |
| Probed accessors; stricter counting | 42.7% | 7.1% |
| Overload resolution | 44.5% | 8.9% |
| Measured 2026-11-12 | 45.5% | 10.0% |

**Counting rule.** A Java constructor counts as covered only when SLeeLa
declares a constructor with the same number of parameters. The rules are in
the docstring of `jdk_api_coverage.py`.

---

## 2. What a `/lib/java` file looks like

Each type file holds, in order:

1. **Metadata block.** The eleven-attribute Metadata Class metapattern
   required for all SLeeLa classes.
2. **`java implementation` block.** Hand-written SLeeLa logic, where it
   exists.
3. **`java declarations` block.** The parts of the type that the API fully
   determines: constants with real values, fields, and complete enums and
   records.
4. **`java signatures` block.** Every remaining method, with its exact Java
   signature.

```
class ArrayList<E> extends AbstractList {
  ...
  public <T> T[] toArray(T[] a) { throw new UnsupportedOperationException("not yet implemented in SLeeLa: java.util.ArrayList.toArray(T[])"); }
}
class List<E> {
  public static <E> java.util.List<E> of(E e1, E e2) { ... }
}
class AbstractList<E> {
  public abstract E get(int index) { throw new AbstractMethodError("java.util.AbstractList.get(int)"); }
}
```

---

## 3. Hand-written behavior (566 types, 2026-11-11 snapshot)

> The category table below was written on 2026-11-11 and was not re-counted.
> The tool now counts **920** hand-implemented types in `jdk-25/` and **849** in
> `jdk-28/` (section 1); the difference is that the tool also counts the
> declaration-only types and the later `UUID`, `StringTokenizer`, `Base64` and
> operand work.

| Area | Types | Notes |
|---|---:|---|
| Throwable hierarchy, `java.*` | 290 | Real JDK superclass chain; Throwable state (message, cause, `initCause`, suppressed, `printStackTrace`); class-specific state (ParseException, URISyntaxException, PatternSyntaxException, FileSystemException, the SQLException chain, …) |
| Throwables outside `java.*` | 208 | Every `javax.*`, `jdk.*`, `com.sun.*` and `org.*` exception and error, generated from the reflected API |
| `java.lang` numerics | 10 | `Math`, `StrictMath`, `Integer`, `Long`, `Short`, `Byte`, `Double`, `Float` (exact IEEE bit views, shortest round-trip `toString`, correctly rounded parse), `Character` (Unicode 14 tables), `Boolean` |
| `java.lang` other | 1 | `StackTraceElement` |
| `java.util` collections | 14 | `ArrayList`, `LinkedList`, `ArrayDeque`, `Vector`, `Stack`, `HashMap`, `LinkedHashMap`, `HashSet`, `LinkedHashSet`, `TreeMap`, `TreeSet`, `PriorityQueue`, `Iterator`, `KeyValueHolder` |
| `java.util` utilities | 13 | `Arrays`, `Collections`, `Objects`, `Optional*`, `Random` (bit-exact Java LCG), `BitSet`, `Int`/`Long`/`DoubleSummaryStatistics`, `Enumeration`, `Hashtable` |
| `java.util.concurrent` | 21 | Atomics ×9 (VM lock 31), `ConcurrentHashMap`, `CopyOnWriteArrayList`/`Set`, `ConcurrentLinkedQueue`/`Deque`, `LinkedBlockingQueue`/`Deque`, `ArrayBlockingQueue`, `ConcurrentSkipListMap`/`Set` (VM lock 30) |
| `java.util.zip` | 3 | `CRC32`, `CRC32C`, `Adler32` |
| `java.time` core | 11 | `LocalDate`, `LocalTime`, `LocalDateTime`, `Instant`, `Duration`, `Period`, `Year`, `YearMonth`, `MonthDay`, `DayOfWeek`, `Month` |
| `java.math` | 1 | `BigInteger`, on the native big-integer opcodes |

### 3a. Probed accessor contracts (330 types)

`tools/ApiAccessorProbe.java` exercises the **public JDK API only**; it never
reads implementation source. For **every public constructor** of each concrete
public class it builds two instances with different arguments, plus a third
instance 5 ms later, and records what holds on every run:

- a getter returns constructor argument *i*;
- a getter returns the same initial-state value regardless of the
  arguments. This is only trusted when every argument varies between runs;
- a setter / getter round trip.

Classes that touch the host (files, sockets, threads, windows, audio,
printing, clocks, …) are never instantiated. Setters of beans that fire
property-change events are excluded, because a stored setter would miss the
event.

`tools/jdk_api_accessors.py` turns the observations into SLeeLa:

- one state field per property;
- every probed constructor that determines all of that state, each with its
  exact Java signature (overload resolution picks among them);
- getters and setters, with their exact Java signatures.

For example:

```
public ActionEvent(java.lang.Object source, int id, java.lang.String command, long when, int modifiers) { this.heldModifiers = modifiers; this.heldWhen = when; this.heldActionCommand = command; }
public long getWhen() { return this.heldWhen; }
```

Initial-state values are fields, not hard-coded returns, so later logic that
changes the state stays consistent. `tools/jdk_api_accessors_check.py`
constructs every top-level probed class in SLeeLa and re-asserts each
contract for every written constructor. On 2026-11-12 it covers 427 classes in
78 packages: 48 packages pass and 30 fail (section 4). Object arguments differ in class between runs, so a
value derived from the argument class, such as `Binding.getClassName()`, is
never taken for a constant.

---

## 4. Verification (re-run 2026-11-12 unless marked)

| Check | Result |
|---|---|
| **Side-by-side with the JDK** (`examples/java/differential/run-differential.sh`): each `<Name>.java` runs on OpenJDK 25.0.2 and `<Name>.sleela` on the SLeeLa VM, and the output must be identical | **18 pairs** since 2026-11-13: `MathSqrt` (242 lines: bits of `Math.sqrt` for 240 values, then `Math.abs` / `absExact` at `Integer.MIN_VALUE`) added and passing; the full runner was last run 2026-11-12, **17 / 17 pass**, 641 lines identical: OperandSemantics 189, PropertiesCore 64, UUIDCore 53, IntegerParse 49, Base64Core 44, SmallWrappers 39, TimeCore 29, MathBigInteger 27, BeanEvents 27, ConcurrentZipBits 23, StringTokenizerCore 18, Jdk28Throwables 17, MathStatics 16, ConcurrentCollections 14, LongStatics 13, ExtendedThrowables 11, IntegerStatics 8 |
| **Self-checking programs** (`examples/java/run-conformance.sh`) | **9 / 9 pass**, including a 1,348-value `Double.toString`/`parseDouble` round trip |
| **Demos** (`examples/java/jdk-28/*.sleela`, 6 programs) | All 6 run to completion. They print results and carry no assertions, so this shows they execute, not that the output is correct |
| **Probed accessor contracts** (`lib/java/jdk-25/tools/jdk_api_accessors_check.py`, 427 classes in 78 packages) | **48 / 78 packages pass, 30 fail** with `uncaught language exception` (for example `java.util`, `java.io`, `java.text`, `javax.swing`). The failing set is identical with the compiler from before today's change, so it is not a regression. Cause not yet diagnosed. The earlier "386 constructors, 79 packages, 0 failures" no longer reproduces. The tool itself had been broken by the move into `jdk-25/` (wrong repo root, wrong import prefix); both are fixed |
| **Every type compiles** (217 test programs, each referencing up to 120 types) | Not re-run on 2026-11-12. The 2026-11-11 result was all clean except 1 batch (`ParagraphView` is defined in two `javax.swing.text` packages) |
| **Compiler test suite** (`cd impl && make -k test`, 50 targets) | **49 / 50 pass.** `test-osls-isa` fails: `OSLS ISA verification failed: native/source counts differ: 192 != 185`. It fails the same way on the untouched baseline `e8e64299`, so it predates the operand work. Earlier "all pass" was not true at this commit |
| **Repo-wide regression** (`sleela check` on all 374 main programs) | Not re-run on 2026-11-12. For the operand change, the 4 other programs that declare `byte`/`short`/`char`/`int32` produce identical output with the old and new compiler |
| **Gates** (`lib/java/validate.py`, `test-suites/test-library-inventory.sh`, `make check-manifest`) | See section 10 for the figures at the time of the last commit |

---

## 5. Compiler and VM work done for this (SLEELA.syntax section 38)

- **Java primitives:** `char`, `byte` and `short` use the integer slot;
  `float` uses the double slot.
- **Generics:** class and method type parameters. Type variables and
  package-qualified reference types erase to `Any`; `java.lang.String` maps
  to `String`.
- **Signature syntax:**
  - generic methods (`<T> T[] toArray(T[] a)`);
  - varargs (`T...`);
  - keyword and array type arguments;
  - `>>` / `>>>` closers of nested generic lists;
  - keyword segments in qualified names (`java.lang.String`).
- **Static access through the class name:** `DayOfWeek.MONDAY`,
  `DayOfWeek.valueOf("FRIDAY")`, `Integer.MAX_VALUE`, `Class.method(...)`,
  and chained calls on the result.
- **Library loader:** an identifier after `.` no longer pulls in a `/lib`
  unit with that name.
- **Any value is assignable to `Object`.**
- **Overload resolution (SLEELA.syntax section 40).** Several methods with
  the same name and several constructors per class. A call binds by arity,
  then by best type match (exact match beats assignable; ties keep the first
  declaration). Each overload compiles to its own function.

---

## 6. Where it differs from Java today (each item maps to a follow-up)

| Gap | Effect | Planned fix |
|---|---|---|
| ~~No overload resolution~~ **done** (section 40) | Hand-implemented classes still use the earlier suffixes (`absInt`, `addAt`, `toStringRadix`); stub types write no constructors yet | Rename to the Java names class by class; write constructors where their behavior is known |
| Hand-implemented classes expose statics as instance methods | `new Double(0.0).parseDouble("2.5")` instead of `Double.parseDouble("2.5")`. `Math`, `Integer` and `Long` in `jdk-28/` are converted (`Math.abs(x)`, `Integer.parseInt(s)`, `Long.parseLong(s)`; each differential-tested against a real JDK), and every `StrictMath` static maps to the `Math` function of the same name | Convert the remaining classes class by class now that static access works |
| ~~No virtual dispatch through a base-typed receiver~~ **done** (SLEELA.syntax 43.6 rules 1, 6, 7; `impl/tests/java_references/oop/polymorphism`, `interface`, `super_method`) | Library units written before it still compose exception messages at construction | Convert them class by class |
| ~~Base-class field initializers don't run for subclasses; `super(...)` not lowered~~ **done** (43.6 rules 2 to 5; `oop/super_args`, `implicit_super`, `this_chain`) | `/lib` units still construct themselves (no implicit `super()` for them) | — |
| ~~Only `catch (Any e)`; no `finally`~~ **done** (43.6 rule 11; `oop/catch_typed`, `catch_rethrow`, `try_finally`); a `return`, `break` or `continue` out of a try with `finally` runs the `finally` since syntax 3.1 (44.5) | Still open: VM faults (`/ 0`, index out of range) are not catchable; converted code avoids `/ 0` through `JavaOperands.divInt` | VM |
| ~~String bridge missing~~ **done** (`javaStringToUtf8`, `javaStringFromCodeUnit`, opcodes 183-184, ABI 1.8) | Text classes carry any Unicode text; only NUL and unpaired surrogates cannot become a plain String (catchable `UnsupportedOperationException`); `java.lang.String` instance methods work on built-in strings (`JavaStringMethods`, SLEELA.syntax section 41) | Regex members and the static `String.valueOf`/`join`/`format` |
| No lambdas or functional interfaces | No `Comparator` sorting, `compute`/`merge`, streams | Compiler |
| ~~No bitwise operators or `L`/`f` literal suffixes~~ **done** (`& | ^ ~ << >> >>>`, compound forms, `L`/`f` suffixes, `0x`/`0b` and `_` literals all compile) | — | — |
| ~~Java fixed-width operand semantics~~ **done for `byte`, `short`, `char`, `int32` and explicit casts** (section 8) | Stores into those types and `(byte)`/`(short)`/`(char)`/`(int32)` casts wrap at the Java width, 189 lines identical to a real JDK. Plain `int` stays 64-bit by design, so Java 32-bit `int` arithmetic and shifts come from `lib/java/JavaOperands`. | Open (checked 2026-11-13 with `impl/build/sleela`, `#sleela 3.1`): stores into `byte[]` / `short[]` elements do not wrap (`a[0] += 1` on 127 gives `128`; syntax 3.2 narrows `char[]` stores only); `print` of a double shows `inf` / `-nan` (converted code goes through `Double.toString`); `long.MIN_VALUE / -1` still crashes the VM (SIGFPE) and `x / 0` is a fatal, uncatchable error, both avoided in converted code by `JavaOperands.divLong` / `divInt`. Char literals exist since syntax 3.1 (44.2). Plain `int` stays 64-bit; `lib/java/converter` 0.5.0 writes Java `int` arithmetic exactly (one `JavaOperands.toInt` where a value leaves int arithmetic) |
| ~~Integer comparison went through `double`~~ **done** (`OP_EQ/NE/LT/LE/GT/GE`, both operands `SL_INT` now compare as exact int64) | Before, `long` values that differ above 2^53 compared equal (`9007199254740993 == 9007199254740992`), which broke Java `long` ordering and overflow checks | — |
| ~~Elements of a new numeric array are `null`~~ **done**: `new T[n]` starts at zero / `false` (SLEELA.syntax 44.3 fill kind; `int32` and the other 64-bit series members since the 2026-11-13 compiler fix) (checked 2026-11-13 with `impl/build/sleela`, `#sleela 3.1`) | Library code that still clears arrays by hand (`slMd5` in `UUID`) can drop it | Regenerate those units |
| Nested types (`Map.Entry`) live in `Outer.Inner.sleela` files the loader does not resolve | Not reachable by name | Loader |
| `jdk-28/` tree lacks the generated declarations `jdk-25/` has | 24.4% covered versus 45.5% (section 1) | Carry constants, fields, enums, records, abstract methods and accessor contracts into `jdk-28/` |
| Protected instance members | Carry `@JavaProtected`, because SLeeLa reserves `protected` for static members | Language decision |
| ~~Prebuilt `working-binaries/sleela` predates these compiler changes~~ **done** (refreshed 2026-11-12, ABI 1.8) | Either binary works | — |

---

## 7. Commit history (`master`)

| Commit | Change | Coverage after |
|---|---|---:|
| `d9b0aa60` | Pilot: `java.lang.Math` in SLeeLa | — |
| `86ebf299` | 322 types: Throwable hierarchy, `java.lang` numerics, `java.util` collections and utilities | 3.0% |
| `8b71cc6a` | Reflected the full JDK API; coverage tool; constants, fields, enums, records; created 1,235 missing types | 38.6% |
| `e3bd79a6` | Atomics, summary statistics, zip checksums, `BitSet`, `StackTraceElement`; side-by-side tests against the JDK | 39.2% |
| `627d6937` | `java.time` core | 39.9% |
| `565e386e` | 208 Throwable types outside `java.*` | 40.7% |
| `7c275791` | `java.math.BigInteger`; concurrent collections | 41.4% |
| `7e340cc2` | First signature transcription (24,970 stubs); `ConcurrentSkipListMap` deadlock fix | 41.4% |
| `4d913e06` | **Exact Java signatures** (35,157); compiler reads the Java signature surface; static member access | 41.5% |
| `398f0f32` | `STATUS.md` | 41.5% |
| `0572108e` | Probed accessor contracts for 294 types; stricter constructor counting | 42.7% (7.1% behavior) |
| (2026-11-11) | **Overload resolution**; all Throwable-pattern constructors; every probed accessor constructor (330 types) | **44.5%** (8.9% behavior) |
| `3340262e` | Java operand semantics: `byte`/`short`/`char`/`int32` wrap at the Java width; `JavaOperands`; `OperandSemantics` differential test | 45.5% (10.0%), `jdk-25/` tree |
| `07d93a81` | Both trees re-measured; coverage tool `--tier`; `COVERAGE-JDK28.md` | jdk-25 45.5% / 10.0%, jdk-28 24.4% / 8.0% |
| `15bb3c93` | `lib/java/converter/`: Java source to SLeeLa source converter (section 11); parser: bodiless methods returning `int32` and the other series integers default to `0` | coverage unchanged (the converter writes outside `lib/java`) |

The percentages above are for the `jdk-25/` tree. The `jdk-28/` tree was first
measured on 2026-11-12: 24.4% covered, 8.0% behavior.

---

## 8. Tools (all idempotent, run from the repository root)

```sh
# 1. Refresh the reflected API (needs a JDK)
javac -d /tmp/apid lib/java/jdk-25/tools/ApiDump.java   && java -cp /tmp/apid ApiDump       > lib/java/jdk-25/api/jdk-api.jsonl
javac -d /tmp/sigs lib/java/jdk-25/tools/ApiSignatures.java && java -cp /tmp/sigs ApiSignatures > lib/java/jdk-25/api/jdk-signatures.jsonl

# 2. Hand-written behavior
python3 lib/java/jdk-25/tools/jdk28_implement_throwables.py
(cd lib/java/jdk-25/tools/implementation && for g in gen_integral gen_floating gen_character gen_collections \
   gen_util2 gen_util3 gen_time gen_math gen_concurrent; do python3 $g.py; done)

# 3. Probed accessor contracts (needs a JDK; runs headless in a scratch directory)
javac -d /tmp/probe lib/java/jdk-25/tools/ApiAccessorProbe.java
java -Djava.awt.headless=true -cp /tmp/probe ApiAccessorProbe < types.txt > lib/java/jdk-25/api/jdk-accessors.jsonl
python3 lib/java/jdk-25/tools/jdk_api_accessors.py

# 4. API-determined declarations, then exact signatures (signatures last)
python3 lib/java/jdk-25/tools/jdk_api_declare.py
python3 lib/java/jdk-25/tools/jdk_api_signatures.py

# 5. Measure and verify
python3 lib/java/jdk-25/tools/jdk_api_coverage.py --tier jdk-25 --markdown lib/java/COVERAGE.md
python3 lib/java/jdk-25/tools/jdk_api_coverage.py --tier jdk-28 --markdown lib/java/COVERAGE-JDK28.md
python3 lib/java/jdk-25/tools/jdk_api_accessors_check.py
examples/java/run-conformance.sh
examples/java/differential/run-differential.sh
python3 tools/generate-sha256-manifest.py --root . --output security/sha256-manifest.json

# 6. Convert Java source to SLeeLa source (writes outside lib/java; section 11)
JCONV_IN=/path/to/jdk-src JCONV_OUT=/tmp/jdk28-sleela JCONV_FILTER=java.base/java/util bash lib/java/converter/run-converter.sh
```

---

### 8a. Shared helpers

`lib/java/support/` (`SLJavaBits`, `SLJavaHex`, `SLJavaMd5`) holds code several
Java types need, so none carries a private copy. `UUID` uses it; `MessageDigest`,
`Base64`, `HexFormat` and the CRC classes should too. See `support/README.md`.

## 9. Suggested next steps

0. **Bring the `jdk-28/` tree up to the `jdk-25/` tree.** It is the tree new
   code imports and it trails by 21 points of coverage (section 1). Carry the
   generated declarations (constants, fields, enums, records, abstract methods,
   probed accessors) into the `jdk-28/` files, create the 145 missing types
   (nested types need the loader work in section 6), and make the generators
   in `jdk-25/tools/` write to the `jdk-28/` layout. Until then every "done"
   item below must say which tree it landed in.
0b. **Diagnose the 30 failing accessor-contract packages** (section 4). They
   fail identically on the earlier compiler, so the cause is in the library or
   the generated check, not in the operand work.
0d. ~~**Decide the policy of `lib/java/converter`**~~ **decided 2026-11-13:
   per-member fail-loud stubs** (section 11).
0c. **Fix or waive `test-osls-isa`** (the check reports native count 192 versus source count 185; what is being counted has not been inspected),
   so `make test` can be green again.
1. **Rename the hand-implemented overload suffixes to the Java names** now
   that overload resolution exists (`Math.abs(int)`/`abs(double)`,
   `List.add(int, E)`), and convert their static API to real static methods.
2. **Write the constructors of the stub types** wherever their behavior is
   determined, the same way the Throwable and accessor constructors were.
3. ~~**The String bridge**~~ done (section 6); `java.lang.String` instance
   methods work on built-in strings. Still open: the static `String.valueOf`,
   `join`, `format`, regex members, and `StringBuilder`.
4. **Keep converting stubs to logic** in `java.base`, highest use first:
   `BigDecimal`, `Base64`, `Calendar`, `java.time.format`, `MessageDigest`
   (the MD5 inside `UUID` is the starting point). ~~`UUID`~~ done 2026-11-12
   (15/15 members, side-by-side test `UUIDCore`, 53 lines identical).
   ~~`StringTokenizer`~~ done 2026-11-12 (all members; replaces the wrong probed
   accessor stubs; `StringTokenizerCore`, 18 lines identical). `Base64Core`
   (44 lines identical) covers basic, URL-safe and MIME encoding; the nested
   `Base64.Encoder` / `Base64.Decoder` types stay unreachable until the
   nested-type loader gap is closed.
   Add a side-by-side test for each.
5. ~~**Refresh `working-binaries/`** so the prebuilt toolchain carries the new
   compiler.~~ Done 2026-11-12 (VM ABI 1.8, 185 opcodes).

---

## 10. Java operand semantics (2026-11-12)

SLeeLa keeps every integer in one 64-bit slot, and `int` is the 64-bit workhorse
of the whole library, so `int` itself is not narrowed. Java widths attach to the
Java-width types instead:

| Construct | Behavior | Example |
|---|---|---|
| Store into `byte` / `short` / `char` / `int32` (local, field, parameter, return, assignment) | Wraps at 8 / 16 / 16 unsigned / 32 bits | `byte b = 127; b = b + 1;` gives `-128` |
| Cast `(byte)` `(short)` `(char)` `(int32)` | Truncates a double first, then wraps | `(byte) 200` gives `-56`, `(short) 70000` gives `4464` |
| `JavaOperands.addInt` `subInt` `mulInt` `negInt` `divInt` `remInt` | Java `int` result, `ArithmeticException` on `/ 0` | `addInt(2147483647, 1)` gives `-2147483648` |
| `JavaOperands.shlInt` `shrInt` `ushrInt` | Shift count masked to 5 bits, result wrapped | `ushrInt(-1, 1)` gives `2147483647` |
| `JavaOperands.divLong` `remLong` `shlLong` `shrLong` `ushrLong` | MIN_VALUE / -1 wraps, count masked to 6 bits | |
| `JavaOperands.d2i` `d2l` `d2b` `d2s` `d2c` | NaN is 0, out of range saturates (JLS 5.1.3) | `d2i(1e10)` gives `2147483647` |

Implementation: `emitJavaNarrow` in `impl/frontend/compiler.cpp` lowers the
narrowing to the existing `OP_BSHL`/`OP_BSHR` (and `OP_BAND` for `char`), so no
opcode, ABI or VM change. Array-element and iterator loads are not narrowed,
because a new numeric array element reads as `null` (section 6).

Verification: `examples/java/differential/OperandSemantics` (189 lines, identical
to OpenJDK 25.0.2), plus the 9 conformance programs and the 16 earlier
differential tests (17 in total), all unchanged.

---

## 11. Converting Java source: `lib/java/converter` (2026-11-13)

A SLeeLa program that walks a Java source tree (for example a JDK `src.zip`)
and writes one SLeeLa file per type, nested types as `Outer.Inner.sleela`. All
Java-to-SLeeLa decisions are in `converter/JConvRules.sleela`; the full rule
table, the stub reasons and the reproduction steps are in
[`converter/README.md`](converter/README.md). It writes outside `lib/java`
and refuses to write into `jdk-28/`, so it does not change the numbers in
section 1.

| Measured (OpenJDK 25.0.2 sources) | Result |
|---|---:|
| Sample of 17 JDK files: converted | 16 (`UUID.java` skipped: non-ASCII bytes) |
| SLeeLa files written / members / fail-loud stubs | 36 / 627 / 35 (5.6%; 47 in 0.1.0) |
| Output files with a syntax error | 0 |
| Output files passing `sleela check` | 8 of 36 |
| `converter/samples/demo/ConvDemo.java` against a real JDK | 31 of 31 lines identical (0.5.0, `JCONV_BUNDLE=1`) |
| `converter/samples/lambda/LambdaDemo.java` against a real JDK | 17 of 17 lines identical (0.6.0: lambdas and method references lowered to `(id, env)`) |
| Eight lambda-dense `java.util` files: stubbed members | 31 (0.5.0) to 15 (0.6.0) |
| `sleela check` errors on the sample, separate files / bundled | 903 / 553 (522 bundled with SLEELA.syntax 43.7; **475** with converter 0.7.0, inner classes) |
| Stubbed members on the sample, bundled | 35 (0.6.0) to **26** (0.7.0: `Outer.this` lowered) |

Converter 0.2.0 (2026-11-13) declares `#sleela 3.1` and uses its array
initializers, character literals, labels and jumps through `finally`, and lowers
`for` headers with commas and pattern `instanceof` (two exact shapes).

The `check` errors come from the gaps in section 6 (nested-type files, statics
written as instance methods (the largest group, 94 of 475), inner classes
reaching the enclosing instance (closed by converter 0.7.0, `JConvOuter`),
`Any` values stored into typed variables; calls on `Any` and interface casts
were closed on 2026-11-13 by SLEELA.syntax 43.7) and from JDK-internal types that have no
`jdk-28` file. (Update 2026-11-13, later: `sleelactl java-convert` was re-measured on the JDK 25.0.2
`java.base` subset of 884 files: 423 converted, 429 with `--single-thread`, against 344 on 2026-11-12; its
differential suite is 28 of 28. See `sleelactl/convert/README.md`.) A separate converter, `sleelactl java-convert` (C/C++, semantics
in `impl/subjects/conversion/conversion.model`), landed the same day and fails
closed per file. `lib/java/converter` keeps per-member stubs (decided
2026-11-13); where the two share a law (`binop`, `tostr` rows of the model) it
applies the same template. Converter 0.3.0 adds operand types (int shifts,
`double` / `char` shown as text) and `JCONV_BUNDLE=1`; 0.4.0 reads those laws
from the model itself, and types call results through its `ret` rows. The model
is at revision 1.1.0: 8,922 result types reflected from the OpenJDK 25.0.2 API
(section 10b, `impl/subjects/conversion/generate_ret_rows.py`).

**`Math.sqrt` (jdk-28 tier) is correctly rounded since 2026-11-13.** It was
Newton's method, which stops up to one ulp off (`Math.sqrt(2.0)` printed
`1.414213562373095`, Java `1.4142135623730951`) and did not converge in 200 steps
for magnitudes far from 1 (results wrong in the leading digits for about 1e-150).
It now scales the argument into [2^-64, 2^64] by exact powers of two, runs Newton,
and picks the correctly rounded root with the Tuckerman test using exact
products (Dekker two-product), as IEEE 754 and the Java specification require.
`Math.abs(int)` now returns `Integer.MIN_VALUE` for `Integer.MIN_VALUE` and
`Math.absExact(int)` throws `ArithmeticException` there, as Java does (both found
by converter 0.5.0, which wraps int arithmetic exactly; the converter keeps its
output at syntax 3.1).
`examples/java/differential/MathSqrt` checks the bits for 240 values and the
`abs` edge cases; the
`Math` conformance program and the `MathStatics` and `MathBigInteger` pairs still
pass.


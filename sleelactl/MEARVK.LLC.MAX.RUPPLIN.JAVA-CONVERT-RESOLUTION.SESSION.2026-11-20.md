# Session record: javaconv access and resolution fixes (2026-11-20)

Session with Kiro on `mearvk/SLeeLa`, branch `master`. It started from `7fb185f8`, checked the `sleelactl`
Java converter (`sleelactl/convert`, registered as `java-convert`), repaired the build gate and the prebuilt
compiler, and then fixed converter resolution defects. Measurements use Corretto 25.0.5.1 and its `src.zip`
(extracted with Python `zipfile`).

## 1. Check of the converter as found

| Check | Result |
|---|---|
| `make -C sleelactl/convert`, `javaconv --self-check` | builds clean; model `java-to-sleela 1.5.0` |
| `bin/sleelactl check-registry --strict` | 44 modules, 0 errors |
| `tests/sleelactl_cli_test.sh`, `tests/sleelactl_systemctl_scope_test.sh` | PASS |
| `tests/javaconv_differential_test.sh` with `impl/build/sleela` | 103 of 103 |
| same, default binary on a fresh clone | 45 pass, 58 fail (two repository faults, below) |
| ShellCheck gate | not run: `shellcheck` is not installed in the sandbox |

## 2. Repository faults fixed

- **Integrity gate.** `impl/core/terminal_platform_smoke.c` changed in `70653c59` (Windows ConPTY smoke command)
  without a manifest update, so `check-manifest` and every `sleela run/check` failed on a clean checkout. The diff
  was reviewed, then `make regen-manifest`. One entry changed. On `main` the same fault covered
  `sleela_terminal.c` and `terminal_platform_smoke.c` (ConPTY commits since 2026-11-13), and it was fixed the same way
  (`e257e138e`).
- **Stale prebuilt compiler.** `working-binaries/sleela` (2026-11-19) predated the `driver.cpp` fix of `833b3a44`
  (resolve `/lib` units once for a multi-file run), so the six multi-file project checks failed with
  duplicate-declaration errors. Rebuilt from `cd43c1db`: the product version is still `1.0121`, and the artifact
  identity is now `SLEELA-BIN-1.0122`. `nordshrift` does not link the driver and was not rebuilt. Checked with the new
  binary: `test-version`, `test-opcode-registry`, `test-runtime`, `test-osls-vm`, `test-sleela`, the syntax 3.1 and 3.2
  fixture scripts, 214 of 214 regression tests, and 103 of 103 differential checks without `impl/build/sleela`.
  `working-binaries/README.md`, `VERSION-IDENTITIES.md`, `binary-identities.json`, the two toolchain steering notes
  and `SHA256-DIGESTS.md` were updated (`9ce59972`).

`main` received only the manifest fix. Its compiler, VM core and converter are an older generation than master's,
so a converter sync to `main` is a full "bring main level with master" change and was left for a decision.

## 3. Converter fixes, round 1 (`7fbcdd0ae`)

- Member classes keep the type variables of their enclosing class (JLS 6.3). `E` in an inner class was refused
  `[library-type]`.
- A single static import of a field binds a bare `NAME` to `Owner.NAME` (JLS 7.5.3), for project and `/lib` owners.
- A bare field inherited from a `/lib` superclass is looked up in the unit chain.
- `/lib` static fields are indexed and checked. A non-final static in a unit's "java declarations" section is a
  placeholder (`= null`, `0.0`), and reading it is `[library-stub]`.
- Enum constants are public (JLS 8.9.3).

## 4. Converter fixes, round 2 (this commit)

A parallel session pushed `69716e1bd` (20:48) while this round was in progress. It contains two of the changes below:
interface methods are public unless `private`, and method calls on arrays are refused, with `reject/ArrayClone` and
the `project-iface-public` project. This work was rebased onto it. The conflicting lines were identical in effect,
and the upstream versions were kept. Its analysis is in `docs/reference/JAVA.CONVERTER.ANALYSIS.2026-11-19.md`
(854 of 3344 with those two changes alone).

- **Implicitly public members.** Interface methods other than `private` ones (JLS 9.4, also in `69716e1bd`), record accessors,
  `toString`/`equals`/`hashCode`, the implicit canonical constructor (with the access of its record), and the
  generated enum methods were recorded as package-private. Any use from another package was refused
  `[member-access]`.
- **Indirect `/lib` calls are checked.** An instance call on a project type that does not declare the method,
  and an unqualified call in a class (nested classes included), were emitted with no `/lib` check. Calls into stubs
  that return `null` therefore converted and then failed at link time or ran wrong. The converter now walks the project
  superclasses and interfaces to their `/lib` ancestors (`checkInheritedLibraryCall`). A real member converts the call,
  and a stub refuses it. A missing member refuses it only on the superclass chain, because unit headers list only
  `extends` (so `SortedSet` does not show `isEmpty`). An abstract interface method (no `default`, no `static` in an
  interface unit) is dispatch, not a stub.
- **Static-imported `/lib` methods.** `import static java.util.Objects.requireNonNull;` bound the bare call to nothing,
  and modeled owners called the tier's instance methods statically. The call now goes through `checkLibraryCall`
  and is written `Owner.m(...)` or `new Owner().m(...)`.
- **Seeded functional interfaces.** `Runnable`, `Comparator`, `Function` and nine others are seeded into the symbol
  table to type lambdas. Every "is this a project type" test treated them as project types, so calls such as
  `comparator.reversed()` skipped the `/lib` checks. `ClassInfo::seeded` and `isProjectType()` now separate them.
- **Array method calls** (`a.clone()`) are refused `[object-method]` (kept from `69716e1bd`). A SLeeLa array has no
  methods, and these calls did not link.

Tests added: `StaticImportLibrary`, `reject/StaticImportStub`, `reject/InterfaceDefaultStub`,
`reject/InheritedInterfaceStub`, and the `project-implicit-public` project in `tests/javaconv_differential_test.sh`.

## 5. Measurements

| Input | Session start | After round 1 | After round 2 |
|---|---|---|---|
| differential suite | 103 of 103 | 108 of 108 | **115 of 115** (after the rebase) |
| `java.base` subset (`lang`, `util`, `math`, `text`; 884 files) | 254 | 259 | **262** (270 SLeeLa files) |
| all of `java.base` (3344 files) | 841 | 842 | **862** (879 SLeeLa files) |
| `member-access` refusals, all of `java.base` | 3,877 | 3,753 | **114** |

The rebased tree was re-measured and gave the same converted files as before the rebase. Compared with the start of round 2, all of `java.base` gains 32 converted files and loses 12. All 12 are array
`clone()` calls, which are now refused. Their earlier conversions did not link: the whole-tree check of that
generation attributed `clone` errors to them. In the subset no file newly fails.

Whole-tree semantic check (`tests/javaconv_check_tree.sh`): subset after round 1, 22 errors in 11 of 267 files
(unchanged). All of `java.base` in the intermediate generation (before the stub checks), 148 errors in 53 of 885 files.
That run found the unchecked calls fixed in round 2. **Not re-run on the final generation.**

## 6. Not run

- `tests/javaconv_check_tree.sh` on the final output (about 5 minutes for the subset, 14 for all of `java.base`).
- `make -k test`, the jdk-28 whole-tree compiler check, and the other long suites (`validation-cadence.md`).
- ShellCheck (not installed in the sandbox).

## 7. Open items

- Placeholder values in `lib/java/jdk-28`: `BigInteger.ONE/ZERO/TEN`, `Collections.EMPTY_*`, the `StandardCharsets`
  charsets, and similar. They need real `static final` values.
- Unit headers do not record interface `extends`, so a missing member reached through an interface is not checked.
  `jdk.internal.classfile.impl.ChainedFieldBuilder` (`FieldBuilder.constantPool`) converts again and fails at link time.
- `Instant` has no no-argument constructor (five JDK files `[library-member]`).
- `List.of(array)` and similar generic most-specific choices are refused `[overload-ambiguous]` (javac picks `of(E...)`).
- An array `clone()` lowering (an array copy) would recover the 12 files above.
- Largest first refusals on all of `java.base`: `type-ambiguous` (488 files), `library-type` (269), `library-member` (231).
- `main` does not have the converter work.

## 8. Review of the conversion subject and its fixes (model 1.6.0)

Review of `impl/subjects/conversion/` (model, generator, README, `CONVERTER-IDEMPOTENCE.md`,
`CONVERTER-MAPPING-AUDIT.md`, `IDENTIFIER-COLLISIONS.md`). Section 10b was up to date
(`generate_ret_rows.py --check`, 8,922 rows), and the self-check passed. Findings and what was done:

| Finding | Fix |
|---|---|
| The header claimed the converter holds no Java semantic knowledge of its own; declaration and name-resolution laws (JLS 6, 8.9.3, 8.10, 9.4, 15.12, inference, lowerings) live in C++ | Claim narrowed to expression-level laws in the model header, subject README, steering rule 1 and `JAVA.SPEC.md` section 3 |
| The converter emitted 57 refusal kinds; the model had rows for 21 | 38 rows added (section 12b). `syntax` and `model` are errors, not refusals. The translator reports a kind without a row as `[model]`; check `model-registry` |
| Rows `float`, `enum`, `lambda`, `assign-expr` were never emitted and two were wrong | Marked superseded, with the current behaviour and its test |
| A second library typing source (the `lib/java/jdk-28` units) was undocumented | Described in the subject README |
| `chartext` was missing from the record grammar | Added |
| Stale "not yet verified" claims (Floats, provenance, repeat-run, transaction, package-private access, nested imports); "extensive field rows" (there are 4) | Dated verification updates in both audit files and the README; history kept. The native converter claims stay pending (not run) |
| README revision table out of order; latest measurement from 1.3.0 | Reordered newest first; 1.6.0 row and a current measurement added |

`CONVERSION-REPORT.md` now lists each refusal kind with its model reason.

Validation with model 1.6.0: differential suite **116 of 116** (adds `model-registry`). The `java.base` subset
(262 of 884) and all of `java.base` (862 of 3344) were file-for-file identical to 1.5.0, with identical refusal
counts and no `[model]` refusals. `generate_ret_rows.py --check` up to date.

## 9. Performance measurement

Recorded in `sleelactl/convert/PERFORMANCE.md`. `javaconv` converts the `java.base` subset (884 files, 488,871 lines)
in 60 s on one core with 54 MB peak; one small file takes 0.8 s, mostly fixed setup. Converted benchmark programs print
exactly the JDK output. Each run takes about 3.5 s on the SLeeLa VM against about 0.05 s on the JDK, and that time is
start-up: a hand-written SLeeLa program takes 3.56 s, 1.89 s of it the SHA-256 execution gate. Steady-state compute
speed was not measured: the larger-workload run was interrupted.

## 10. Statements, model lowering and parser gaps (model 1.7.0)

Worked in that order, measured on all of `java.base` against the start of the round (model 1.6.0, 862 of 3344).

**Statements.** Type-pattern switches (41 sites in 22 files, all `case T v ->`, one `case null`, one `case T _`)
are lowered to an `instanceof` chain in source order, inside a loop that runs once so `break` and `yield` leave it.
The lowering covers JLS null handling (`case null` or `NullPointerException`) and `MatchException` when there is no
match. Guards, record patterns, `:` groups and an unlabeled `continue` in an arm stay refused. Tests `PatternSwitch`,
`reject/SwitchPattern` (now a `when` guard), `reject/SwitchRecordPattern`, `reject/SwitchPatternContinue`.

**Model lowering** (304 `[model]` sites):
- `for` loops with several declarators, initializers or updates (228 sites);
- control characters in strings (53 sites), written `\u00XX`;
- `double %` and `%=`, and the existing `float %` row, through a new `JavaOperands.remDouble`. The VM `%` traps on a
  zero divisor where Java gives NaN; 17 edge cases are checked against the JDK. This is the only `lib/**` edit, and
  the gate manifest changed in that one entry;
- boxing casts to the own wrapper.

The rest now has real refusal kinds: `unboxing-cast`, `print-stream`, `string-nul`, `string-static`, and the
intersection casts below.

**Parser gaps** (435 `[syntax]` sites):
- annotations on statements, locals and parameters, with dotted names and array values (197 sites), through one
  skipper;
- hexadecimal floating literals (70), decoded exactly. Subnormal doubles become an exact expression, because SLeeLa
  has no literal spelling for them;
- class literals (66), as `Class.ofLiteral`. The package of a `/lib` type comes from an explicit import, `java.lang`
  or a wildcard-imported package, else the literal is refused;
- nested and `private` enum constructors;
- arrays of array expressions;
- unpaired surrogates (`string-surrogate`);
- intersection casts (`intersection-cast`), with the bitwise `(x & y) + 1` kept as an expression.

**Defects found and fixed:**
1. `"a\0b"` was converted to `"a"`. The C lexer stored decoded literals NUL-terminated. Now refused, `[string-nul]`.
2. `System.out.print` and `System.err` calls were emitted unchanged, as code that does not parse or run. Now refused,
   `[print-stream]`.
3. Recovery after a refused member stopped inside braces of an annotation value, and left a half-consumed `>>`
   pending. Every later member of the class was then parsed out of step and reported as `[syntax]`. Now exact,
   checks `member-recovery` and `generic-recovery`. Most of the remaining 33 `[syntax]` sites were already
   `[syntax]` at the start of the session; they are cascades of `nested-ambiguous`/`type-ambiguous` at the same
   locations.

**Found, not fixed** (outside these three areas):
- printing an `Object` that holds a boxed primitive stops the VM in `JavaFormat.objectText` (`JAVA.SPEC.md` 5);
- `/lib` `ArrayList` does not declare that it implements `List`, so `List<String> xs = new ArrayList<>()` converts
  but fails the SLeeLa checker;
- `/lib` `Double` has no `MIN_VALUE`;
- `/lib` `Double.toString` prints subnormals with too few digits (`1.112536929253601E-308` for Java
  `1.1125369292536007E-308`).

**Results:**

| Measure | Model 1.6.0 | Model 1.7.0 |
|---|---|---|
| differential suite | 116 of 116 | **138 of 138** |
| `java.base` subset | 262 of 884 | 262 of 884 |
| all of `java.base` | 862 of 3344 | **864 of 3344**, none newly failing |
| `[syntax]` / `[model]` / `switch-pattern` sites | 435 / 304 / 41 | **33 / 0 / 11** |
| linked semantic errors, emitted files | 148 (intermediate generation) | 109 (881 files) |

The converted count barely moves because these files have other blockers behind the ones fixed (largest first
refusals: `type-ambiguous` 496 files, `library-type` 292, `library-member` 225). `working-binaries/javaconv` was
rebuilt as `JAVACONV-BIN-1.0122`. The APML binary set was not refreshed.

## 11. Iteration: boxed primitives held as Object

`System.out.println(o)` with `Object o = 42` stopped the VM: `JavaFormat.objectText` called `toString()` on a
primitive. The VM supports `instanceof int/double/boolean/String` on an `Any`, so `objectText` now returns the Java
text for null, String, boolean, double (through `Double.toString`) and the integer kinds, and calls `toString()` only
on objects. `char` and `float` are stored as an int and a double and cannot be told apart at run time, so boxing them
into `Object` is refused (`[boxed-reference]`). Tests `BoxedObjectText` (10 lines identical to the JDK) and
`reject/BoxedCharObject`. `lib/java/JavaFormat.sleela` edited, gate manifest regenerated (one entry). Differential
suite 140 of 140.

## 12. Iteration: Double and Float constants, subnormal text, append(double), negation

- `/lib` `Double` had no `MIN_VALUE`, and `Double`/`Float` `POSITIVE_INFINITY`, `NEGATIVE_INFINITY` and `NaN` were
  `0.0` placeholders, so reads were refused `[library-stub]`. `MIN_VALUE` is now an exact product
  (`(1/2^52) * 2^-1022`), and the others are `static final` expressions (`1.0 / 0.0`, `0.0 / 0.0`). The model gained
  22 hand `field` rows for these constants with their Java types: `/lib` declares the `Float` ones as `double`.
- `Double.toString` printed subnormals too short (`1.112536929253601E-308` for 2^-1023), because the round-trip test
  `decimalToDouble` rounded the sum of a double-double to a double before `rintD`. It now rounds the high and low
  parts separately, as `scaledRound` already did (fixed in `Double` and `Float`).
- `StringBuilder.append(double)` and `StringBuffer.append(double)` were not in `/lib`, and the call fell through to
  another overload that throws. The `/lib` member check compares only arity, so the converter did not refuse it.
  Both now append `Double.toString(d)`. A `float` argument is passed as its Java text through the 9c `chartext`
  rows, because SLeeLa stores a float as a double.
- Floating negation was `0.0 - x` in the model, which gives `+0.0` for `0.0`. It is now `-1.0 * x` (JLS 15.15.4).

Test `DoubleConstants` (10 lines identical to the JDK). Four `lib/**` files edited, gate manifest regenerated
(four entries). Differential suite 141 of 141.

## 13. Iteration: member types resolved by scope (JLS 6.4.1)

A simple type name was looked up in one project-wide index of nested-type names. A name such as `Entry` that several
classes declare as a member type was refused everywhere (`[nested-ambiguous]`, the first refusal in 175 files of
`java.base`), even inside the class that declares it. `resolveTypeName` now first checks the member types in scope:
those of the current class, of its enclosing classes, and of their supertypes. The nearest wins, as in Java. Two
different inherited member types of that name at one level are still refused. `new Outer.Inner()` now resolves
`Outer` by scope and imports, then its member `Inner`, like a type in a declaration. Check `scoped-nested` (a
three-file project, output identical to the JDK) in `tests/javaconv_differential_test.sh`. Pushed before the full
differential suite completed; its result is recorded in the next section.

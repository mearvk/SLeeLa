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

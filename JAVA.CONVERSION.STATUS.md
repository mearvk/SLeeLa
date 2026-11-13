<img align="right" src="images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# Java to SLeeLa Conversion: Overall Status

**As of:** 2026-11-13. **Branches:** `master` and `main` (same content; see section 7).
**Subject:** `sleelactl java-convert` (the C/C++ converter in `sleelactl/convert`), the conversion
model `impl/subjects/conversion/conversion.model` (revision **1.3.0**), and the Java tier under
`lib/java` that converted programs run on.

This page records **measured** status only. Every number below comes from a run on the date shown;
nothing is projected. The rules are in `.kiro/steering/java-to-sleela-conversion.md`, the
specification in `lib/java/JAVA.SPEC.md`, the principles and the done/open ledger in
`CONVERSION.PRINCIPLES.md`. Where those and this page disagree, those win and this page is stale.

## 1. Summary

| Question | Answer (2026-11-13) |
|---|---|
| Do converted programs behave like Java? | For every construct with a test, yes: **31 of 31** differential checks pass. 21 programs give output byte-identical to OpenJDK 25.0.2, 9 refusal programs are refused with the right construct, and a stale-output check passes. |
| How much of a real JDK converts? | **420 of 884** files of the JDK 25.0.2 `java.base` subset (496 SLeeLa files); **426** with `--single-thread`. The 2026-11-12 baseline was 344. |
| Does "converted" mean "compiles"? | **No.** A converted file passed the converter, which refuses what it cannot convert exactly. A tree-wide `sleela check` of the output has **not** been measured (section 4). |
| Is the JDK converted? | No. 464 files fail closed and write nothing. |
| Is it gated in CI? | Yes, from 2026-11-13: `.github/workflows/java-convert.yml`, first run 31 of 31 (section 6). |

## 2. Differential suite (`tests/javaconv_differential_test.sh`)

Each program in `sleelactl/convert/tests/java/` runs on a JDK (`javac` + `java`) and, converted, on the
SLeeLa VM built from the same commit (`impl/build/sleela`). The two outputs must be byte-identical.

| Kind | Count | Members |
|---|---|---|
| Programs identical to the JDK | 21 | Anon, Basics, Control, Enums, Floats, Flow, Formats, Functional, Grid, Init, Nested, ObjectText, Operands (191 lines), Records, Refs, Resources, Shapes, Strings, Switches, Sync, Varargs |
| Refusals (must fail closed with the stated construct, write nothing) | 9 | BareObjectMethod, BoxedDecl, IdentityText, LambdaUse, ObjectMethod, QualifiedNested, StringIdentity, SyncBlock, Untyped |
| Stale-output check | 1 | a file converted on one run and refused on the next leaves no `.sleela` behind |
| **Result** | **31 / 31** | run 2026-11-13 against OpenJDK 25.0.2 |

## 3. Whole-tree measurement

Input: JDK 25.0.2 `src.zip`, `java.base` packages `java/lang`, `java/util`, `java/math`, `java/text` with
their subpackages, 884 files (the same set as the 2026-11-12 baseline). Commands in `sleelactl/jdk/README.md`.

### 3.1 History

| Date / converter | Converted of 884 | Note |
|---|---|---|
| 2026-11-12, first converter | 344 | baseline |
| 2026-11-13, `323cc04f` (before the review) | 424 | the 2026-11-13 lowerings (enums, lambdas, nested types, ...) |
| 2026-11-13, `8f617ba2` (review commit) | did not finish | hung on `java/text/{DateFormat,MessageFormat,NumberFormat}`; fixed in `55f52f49` |
| 2026-11-13, `55f52f49` | 423 | qualified nested names fixed (+13); 10 files now refused `[object-method]` whose earlier output the checker rejects |
| 2026-11-13, model 1.3.0 (current) | **420** (426 with `--single-thread`) | 3 more files refused `[boxed]`; their earlier output did not compile |

The count went down twice on purpose: each time the converter stopped writing output that SLeeLa rejects.
A lower honest number is the goal of the fail-closed rule, not a regression.

### 3.2 Why files fail closed

First refusal of each of the 464 files that fail closed:

| Construct | Files | Meaning |
|---|---|---|
| `nested-ambiguous` | 131 | one simple name used by several nested types in a file (`Entry`, `Node`) |
| `nested` | 67 | nested forms not converted (local classes, `outer.new Inner()`, ...) |
| `untyped` | 57 | an operand of unknown type: no generics inference |
| `volatile` | 32 | needs `--single-thread` |
| `object-method` | 31 | `equals`/`hashCode`/`getClass` on a type that does not declare them |
| `syntax` | 28 | Java forms the parser does not take |
| `init-block` | 25 | `static { }` blocks |
| `model` | 18 | model limits (64-field class cap, ...) |
| `synchronized` | 16 | needs `--single-thread` |
| `boxed` | 13 | a boxed declared or cast type (`Integer i`) |
| `native` | 12 | no Java body |
| other | 34 | `lambda-target` 11, `reserved-member` 6, and smaller groups |

All diagnostics (a file can carry many): `nested-ambiguous` 2,566, `untyped` 810, `synchronized` 529,
`nested` 400, `syntax` 274, `model` 269, `volatile` 215, `native` 203, `object-method` 168,
`init-block` 132, `boxed` 125, `lambda-target` 88, `reserved-member` 47, `methodref` 19,
`string-identity` 14, `switch-expr` 14, `outer-instance` 12, `string-static` 10, `anonymous-nested` 9,
`format-nonliteral` 5, `lambda-this` 5, `side-effecting-lvalue` 3, `member-annotation` 2,
`object-text` 2, `format-conv` 1.

## 4. What "converted" does and does not prove

- **Proven:** for each construct covered by section 2, the converted program prints exactly what Java prints.
- **Not proven:** that a converted JDK file passes `sleela check`. Spot check of 4 converted files (2026-11-13):
  split-layout files name sibling nested types (`Format__Field`) that a standalone check does not link, a
  known loader gap (`CONVERSION.PRINCIPLES.md` section 3). Calls into library types behave only as well as
  their `lib/java` units, many of which are signature stubs.

### 4.1 Known ways a converted program can still differ from Java

These are not refused today. Each is listed here so that no result is read as more than it is.

| Case | Java | Converted program |
|---|---|---|
| An object held in an erased generic shown as text (`Box<StringBuilder>.v`) | its `toString()` | the VM handle; the value's run-time kind is unknown at conversion |
| `getClass()` on a type whose ancestor is outside the converted tree (`in.getClass()` with `in` a `java.io` stream) | the class | written; the checker rejects it |
| A nested user exception in `Throwable.toString()` | `Outer$Inner: msg` | `Outer__Inner: msg` |
| A `char` passed to a library member other than `StringBuilder`/`StringBuffer` `append`/`insert` that takes it as text (`list.add('c')` then print) | `c` | `99` |
| An uncaught exception | `Exception in thread "main" ...` and a stack trace on stderr | `uncaught language exception` |
| `x % 0.0` on doubles | `NaN` | a VM fault |
| Null dereference, stack overflow | `NullPointerException`, `StackOverflowError` | fatal VM error |

## 5. Changes made on 2026-11-13 (review and follow-up)

| Commit | Change | Proof |
|---|---|---|
| `8f617ba2` | field default values (JLS 4.12.5); field initializers and instance `{ }` blocks after `super(...)` (JLS 12.5); members inherited from library superclasses typed; `[object-method]` refusal; records declare their generated `equals`/`hashCode`; stale output removed on re-run | `Init`, `Records`, `reject/ObjectMethod`, stale-output check |
| `55f52f49` | `Outer.Inner` in a type resolves to `Outer__Inner` (a nested class had become its own superclass and hung the whole-tree run); bounded superclass walks; bare `getClass()` refused too; model 1.2.0 | `reject/QualifiedNested`, `reject/BareObjectMethod` |
| `1273f634` (main) | `main` brought level with `master` | section 7 |
| `c91a4ecf` | `master` given the loader CMakeLists from `main`; stale `sleela_terminal.c` manifest entry refreshed (it failed `verify-security` in CI since `8e39e535`) | CI section 6 |
| `e3993045` (model 1.3.0) | library objects shown as text through `toString()` (`tostr reference`, `JavaFormat.objectText`) instead of the VM handle; unknown-typed values keep plain text; `[object-text]` for a user object without `toString()`; `chartext` rows (`append('x')` appends `x`, not `120`); `[boxed]` instead of writing `Integer i = 42` that did not compile; CI workflow `java-convert.yml` | `ObjectText`, `reject/IdentityText`, `reject/BoxedDecl` |

## 6. Continuous integration

| Workflow | State on 2026-11-13 |
|---|---|
| `java-convert.yml` (new): manifest gate, runtime build, converter build, model self-check, differential suite on Temurin 25 | **pass, 31 of 31**, on `e3993045` (Temurin 25.0.5, so a second JDK build agrees with the local OpenJDK 25.0.2) |
| Advantage Library Verification, Native Drive | failed on `master` from `8e39e535` (stale manifest entry); **pass** on `c91a4ecf` and `e3993045` |
| Native Drive on `main` (`1273f634`) | failed once (`nm` timed out after 15 s on the runner, passes locally in 0.1 s); **passed on re-run** |
| sleelactl, API documentation build, SLeeLa GUI Document CI, Heterogeneous value regression, Comparison Standards Conformance | pass |
| Native C and C++ Portability, Complete SLeeLa VM and Language Conformance, SLeeLa VM Arithmetic Table Completion | still running on `e3993045` when this page was written; not claimed |

## 7. Branches

`main` and `master` carry the same content. On 2026-11-13 every file of both trees was compared against
1,656 commits of `master` history: nothing in `main`'s compiler, VM, library, `sleelactl`, `bin` or tests
was unique to `main`, so `main` was brought to `master`'s tree in one fast-forward commit (`1273f634`),
keeping the one file `main` had changed on its own (`os-level-support/loader/CMakeLists.txt`, which
`master` then took in `c91a4ecf`). The generated API HTML (`api/`, `docs/api/`, `sleeladoc-api/`) is
rebuilt by CI on each branch and may differ between them; it is generated output, not divergence.

## 8. Open work, in order of measured effect

| # | Item | Effect | Where |
|---|---|---|---|
| 1 | Resolve a bare nested name by scope instead of refusing it (`nested-ambiguous`) | first refusal of 131 JDK files | `sleelactl/convert` |
| 2 | Generics inference (`untyped`) | 57 files; the main blocker for collections and lambdas | both converters |
| 3 | Measure `sleela check` over the converted tree; resolve `Outer__Inner` files across split output | turns "converted" into "compiles" | loader / `sleelactl` |
| 4 | Local classes, `outer.new Inner()` (`nested`) | 67 files | `sleelactl/convert` |
| 5 | Identity `equals`/`hashCode` and a run-time class name (`object-method`, `getClass`) | 31 files; also fixes `Outer$Inner` text | library / converter |
| 6 | Boxing lowering (declare as `Any`, unbox at use, keep `null`) | 13 files first-refused, 125 diagnostics | `sleelactl/convert` |
| 7 | `static { }` blocks (guarded static initialisation, JLS 12.4) | 25 files | `sleelactl/convert` |
| 8 | Section 4.1 cases | correctness, not counts | converter / library / VM |
| 9 | Repeat the measurement on a larger tree (`java.base` whole, `java.io`) | coverage of the status | — |

## 9. Reproduce

```sh
cd impl && make build/sleela && cd ..                      # needs g++, X11, fontconfig, freetype, libpng
make -C sleelactl/convert && sleelactl/convert/build/javaconv --self-check
bash tests/javaconv_differential_test.sh                   # section 2 (needs a JDK on PATH)
# section 3: unzip the four packages from $JAVA_HOME/lib/src.zip, then
sleelactl/convert/build/javaconv --jdk <dir> --out <out> --jdk-version 25   # CONVERSION-REPORT.md/.json in <out>
```

Not run for this page (`.kiro/steering/validation-cadence.md`): the long suites (`make -k test`, the jdk-28
compile check, the hand-converted `examples/java/differential` suite, conformance programs). The second
converter, `lib/java/converter` (SLeeLa, per-member stubs, 0.7.0), was not re-measured; its last numbers
are in its README.

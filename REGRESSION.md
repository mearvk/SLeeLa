# SLeeLa regression set: verification and repair (17 November 2026)

Two passes over the language-wide regression set (`test-suites/regression/`) in one session:

1. **Verification** on `master` at `17814fc4`: the set passed as recorded, and its open items were listed.
2. **Repair**: every open item was fixed or put under CI, and two further defects found along the way were fixed.

## Current result

| Check | Result |
|---|---|
| Full set (with mutation tests), compiler built from `impl/` | 121 tests, 0 failed, 0 errors, 0 skipped, 160 s |
| Full set, refreshed `working-binaries/sleela` | 121 tests, 0 failed, 159 s |
| Run with a relative `SLEELA_BIN=impl/build/sleela` | passes (resolved to an absolute path) |
| New tests against the previous binary | all 8 fail as intended (VM hang caught after 60 s; C arrays and value-used `++` rejected) |
| `events_portable.c` on Linux, GCC and Clang, `-Wall -Wextra -Wpedantic -Werror` | 310 checks, 0 failures |
| Project targets: `test-sleela` (all 43 `impl/examples`), `test-nordshrift`, `-compiler`, `-semantic`, `-e2e`, `-compat`, `test-version`, `test-class-dispatch`, `test-ref`, `test-lineage`, `test-syntax-1.8`, `test-regex`, `test-java-utf16`, `test-java-string`, `-vm`, `-source`, `test-runtime-artifact-abi`, `test-path-platform`, `test-time-platform`, `test-catalog`, `test-unsigned-integer` | all pass |
| `make check-manifest` | OK |

Platform: Linux x86-64, GCC/G++ 11.5.0, Clang 15, Python 3.11.15, OpenJDK 25.0.2. ASan/UBSan, macOS and Windows are covered by the new CI workflow, not by this sandbox (no sanitizer runtime is installed here).

## Defects fixed

| ID | Defect | Fix | Files |
|---|---|---|---|
| HARNESS-RELATIVE-SLEELA-BIN | A relative `SLEELA_BIN` was used as given; the CLI suites run the compiler from scratch directories, so almost every CLI test failed with `FileNotFoundError`. An `SLEELA_BIN` that did not exist silently fell back to another compiler. | The path is resolved to an absolute one; an explicit path that is not an executable stops the run (exit 2) with a message naming it. | `regression_support.py`, `run_regression.py` |
| VM-ARRAY-TEXT-DEADLOCK | Converting an array of arrays to text (`print(m)`, `"m=" + m`) hung forever: `value_to_text` held the non-recursive array lock across the recursive call for each inner array. Found while writing the C-target array tests. | Elements are copied under the lock and rendered after releasing it; nesting deeper than 16 renders `[...]`. | `impl/core/sleela_core.c` |
| NS-C-ARRAYS | The shared Nordshrift lowering had no case for array indexing, so every target rejected arrays, and it skipped array-initializer elements instead of checking them. | Lowering handles indexing and checks initializer elements. The C target emits arrays: `new T[n]` of any rank, `{...}` literals, element read/write and compound assignment, `.length`, `null` and type defaults, printing like the VM, and `runtime error: array index out of range` with exit 1. Operands are sequenced like the VM (base, index, then value; for `a[i] op= v` the element is read before `v`). The Java and Sleela targets still refuse arrays (fail closed). | `impl/nordshrift/lowering.cpp`, `sleela_emit.cpp` |
| NS-C-POSTFIX-VALUE | The C target rejected `x++`/`x--` whose value is used. | Emitted for variables and array elements; prefix forms also work on elements. | `sleela_emit.cpp` |
| NS-C-NULL-EQ | In the C target `null` was the integer 0, so `s == null` was true for any non-null string. | `null` has its own tag; equality on null and arrays compares identity. | `sleela_emit.cpp` |
| EVENTS-WIN-INVALID-HANDLE | On Windows, `SL_EVENT_INVALID` (-1) is the current-process pseudo-handle, so `slevent_wait(SL_EVENT_INVALID, ...)` waited on the process itself (forever with an infinite timeout) and `set`/`reset` returned Win32 codes. | Invalid handles return `EINVAL` before any Win32 call, as on POSIX. | `impl/core/sleela_events.c` |
| EVENTS-WIN-SIGNAL-ONESHOT | The Windows CRT resets a `signal()` handler after one delivery, so a second signal killed the process (the POSIX branch had already been fixed with `sigaction`). | The handler re-installs itself on Windows. | `sleela_events.c` |
| EXAMPLE-FIXED-PORT | `struct_network.sleela` listened only on port 45231 and failed if it was taken. | Tries 45231–45262 and connects to the port it got; if all are taken it prints a message and exits non-zero. Tested with ports held by another process. | `impl/examples/struct_network.sleela` |

## Test changes

- `test_cli_language.ArrayText`: nested-array printing and concatenation, and the 16-level cut-off, each bounded to 60 s so a hang fails quickly.
- `test_cli_transpilers.CTarget`: arrays against both the VM and a fixed expected output; array errors (output before the error, exit code, message, the `a[i] = f()` and `a[i] += f()` evaluation order); value-used `++`/`--`.
- `test_cli_transpilers.Emitters`: the fail-closed check now expects arrays to be refused by the Java and Sleela targets, and the enhanced `for` by the C target, instead of expecting C to refuse arrays.
- `test_integrity_tools.CompilerSelection`: relative-path resolution, bad-path errors, and the runner refusing to start.
- `test_ctypes_differential`: skips with a stated reason under `--sanitize` (an ASan-built shared object cannot be loaded into an uninstrumented Python; the native suites cover the same code).
- New `test-suites/regression/c/events_portable.c`: a standalone test of `sleela_events.c` (timeouts, manual reset, invalid handles including an infinite wait, cross-thread wake, repeated signals, queue bounds) that needs only C11 and the OS thread API.

## CI

New workflow `.github/workflows/regression-set.yml`, on pushes and pull requests that touch `impl/`, `lib/`, the regression set, the binaries or the manifest tooling:

- **linux**: checks the manifest, builds the compiler, runs the full set against it, then the fast set against `working-binaries/sleela`.
- **sanitizers**: the native C/C++ suites under ASan + UBSan.
- **events**: `events_portable.c` with `sleela_events.c` on Ubuntu, macOS and Windows (MinGW-w64).

## Binaries

`working-binaries/sleela` and `nordshrift` were rebuilt from the fixed source; the versions are unchanged (`1.3.2-pro`, `Nordshrift 2.7.1-dev`). New SHA-256 values and the change note are in `working-binaries/README.md`. `security/sha256-manifest.json` was regenerated for the four changed source files.

## Still open

- Nordshrift's Java and Sleela targets do not emit arrays yet, and the C target does not emit the enhanced `for` or objects. All of these fail closed with a "cannot emit" error.
- The unrelated `Native C and C++ Portability` Windows job fails in `test-terminal-platform` (ConPTY); separate work is in progress on that.

## Third pass: coverage, corpus, document versions (17 November 2026)

| Check | Result |
|---|---|
| Full set (with mutation tests), `working-binaries/sleela` | 200 tests, 0 failed, 0 skipped, 193 s |
| Fast set, compiler built from `impl/` | 198 tests, 0 failed, 113 s |
| Native suites under ASan + UBSan, run locally | 17 tests, 0 failed (runtime libraries found through `LD_LIBRARY_PATH`) |
| Mutation test | 85 injected defects, all detected |
| New corpus against the previous `working-binaries/sleela` | 11 cases fail (crashes and the `@next` traversal bypass), as intended |
| Project targets re-run on the rebuilt compiler: `test-sleela`, `test-memmgr`, `-memmgr-grades`, `-thread-platform`, `-threads`, `-network`, `-network-platform`, `-synchro`, `test-nordshrift` (+ `-compiler`, `-semantic`, `-compat`, `-e2e`), `-version`, `-lineage`, `-class-dispatch`, `-ref`, `-syntax-1.8`, `-regex`, `-catalog`, `-runtime-artifact-abi`, `-unsigned-integer`, `-java-utf16`, `-java-string` | all pass |
| `make check-manifest` | OK |

### New coverage

- Native suites `reg_memmgr`, `reg_thread`, `reg_net` (loopback), `reg_synchro` (loopback UDP echo server), `reg_http6` (frame layout, truncation, oversized lengths, round trips, random input) and `reg_router`. The instrumented allocator is now mutex-protected so suites with worker threads may allocate.
- `corpus/` (63 cases at first, 66 after the follow-up below): annotations, `@next` traversal and percent-encoding, HTTP 6.0 framing, parser and semantic rejection, accepted oddities, compile-time and run-time limits. See `test-suites/regression/corpus/README.md`.
- `test_doc_versions.py`: the release and syntax range in the source and the binary must match the documents that state them.
- 32 new mutants for the new code (85 in all).

### Defects fixed

| ID | Defect | Fix | Files |
|---|---|---|---|
| PARSE-STACK-OVERFLOW / TREE-WALK-STACK-OVERFLOW | About 8,000 nested parentheses, 30,000 chained operators or 30,000 chained casts crashed the compiler with a segmentation fault. | A token pre-pass refuses source over fixed limits with a diagnostic. | `impl/frontend/parser.cpp` |
| REGEX-STACK-OVERFLOW | An identifier of about 50,000 characters crashed dependency discovery (`std::regex` recurses per character). | Hand-written identifier scan. | `impl/frontend/driver.cpp` |
| ANN-NEXT-DOTDOT-BYPASS | The parser joins annotation tokens with spaces, so `@next(../../etc)` reached the `..` check as `. . / . .` and was accepted. | The check ignores whitespace. | `impl/annotation/ForwardingAnnotation.cpp` |
| ANN-NEXT-ABSOLUTE-FORMS, ANN-NEXT-PERCENT-SMUGGLE | A drive or scheme (`C:/x`) and a percent-encoded `..` were accepted. | `:`, `%` and control characters are refused. | `ForwardingAnnotation.cpp` |
| MEMMGR-REALLOC-HEADROOM | `slmm_realloc` counted the old and new block together, so a growth that fits under the limit was refused. | The old block is credited while the new size is checked; the copy runs outside the lock. | `impl/core/sleela_memmgr.c` |
| THREAD-NULL-ARGUMENTS | On POSIX a NULL mutex, condition variable or thread handle was passed to pthreads and crashed (the Windows branch returned -1). | NULL is refused with -1 on every platform. | `impl/core/sleela_thread.c` |
| NET-WRITE-INVALID-ZERO | `slnet_write(SL_NET_INVALID, p, 0)` returned 0 (success). | An invalid handle fails first. | `impl/core/sleela_net.c` |

### Documents brought to the current version

Syntax 3.1 (range 1.0 .. 3.1, release 1.3.2-pro) replaces the earlier 3.0 ceiling in 22 documents (`markdown/`, `impl/`, `lib/`, `decompiler/`, `docs/`, `api/sleela/2.1/language/`, `install/`, and others); `markdown/VERSION.md` now names 3.1 as current. `impl/frontend/ANNOTATION_PIPELINE.md` now describes the `@name(value)` form the parser accepts. `impl/RUNTIME-AUDIT.md` finding 11 is closed by `test_doc_versions.py`.

### Open issues closed afterwards (same day)

| Issue | Resolution |
|---|---|
| `ANN-CLASS-BOUND` | Decided: a `@next` before a class, method or field is judged by the document rule (`impl/frontend/semantic.cpp`). New cases `ANN-METHOD-BOUND`, `ANN-FIELD-BOUND`. |
| `ANN-NEXT-SPELLING` | `ForwardingPolicy::normalize_destination` closes the spaces the parser inserts around `/`, `.` and `-`; the forwarder reports `system/net`. New case `ANN-NEXT-SPELLING-PUNCT`. |
| Both cases | Promoted from known issues to hard assertions; the corpus now has 66 cases and no open known issue. |
| `test_frontend_annotations.cpp` | Updated to the `@name(value)` form and `public class`; built against the `impl/build` objects it passes. It is still not part of a make target. |
| Nordshrift arrays | The Sleela target emits arrays (any rank, `{...}` in declarations, element read/write, compound assignment, `.length`); output runs identically and re-emits to itself (`Arrays` added to the emitter tests). The Sleela target also printed annotations in the old line form; it now writes `@name(value)`. |
| Nordshrift Java target | Still refuses arrays, deliberately: a Java array prints as an identity hash and `int` is widened to `long`, so the output would differ from the VM. The error now says so. |
| `SHA256-DIGESTS.md` | Regenerated. |

Result at that point: full set 203 tests, 0 failed, 196 s, against the rebuilt `working-binaries/sleela`; project targets `test-sleela`, `test-nordshrift` (+ `-compiler`, `-semantic`, `-compat`, `-e2e`), `-version`, `-lineage`, `-class-dispatch`, `-ref`, `-syntax-1.8`, `-catalog` and `check-manifest` pass.

### Remaining items, worked in order (same day)

| Item | Result |
|---|---|
| Class-level annotations in the Sleela emitter | Printed before the class; a document annotation makes the first class `public class` so it stays a document annotation (it was silently turned into a class annotation, which dropped its forwarding). A first class with annotations of its own plus document annotations is refused. `DocAnnot` and `ClassAnnot` join the emitter tests (run, round trip, idempotence). |
| Uncapped recursion shapes | Probed 18 shapes at 20,000 to 60,000 levels. Five crashed: an else-if chain, nested `for` and `do` without braces, a member-access chain, a label chain. Statement nesting is now capped at 3000 (`parseStatement`), and `.` and `:` count with the operators. Seven new corpus cases, two of them at the cap (corpus: 73 cases). |
| C target, enhanced `for` | Emitted over arrays (iterable evaluated once, bounds-checked element read). Over an Iterable object it still refuses. New test compares the C program with the VM. |
| Java target arrays and objects | Not done, on purpose: a Java array prints as an identity hash and `int` widens to `long`, so the output would differ from the VM. |
| C target objects | Not done: it needs an object model the C runtime does not have. |
| Windows and macOS builds of `reg_net`, `reg_thread`, `reg_synchro` | Not done: no such toolchain in this sandbox, so a port could not be checked. |

Final result: full set 211 tests, 0 failed, 206 s, against the rebuilt `working-binaries/sleela`; `test-sleela`, `test-nordshrift` (+ `-compiler`, `-semantic`, `-compat`, `-e2e`), `test-version`, `test-syntax-1.8`, `test-catalog` and `check-manifest` pass.

### Still open

- Windows and macOS builds of the POSIX-only suites (see above).
- The C target has no objects; the Java target has no arrays or objects.

## Syntax 3.2 follow-up (19 November 2026)

Master moved 706 commits (ChatGPT session, no shell). On arrival the integrity manifest did not match 14 files and
13 of 209 fast regression tests failed, because 3.2 now gates `a.length` and `new T[a][b]`. After review the manifest
was regenerated, a stale Nordshrift fixture fixed, the regression set moved to the current edition, and
`working-binaries/` rebuilt as `1.0121` (syntax 1.0 – 3.2). Full set: 214 tests, 0 failed. Details:
`test-suites/regression/PROGRESS.NOVEMBER.18.2026.md`, "Follow-up with a shell".

## Verification pass (before the repair)

On `master` at `17814fc4` with the then-committed binary: fast set 111/111 (81 s), full set 113/113 (155 s); the compiler rebuilt from `impl/` was byte-identical to `working-binaries/sleela`; `check-manifest` OK. This matched `test-suites/regression/PROGRESS.NOVEMBER.17.2026.md`.

## Also this session: FRUCHST language dictionaries

The same session extended the FRUCHST 80-language dictionary work under `languages/`; see `languages/FRUCHST_STATUS.md`. In summary: 51 of 80 profiles have 1,000-word lists; 22,344 of the 51,000 listed words have English, French and Spanish definitions or equivalents (French and Spanish found directly or through the English word); close-variant matching added English for 5,277 more words, bringing English coverage to 65.5%; no definition is approved yet.


## November 18 session: Java converter, JDK 28 validation and release qualification

This session continues the Java conversion and generic-resolution work. It is a separate validation track from the language-wide regression results above; the November 17 counts are retained as historical results and were **not re-run** against the November 18 Java changes.

### Repository baseline and merged work

- Baseline: `master` at `c00fd2f3ed760a2a840cecd02a02daec032b6453`.
- PR [#375](https://github.com/mearvk/SLeeLa/pull/375) merged into `master`: deterministic erased signature indexing, generic metadata retention, collision reporting, inherited superclass/interface implementation checks, constructor parameter compatibility, JDK 28 reflection inputs, and JDK 25-to-28 API comparison scaffolding.
- The merged change explicitly leaves validation pending. Generated API envelopes and fail-loud stubs are declarations/contracts, not implemented Java behavior.
- PR [#376](https://github.com/mearvk/SLeeLa/pull/376) adds the production release qualification report. At the time of this status update it is open on `work/production-release-qualification-report`; it is not yet part of `master`.

### Current regression evidence

| Area | November 18 status |
|---|---|
| Language-wide regression set | **Not re-run.** Latest recorded result remains November 17: 211 tests, 0 failed, 206 s, against the rebuilt `working-binaries/sleela`. |
| Fast regression set | **Not re-run.** November 17 record: 198 tests, 0 failed, 113 s. |
| Native ASan/UBSan suites | **Not re-run in this session.** November 17 record: 17 tests, 0 failed. |
| Mutation suite | **Not re-run.** November 17 record: 85 injected defects detected. |
| JDK 28 signature-index and generic-resolution tests | Source changes merged; current execution result **PENDING**. |
| C++ converter and runtime clean build | **NOT RUN in this session**. |
| SLeeLa-native converter differential and per-member stub policy | **NOT RUN in this session**. |
| Representative Java-to-SLeeLa program execution | **NOT RUN in this session**. |
| Hosted workflow status for merge commit | No workflow runs or combined commit statuses were exposed by the repository connection at inspection time. Do not claim CI passed. |

The November 17 regression set remains valid as a dated result for its tested commit and toolchain. It is not evidence that the Java converter changes in `c00fd2f3` passed those tests.

### Release qualification

The report `docs/reference/PRODUCTION.RELEASE.QUALIFICATION.2026-11-18.md` defines the release gates. The decision remains **HOLD** until a clean checkout build, converter self-checks, positive and negative differential tests, repeat-run/provenance checks, JDK 28 campaign, representative executions, and platform evidence are actually collected.

### Known Java conversion refusals and limitations

The current specification and converter status continue to identify:
- unresolved generic target-type inference, wildcard capture, intersection least-upper-bound constraints and generic constructor inference;
- uncollected functional-interface SAM targets;
- nested lambda/anonymous-class capture chains;
- local classes and `outer.new Inner()`;
- switch type/record patterns and sealed exhaustiveness;
- static initializer blocks;
- unsupported inherited `Object` methods;
- boxed null values and wrapper arrays;
- multithreaded `synchronized`/`volatile` semantics (the documented simplification is single-threaded only);
- Java native methods without a verified ABI binding;
- remaining ambiguous nested-type names and model/syntax limits.

The C++ converter remains fail-closed at whole-file scope. The SLeeLa-native converter retains its documented per-member policy: supported siblings may be emitted, while unsupported members are represented by fail-loud stubs and counted. Such stubs are not functional implementations and must not be included in behavioral-equivalence totals.

### Next regression sequence

1. Build the runtime and C++ converter from the exact `master` commit; regenerate and verify the integrity manifest from that checkout.
2. Run converter model self-check and generated signature/result-row checks.
3. Run the complete positive differential suite and all current refusal fixtures; retain raw logs and exact counts.
4. Run the SLeeLa-native per-member stub-policy and repeat-run/provenance tests.
5. Run the JDK 28 validation campaign and archive signature inventories, collision reports, API deltas and functional-equivalence diagnostics.
6. Execute representative converted programs and compare output with the reference JDK.
7. Run native, language-wide and platform regression suites; report Linux, Windows 10+ and macOS separately.
8. Update measured counts only from those outputs; keep the production release on HOLD until every required gate is satisfied.

### Session tooling boundary

Repository files and pull requests were updated through the GitHub repository interface. This session did not have a local checkout or shell execution path for the repository, so no build, compiler invocation, program execution or regression command was run. This limitation is recorded rather than substituting estimates or historical results.

## November 18 repository follow-up — merged documentation state

After the initial status entry above, PR [#376](https://github.com/mearvk/SLeeLa/pull/376) (production release qualification report) and PR [#377](https://github.com/mearvk/SLeeLa/pull/377) (regression status and session continuation record) were both merged into `master`. The recorded merge commit for #377 is `9835bb53591cb27e590a4cf08be959682059f862`; #376's merge commit is `d2b766c7280b1fb8d8b14ceb95bb84bffd1665c5`.

This updates the earlier time-qualified statement that #376 was still open. It does **not** change the validation evidence: current-master build, converter execution, JDK 28 campaign, representative Java execution and native/platform qualification remain unverified until raw current-run artifacts are collected. Production release remains **HOLD**.

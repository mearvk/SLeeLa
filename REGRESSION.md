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
- `corpus/` (63 cases): annotations, `@next` traversal and percent-encoding, HTTP 6.0 framing, parser and semantic rejection, accepted oddities, compile-time and run-time limits. See `test-suites/regression/corpus/README.md`.
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

### Still open

- `ANN-CLASS-BOUND`: an annotation directly before `class` binds to the class and skips `@next` validation; the pipeline document says it is a document annotation. A language decision.
- `ANN-NEXT-SPELLING`: a forwarded destination prints as `system / net`.
- `test-suites/cpp/test_frontend_annotations.cpp` uses the old line form (`@scope system`) and is not built.
- Nordshrift's Java and Sleela targets do not emit arrays; the C target does not emit the enhanced `for` or objects.
- Not verified here: the Windows and macOS branches of `sleela_events.c` and of the new suites (the new network, thread and synchro suites are POSIX-only and skip on Windows; CI covers `events_portable.c` on all three).
- Other recursion shapes are not capped (for example very long `else if` chains parse); a crash from a new shape belongs in the corpus.
- `SHA256-DIGESTS.md` was not regenerated in this pass; it still lists the previous `working-binaries/` digests and the changed files.

## Verification pass (before the repair)

On `master` at `17814fc4` with the then-committed binary: fast set 111/111 (81 s), full set 113/113 (155 s); the compiler rebuilt from `impl/` was byte-identical to `working-binaries/sleela`; `check-manifest` OK. This matched `test-suites/regression/PROGRESS.NOVEMBER.17.2026.md`.

## Also this session: FRUCHST language dictionaries

The same session extended the FRUCHST 80-language dictionary work under `languages/`; see `languages/FRUCHST_STATUS.md`. In summary: 51 of 80 profiles have 1,000-word lists; 22,344 of the 51,000 listed words have English, French and Spanish definitions or equivalents (French and Spanish found directly or through the English word); close-variant matching added English for 5,277 more words, bringing English coverage to 65.5%; no definition is approved yet.

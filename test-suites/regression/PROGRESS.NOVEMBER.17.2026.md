# Regression set: session progress (17 November 2026)

Five stages this session:

1. Commit `8e570460`: the regression set (C, C++, Python only).
2. Commit `0ea1760b`: the defects it found were fixed in the Sleela code, the
   trust-root manifest was reconciled, and each former known issue became a hard assertion.
3. Stage 3 ("improve all"): the open items from stage 2 were closed (see below). The root `REGRESSION.md` records the repair pass that followed (CI, Nordshrift C arrays, events on Windows).
4. Stage 4: more coverage (memory manager, threads, sockets, synchro, HTTP 6.0 frames, preferred router), a populated corpus, sanitizer run, and documents brought to syntax 3.1. Details in "Stage 4" below.
5. Stage 5: the issues left open by stage 4 were closed (see "Stage 5").

## Status

| Item | State |
|---|---|
| C suites (utf16, java_string, codec, regex, path, time/memory, events, memmgr, thread, net, synchro, http6, router) | passing |
| C++ suites (lexer, model-based differential) | passing |
| Python suites (ctypes, CLI language/contract/transpilers, corpus, integrity, document versions, mutation) | passing |
| Native suites under ASan + UBSan | passing (run locally in stage 4) |
| Full run against the committed `working-binaries/sleela` | 203 tests, 0 failures (about 113 s with `--fast`, about 196 s with mutation testing) |
| Mutation test | 85 injected defects, all detected (a hang counts as detected) |
| Open known issues | none |
| `security/sha256-manifest.json` | regenerated; `make check-manifest` prints OK |

## Defects fixed

| ID | Fix | File |
|---|---|---|
| (manifest drift) | regenerated the manifest; it was stale for 14 files | `security/sha256-manifest.json` |
| REGEX-QUANT-OVERFLOW | quantifier counts are overflow-checked; a count that does not fit is an invalid shape | `impl/core/sleela_regex.c` |
| TIME-NEG-FLOOR | whole seconds use floor division, matching the millisecond field | `impl/core/sleela_time.c` |
| TIME-JSON-ESCAPE | string fields are JSON-escaped; a record that does not fit returns `ERANGE` instead of truncated JSON | `impl/core/sleela_time.c` |
| LEX-CMP-TEXT | token text of `!=` `<=` `>=` is the full operator | `impl/frontend/lexer.cpp` |
| LANG-STR-PLUSEQ | `String s; s += <int/double/boolean/String>` is accepted | `impl/frontend/semantic.cpp` |
| NS-C-BITOPS | C target has real `& \| ^ << >> >>> ~`; unknown operators are an error; add/sub/mul/neg wrap without signed-overflow UB | `impl/nordshrift/sleela_emit.cpp` |
| NS-EMIT-SILENT-DROP / QUALIFIED | all three emitters fail closed on any node they cannot print; the Sleela target now prints `new`, method calls, member access, `this`, `super`, field assignment | `sleela_emit.cpp`, `lowering.cpp` |
| NS-EMIT-STATIC | Sleela target keeps `static`, `protected`, `extends`, `implements`; Java and C targets reject inheritance, constructors, interfaces | `sleela_emit.cpp` |
| NS-EMIT-BLOCKS | block bodies are printed once, so `emit(emit(x)) == emit(x)` | `sleela_emit.cpp` |
| MANIFEST-NESTED-BUILD | generator skips `build/` and `.build/` below scanned dirs (no tracked source file is affected) | `tools/generate-sha256-manifest.py` |

## Stage 3 changes

| Item | Change |
|---|---|
| `test-syntax-1.8` | The fixture expectations were stale: the compiler gives more precise messages ("declares no constructor to accept 2 argument(s)", "constructor overload resolution requires syntax 3.0"). Updated the expected text in `tests/sleela-syntax-1.8/run_syntax_1_8_tests.sh`; it passes with old and new binaries. |
| Nordshrift lowering | Assignment expressions (`=` and every compound operator), `++`/`--`, and `?:` now lower and emit for the Sleela, Java and C targets. Loops with counters now transpile. The C target still rejects array access and a postfix `++`/`--` whose value is used, with a clear message. |
| `sleela_events.c` (new `reg_events` suite) | Three defects fixed. (1) `set`/`wait`/`close` on `SL_EVENT_INVALID` dereferenced `-1` and crashed. (2) `slevent_wait` never blocked: a timed wait returned `ETIMEDOUT` at once and `set` from another thread could not wake anyone. The event is now a mutex + condition variable, one `set` wakes every waiter, `UINT32_MAX` waits forever. (3) `signal()` was one-shot under strict feature macros, so a second signal killed the process; now `sigaction`. |
| `run-all.sh --regression` | Now runs `run_regression.py --fast` instead of only checking that the directory exists. It also skips the git-tracked-file scan unless an audit mode needs it (that scan alone took over 15 minutes). |
| Regression harness | Per-process scratch directory, so concurrent runs cannot delete each other's files; results flush after each case so a crash keeps earlier output. |
| `working-binaries/` | `sleela` and `nordshrift` rebuilt from the fixed source; README digests and provenance note, the toolchain steering doc, and `SHA256-DIGESTS.md` updated. |

## Stage 4 (coverage, corpus, document versions)

| Item | Change |
|---|---|
| New C suites | `reg_memmgr`, `reg_thread`, `reg_net`, `reg_synchro`, `reg_http6`, `reg_router`. The instrumented allocator got a mutex so suites with worker threads can allocate. |
| Corpus | `corpus/` held 63 cases at this stage (66 after stage 5) in seven families, a `.expect` sidecar per fixture, generated cases for oversized inputs, and a README whose table is checked against the fixtures. Run as the `corpus` group. |
| Defects fixed | Compiler crashes on deep nesting, long operator chains and long identifiers (now diagnostics); `@next` traversal bypass through token spacing, drive letters and `%`; `slmm_realloc` double counting; NULL arguments in `slthread_*` on POSIX; zero-length `slnet_write` on an invalid handle. Table in the root `REGRESSION.md`. |
| Sanitizers | The native suites pass under ASan + UBSan locally (the libraries are installed here under a versioned name; `LD_LIBRARY_PATH` points the loader at them). The earlier note that they were not installed is out of date. |
| Document versions | Syntax 3.1 replaces 3.0 as the stated ceiling in 22 documents. `test_doc_versions.py` keeps the release, the range and those documents in step. `impl/RUNTIME-AUDIT.md` finding 11 is closed. |
| CI | `regression-set.yml` now also triggers on `http-6.0/`, `preferred-routers/` and the documents that state the version. (The workflow itself was added in the repair pass; the "Not wired into CI" note below is out of date.) |
| Mutants | 85: 32 new, for memmgr, thread, net, synchro, http6 and router. |
| Test timing | The network suites scan from a per-process port offset and the synchro loss test uses a 250 ms budget, after one mutation run failed under parallel load. |
| `working-binaries/` | `sleela` and `nordshrift` rebuilt from the fixed source; README digests and note updated. |

Left open by stage 4 and closed in stage 5: Nordshrift arrays (Sleela target), regeneration of
`SHA256-DIGESTS.md`, and the decision on `ANN-CLASS-BOUND`. Windows and macOS builds of the new suites remain open.

## Stage 5 (the open issues from stage 4)

| Item | Resolution |
|---|---|
| `ANN-CLASS-BOUND` | A `@next` on a class, method or field is validated like a document annotation. |
| `ANN-NEXT-SPELLING` | The forwarder reports `system/net`; the spaces the parser inserts are closed around `/`, `.` and `-`. |
| Corpus | Both cases are hard assertions; `ANN-METHOD-BOUND`, `ANN-FIELD-BOUND` and `ANN-NEXT-SPELLING-PUNCT` added (66 cases, no known issue). |
| `test_frontend_annotations.cpp` | Brought to the current syntax; passes when built against `impl/build` objects. |
| Nordshrift | The Sleela target emits arrays and prints annotations as `@name(value)`. The Java target refuses arrays on purpose (identity-hash printing, int widened to long). |
| `working-binaries/`, manifest, `SHA256-DIGESTS.md` | Rebuilt and regenerated. |

Not done: Windows and macOS builds of the POSIX-only suites (no such toolchain here), the C target's
enhanced `for` and objects, the Java target's arrays and objects, class-level annotations in the Sleela emitter.

## Verified against the project's own tests

With the rebuilt compiler: `test-regex`, `test-time-platform`, `test-path-platform`,
`test-java-utf16`, `test-java-string`, `test-version`, `test-class-dispatch`,
`test-lineage`, `test-ref`, `test-runtime-artifact-abi`, `test-unsigned-integer`,
`test-nordshrift`, `-compiler`, `-semantic`, `-e2e`, `-compat`, `test-catalog`,
`test-syntax-1.8` and the events platform smoke test all pass.

`test-sleela` still stops at `impl/examples/struct_network.sleela`. Its fixed port 45231
is already answered by an HTTP proxy in this sandbox (it replies `400 Bad Request`), so
`listen` fails and the example's error path closes an invalid socket. This is the
environment, not the code; on a host where the port is free the example is unaffected.

## Not done

- Verified only on Linux x86-64 with GCC 11.5 (superseded: stage 4 ran ASan/UBSan locally and CI covers
  `events_portable.c` on all three systems).
- Not wired into CI (superseded: `.github/workflows/regression-set.yml` exists).
- Nordshrift's C target still rejects array access and value-producing postfix `++`/`--`.

## How to run

    python3 test-suites/regression/python/run_regression.py            # everything
    python3 test-suites/regression/python/run_regression.py --fast     # skip mutation tests
    SLEELA_BIN=impl/build/sleela python3 test-suites/regression/python/run_regression.py

Build the compiler first with `cd impl && SLEELA_SHA256_MANIFEST="$(pwd)/../security/sha256-manifest.json" make build/sleela`.
After any edit under `lib/**` or `impl/` source, regenerate the manifest:
`python3 tools/generate-sha256-manifest.py --root . --output security/sha256-manifest.json`.

## Possible next steps

1. Port the POSIX-only suites (net, thread, synchro, path) to Windows and macOS runners.
2. Array and object support in the Nordshrift Java and Sleela targets.
3. Print class-level annotations in the Sleela emitter.
4. Suites for more of `impl/core` (io, library, terminal, ai, media) and for `impl/frontend` semantic and compiler passes.

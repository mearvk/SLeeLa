# Regression set: session progress (17 November 2026)

Three stages this session:

1. Commit `8e570460`: the regression set (C, C++, Python only).
2. Commit `0ea1760b`: the defects it found were fixed in the Sleela code, the
   trust-root manifest was reconciled, and each former known issue became a hard assertion.
3. This stage ("improve all"): the open items from stage 2 were closed (see below).

## Status

| Item | State |
|---|---|
| C suites (utf16, java_string, codec, regex, path, time/memory, events) | passing |
| C++ suites (lexer, model-based differential) | passing |
| Python suites (ctypes, CLI language/contract/transpilers, integrity, mutation) | passing |
| Full run against the committed `working-binaries/sleela` | 113 tests, 0 failures (about 90 s with `--fast`, about 3 min with mutation testing) |
| Mutation test | 51 injected defects, all detected (a hang counts as detected) |
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

- Verified only on Linux x86-64 with GCC 11.5. ASan/UBSan is not installed in the sandbox,
  so `--sanitize` is untested. The Windows and macOS branches of `sleela_events.c` were not
  compiled here.
- Not wired into CI (`.github/workflows`).
- Nordshrift's C target still rejects array access and value-producing postfix `++`/`--`.

## How to run

    python3 test-suites/regression/python/run_regression.py            # everything
    python3 test-suites/regression/python/run_regression.py --fast     # skip mutation tests
    SLEELA_BIN=impl/build/sleela python3 test-suites/regression/python/run_regression.py

Build the compiler first with `cd impl && SLEELA_SHA256_MANIFEST="$(pwd)/../security/sha256-manifest.json" make build/sleela`.
After any edit under `lib/**` or `impl/` source, regenerate the manifest:
`python3 tools/generate-sha256-manifest.py --root . --output security/sha256-manifest.json`.

## Possible next steps

1. Run the set in CI on Linux, macOS and Windows runners, with `--sanitize` where available.
2. Make `struct_network.sleela` pick a free port (or skip when the port is taken).
3. Array support in the Nordshrift C target.
4. Suites for more of `impl/core` (net, memmgr, synchro) and the annotation pipeline.

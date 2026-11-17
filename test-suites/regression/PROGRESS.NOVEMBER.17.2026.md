# Regression set: session progress (17 November 2026)

Two stages this session:

1. Commit `8e570460`: the regression set (C, C++, Python only).
2. This stage: the defects it found were fixed in the Sleela code, the trust-root
   manifest was reconciled, and each former known issue became a hard assertion.

## Status

| Item | State |
|---|---|
| C suites (utf16, java_string, codec, regex, path, time/memory) | passing |
| C++ suites (lexer, model-based differential) | passing |
| Python suites (ctypes, CLI language/contract/transpilers, integrity, mutation) | passing |
| Full run against the source-built compiler | 110 tests, 0 failures, about 110 s |
| Mutation test | 45 injected defects, all detected |
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

## Verified against the project's own tests

With the rebuilt compiler: `test-regex`, `test-time-platform`, `test-path-platform`,
`test-java-utf16`, `test-java-string`, `test-version`, `test-class-dispatch`,
`test-lineage`, `test-ref`, `test-runtime-artifact-abi`, `test-unsigned-integer`,
`test-nordshrift`, `-compiler`, `-semantic`, `-e2e`, `-compat` and `test-catalog` all pass.

Two project tests fail, neither caused by this work:

- `test-syntax-1.8`: two rejection messages differ from the expected text. It fails the
  same way with the old committed `working-binaries/sleela`.
- `test-sleela`: `struct_network.sleela` makes a network request, and the sandbox has no
  network.

## Not done

- `working-binaries/sleela` and `nordshrift` are still the old builds. They run against the
  new manifest, but they do not contain these fixes. Refreshing them means copying
  `impl/build/{sleela,nordshrift}` and updating the documents that carry digests and
  version claims (`working-binaries/README.md`, the steering docs, `SHA256-DIGESTS.md`).
- Verified only on Linux x86-64 with GCC 11.5. ASan/UBSan is not installed in the sandbox,
  so `--sanitize` is untested.
- Not wired into `test-suites/run-all.sh` or CI.
- Nordshrift's lowering still rejects assignment expressions, array access, ternaries and
  a few other nodes. It now fails with a clear message instead of emitting wrong code.

## How to run

    python3 test-suites/regression/python/run_regression.py            # everything
    python3 test-suites/regression/python/run_regression.py --fast     # skip mutation tests
    SLEELA_BIN=impl/build/sleela python3 test-suites/regression/python/run_regression.py

Build the compiler first with `cd impl && SLEELA_SHA256_MANIFEST="$(pwd)/../security/sha256-manifest.json" make build/sleela`.
After any edit under `lib/**` or `impl/` source, regenerate the manifest:
`python3 tools/generate-sha256-manifest.py --root . --output security/sha256-manifest.json`.

## Possible next steps

1. Refresh `working-binaries/` and the documents that describe them.
2. Wire `run_regression.py --fast` into `run-all.sh --regression` and CI.
3. Decide on the two unrelated failing project tests (`test-syntax-1.8`, and `test-sleela` offline).
4. Extend Nordshrift lowering to the nodes it still rejects, with matching regression tests.
5. Add suites for more of `impl/core` (net, events, memmgr) and the annotation pipeline.

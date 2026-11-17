# Regression set: session progress (17 November 2026)

Commit `8e570460` on `master` added the regression set (C, C++, Python only).
This file records where the work stands.

## Status

| Item | State |
|---|---|
| C suites (utf16, java_string, codec, regex, path, time/memory) | done, passing |
| C++ suites (lexer, model-based differential) | done, passing |
| Python suites (ctypes, CLI language, CLI contract, transpilers, integrity, mutation) | done, passing except one |
| Runner `python/run_regression.py` | done |
| Last full run | 107 tests, 106 pass, 1 fails, about 100 s |
| Mutation test | 41 injected defects, all detected |
| Pushed to `master` | yes |

## Open failure (a real repo defect)

`test_integrity_tools.CommittedTrustRoot.test_committed_manifest_matches_the_committed_tree`
fails: `security/sha256-manifest.json` does not match 14 files
(`impl/core/sleela_artifact.c`, `sleela_core.c`, `sleela_core.h`, `sleela_terminal.c`,
`terminal_platform_smoke.c`, `impl/frontend/compiler.cpp`, seven
`lib/java/converter/JConv*.sleela`, `lib/vm/InstructionSet.sleela`).
The committed `working-binaries/sleela` therefore refuses to run on a fresh clone.

Fix, not yet applied (it changes the trust root, so it needs a decision):

    python3 tools/generate-sha256-manifest.py --root . --output security/sha256-manifest.json

## Known issues found by the suites (open, reported, do not fail the run)

| ID | Defect |
|---|---|
| NS-C-BITOPS | Nordshrift C target lowers `&` `^` `<<` `>>` to logical-or |
| NS-EMIT-SILENT-DROP | Sleela-target emitter drops `extends` and `new`, and emits `b.hi()` as an empty call |
| NS-EMIT-STATIC | `static` dropped by the Sleela-target emitter |
| NS-EMIT-QUALIFIED | `print(A.f())` emitted as `print()` |
| NS-EMIT-BLOCKS | re-emitting wraps each `if` body in another `{ }` |
| TIME-NEG-FLOOR | negative non-whole-second instants format a second late |
| TIME-JSON-ESCAPE | `sltime_json` does not escape string fields |
| REGEX-QUANT-OVERFLOW | `digit{18446744073709551617}` wraps to `{1}` |
| LANG-STR-PLUSEQ | `String s; s += 1;` rejected by the semantic checker |
| LEX-CMP-TEXT | token text of `!=` `<=` `>=` truncated to the first character |
| MANIFEST-NESTED-BUILD | generator hashes `build/` dirs nested in scanned source dirs |

## How to run

    python3 test-suites/regression/python/run_regression.py            # everything
    python3 test-suites/regression/python/run_regression.py --fast     # skip mutation tests
    python3 test-suites/regression/python/run_regression.py --only native cli
    python3 test-suites/regression/python/run_regression.py --list

Environment: `CC`, `CXX`, `SLEELA_BIN`, `REG_SANITIZE=1`, `REG_SKIP_MUTATION=1`.
Build output goes to `test-suites/regression/build/` (ignored by git).

## Limits

- Verified only on Linux x86-64 with GCC 11.5. ASan/UBSan is not installed in the
  sandbox, so `--sanitize` is untested. The path suite is POSIX-only.
- CLI tests default to the committed `working-binaries/sleela`; set `SLEELA_BIN` to
  test a source build.
- Not wired into `test-suites/run-all.sh`; its `--regression` option only checks that
  the directory exists.

## Possible next steps

1. Regenerate `security/sha256-manifest.json` to clear the one failing test.
2. Fix the open known issues; each one flips to "FIXED" in the runner output and its
   check can then be promoted to a hard assertion.
3. Hook `run_regression.py --fast` into `run-all.sh --regression` and CI.
4. Run once with `--sanitize` on a host that has ASan/UBSan.
5. Add suites for more of `impl/core` (net, events, memmgr) and the annotation pipeline.

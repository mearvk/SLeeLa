# SLeeLa regression set: verification session (17 November 2026)

This session re-ran the language-wide regression set (`test-suites/regression/`) on current `master` to confirm it still holds. No source, test, or binary was changed; this file is the only addition.

## What was tested

| Item | Value |
|---|---|
| Repository | `mearvk/SLeeLa`, branch `master` at `17814fc4` |
| Regression set | `test-suites/regression/` (C, C++ and Python suites), last changed in `381747f7` |
| Compiler under test | `working-binaries/sleela`, `SLeeLa™ Language/Runtime 1.3.2-pro`, source syntax 1.0 – 3.1 |
| Platform | Linux x86-64, GCC/G++ 11.5.0, Python 3.11.15, OpenJDK/javac 25.0.2 |
| Sanitizers | off (ASan/UBSan are not installed in this environment) |

No commit after `381747f7` touches `impl/`, `lib/`, `working-binaries/` or `test-suites/`.

## Results

| Check | Command | Result |
|---|---|---|
| Fast run (mutation tests skipped), committed binary | `python3 test-suites/regression/python/run_regression.py --fast` | 111 tests, 0 failed, 0 errors, 0 skipped, 81 s |
| Full run (with mutation tests), committed binary | `python3 test-suites/regression/python/run_regression.py` | 113 tests, 0 failed, 0 errors, 0 skipped, 155 s |
| Rebuild compiler from source | `cd impl && SLEELA_SHA256_MANIFEST="$(pwd)/../security/sha256-manifest.json" make build/sleela` | builds cleanly; output is byte-identical to `working-binaries/sleela` (SHA-256 prefix `d3276fc7f5731b7a`) |
| Full run, source-built compiler (absolute path) | `SLEELA_BIN="$(pwd)/impl/build/sleela" python3 test-suites/regression/python/run_regression.py` | 113 tests, 0 failed, 155 s |
| Trust-root manifest | `cd impl && make check-manifest` | `check-manifest: OK - manifest matches the source tree` |

These match the set's own record in `test-suites/regression/PROGRESS.NOVEMBER.17.2026.md`: 113 tests, the mutation test detecting all 51 injected defects, and no open known issues.

## Finding: relative `SLEELA_BIN` path

The run command documented in the progress note, `SLEELA_BIN=impl/build/sleela python3 test-suites/regression/python/run_regression.py`, fails two tests:

- `test_cli_transpilers.JavaTarget.test_generated_integer_expressions_agree_across_vm_jvm_and_oracle`
- `test_cli_transpilers.JavaTarget.test_pure_program_agrees_with_vm_and_oracle`

Cause: `sleela_binary()` in `test-suites/regression/python/regression_support.py` (line 211) returns `$SLEELA_BIN` exactly as given. These tests run the compiler from a scratch directory, where a relative path does not resolve. With an absolute path all 113 tests pass. Fix (not applied): resolve the path to an absolute one, for example `return Path(cand).resolve()`. Until then, pass an absolute path.

## Still open

- The set is not run by GitHub Actions; only `test-suites/run-all.sh --regression` calls it, so it does not run on each push.
- Verified only on Linux x86-64 with GCC 11.5. `--sanitize` is untested, and the Windows and macOS branches of `impl/core/sleela_events.c` have not been compiled.
- Nordshrift's C target still rejects array access and a postfix `++`/`--` whose value is used.
- `test-sleela` still stops at `impl/examples/struct_network.sleela` in this sandbox, because its fixed port 45231 is taken by the environment proxy (recorded in the progress note).

## Also this session: FRUCHST language dictionaries

The same session extended the FRUCHST 80-language dictionary work under `languages/`; see `languages/FRUCHST_STATUS.md`. In summary: 51 of 80 profiles have 1,000-word lists; 22,344 of the 51,000 listed words have English, French and Spanish definitions or equivalents (French and Spanish found directly or through the English word); close-variant matching added English for 5,277 more words, bringing English coverage to 65.5%; no definition is approved yet.

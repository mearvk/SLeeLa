<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa Test Suites

/test-suites is the repository-level verification system for SLeeLa.

Layers:
1. C ABI/API tests.
2. C++ contract and behavior tests.
3. Header and translation-unit audits.
4. Runtime smoke and integration tests.
5. Explicit TEST.MAP.yml function-to-test mapping.
6. Generated FUNCTION.COVERAGE.md and FUNCTION.COVERAGE.json inventories.
7. Behavioral-test skeleton generation for compile-only gaps.
8. Negative/security and regression corpora.
9. ASan/UBSan sanitizer execution where supported.
10. Optional gcov/LLVM runtime coverage.
11. CI artifact collection.

Quick start:
    ./test-suites/run-all.sh
    ./test-suites/run-all.sh --audit
    ./test-suites/run-all.sh --coverage
    ./test-suites/run-all.sh --sanitizers

## Standalone translation-unit audit (`--audit` / `--headers`)

The audit compiles every SLeeLa-authored `.c`/`.cpp` (and, in `--headers`
mode, every `.h`/`.hpp`) on its own with the strict warning set
(`-Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror`,
`-std=c11` / `-std=c++20`). It verifies each file is self-consistent, not that
the whole program links. Two rules keep the result meaningful:

- **Include resolution mirrors the real build.** Each unit is compiled with its
  own directory and its module's `include/` and `src/` directories FIRST (so a
  subsystem's own header wins over a same-named header elsewhere in the tree —
  several subsystems define different structs under the same header name), then
  the common repository roots the per-module Makefiles add (`impl/core`,
  `impl/frontend`, the subject-library dirs, `runtime`, `bash`, `include`).
  Overridable via `C_STD` / `CXX_STD`.
- **Only committed source is audited.** Transient build output (anything under
  `build/`/`.build/`, including the unpacked vendored nghttp2 tree, and the
  compiled smoke-test binaries the `tests/` Makefile emits) is never fed to the
  compiler as if it were source.

Units that require a **foreign OS SDK not present on the audit host** are
reported as **SKIP**, not FAIL, because they can only build on their target OS
(and are covered by the per-OS build workflows): the macOS **CoreAudio**
backend, the **Windows SDK** (WASAPI / WRL) backend, and GTK-based GUI units.

Important: source-name inventory is heuristic evidence, not semantic proof. The
audit is a lint/consistency gate; runtime instrumentation and explicit
behavioral assertions remain the authoritative evidence for behavior.

See TEST.MATRIX.md, FUNCTION.COVERAGE.POLICY.md, TEST.MAP.yml, CI.MATRIX.md, negative/, regression/, and sanitizers/.
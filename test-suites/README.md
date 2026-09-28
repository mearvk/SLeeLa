# SLeeLa Test Suites

/test-suites is the repository-level testbed for SLeeLa. It is independent of any one build system so it can be used from a developer checkout, CI, a Server Edition build, or a platform-specific build.

The testbed has four layers:

1. C tests for ABI-facing and C API contracts.
2. C++ tests for class/type contracts and implementation behavior.
3. Translation-unit audit that discovers every C/C++ implementation unit and attempts a syntax-only compile where the host toolchain permits it.
4. Runtime smoke tests for stable annotation, forwarding, and HTTP bridge contracts.

The testbed does not silently declare an unbuildable module passed. Every skipped, unavailable, compile-failed, or runtime-failed test is reported.

## Layout

- run-all.sh — portable test orchestrator.
- c/ — C assertions and C/ABI tests.
- cpp/ — C++ class and runtime tests.
- logs/ — generated locally and ignored by the harness.
- TEST.MATRIX.md — coverage model and pass/fail rules.

## Quick start

From the repository root:

    ./test-suites/run-all.sh

Useful modes:

    ./test-suites/run-all.sh --smoke
    ./test-suites/run-all.sh --headers
    ./test-suites/run-all.sh --audit
    ./test-suites/run-all.sh --clean

--smoke runs deterministic C/C++ tests. --headers checks the SLeeLa C/C++ header surface. --audit additionally attempts syntax-only compilation of discovered implementation units.

## Pass/fail rule

A test passes only when its executable returns 0. A translation unit passes the audit only when its syntax-only compilation succeeds. Toolchain-unavailable cases are reported separately from failures.

## Coverage principle

The repository contains substantially more source than the foundational API classes. The testbed therefore uses a discover-and-audit model rather than pretending that a hand-maintained list is the complete universe of SLeeLa code.

New C/C++ files automatically enter the audit. New public behavior should receive a focused runtime test in c/ or cpp/.


## Function-by-function verification inventory

`generate-function-coverage.py` scans repository C/C++ implementation files and produces `FUNCTION.COVERAGE.md`.

The inventory distinguishes:

- **tested** — a behavioral test maps to the function;
- **integration-tested** — the function translation unit is exercised through an explicit integration path;
- **compile-only** — the function is discovered and syntax-checked, but no behavioral mapping is present;
- **untested** — reserved for functions without test or compile evidence.

The generator is deliberately conservative. It is a verification inventory, not a replacement for runtime instrumentation such as gcov or LLVM source-based coverage.

Run:

    ./test-suites/generate-function-coverage.sh

The generated inventory becomes the backlog for converting compile-only functions into behavioral tests.

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">

<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">



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
    ./test-suites/run-all.sh --coverage
    ./test-suites/run-all.sh --sanitizers

Important: source-name inventory is heuristic evidence, not semantic proof. Runtime instrumentation and explicit behavioral assertions remain the authoritative evidence for behavior.

See TEST.MATRIX.md, FUNCTION.COVERAGE.POLICY.md, TEST.MAP.yml, CI.MATRIX.md, negative/, regression/, and sanitizers/.
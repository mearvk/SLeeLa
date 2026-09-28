# SLeeLa Debugger Version

Version: 0.2.0
Date: 2026-09-27
Language implementation: C++17
C implementation: not yet present

## 0.2.0

- Formalized the C/C++ structural review.
- Documented ownership and identity rules.
- Documented diagnostic evidence methodology.
- Documented future C ABI requirements.
- Documented backend separation.
- Documented unit, behavioral, backend, cross-platform, and regression testing.
- Explicitly separated debugger evidence from test pass/fail status.

## Versioning policy

Use semantic versioning:

- MAJOR: incompatible public API or diagnostic-contract changes.
- MINOR: backward-compatible capability or API additions.
- PATCH: backward-compatible corrections, tests, and documentation.

The diagnostic report schema must receive an explicit schema version before external automation treats report text as a stable interface.

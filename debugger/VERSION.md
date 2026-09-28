# SLeeLa Debugger Version

Version: 0.3.0
Date: 2026-09-27
Language implementation: C++17
C implementation: C11+
Line-control API: C and C++
Backend abstraction: C++

## 0.3.0

- Added the C debugger ABI.
- Added C line-control points.
- Added C++ line-control support.
- Added TRACE, STOP, and EXCEPTION line actions.
- Added voice-command parsing for debugger control.
- Added a backend-neutral C++ debugger interface.
- Added explicit backend capability reporting.
- Added portable unsupported-operation behavior.
- Added documentation for IDE, terminal, and native-backend integration.
- Established the architecture for Linux, macOS, and Windows native backends.

## 0.2.0

- Formalized the C/C++ structural review.
- Documented ownership and identity rules.
- Documented diagnostic evidence methodology.
- Documented backend separation.
- Documented debugger testing methodology.

## Versioning policy

Use semantic versioning:

- MAJOR: incompatible public API or diagnostic-contract changes.
- MINOR: backward-compatible capability or API additions.
- PATCH: backward-compatible corrections, tests, and documentation.

Native process-control implementations and externally consumed diagnostic schemas must document their capability and schema versions independently.

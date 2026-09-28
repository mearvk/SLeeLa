# SLeeLa Debugger Version

Version: 0.4.0
Date: 2026-09-27
Language implementation: C++17
C implementation: C11+
Line-control API: C and C++
Backend abstraction: C++
Action model: C++

## 0.4.0

- Added the typed DebugAction model.
- Added continue, pause, step-in, step-over, step-out, break, conditional break, and one-shot break actions.
- Added watch, exception, thread, stack, memory, register, inspect, trace, and history actions.
- Added launch, attach, detach, restart, terminate, and report actions.
- Added explicit action lifecycle states: requested, validated, supported, executed, completed, and failed.
- Added action validation and bounded argument handling.
- Added regression coverage for every action type and validation failure.
- Added ACTION_MODEL.md defining the action-to-backend contract.
- Established the contract for future native Linux, macOS, and Windows action execution.

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

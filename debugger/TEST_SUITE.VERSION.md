# SLeeLa Debugger Test Suite Version

Version: 0.2.0
Date: 2026-09-27
Scope: C/C++ debugger core, line control, voice parser, and backend abstraction

## 0.2.0

- Added C++ debugger core regression coverage.
- Added C ABI regression coverage.
- Added C++ line-control coverage.
- Added C line-control and voice-command coverage.
- Added portable backend capability/unsupported-operation coverage.
- Added a unified test runner for C and C++ debugger components.

## Test policy

The test suite verifies observable debugger behavior and API contracts. It does not claim that native operating-system process control works until a native backend integration test exercises that backend.

Native backend tests should be capability-aware and should clearly distinguish unavailable platform facilities from test failures.

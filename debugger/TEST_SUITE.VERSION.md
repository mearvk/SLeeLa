# SLeeLa Debugger Test Suite Version

Version: 0.3.0
Date: 2026-09-27
Debugger baseline: 0.3.0
Scope: C/C++ debugger core, line control, voice parser, and backend abstraction

## 0.3.0

- Added C++ debugger core regression coverage.
- Added C ABI regression coverage.
- Added C++ line-control coverage for TRACE, STOP, and EXCEPTION.
- Added C line-control coverage.
- Added C voice-command coverage.
- Added portable backend capability and unsupported-operation coverage.
- Added a unified C/C++ debugger test runner.
- Established capability-aware native-backend integration testing requirements.

## Test policy

The suite verifies observable debugger behavior and API contracts. It does not claim that native operating-system process control works until a native backend integration test exercises that backend.

Native backend tests must distinguish unavailable platform facilities from actual test failures.

## Coverage layers

1. Core API and event/report behavior.
2. C ABI behavior.
3. C++ line-control behavior.
4. C line-control and voice parsing.
5. Backend capability contracts.
6. Native launch/attach/step/breakpoint integration when platform backends are available.
7. Regression fixtures for every confirmed debugger defect.

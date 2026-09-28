# SLeeLa Debugger Test Suite Version

Version: 0.8.0
Date: 2026-09-27
Debugger baseline: 0.8.0
Scope: C/C++ debugger core, line control, backend abstraction, DebugEngine, DiagnosticsEngine, and Conformance Suite

## 0.8.0

- Added executable DebugEngine tests.
- Added executable DiagnosticsEngine tests.
- Added Debugger Conformance Suite coverage.
- Added Makefile conformance target.
- Added explicit separation between model/conformance passes and native backend verification.
- Established release language for Modelled → Implemented → Integrated → Verified capability status.

## Test policy

The suite verifies observable debugger behavior and API contracts. It does not claim that native operating-system process control works until a native backend integration test exercises that backend.

Native backend tests must distinguish unavailable platform facilities from actual test failures.

## Coverage layers

1. Core API and event/report behavior.
2. C ABI behavior.
3. C++ and C line-control behavior.
4. Backend capability contracts.
5. DebugEngine contracts.
6. Diagnostics and security-policy contracts.
7. Conformance lifecycle and release-readiness contracts.
8. Native launch/attach/step/breakpoint integration when platform backends are available.
9. Regression fixtures for every confirmed debugger defect.

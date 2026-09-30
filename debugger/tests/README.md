<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">

<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">




# SLeeLa Debugger Test Suite

Version: 0.8.0

## Purpose

This directory contains the executable regression and conformance suite for the SLeeLa debugger.

## Test groups

| Test | Coverage |
|---|---|
| debugger_core_test.cpp | C++ sessions, breakpoints, watchpoints, events, reports, removal |
| debugger_line_test.cpp | C++ TRACE, STOP, EXCEPTION line control |
| debugger_backend_test.cpp | Backend kind, capabilities, and unsupported portable operations |
| debugger_c_test.c | C ABI sessions, breakpoints, watchpoints, events, reports |
| debugger_line_c_test.c | C line control and voice-command parser |
| debug_engine_test.cpp | DebugEngine breakpoint, watchpoint, thread, symbol, memory, register, exception, evidence, and capability contracts |
| debug_diagnostics_test.cpp | Replay, crash, sanitizer, memory, profiling, coverage, locks, artifacts, and security policy |
| debug_conformance_test.cpp | Model → Implement → Integrate → Verify conformance records and release readiness |

## Running

From debugger/: `make test`

Conformance only: `make conformance`

Or: `./tests/run-tests.sh`

The Makefile uses C++17 for C++ tests. The runner uses C11 for C tests.

## Native backend policy

Portable/model and conformance tests must pass without an operating-system debugger facility. Native Linux, macOS, and Windows tests must be capability-aware integration suites.

A conformance pass does not certify native execution. Native capabilities are advertised only after the corresponding backend has been exercised and verified with platform-specific evidence.

A skipped or unavailable native facility is not equivalent to a successful native debugger operation.

## Regression and release rule

Every confirmed debugger defect should receive a deterministic test fixture where practical. Tests should verify observable behavior rather than implementation-private details.

The Debugger Conformance Suite is the release gate for individual capabilities: Modelled → Implemented → Integrated → Verified. A model-level pass cannot be represented as native backend verification.
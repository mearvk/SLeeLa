# SLeeLa Debugger Test Suite

Version: 0.3.0

## Purpose

This directory contains the executable regression suite for the SLeeLa debugger foundation.

## Test groups

| Test | Coverage |
|---|---|
| debugger_core_test.cpp | C++ sessions, breakpoints, watchpoints, events, reports, removal |
| debugger_line_test.cpp | C++ TRACE, STOP, EXCEPTION line control |
| debugger_backend_test.cpp | Backend kind, capabilities, and unsupported portable operations |
| debugger_c_test.c | C ABI sessions, breakpoints, watchpoints, events, reports |
| debugger_line_c_test.c | C line control and voice-command parser |

## Running

From debugger/: make test

Or: ./tests/run-tests.sh

The runner uses C11 for C tests and C++17 for C++ tests.

## Native backend policy

Portable-backend tests must pass without an operating-system debugger facility. Native Linux, macOS, and Windows tests should be added as capability-aware integration suites when those backends are implemented.

A skipped or unavailable native facility is not equivalent to a successful native debugger operation.

## Regression rule

Every confirmed debugger defect should receive a deterministic test fixture where practical. Tests should verify observable behavior rather than implementation-private details.

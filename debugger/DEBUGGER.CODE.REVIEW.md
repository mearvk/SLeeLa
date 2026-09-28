# SLeeLa Debugger — C/C++ Structural and Methodological Review

Review version: 0.6.0
Review date: 2026-09-27
Scope: debugger C/C++ sources, line control, actions, platform backends, tests, build files, and architecture/roadmap documentation.

## Current inventory

The debugger contains a C++17 diagnostic core, C11+ ABI, C/C++ line control, voice-command parsing, typed actions, backend capability reporting, platform adapter boundaries, Linux initial process control, and the Second Arrangement evidence architecture.

## Native status

- Linux: initial ptrace launch, attach, resume, single-step, and wait/poll foundation.
- macOS: LLDB adapter boundary; native process integration remains required.
- Windows: Windows Debug API adapter boundary; native process integration remains required.
- Other platforms: portable backend with explicit unsupported behavior.

## Major remaining implementation

Native breakpoints/watchpoints; registers; memory; threads; stack unwinding; exceptions/signals; modules; DWARF/PDB symbols; source mapping; expression/value evaluation; crash/reproduction; record/replay; sanitizer adapters; test integration; DAP; terminal UI; sessions/profiles; plugins; HTTP/server correlation; and security controls.

## Architectural rule

Interfaces must not claim native capability merely because an adapter or schema exists. Capability reports must match executable behavior.

## Evidence rule

Observed target facts, debugger actions, backend results, and derived diagnostic interpretations must remain distinguishable.

## Testing

The test suite must grow from structural/API tests into capability-aware native integration, symbol/source mapping, breakpoint/watchpoint, register/memory, thread/stack, crash/reproduction, sanitizer, HTTP correlation, session, DAP, and security tests.

## Completion standard

A subsystem is complete only when implementation, capability reporting, deterministic tests, failure diagnostics, and documentation agree.

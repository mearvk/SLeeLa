# SLeeLa Debugger — Remaining Work

Version: 0.6.0
Date: 2026-09-27

This document is the implementation roadmap following the Second Arrangement. It separates debugger architecture from native capabilities that still require platform-specific engineering.

## 1. Native Breakpoint Engine
- Software breakpoints.
- Hardware breakpoints and watchpoints.
- Conditional and one-shot breakpoints.
- Function-entry and function-exit breakpoints.
- Per-thread breakpoint state.
- Breakpoint restoration across process events.

## 2. Native Process Backends
### Linux
Complete ptrace support for registers, memory, threads, stack unwinding, signals, fork/clone/exec tracking, module loading, breakpoints, and source mapping.

### macOS
Complete LLDB process integration for launch, attach, execution control, threads, frames, registers, memory, breakpoints, exceptions, modules, symbols, and source mapping.

### Windows
Complete Windows Debug API integration for process/thread events, CONTEXT registers, memory, breakpoints, exceptions, DLL events, symbols, and PDB/source mapping.

## 3. Symbols and Source Mapping
Provide a common symbol service capable of translating address -> module -> function -> source file -> line -> column, with DWARF and PDB support and explicit unavailable-symbol states.

## 4. Expression and Value Engine
Provide a safe debugger expression parser and evaluator for variables, pointers, members, arrays, arithmetic, types, and debugger-provided pseudo-values. Expressions must never become shell commands.

## 5. Threads, Stacks, and Memory
Implement thread enumeration/selection, thread state, thread-local information, real stack unwinding, memory maps, protected-memory handling, read/write operations, and optional memory snapshots.

## 6. Exceptions and Signals
Normalize native signals, exceptions, faults, assertions, sanitizer failures, aborts, and termination while preserving original platform evidence.

## 7. Crash and Reproduction
Generate deterministic crash packages and reusable reproduction sessions containing executable/build identity, modules, arguments, environment policy, stop reason, thread state, stacks, registers, memory evidence, events, and hashes.

## 8. Record/Replay
Extend timeline history toward checkpoints and replay. Reverse execution must be represented as a backend capability and must not be implied by ordinary event history.

## 9. Build Identity
Verify source, binary, symbols, compiler, linker, architecture, operating system, build ID, and debugger version. Detect source/binary/symbol mismatches.

## 10. Sanitizers
Normalize ASan, UBSan, TSan, and LSan reports into debugger events and evidence records.

## 11. Test Integration
Allow failed tests to create debugger reproduction sessions, stop at failure, capture evidence, and produce deterministic regression fixtures.

## 12. HTTP and Server Debugging
Add domain events for connections, sockets, packets, HTTP parsing, requests, routing, handlers, responses, timeouts, protocol violations, and server exceptions. Correlate debugger, HTTP, logs, and tests by stable IDs.

## 13. DAP and Terminal UI
Implement a real DAP adapter and a terminal debugger interface using the same typed action and event models.

## 14. Sessions and Profiles
Implement versioned .sldebug session files and reusable development, server, HTTP, memory, thread, crash, and production-diagnostics profiles.

## 15. Plugin ABI
Define a stable plugin ABI for event decoders, inspectors, source mappings, protocol decoders, visualizers, specialized breakpoints, and sanitizer adapters.

## 16. Security
Define authorization for attach, privilege boundaries, untrusted target memory handling, hostile symbols/crash artifacts, plugin trust, and safe diagnostic-package extraction.

## Completion gates
A feature is not considered complete merely because an interface exists. It requires implementation, capability reporting, deterministic tests, failure diagnostics, documentation, and platform-specific integration where applicable.

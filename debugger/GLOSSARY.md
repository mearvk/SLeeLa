# SLeeLa Debugger Glossary

## Core
- DebugSession — authoritative debugger session and event boundary.
- DebugEngine — reusable debugger-domain state layer.
- DebugEvent — normalized debugger event.
- Capability Truth — distinction between unavailable, declared, implemented and tested functionality.
- Stop Record — normalized explanation of why execution stopped.
- Evidence Bundle — reproducible diagnostic context for an incident.

## Execution
- Breakpoint — execution stop condition.
- Watchpoint — data-access condition observing memory or an expression.
- Hardware Breakpoint — processor/OS debug-facility breakpoint.
- Temporary Breakpoint — breakpoint disabled after its configured hit.
- Reverse Debugging — moving through recorded execution toward an earlier state.
- Checkpoint — named execution-state reference.
- Replay — reconstruction of recorded execution.
- Deterministic Replay — replay designed to reproduce relevant execution state.

## Program State
- Stack Frame — one invocation in a call stack.
- Symbol — named program entity associated with an address or source location.
- Source Mapping — relationship between machine addresses and source code.
- DWARF — debugging information format commonly used with ELF toolchains.
- PDB — Microsoft debugging-symbol database format.
- Register Snapshot — captured processor register state.
- Memory Map — process address-space regions and permissions.

## Diagnostics
- Crash Dump — persisted evidence from a process failure.
- Crash Fingerprint — stable grouping identity for materially similar failures.
- Sanitizer Finding — ASan, UBSan, TSan or related diagnostic.
- Memory Diagnostic — evidence about leaks, invalid access, double frees and related faults.
- Profiler Sample — sampled execution evidence for hot functions or threads.
- Coverage Record — measured line, function or branch execution evidence.
- Lock Record — evidence about lock ownership or waiting.
- Deadlock Detection — identification of synchronization wait cycles.

## Interfaces
- DAP — Debug Adapter Protocol.
- Terminal UI — interactive text debugger interface.
- ActionController — validates debugger actions before backend execution.
- HTTP Debugging — debugger integration through SLeeLa HTTP architecture.

## Artifacts and Security
- Debug Artifact — portable persisted debugging session and evidence package.
- .sleela-debug — proposed directory format for persisted debugger artifacts.
- Expression Engine — bounded debugger-expression evaluator.
- Security Policy — authorization boundary for attach, memory writes and expression execution.
- Audit Log — record of security-sensitive debugger actions.
- Readiness Report — generated assessment of capability states and test evidence.

## Platform Integration
- ptrace — Linux process-tracing facility.
- LLDB — debugger technology targeted by the macOS adapter.
- Windows Debug API — Windows process-debugging interface.
- ELF — executable/object format commonly used on Linux.
- Mach-O — executable/object format used by macOS.
- PE — Portable Executable format used by Windows.

## Development Status
- Modelled — data structure/API exists.
- Implemented — behavior exists in source.
- Integrated — connected to a backend or SLeeLa subsystem.
- Tested — executable test demonstrates the behavior.
- Production-ready — project-defined release gate is satisfied; it is not inferred from compilation alone.

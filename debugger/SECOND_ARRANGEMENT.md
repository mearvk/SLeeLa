# SLeeLa Debugger — Second Arrangement

Version: 0.5.0

The Second Arrangement expands the debugger from a process-control foundation into a unified development, testing, observability, and diagnostic system.

## 1. Unified Event Bus

All debugger-relevant activity should become typed events, including process, thread, breakpoint, watchpoint, exception, signal, source-line, function entry/exit, assertion, sanitizer, memory fault, crash, pause, and resume.

## 2. Timeline and History

Maintain ordered execution evidence so developers can inspect what happened before a stop or failure. Event history is not reverse execution; reverse execution requires separate backend support.

## 3. Why Did I Stop?

Every stop should have a structured explanation containing stop reason, breakpoint/watchpoint, source location, function, thread, condition, exception/signal, previous relevant event, and captured evidence where available.

## 4. Crash Evidence

A crash package should be able to contain SUMMARY.md, EVENTS.log, STACK.txt, THREADS.txt, REGISTERS.txt, MEMORY.txt, SOURCE.txt, ENVIRONMENT.txt, MODULES.txt, and HASHES.txt.

## 5. Deterministic Sessions

A session file should preserve executable, arguments, debugging policy, breakpoints, watchpoints, source mappings, exception filters, thread filters, trace configuration, logging configuration, and backend selection.

## 6. Build Identity

Record source and binary SHA-256 values, compiler/toolchain, linker, debug-symbol format, build ID, architecture, OS, and debugger version. Warn when source and binary identity do not match.

## 7. Test Integration

A failed test should be able to hand its failure context to the debugger for reproduction, stopping, evidence capture, and regression-fixture creation.

## 8. Sanitizers

Normalize sanitizer diagnostics into the debugger event model.

## 9. Profiles

Support reusable profiles such as development, server, HTTP, memory, thread, crash, and production-diagnostics.

## 10. IDE and Terminal

Provide a DAP-facing architecture for IDE integration and a dedicated terminal debugger interface.

## 11. Voice Safety

Voice commands remain debugger commands. Destructive operations should support explicit confirmation.

## 12. Plugins

Domain-specific plugins may contribute event decoders, inspectors, source mappings, protocol decoders, visualizations, and specialized breakpoints.

## 13. Debug Evidence

A machine-readable evidence chain connects:

Observation -> Event -> Source -> Thread -> Stack -> Registers/Memory -> Action -> Backend Result -> Diagnostic.

The evidence chain must distinguish observed facts from interpretation.

## Development sequence

0.5: event bus, evidence, timeline, crash diagnostics, session identity.
0.6: DAP, terminal UI, session files, richer native backends.
1.0: production-quality cross-platform debugger contract and native backend coverage.

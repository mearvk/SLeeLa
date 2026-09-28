# SLeeLa Debugger Version

Version: 0.5.0
Date: 2026-09-27
Language implementation: C++17
C implementation: C11+
Line-control API: C and C++
Backend abstraction: C++
Action model: C++

## 0.5.0 — Second Arrangement

This arrangement expands the debugger from process-control foundations into an integrated development and diagnostic instrument.

- Defined a unified debugger event bus covering process, thread, breakpoint, watchpoint, exception, signal, source-line, function, assertion, sanitizer, memory-fault, crash, pause, and resume events.
- Defined structured execution history and timeline concepts.
- Added the "Why Did I Stop?" diagnostic concept for evidence-based stop explanations.
- Defined automatic crash-evidence packages.
- Defined deterministic debug-session files and reusable debugging profiles.
- Defined source/build identity tracking using binary and source hashes, compiler/toolchain information, symbols, architecture, OS, and debugger version.
- Defined debugger-to-test-suite failure handoff and regression-fixture generation.
- Defined sanitizer integration for AddressSanitizer, UndefinedBehaviorSanitizer, ThreadSanitizer, and LeakSanitizer.
- Defined IDE integration through the Debug Adapter Protocol (DAP).
- Defined a dedicated terminal debugger UI.
- Defined safe voice confirmation for potentially destructive actions.
- Defined a debugger plugin architecture for domain-specific extensions.
- Defined machine-readable Debug Evidence chains connecting observations, events, source, threads, stacks, registers/memory, actions, backend results, and diagnostics.
- Established the 0.5 development target around native execution, evidence, timeline, and crash diagnostics.

## 0.4.0

- Added the typed DebugAction model and explicit action lifecycle.
- Added the Linux ptrace backend adapter with launch, attach, resume, single-step, wait/poll, and capability reporting.
- Added macOS LLDB backend adapter architecture.
- Added Windows Debug API backend adapter architecture.
- Added platform backend selection and portable fallback.
- Integrated platform backend sources into the debugger build.
- Preserved capability-aware behavior.

## Versioning policy

Use semantic versioning. Native backend capabilities and externally consumed diagnostic schemas must document their capability and schema versions independently.

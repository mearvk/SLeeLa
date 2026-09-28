# SLeeLa Debugger Version

Version: 0.6.0
Date: 2026-09-27
Language implementation: C++17
C implementation: C11+
Line-control API: C and C++
Backend abstraction: C++
Action model: C++

## 0.6.0 — Native and Reproduction Roadmap

The debugger documentation now defines the complete next-stage implementation program.

- Native breakpoint and watchpoint engine.
- Complete Linux ptrace capability roadmap.
- Native macOS LLDB integration roadmap.
- Native Windows Debug API integration roadmap.
- DWARF/PDB symbol and source-mapping engine.
- Safe expression/value evaluation.
- Real thread, stack, register, and memory inspection.
- Native exception and signal normalization.
- Crash packaging and deterministic reproduction.
- Record/checkpoint/replay architecture.
- Source, binary, symbol, and build identity verification.
- ASan, UBSan, TSan, and LSan integration.
- Debugger-to-test-suite reproduction and regression fixtures.
- HTTP/server-aware debugging and correlated timelines.
- DAP and terminal debugger implementation roadmap.
- Versioned .sldebug sessions and debugging profiles.
- Stable plugin ABI.
- Debugger security and trust boundaries.
- Completion gates requiring implementation, truthful capability reporting, tests, diagnostics, and documentation.

See REMAINING_WORK.md, NATIVE_BACKEND_PLAN.md, DEBUGGER.ARCHITECTURE.md, and HTTP_DEBUGGING.md.

## 0.5.0 — Second Arrangement

- Unified event bus architecture.
- Execution history and timeline.
- Why Did I Stop? evidence model.
- Crash evidence package design.
- Deterministic debug sessions.
- Build identity tracking.
- Test-suite integration.
- Sanitizer integration architecture.
- DAP and terminal UI architecture.
- Voice safety.
- Plugin architecture.
- Machine-readable Debug Evidence chain.

## Versioning policy

Use semantic versioning. Native backend capabilities and externally consumed diagnostic schemas must document their capability and schema versions independently.

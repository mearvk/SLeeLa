# SLeeLa Debugger Version

Version: 0.8.0
Date: 2026-09-27
Language implementation: C++17
C implementation: C11+
Line-control API: C and C++
Backend abstraction: C++
Action model: C++

## 0.8.0 — Debugger Conformance and Native Execution

- Machine-checkable Model → Implement → Integrate → Verify conformance lifecycle.
- Debugger Conformance Suite and release-readiness reporting.
- Versioned .sleela-debug session/artifact contract.
- Explicit debugger command-language boundary.
- Security boundary for attach, memory writes, expression execution, plugins, paths, resources, and audit.
- Native execution integration contract for Linux ptrace, macOS LLDB, and Windows Debug API.
- Capability truth remains distinct from model-level availability.
- Overall engineering quality assessment: **82/100**.

### 0.8.0 Quality Assessment

| Area | Assessment |
|---|---:|
| Architecture & separation of concerns | 90/100 |
| API/data-model design | 87/100 |
| Documentation | 91/100 |
| Testing & conformance framework | 84/100 |
| Diagnostics/evidence model | 86/100 |
| Security model | 83/100 |
| C/C++ integration | 82/100 |
| Native debugging implementation | 63/100 |
| Symbol/source integration | 60/100 |
| Replay/reverse debugging | 55/100 |
| DAP integration | 55/100 |
| Production readiness | 70/100 |
| **Overall quality** | **82/100** |

The assessment is an engineering snapshot, not a certification or guarantee of production readiness. See DEBUGGER.QUALITY.md for interpretation and remaining gaps.

## 0.7.0 — Four-Stage Debugger Contract

- Model → Implement → Integrate → Verify lifecycle.
- DebugEngine and DiagnosticsEngine expansion.
- Execution recording/checkpoint metadata.
- Crash, sanitizer, memory, profiling, coverage and synchronization diagnostics.
- Persistent `.sleela-debug` artifact model.
- Explicit debugger security authorization boundary.
- Debugger glossary and completion contract.

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

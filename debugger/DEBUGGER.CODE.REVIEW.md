# SLeeLa Debugger — C/C++ Structural and Methodological Review

Review version: 0.2.0
Review date: 2026-09-27
Scope: debugger C++ sources, debugger tests, build file, and documented architecture.

## 1. Inventory

The current debugger is a C++17 diagnostic core consisting of debugger.hpp, debugger.cpp, main.cpp, the debugger core regression test, the test runner, and the Makefile.

There are currently no C source files in /debugger. This review therefore evaluates the existing C++ implementation directly and records the methodology required for a future C ABI or C implementation. It does not represent a nonexistent C implementation as complete.

## 2. Structural review

The public header contains the domain model: EventType, SourceLocation, Breakpoint, Watchpoint, StackFrame, DebugEvent, DiagnosticReport, and DebugSession.

The implementation keeps platform-specific debugger APIs outside the public model. This is the correct architectural boundary for later GDB, LLDB, WinDbg, ptrace, Mach, and Windows-native adapters.

Ownership is straightforward: STL value types own breakpoint, watchpoint, event, and string data. The core contains no raw owning pointers.

Breakpoint and watchpoint IDs share one monotonically increasing session-local counter. This provides unique identifiers across both categories during a session. The contract should remain explicitly session-local until persistent identifiers are required.

Events are appended to a vector, preserving emission order. Event fields use a map, giving stable key ordering during report generation.

## 3. C++ implementation review

Strengths:

- C++17 is explicit.
- The core is compact and readable.
- Ownership is value-based.
- Event chronology is preserved.
- Platform debugger dependencies are not required to compile the core.
- Removal operations report whether an object existed.
- The report formatter is deterministic for equivalent input.

Corrections and hardening:

1. Standard-library dependencies should be explicit. Any symbol such as move should have its direct standard header included rather than relying on transitive includes.
2. Every EventType must have formatter test coverage whenever the enum changes.
3. The diagnostic report format needs an explicit schema version before external automation depends on its exact text.
4. The executable self-test currently uses an annotation-specific example. That is useful integration evidence but should not become the generic debugger core's only self-test.
5. The Makefile should expose one canonical test target that runs both executable self-tests and debugger-core regression tests.

## 4. Methodological review

The debugger is an evidence-producing diagnostic instrument, not a second test framework.

Recommended lifecycle:

1. reproduce;
2. record exact build identity;
3. record target and arguments;
4. establish backend capability;
5. stop at a reproducible boundary;
6. capture source and execution context;
7. capture thread and stack state;
8. correlate with tests and function coverage;
9. produce a deterministic report;
10. turn the failure into a regression test.

The system must distinguish:

- requested operation;
- backend capability;
- operation accepted;
- operation actually observed;
- diagnostic interpretation;
- test result.

This prevents unsupported debugger operations from being mistaken for successful diagnostics.

## 5. C methodology

No C implementation currently exists.

If a C interface is added, it should expose an opaque-handle ABI rather than duplicate the C++ engine. Suitable concepts include sleela_debug_session_t, sleela_debug_event_t, and sleela_debug_breakpoint_t.

The C ABI should use explicit create/destroy functions, explicit result codes, caller-owned output buffers, and no STL types or C++ exceptions across the ABI boundary.

The C layer should call the same backend-neutral diagnostic core.

## 6. Backend methodology

The core should remain independent from GDB, LLDB, WinDbg, Linux ptrace, macOS Mach task APIs, and Windows debugging APIs.

Backend adapters should translate native observations into the common event model.

A backend must explicitly report unsupported capabilities. It must never fabricate a successful breakpoint, memory read, register read, watchpoint, or stack capture.

## 7. Security and operational review

Process attachment is privileged diagnostic activity.

The debugger should rely on operating-system access controls, avoid privilege escalation, record attach/launch intent, avoid silently modifying target files, treat target arguments as untrusted input, and distinguish diagnostic output from executable input.

## 8. Test methodology

Current core coverage verifies breakpoint creation, watchpoint creation, event emission, report generation, field preservation, removal, and removal of an already-removed object.

Next test layers:

### Unit
Every event formatter, defaults, duplicate/removal behavior, multiple sessions, large event streams, and report ordering.

### Behavioral
Breakpoint and watchpoint lifecycle, source locations, stack frames, exception/crash events, sanitizer events, and test/coverage correlation.

### Backend
GDB, LLDB, and WinDbg launch/attach behavior plus unavailable-backend behavior.

### Cross-platform
Linux, macOS, and Windows.

### Regression
Each debugger defect should become a deterministic regression fixture where practical.

## 9. Current-state conclusion

The current debugger is a coherent C++ diagnostic foundation, not yet a complete native debugger.

Current state:

- C++ core: implemented foundation.
- C implementation: not present.
- Native backend process control: architectural boundary established; native adapters remain.
- Deterministic diagnostic reporting: foundation present.
- Test integration: foundation present; expansion required.
- Production debugger: not yet complete.

The strongest architectural decision is separation of the diagnostic event/session model from platform debugging APIs.

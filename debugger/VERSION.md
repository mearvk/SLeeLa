# SLeeLa Debugger Version

Version: 0.4.0
Date: 2026-09-27
Language implementation: C++17
C implementation: C11+
Line-control API: C and C++
Backend abstraction: C++
Action model: C++

## 0.4.0

- Added the typed DebugAction model and explicit action lifecycle.
- Added the Linux ptrace backend adapter with launch, attach, resume, single-step, wait/poll, and capability reporting.
- Added macOS LLDB backend adapter architecture with explicit unsupported-operation diagnostics until native LLDB process integration is connected.
- Added Windows Debug API backend adapter architecture with explicit unsupported-operation diagnostics until native process integration is connected.
- Added platform backend selection for Linux, macOS, Windows, and portable fallback.
- Integrated platform backend sources into the debugger build.
- Preserved capability-aware behavior so an adapter never reports an unimplemented operation as successful.

## 0.3.0

- Added the C debugger ABI.
- Added C line-control points.
- Added C++ line-control support.
- Added TRACE, STOP, and EXCEPTION line actions.
- Added voice-command parsing for debugger control.
- Added a backend-neutral C++ debugger interface.
- Added explicit backend capability reporting.
- Added portable unsupported-operation behavior.
- Added documentation for IDE, terminal, and native-backend integration.
- Established the architecture for Linux, macOS, and Windows native backends.

## 0.2.0

- Formalized the C/C++ structural review.
- Documented ownership and identity rules.
- Documented diagnostic evidence methodology.
- Documented backend separation.
- Documented debugger testing methodology.

## Versioning policy

Use semantic versioning. Native backend capabilities and externally consumed diagnostic schemas must document their capability and schema versions independently.

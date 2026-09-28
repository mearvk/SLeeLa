# Debugger Backends

SLeeLa keeps the diagnostic model independent from the operating-system debugger.

## Backend contract

A backend reports:

- availability
- target launch capability
- process attach capability
- breakpoint capability
- watchpoint capability
- stack-trace capability
- register inspection capability
- memory inspection capability
- thread enumeration capability

A capability that is unavailable must be reported as unavailable. It must never be represented as a successful diagnostic.

## Current adapters

| Backend | Platforms | Discovery |
|---|---|---|
| GDB | Linux and other Unix-like systems | command -v gdb |
| LLDB | macOS and other supported systems | command -v lldb |
| WinDbg | Windows | where windbg |

The current process-control layer is deliberately conservative. Native OS adapters can be added behind this interface without changing DebugSession, event types, reports, or test correlation.

## Build-time rule

The core debugger must build without requiring a debugger package to be installed. Host debugger availability is a runtime capability.

# SLeeLa Debugger — Native Backend Completion Plan

Version: 0.6.0

## Common contract

Every native backend must expose truthful capabilities for:

- launch and attach
- continue/pause
- step in/over/out
- software and hardware breakpoints
- watchpoints
- threads
- stack frames
- registers
- memory
- exceptions/signals
- modules
- symbols
- source mapping
- process lifecycle

Unsupported operations must fail explicitly.

## Linux / ptrace

Current foundation: launch, attach, resume, single-step, wait/poll.

Remaining native layers:

1. Breakpoint insertion/removal.
2. Hardware debug registers and watchpoints.
3. Register read/write.
4. Memory read/write.
5. Thread and clone tracking.
6. Signal and ptrace-event handling.
7. Fork/exec/module tracking.
8. Stack unwinding.
9. ELF/DWARF symbol integration.
10. Address-to-source mapping.
11. Robust stop/exit classification.

## macOS / LLDB

Current state: adapter boundary.

Required integration:

1. LLDB session/process lifecycle.
2. Launch and attach.
3. Continue and stepping.
4. Breakpoints/watchpoints.
5. Threads and frames.
6. Registers and memory.
7. Exceptions/signals.
8. Modules and symbols.
9. Source mapping.
10. Capability-aware error translation.

## Windows / Debug API

Current state: adapter boundary.

Required integration:

1. Debug process creation/attachment.
2. WaitForDebugEvent loop.
3. ContinueDebugEvent.
4. Thread/process lifecycle.
5. CONTEXT register access.
6. Memory access.
7. Software/hardware breakpoints.
8. Exception handling.
9. DLL/module events.
10. PDB/symbol/source mapping.

## Native integration test policy

Each platform requires deterministic integration fixtures. An unavailable OS facility is a skip/unavailable result, not a pass for native behavior.

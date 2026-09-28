# SLeeLa Debug Engine Architecture

The Debug Engine is the stable domain layer between the existing DebugSession/event bus and native process-control backends.

It adds breakpoint and watchpoint management, thread state, stack frames, symbols/source mappings, memory helpers, register snapshots, normalized exceptions, bounded expression evaluation, deterministic evidence bundles, and capability truth.

Native backends remain responsible for ptrace, LLDB, Windows Debug API and other operating-system operations. A native backend should update DebugEngine only after the native operation succeeds, then emit the corresponding DebugEvent through DebugSession.

Connection:
SLeeLa target -> native backend -> DebugEngine -> DebugSession/event bus -> Test Suite, reports, UI and HTTP debugging.

The portable engine model never implies that a native platform capability is implemented.
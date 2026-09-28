# SLeeLa Debugger Command Model

The command model is intentionally small and maps to capabilities common to GDB, LLDB, and WinDbg.

- break SYMBOL — create a symbolic breakpoint.
- delete ID — remove a breakpoint or watchpoint.
- watch EXPRESSION — register a watch expression.
- info breakpoints — enumerate breakpoint state.
- run — start the target.
- continue — resume after a stop.
- step — execute one source-level step.
- next — step over the current call.
- backtrace — capture stack frames.
- threads — enumerate threads.
- registers — capture register state where supported.
- memory ADDRESS LENGTH — inspect memory where supported.
- report FILE — write a deterministic diagnostic report.
- quit — end the session.

Commands are requests. The backend records whether each request was accepted, rejected, or unsupported.

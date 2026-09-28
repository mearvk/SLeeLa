# Debugger API

Core types:
- Breakpoint: source/function/location condition.
- Watchpoint: symbolic observation of a value.
- StackFrame: source/function/module/line.
- DebugEvent: breakpoint, watchpoint, exception, assertion, thread, log, coverage, sanitizer, test, regression, or crash.
- DebugSession: event bus plus breakpoint/watchpoint registries.
- DiagnosticReport: deterministic human-readable output.

The API is backend-neutral so OS-specific process control can be added without changing the diagnostic model.

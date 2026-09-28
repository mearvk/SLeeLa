# SLeeLa Debugger — Line Control and Voice

## Purpose

The debugger now defines a source-line control layer so every instrumented source line can become a debugger/exception point.

A line point is not necessarily a native OS breakpoint. It is a **logical control point** that can be compiled into the program, consumed by the debugger, and later bound to a native backend.

### Three line actions

- **TRACE** — record that execution reached the line.
- **STOP** — request debugger suspension at the line.
- **EXCEPTION** — record an exception-class stop at the line.

This provides fine-grained software control without requiring a permanent native hardware/software breakpoint on every line.

## Voice control

The C layer includes a conservative command parser for debugger voice control:

- `continue` / `resume`
- `step` / `step into`
- `next` / `step over`
- `stop here` / `break here`
- `trace line` / `trace lines`

A speech-recognition front end should convert audio to text; the debugger only interprets the resulting command. Voice input must never be treated as target-program input or arbitrary shell input.

## Instrumentation model

Compiler-generated or manually inserted calls can use:

`sleela_debugger_hit_line(session, &point, thread, function)`

A future compiler/LLVM/GCC/MSVC instrumentation adapter can generate these calls automatically from source/debug information.

## Performance

Instrumenting literally every source line can be expensive. The recommended modes are:

1. **OFF** — no line instrumentation.
2. **TRACE** — low-cost event recording.
3. **STOP-ALL** — every instrumented line is a stop point.
4. **FOCUS** — instrument all lines but only stop on selected files/functions/ranges.
5. **EXCEPTION** — line events are promoted to exception points under configured conditions.

Native backends should use ordinary source mappings and debugger facilities when available rather than forcing a native breakpoint instruction onto every line.

## Security

Voice commands are debugger control commands only. They must not execute arbitrary shell commands, modify target memory, or elevate privileges. Native process control remains subject to normal OS permissions.

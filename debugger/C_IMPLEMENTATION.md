# SLeeLa Debugger C Implementation

The C implementation provides a C11+ ABI over the debugger's diagnostic model.

## Implemented scope

- opaque debugger sessions;
- breakpoints;
- watchpoints;
- diagnostic events;
- deterministic reports;
- explicit result codes;
- source-line control points;
- TRACE, STOP, and EXCEPTION line actions;
- voice-command parsing;
- no C++ types or exceptions across the ABI.

## Line-level control

Compiler instrumentation or manually instrumented code can notify the debugger when execution reaches a source line. The event identifies file, line, column, thread, and function where available.

A logical STOP or EXCEPTION point is not by itself a native process suspension. Native backend confirmation is required before claiming that execution actually stopped.

## Voice

Voice recognition is intentionally outside this C library. The expected pipeline is:

speech -> speech-to-text -> debugger command parser -> backend.

Only debugger commands are accepted. Arbitrary shell execution is outside the interface.

## Backend integration

The C ABI remains backend-neutral. Native backends should provide launch/attach, resume, stepping, breakpoint binding, thread and stack inspection, registers, memory, exceptions, source mapping, and capability reporting.

## Build

Compile the C implementation as C11 or later using the standard C library. The C tests should be included in the debugger's canonical test target.

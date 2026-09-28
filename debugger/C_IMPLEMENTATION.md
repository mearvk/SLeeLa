# SLeeLa Debugger C Implementation

The C implementation provides a stable C ABI over the debugger's core diagnostic concepts.

## Scope

This layer provides:

- opaque debugger sessions;
- breakpoints;
- watchpoints;
- diagnostic events;
- deterministic text reports;
- explicit result codes;
- no C++ types or exceptions in the ABI.

It is intentionally backend-neutral.

## IDE and terminal operation

The C ABI is the programmatic diagnostic layer. Interactive stopping in an IDE or terminal requires a native backend adapter.

The intended architecture is:

C/C++ target -> SLeeLa debugger session -> backend adapter -> native debugger/process-control facility -> IDE/terminal presentation.

The next backend stage should provide launch/attach, continue, step, breakpoint binding, thread enumeration, stack capture, registers, memory access, exception/crash events, and capability reporting for Linux, macOS, and Windows.

## Safety contract

The C API does not claim to control a process merely because a breakpoint was registered in the session. Native backend success must be reported separately from request creation.

## Build

Compile the C implementation as C11 or later. It uses only the C standard library.

The test should be run through the debugger test target once integrated into the top-level build.

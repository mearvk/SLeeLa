# SLeeLa Debugger — C/C++ Structural and Methodological Review

Review version: 0.3.0
Review date: 2026-09-27
Scope: debugger C/C++ sources, line-control layer, backend abstraction, tests, build files, and architecture documentation.

## Current inventory

The debugger now contains a C++17 diagnostic core, a C11+ ABI, C and C++ source-line control APIs, voice-command parsing, and a backend-neutral C++ interface.

The architecture separates logical debugger events from native process-control APIs.

## Line control

A source line may be represented by a logical control point:

- TRACE — record execution reaching the line.
- STOP — request debugger suspension.
- EXCEPTION — record an exception-class stop.

This supports fine-grained source observability. It does not imply that every line receives a permanent hardware breakpoint. Native backends should use the most appropriate source mapping, single-step, instrumentation, or breakpoint mechanism.

## Voice control

Voice is an input modality for debugger commands. Speech recognition should produce text; the debugger accepts only defined debugger commands. Voice input must not be interpreted as arbitrary shell input or target-program input.

Supported command concepts include continue, step, step over, stop here, and trace line.

## Backend architecture

The C++ backend interface now defines the boundary for:

- launch;
- attach;
- resume;
- stepping;
- source-line stop binding;
- event polling;
- capability reporting.

The portable backend intentionally reports unsupported native operations instead of fabricating success.

Native adapters remain required for Linux, macOS, and Windows process control.

## C ABI review

The C layer uses opaque sessions, explicit result codes, caller-owned input/output contracts, and no C++ exceptions or STL types across the ABI.

C and C++ line-control APIs should remain behaviorally aligned.

## Methodology

The debugger must distinguish:

1. requested operation;
2. backend capability;
3. operation accepted;
4. operation actually observed;
5. diagnostic interpretation;
6. test result.

This distinction is especially important when a line point is registered but a native backend cannot bind it.

## Security

Process attachment remains subject to operating-system authorization. Voice commands must not provide a privilege-escalation path. Target arguments, source data, and debugger observations should be treated as untrusted diagnostic data.

## Testing

Required layers now include:

- C API unit tests;
- C++ core tests;
- line-point tests;
- voice-command parser tests;
- backend capability tests;
- native backend integration tests;
- Linux/macOS/Windows tests;
- regression fixtures;
- IDE/terminal protocol tests.

## Current state

C and C++ logical debugger control is implemented. The native backend interface is implemented. Native OS process-control adapters and an IDE-facing protocol remain the next implementation stage.

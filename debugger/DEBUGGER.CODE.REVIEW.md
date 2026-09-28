# SLeeLa Debugger — C/C++ Structural and Methodological Review

Review version: 0.4.0
Review date: 2026-09-27
Scope: debugger C/C++ sources, line-control layer, action model, platform backends, tests, build files, and architecture documentation.

## Current inventory

The debugger contains a C++17 diagnostic core, C11+ ABI, C/C++ line control, voice-command parsing, typed debugger actions, backend capability reporting, and platform-specific backend adapters.

## Platform backend status

- Linux: initial ptrace process-control implementation for launch, attach, resume, single-step, and wait/poll.
- macOS: LLDB adapter boundary established; native process integration remains to be connected.
- Windows: Windows Debug API adapter boundary established; native process integration remains to be connected.
- Other platforms: portable backend with explicit unsupported-operation behavior.

The implementation deliberately distinguishes an adapter's identity from the operations it can actually execute.

## Action model

Actions progress through requested, validated, supported, executed, completed, or failed states.

## Line control

TRACE, STOP, and EXCEPTION remain logical source-line controls. Native source-line binding requires source-to-instruction mapping and must not be implied merely by registration.

## Voice control

Voice is restricted to defined debugger commands and cannot become arbitrary shell or target-program input.

## Testing

Required layers include C/C++ API tests, line control, voice parsing, action validation, backend capability tests, and native integration tests for each supported operating system.

## Security

OS authorization governs process attachment. Debugger inputs and target observations are untrusted diagnostic data.

## Current state

The cross-platform backend architecture is implemented. Linux has initial native process control. macOS and Windows have explicit native adapter boundaries but still require their platform process-control implementations. Source mapping, hardware/software breakpoints, registers, memory, threads, stack unwinding, and rich exception handling remain native-backend work.

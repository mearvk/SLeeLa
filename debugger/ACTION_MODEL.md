# SLeeLa Debugger Action Model

Version: 0.4.0

Every debugger operation is represented as a typed action before backend execution.

## Actions

Continue, pause, step-in, step-over, step-out, break, conditional break, one-shot break, watch, exception, thread, stack, memory, register, inspect, trace, history, attach, detach, launch, restart, terminate, and report.

## Lifecycle

requested -> validated -> supported -> executed -> completed

Failure may occur at any stage after request.

A command being accepted does not imply that the target process performed it.

## Platform mapping

### Linux
The ptrace adapter currently provides the initial native foundation for launch, attach, resume, single-step, process waiting/polling, and native capability reporting. Source-line binding remains dependent on symbol/instruction mapping.

### macOS
The LLDB adapter establishes the platform boundary and reports unsupported operations until native LLDB process control is connected.

### Windows
The Windows Debug API adapter establishes the platform boundary and reports unsupported operations until native process control is connected.

### Portable
The portable backend remains an explicit no-native-process-control implementation.

## Capability rule

Backends must advertise only capabilities they can actually perform. Unsupported operations must return failure with an actionable diagnostic.

## Safety

Voice and IDE command layers may submit debugger actions only. They must not turn debugger arguments into arbitrary shell execution or target-program input.

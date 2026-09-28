# SLeeLa Debugger Action Model

Version: 0.4.0

Every debugger operation is represented as a typed action before backend execution.

Actions include continue, pause, step-in, step-over, step-out, break, conditional break, one-shot break, watch, exception, thread, stack, memory, register, inspect, trace, history, attach, detach, launch, restart, terminate, and report.

## Lifecycle

An action progresses through explicit states:

1. requested
2. validated
3. supported
4. executed
5. completed
6. failed

A command being accepted does not imply that the target process performed it. Native backend capability and execution results remain separate.

## Native integration

Future Linux, macOS, and Windows backends must map actions to platform operations and report unsupported operations explicitly.

## Safety

Voice and IDE command layers may submit debugger actions only. They must not turn debugger arguments into arbitrary shell execution or target-program input.

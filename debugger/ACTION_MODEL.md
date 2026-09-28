# SLeeLa Debugger Action Model

Version: 0.5.0

The action model remains the command boundary for every debugger interface.

## Actions

Continue, pause, step-in, step-over, step-out, break, conditional break, one-shot break, watch, exception, thread, stack, memory, register, inspect, trace, history, attach, detach, launch, restart, terminate, and report.

## Lifecycle

requested -> validated -> supported -> executed -> completed

Failure may occur at any stage after request.

## Second Arrangement extensions

Actions should emit structured events and contribute to a Debug Evidence chain. Stop-producing actions should expose a machine-readable "why did I stop?" record.

Potentially destructive actions such as terminate and restart should support explicit confirmation in voice interfaces.

## Platform mapping

Linux currently has the initial ptrace process-control foundation. macOS has the LLDB adapter boundary. Windows has the Windows Debug API adapter boundary. Portable remains an explicit no-native-process-control backend.

## Capability rule

Backends advertise only operations they can actually perform. Unsupported operations return failure with an actionable diagnostic.

## Interface rule

Voice, terminal, IDE/DAP, test integration, and future plugins should submit the same typed actions rather than maintaining independent debugger semantics.

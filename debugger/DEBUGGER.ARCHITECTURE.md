# SLeeLa Debugger Architecture

Version: 0.6.0

## System model

```
Interface
  |-- Terminal
  |-- Voice
  |-- IDE / DAP
  |-- Test Suite
  |-- Plugins
        |
        v
Typed Action Controller
        |
        v
Debugger Session
        |
   +----+----+
   |         |
Event Bus   Evidence Chain
   |         |
   +----+----+
        |
        v
Native Backend
   |-- Linux ptrace
   |-- macOS LLDB
   |-- Windows Debug API
   |-- Portable
        |
        v
Target Process
```

## Evidence model

Every significant observation should be correlatable through:

Observation -> Event -> Source -> Thread -> Stack -> Registers/Memory -> Action -> Backend Result -> Diagnostic.

Observed facts and derived interpretations must remain distinguishable.

## Stop model

A stop record should identify:

- reason
- process/thread
- source location
- function
- breakpoint/watchpoint
- exception/signal
- triggering action
- previous relevant events
- available evidence

## Identity model

A debug session should track:

- executable hash
- source identity
- symbol identity
- compiler/toolchain
- linker
- build ID
- architecture
- OS
- debugger version

## Domain integration

The architecture permits SLeeLa-specific observers for HTTP/server traffic, tests, sanitizers, logs, and other diagnostic domains without changing native backend semantics.

## Security boundary

Debugger commands are typed actions. They are never interpreted as arbitrary shell input or target-program input.

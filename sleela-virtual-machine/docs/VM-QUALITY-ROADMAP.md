# SLVM Quality Roadmap

The VM is being designed for production-grade language/runtime breadth from the beginning.

## Quality Areas

### 1. Execution Correctness

- deterministic instruction semantics
- bounds checking
- malformed-bytecode rejection
- explicit integer overflow policy
- division-by-zero handling
- invalid opcode handling
- controlled stack growth
- call-frame validation
- exception propagation

### 2. Memory Safety

- no unchecked VM-owned pointer exposure
- opaque resource handles
- ownership/lifetime rules
- GC integration points
- native allocation accounting
- cleanup on exceptional paths
- resource quotas

### 3. OS Coverage

The VM architecture must accommodate the normal OS surface of a modern language without requiring a new core opcode for every native API.

Coverage is provided through capability domains and platform adapters.

### 4. Concurrency

The design reserves explicit support for:

- VM execution contexts
- threads
- tasks/futures
- synchronization
- cancellation
- timers
- event/completion queues
- thread-safe native handles

### 5. I/O

I/O must cover files, directories, terminals, pipes, sockets, DNS, IPC, devices, and asynchronous completion.

### 6. Observability

The VM should expose structured hooks for:

- instruction tracing
- execution diagnostics
- resource usage
- OS-call tracing
- capability decisions
- fault reporting
- profiling

Tracing must be disableable or appropriately bounded for production.

### 7. Portability

The core must remain portable across Linux, Windows 10+, and macOS. Platform code belongs behind adapter interfaces.

### 8. Testing

Each capability domain should eventually have:

- unit tests
- malformed-input tests
- resource-exhaustion tests
- concurrency tests where applicable
- platform adapter tests
- integration tests
- negative/security tests

## Quality Gate

No new OS capability should be considered complete merely because one native call works.

A capability is complete when its contract, validation, error translation, lifetime behavior, portability boundary, tests, and documentation are defined.

Copyright (c) Max Rupplin - MEARVK LLC - 2026


## Command-line source/artifact quality boundary

Both supported `.sleela` command-line forms must converge on the same validated execution contract:

- textual source must pass the authoritative version, lexer, parser, semantic, and compiler stages before entering SLVM;
- persistent artifacts must pass artifact/ABI validation before entering SLVM;
- neither path may introduce a parallel SLeeLa grammar or interpreter;
- security, memory, I/O heuristic, capability, and native OS boundaries remain below the common execution engine.

This keeps direct/native SLeeLa execution and persistent SLVM execution behaviorally aligned while allowing source files and precompiled artifacts to be used as distinct command-line inputs.

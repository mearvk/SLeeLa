# SLVM Layer Architecture

1. **Execution Layer** — instruction decode, dispatch, stack, and control flow.
2. **Memory/GC Layer** — existing SLeeLa garbage collector with a configurable 512 MiB default managed-memory ceiling.
3. **Security Layer** — observes load, resource/object request rates, I/O, and behavioral anomalies.
4. **I/O Heuristic Layer** — evaluates file, network, initial-load, and dynamic-instantiation patterns.
5. **Capability Layer** — determines authority for an OS operation.
6. **Adapter Layer** — performs Linux/POSIX, Windows, and macOS operations.

Security observation never grants authority. Heuristics never replace capability checks. Platform adapters remain the only layer that reaches the OS.

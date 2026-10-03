# SLeeLa Runtime Services — Native Construction

This document maps the common runtime-services contract to the current native `/impl` substrate.

| Area | Current construction | Required model |
|---|---|---|
| Networking | Native networking/platform smoke-test boundary and socket opcodes in the Core VM | Capability-mediated, cancellable, deadline-aware, bounded I/O with explicit transport states |
| File I/O | `/impl/fileio`, `FILEOPEN`/`FILEREAD`/`FILEWRITE`/`FILECLOSE`/`FILEUNLINK`, platform-specific handles | Opaque VM handles, ownership, short-I/O handling, EOF/error distinction, identity/epoch validation |
| Sockets | `LISTEN`/`ACCEPT`/`CONNECT`/`SOCKREAD`/`SOCKWRITE`/`SOCKCLOSE` | Explicit lifecycle, half-close, cancellation, deadlines, stale-handle rejection |
| Threading | Core `SPAWN`/`JOINALL`/locks/mailboxes, pthread compatibility boundary, Windows threading adapter | Structured concurrency with ownership, cancellation propagation, join results, safe points |
| Cancellation | `impl/fundamental/CancellationToken.hpp` exists as an atomic shared cancellation primitive | Propagate tokens through VM scopes and blocking I/O; cancellation must be observable at safe points |
| Memory | `/impl/MEMORY_SYSTEM_OS.md` provides allocation/page/time primitives | Add VM heap ownership, precise roots, write barriers, safepoints, and incremental/generational GC |
| Garbage collection | No dedicated GC implementation or GC-specific ISA opcode was found in the current construction | **Architecture gap:** GC remains a runtime service, not an opcode; implement it behind the common contract before claiming production GC |
| Teardown | `slvm_free` and file cleanup exist; VM generations 7/8/10 have lifecycle/quiesce/recovery states | Ordered, idempotent quiesce/cancel/drain/join/close/finalize/stop state machine |
| Recovery | SLVM/7–11 contain checkpoint/resource/filesystem identity concepts | Stale native resources fail closed and are reacquired after epoch changes |

## Construction rule

The Core VM remains the semantic execution substrate. Runtime services sit below the 98-opcode ISA and above the operating-system adapters. They may implement an opcode synchronously or asynchronously without changing its source meaning.

GC is intentionally not represented as a fake `GC` opcode. Allocation, safepoints, root scanning, barriers, and collection are runtime mechanisms. File descriptors, sockets, threads, pipes, and pending I/O are explicit resources and require deterministic close/cancel behavior even when the owning language object becomes unreachable.

See `/lib/vm/RUNTIME-SERVICES.md` for the generation-wide contract.

## Required implementation order

1. Establish a VM heap/object header and root-registration interface.
2. Add safepoint polling to interpreter execution, allocation, calls/returns, loop back-edges, blocking I/O, and cancellation boundaries.
3. Implement tracing collection with a simple stop-the-world baseline, then add incremental/generational collection behind the same interface.
4. Add write barriers before enabling generational/incremental modes.
5. Attach resource ownership/scope metadata to file, socket, pipe, and thread handles.
6. Propagate cancellation and deadlines into blocking operations.
7. Make teardown idempotent and observable, with double-close/use-after-close/stale-handle diagnostics.
8. Add cross-platform stress tests for Linux, Windows, and macOS.

## Standard GC implementation

The native runtime now uses the common generational incremental collector in runtime/garbage_collector.c. It provides stable-handle young/old generations, tri-colour incremental marking, explicit roots, safepoints, SATB-style pre-write protection, remembered old-to-young references, promotion and deterministic full collection. The Core VM owns one collector per VM and manages struct instances through it.

During active worker threads, reclamation is deferred until a structured-concurrency quiescent point so the current C VM does not scan unsafely moving thread stacks. Native resources remain under their explicit ownership/teardown APIs.

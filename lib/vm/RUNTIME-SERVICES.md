# SLeeLa Runtime Services Contract

## Purpose

The SLVM generations share one language and one source-defined ISA, but the VM needs more than opcode dispatch. Networking, file I/O, sockets, process/thread execution, memory reclamation, and shutdown must have explicit lifecycle contracts.

This document is the common runtime-services construction for SLVM/1 through SLVM/11. It supplements the opcode map; it does not add hidden opcodes.

## Services

| Service | Contract |
|---|---|
| Networking | Address resolution, connection/listen/accept, nonblocking readiness, deadlines, cancellation, backpressure, bounded buffers, protocol errors, and capability checks. |
| File I/O | Opaque VM handles, open/read/write/close/unlink, explicit ownership, EOF/error distinction, atomic close, platform adapters, and recovery-safe invalidation. |
| Sockets | Sockets are resources with states: NEW, BOUND, LISTENING, CONNECTING, CONNECTED, HALF_CLOSED, CLOSING, CLOSED, FAILED. Native descriptors/handles never escape the VM. |
| Threading | Structured concurrency: every spawned execution has an owner/scope, cancellation token, join/detach policy, deadline, and terminal result. Unowned threads are rejected. |
| Garbage Collection | VM-managed heap objects use tracing reachability as the semantic baseline. Collection is incremental/generational where practical, with safepoints and write barriers. Native resources are not reclaimed by GC alone. |
| Teardown | Shutdown is idempotent and ordered: stop admission, cancel child work, quiesce I/O, close sockets/files, join owned threads, drain deferred work, run finalizers for language objects, then release VM memory. |
| Recovery | Stale handles and resources from an invalidated epoch fail closed and must be reacquired. Recovery never silently resurrects a native descriptor. |

## Modern execution model

### 1. Structured concurrency

`SPAWN` creates a child execution owned by the current VM scope. `JOINALL` is the legacy bulk-join primitive; the runtime service model additionally requires:

- cancellation propagation from parent scope to children;
- a join result for every child;
- no implicit detached execution;
- bounded worker/resource accounting;
- deadlines and cooperative cancellation at blocking points;
- safe-point participation before shutdown.

The existing pthread/Windows abstraction remains an OS adapter, not the language concurrency model.

### 2. Async-capable I/O

The language may retain synchronous source operations while the runtime service layer supports asynchronous implementations underneath them. Blocking operations must therefore be represented as cancellable resource waits rather than assuming that a native thread can always be abandoned.

The native backend may use `poll`/`select`/`epoll`/`kqueue`/IOCP or another platform appropriate mechanism. These are implementation choices; the SLeeLa semantic contract remains stable.

### 3. Resource ownership

Every file, socket, pipe, FIFO, network connection, thread, and pending I/O request has one explicit owner/scope and one terminal lifecycle. Close/cancel operations are idempotent. Double-close, use-after-close, stale-handle, and wrong-owner access return a runtime error rather than operating on a recycled native resource.

### 4. Garbage collection and native resources

GC manages language-visible heap objects. It does not make native OS resources implicitly safe to abandon.

A resource-owning object must have an explicit close/release path. Finalization is a backstop for language objects, not the primary mechanism for sockets, files, threads, or other scarce OS resources.

The collector should use:

- generational collection for short-lived allocations;
- incremental marking/sweeping to limit pause time;
- precise roots where type information permits;
- write barriers for old-to-young and incremental marking edges;
- VM safepoints at allocation, call/return, loop/back-edge, blocking I/O, and explicit yield/cancellation boundaries;
- allocation/resource budgets integrated with the SLVM memory/resource managers.

### 5. Networking and sockets

Networking is capability-mediated. A connection must carry its address family, transport, peer/local identity, timeout/deadline, cancellation state, and lifecycle epoch.

Reads/writes distinguish:

- successful progress;
- orderly EOF/peer close;
- retryable readiness;
- timeout/cancellation;
- permanent transport failure.

A close transitions the resource toward CLOSED and wakes pending waiters. Half-close is represented explicitly where the transport supports it.

### 6. File I/O

File handles are opaque VM-local capabilities. Native descriptor numbers and Windows HANDLE values remain private to the platform adapter.

Open/read/write/close are subject to:

- capability and path policy;
- ownership;
- cancellation/deadline where the platform permits;
- short-read/short-write handling;
- EOF/error distinction;
- generation/identity checks for recovery;
- deterministic close during teardown.

This builds on the existing `impl/fileio` platform boundary and SLVM/9 filesystem identity model.

### 7. Teardown state machine

`RUNNING -> QUIESCING -> CANCELLING -> DRAINING -> JOINING -> CLOSING_RESOURCES -> FINALIZING -> STOPPED`

Any unrecoverable lifecycle violation moves to `QUARANTINED` rather than reopening or silently reusing a resource.

The sequence is idempotent: repeated shutdown requests observe the current state and do not execute destruction twice.

## Mapping to the 98-opcode ISA

The existing ISA already contains direct primitives for:

- threading: `SPAWN`, `JOINALL`, `LOCK`, `UNLOCK`, `SEND`, `RECV`;
- sockets: `LISTEN`, `ACCEPT`, `CONNECT`, `SOCKREAD`, `SOCKWRITE`, `SOCKCLOSE`;
- pipes/FIFOs: `PIPE`, `PIPEPEER`, `FIFO_MK`;
- files: `FILEOPEN`, `FILEREAD`, `FILEWRITE`, `FILECLOSE`, `FILEUNLINK`;
- asynchronous/synchronization facilities: `SYN_*`, `MUN_*`.

GC, cancellation, ownership, deadlines, resource epochs, and teardown are deliberately runtime services rather than one opcode per concept. This avoids bloating the ISA while allowing the native implementation and future VM generations to evolve.

## Generation responsibilities

- **SLVM/1:** establish resource ownership, thread, socket, file, and VM teardown invariants at the execution boundary.
- **SLVM/2–3:** preserve those invariants through cryptographic, isolation, and verification layers.
- **SLVM/4–6:** carry ownership, cancellation, resource epochs, lineage, leases, and migration state across distributed execution.
- **SLVM/7:** integrate memory, resource, checkpoint, health, and recovery managers.
- **SLVM/8:** enforce admission, policy, capability leases, transactions, audit, supervision, and cancellation.
- **SLVM/9:** bind file resources to filesystem identity/generation and native adapters.
- **SLVM/10:** revalidate storage/resource identity before verified execution.
- **SLVM/11:** apply the same ownership and lifecycle rules to filesystem modules.

All generations preserve the same SLeeLa source semantics. A generation may add stronger validation or more capable adapters, but it must not weaken resource lifetime, cancellation, safety, or teardown guarantees.

## Standard GC implementation

The previous runtime GC placeholder has been replaced by the common collector in runtime/garbage_collector.c. It is a stable-handle, generational incremental tracer with explicit roots, safepoints, SATB-style pre-write protection, remembered old-to-young references, promotion, and deterministic full collection. The Core VM now owns one collector and treats struct instances as managed objects.

The C++ VM generations use lib/vm/include/sleela_vm_gc.hpp as an RAII facade over the same C collector. The source-side declarations are in lib/vm/GarbageCollector.sleela, GCObject.sleela, GCQuality.sleela, GCPermutation.sleela, GCRoot.sleela, and GCBarrier.sleela.

Collection is deliberately non-moving because the current SLeeLa value model uses stable VM-local handles. This avoids relocation complexity while preserving the modern collector invariants. Native sockets, files, threads and other OS resources remain governed by the resource-ownership/teardown layer rather than by GC reachability.

# SLVM Memory Manager Security Model

The SLVM memory manager is a security boundary as well as a resource-management facility. The default managed-memory ceiling is **512 MiB**. The ceiling is enforced before a managed allocation is handed to the garbage collector.

## Security objectives

The memory security layer protects the VM against unbounded allocation loops, unusually large individual object requests, allocation bursts, object-count pressure, repeated allocation failures, memory pressure approaching the configured ceiling, and resource-request amplification.

It does not classify software as malware. It is a runtime resource-control mechanism.

## Decision model

Every managed allocation passes through: SLeeLa program → SLVM security observation → memory-security policy → GC allocation → managed object.

| Decision | Meaning |
|---|---|
| **ALLOW** | Allocation is within the current policy envelope. |
| **COLLECT** | Allocation is permitted while the VM is under memory pressure; the runtime should prefer collection before further growth. |
| **THROTTLE** | Request is unusually large or bursty and is refused until pressure is reduced. |
| **DENY** | Request would exceed the security ceiling or the VM is in a denied memory state. |

The policy is independent from OS capabilities: an OS capability does not grant permission to consume unlimited VM memory.

## States

The memory security layer uses NORMAL, PRESSURE, RESTRICTED, and DENIED states. Pressure begins at approximately 75% of the configured ceiling; restriction begins at approximately 90% or when allocation bursts exceed policy; denial occurs when a request would exceed the ceiling or repeated denial conditions are reached.

## Allocation controls

The memory layer records live managed bytes, live managed object count, allocation requests, requested bytes, large allocation count, denied allocation count, burst count, configured ceiling, and configured per-allocation limit. The default per-allocation limit is one quarter of the configured memory ceiling.

## GC relationship

The memory security layer sits before gc_allocate(). When collection reclaims memory, security accounting is reduced by the reclaimed payload bytes. Failed allocations are rolled back in the security accounting so a failed request cannot permanently consume the policy budget.

The garbage collector remains responsible for reachability and destruction. The security layer decides whether a new allocation is acceptable.

## Important boundary

The 512 MiB limit currently measures **managed GC payload bytes**. It is not a promise that the entire operating-system process, native stack, executable code image, C/C++ allocator metadata, or every external resource consumes exactly the same accounting pool. Those resources require their own limits and capability controls.

## Interaction with other SLVM security layers

The intended order is: Execution → runtime security → memory security/GC → I/O heuristic → capability broker → OS adapter.

A workload may therefore be stopped for exceeding memory policy or for broader resource/I/O behavior that violates runtime security policy. No heuristic grants authority, and no memory policy grants OS access.

## C and C++ boundary

The policy has a stable C ABI in include/slvm_memory_security.h and src/slvm_memory_security.c. C++ VM components may wrap this ABI with higher-level policy objects, but the security decision remains at the common VM boundary so C and C++ paths cannot silently bypass it.

Copyright (c) Max Rupplin - MEARVK LLC - 2026

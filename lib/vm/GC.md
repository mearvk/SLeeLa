# SLeeLa Garbage Collection

SLeeLa uses one common managed-memory contract for SLVM/1 through SLVM/11.

## Collector

The native collector is a stable-handle, non-moving, generational incremental tracing collector:

- young and old generations;
- tri-colour marking with a bounded incremental work budget;
- explicit root registration and VM root scanning;
- safepoint polling;
- SATB-style pre-write protection during marking;
- old-to-young remembered references;
- automatic promotion of survivors;
- deterministic full collection for teardown and diagnostics.

The design intentionally does not relocate objects. Current SLeeLa values expose VM-local handles, so stable handles avoid pointer relocation while still providing generational and incremental collection.

Modern production collectors use the same broad concepts: generational allocation, incremental work, remembered sets, safepoints, and SATB-style barriers. Java HotSpot G1 is explicitly generational and incremental and uses remembered sets and SATB marking; LLVM documents safepoints and barriers as compiler/runtime contracts.

## Native and C++ layers

- runtime/garbage_collector.c
- runtime/garbage_collector.h
- impl/core/sleela_core.c
- lib/vm/include/sleela_vm_gc.hpp
- lib/vm/src/sleela_vm_gc.cpp

## SLeeLa source model

- GarbageCollector.sleela
- GCObject.sleela
- GCQuality.sleela
- GCPermutation.sleela
- GCRoot.sleela
- GCBarrier.sleela

These classes describe the GC object model and policy surface. They do not create a second collector.

## Threading boundary

The current Core VM performs heap reclamation at a quiescent VM safepoint when worker threads are not active. Active worker-thread execution is allowed to continue under the structured-concurrency contract and collection is deferred until a safe quiescence point. This is deliberate: accurate root scanning requires all managed execution stacks to be observable at a GC safepoint.

Native sockets, files, pipes, mutexes, threads, libraries, and other OS resources are never reclaimed merely because an object becomes unreachable. Their lifecycle remains governed by runtime resource ownership and teardown.

## Quality / permutation

GCQuality and GCPermutation provide descriptive source-level policy dimensions for latency, throughput, memory pressure, generation, barriers, roots, resources, and relocation. They are intended for compiler/runtime planning and diagnostics, not for silently changing the collector semantics of an existing VM generation.

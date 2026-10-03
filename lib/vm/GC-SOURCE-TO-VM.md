# GC Source-to-VM Contract

The GC Sleela classes are source-side declarations of the common runtime memory model. They are compiler-facing types, not a second collector.

The native collector is runtime/garbage_collector.c. The C++ RAII seam is lib/vm/src/sleela_vm_gc.cpp.

The common model provides young/old generations, tri-colour incremental marking, SATB-style pre-write protection, remembered old-to-young references, explicit roots, safepoints, promotion, and deterministic full collection.

SLVM/1 through SLVM/11 consume the same memory-management contract. Generation-specific security, filesystem, module, recovery, or policy layers may constrain the collector, but they do not define incompatible object semantics.

SLeeLa uses stable VM-local handles rather than a relocating pointer heap. This makes the collector suitable for the current C ABI while retaining modern generational and incremental behavior.

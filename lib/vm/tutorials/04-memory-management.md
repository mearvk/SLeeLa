# VM Tutorial 04 — Memory Management

## Simple

Use `SleelaVMMemoryManagementSimple` for a small bounded VM. It gives the compiler a finite heap/stack/object budget and requires explicit cleanup.

## Managed

Use `SleelaVMMemoryManagementManaged` when GC is desired. Add guard, quarantine, and scrub policy so freed or sensitive regions are not treated as ordinary reusable bytes.

## Advanced

Use `SleelaVMMemoryManagementAdvanced` for checkpointed or migratable workloads. Checkpointing requires integrity; migration requires checkpointing; memory encryption requires integrity evidence in this model.

The intended lowering is:

`.sleela MM class -> compiler resource plan -> C/C++ MM module -> SLVM/SLJVM executable component`.

The compiler must reject a required MM feature when the target cannot satisfy its physical or security constraints.

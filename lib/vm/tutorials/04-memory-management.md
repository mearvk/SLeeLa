# VM Tutorial 04 — Memory Management

**Simple:** Use `SleelaVMMemoryManagementSimple` for bounded heap/stack/object budgets and explicit cleanup.

**Managed:** Use `SleelaVMMemoryManagementManaged` when GC is desired, with guard, quarantine, and scrubbing.

**Advanced:** Use `SleelaVMMemoryManagementAdvanced` for checkpointed or migratable workloads. Checkpointing requires integrity; migration requires checkpointing; encryption requires integrity in this model.

Lowering: `.sleela MM class -> compiler resource plan -> C/C++ MM module -> SLVM/SLJVM executable component`.
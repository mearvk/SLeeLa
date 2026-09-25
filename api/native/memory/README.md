# SLeeLa Memory Manager

Portable C/C++17 native memory-management foundation for SLeeLa. It provides tracked allocation, limits, structure insertion, named Leech attachments, validation, and runtime telemetry.

## Purpose

The Memory Manager gives SLeeLa a controlled native boundary for program memory. Memory activity can be observed as an abstract state model:

**memory pressure → voltage → current → flow → condition**

The voltage/current/flow values are **program-state metrics**, not physical electrical measurements in RAM.

## Core capabilities

- sleela_mm_create / sleela_mm_destroy lifecycle.
- Tracked alloc, calloc, realloc, and free.
- Allocation counts, free counts, resident bytes, peak resident bytes and cumulative allocation totals.
- Soft and hard memory limits.
- Structure insertion through sleela_mm_insert_struct.
- Managed removal of inserted structures.
- Named Leech attachments through sleela_mm_leech_attach / detach.
- Non-owning Leech semantics: a Leech observes or attaches to a managed entry and does not implicitly free its target.
- Configurable abstract flow model.
- Statistics retrieval containing pressure, voltage, current, flow and condition.
- Validation entry point for manager state.

## Structure model

A structure insertion represents a byte-described memory structure owned by the manager. The API accepts an application structure identifier, source bytes and size, alignment metadata, flags, and an optional caller-provided name.

This allows higher layers to represent structs and other explicitly described memory objects without requiring the Memory Manager to understand every application type.

## Leech model

A Leech is deliberately non-owning. It is a named relationship to an inserted structure or managed memory entry. Detaching a Leech removes the relationship; it does not release the target memory.

This distinction is important for safety: attachment must not silently change ownership or produce an unexpected free.

## Flow and condition model

The current implementation derives abstract telemetry from manager state. Pressure is based on resident bytes relative to the configured hard limit. The configured flow model converts pressure and allocation activity into abstract voltage, current and flow values.

These values provide a deterministic software-state vocabulary for higher-level SLeeLa systems. They should not be interpreted as measurements of electrical potential, electrical current, heat, or physical RAM behavior.

## Integration contract

A Sleela-facing layer can expose higher-level classes such as:

- MemoryManager
- MemoryBlock
- MemoryStructure
- StructMemory
- Leech
- MemoryFlow
- MemoryCondition
- MemoryPolicy
- MemorySnapshot
- MemoryTelemetry
- MemoryAllocator
- MemoryArena
- MemoryPool
- MemoryGuard
- MemoryValidation

Those classes should treat the C ABI as the stable native boundary rather than duplicating allocator state.

## Safety and completeness notes

This package is the native foundation, not a claim that every advanced allocator feature is already present. A production expansion should add aligned allocation enforcement, allocation headers/canaries, stronger validation, safe Leech invalidation, explicit condition thresholds and hysteresis, arenas and pools, quarantine and guard options, snapshots, zeroization policies, telemetry callbacks, allocator backends, and platform-specific hardening.

The repository should keep these responsibilities separate:

1. **Ownership** — who may release memory.
2. **Structure** — what the memory represents.
3. **Observation** — what a Leech can observe or reference.
4. **Telemetry** — what memory activity reports.
5. **Condition** — what deterministic program state follows from those reports.

See also the native header and implementation in this directory.
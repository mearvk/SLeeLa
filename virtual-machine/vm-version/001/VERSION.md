# VM Version 001 — The Start (baseline)

*Idea System(TM) / Idea(TM). From Author; distributive.*

## Status

**Baseline.** Version 001 is the starting position of the think-tank: an exact
copy of the native VM execution core (`virtual-machine/core`, itself a copy of
`impl/core`). No changes have been made yet — this is the reference every later
version is measured against.

## What this VM is

The 71-file C/C++ execution core: the bytecode VM loop (`sleela_core.c/.h`), the
value model and bounded VM-local handle tables, and the native service bridges
it owns — threads, io, net, time, path, os, memory + memory-manager (grades),
synchro, best-of, munction, library, platform, terminal, ai, media, events,
idea-canvas — plus the per-family `*_smoke.c` self-tests.

## Think-tank method (how 001 → 010 proceeds)

Each subsequent version (`002`…`010`) is **one deliberate improvement built on
the version before it**, with a `VERSION.md` recording:

1. **Change** — the concrete edit(s) made to the C/C++ this round.
2. **Rationale** — why this improves correctness, safety, performance, or
   clarity.
3. **Carried forward** — that it includes everything from the prior version.
4. **Next** — the improvement the next review should take up.

Improvements are focused and real (one well-argued change per round), so the
chain is a true progression rather than parallel copies.

## Improvement agenda (candidate backlog for 002→010)

A starting list the think-tank will draw from and refine as it goes:

1. Harden the memory manager (bounds, pressure thresholds, leak accounting).
2. Strengthen the bytecode dispatch (validation, clearer errors).
3. Thread-safety review of the shared handle tables.
4. GC quality: reachability, barriers, collection pacing.
5. Deterministic, auditable error reporting across bridges.
6. Performance: hot-path review of the fetch-decode-execute loop.
7. Capability/security posture of the native bridges.
8. Observability: structured counters/stats from the VM.
9. Portability review (Win32/POSIX/Apple parity).
10. Consolidation + a clean, documented v010 "eventual program".

## Baseline

- Files: 71 (identical to `virtual-machine/core`).
- Edits this version: none (this is the Start).

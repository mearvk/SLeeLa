# VM Version 001 — The Start (baseline)

*Idea System(TM) / Idea(TM). From Author; distributive.*

## Status

**Ground floor laid.** Version 001 starts from the native VM execution core
(`virtual-machine/core`, itself a copy of `impl/core`) and adds the **user-level
ground floor** — the configuration, diagnostics, and status surface that every
later think-tank round (002…010) is tuned and observed through. The 71 core
files are carried forward unchanged; the ground floor is layered on top.

## Change (this version)

Added a thin, pure-C user-level surface over the existing core knobs:

- **`vm_config.{h,c}`** — one place a user tunes the VM before/at startup:
  memory limit (clamped to the real 256 MiB–2 GiB range, binding to the core's
  `slmm_*` memory manager), thread cap (1–128), verbosity, and a capability
  bitset (mirrors `SLPlatformCapability`). `vm_config_apply()` pushes it to the
  live core; `vm_config_current()` reports it back.
- **`vm_report.{h,c}`** — one uniform, user-readable diagnostic format for every
  bridge: `[idea-vm] <SEV> (<subsystem>[/<code>]): <message>`, severity-gated by
  the active verbosity. Replaces raw status ints with legible messages.
- **`vm_status.{h,c}`** — the "what am I running" banner: version, applied
  config, and (when the memory manager is linked) live memory stats pulled from
  the real `slmm_stats()`.
- **`vm_ground_test.c`** — a smoke test exercising all three.

## Rationale

Later improvements (memory hardening, GC, dispatch, security) are only
meaningful to a *user* if they can be configured and observed. The ground floor
is that substrate: set a limit (config) → run → see the result and any failure
(status + report). Building internals first would improve things no user could
reach. Scope was kept to new files + minimal surface — the 71 core files are
untouched.

## Carried forward

All 71 files of the baseline core (identical to `virtual-machine/core`).

## Verification

- Builds clean standalone: `gcc -std=c11 -Wall -Wextra -I. vm_ground_test.c
  vm_config.c vm_report.c vm_status.c`.
- Builds and runs memmgr-wired: add `-DVM_HAVE_MEMMGR sleela_memmgr.c
  sleela_memory.c sleela_thread.c -pthread` — `vm_config_apply` enables `slmm`
  at the chosen limit and `vm_status` prints real live stats.
- Verbosity gating confirmed (a `debug` line is suppressed below debug level).

## Next (for 002)

Build on this surface: harden the memory manager (pressure thresholds, clearer
limit/OOM reporting *through `vm_report`*), so the user sees and controls memory
behavior end to end.

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

## Files

- 71 core files carried forward from `virtual-machine/core`, unchanged.
- 7 new ground-floor files: `vm_config.{h,c}`, `vm_report.{h,c}`,
  `vm_status.{h,c}`, `vm_ground_test.c`.

## Sleela OOD manifest

`SLVmVersion001.sleela` (this folder) is the loadable manifest that lets a user
**select this VM** on system start or in a conference: it declares the native
dir + `main`, the hardening units composed through v001, and the default config
profile, and `build()` returns a configured `SLVmRuntime`. The shared OOD
classes it uses live in `lib/vm/src/sleela` (see its README). Selection routes through
the `OP_VM_SELECT` (148) opcode.

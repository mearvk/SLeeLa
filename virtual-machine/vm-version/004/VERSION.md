# VM Version 004 — Thread-safe telemetry + handle-table saturation watch

*Idea System(TM) / Idea(TM). From Author; distributive.*

## Change (built on 003)

Added `vm_telemetry.{h,c}` + `vm_telemetry_test.c` — a lock-free observability
layer (C11 `<stdatomic.h>`):
- VM-wide monotonic counters (steps, allocs, bridge calls, errors) bumped
  without a mutex from any thread.
- a per-table gauge (live / capacity + high-water mark) for the core's bounded
  handle tables (best-of 32, synchro 64, munction 64, ai, canvas), updated with
  an atomic CAS loop.
- a saturation watch: when a table crosses the watermark (default 0.80) it emits
  ONE edge-triggered typed result (`VM_E_EXHAUSTED`) through v003 → v001, so a
  user sees near-exhaustion *before* allocation fails; it re-arms when usage
  falls back.
- `vm_telemetry_report()` renders counters + per-table gauges.

## Rationale

Rounds 002–003 made memory and dispatch legible; 004 makes the VM's *concurrent
work and resource headroom* observable — the third pillar a user needs. It is
genuinely native: lock-free atomics (verified correct under contention), tied to
the real table capacities, and reported through the typed-result chain built in
the prior rounds. Graceful fallback to plain stores if a toolchain lacks
atomics.

## Carried forward

All of v001–v003: the 71 core files + `vm_config`, `vm_report`, `vm_status`,
`vm_memory`, `vm_result`.

## Verification

Builds clean (`-Wall -Wextra -pthread`) and runs:
- 8 threads × 100,000 lock-free increments summed to **exactly 800,000** (no
  lost updates — atomics correct under contention).
- Filling the best-of table (real `SL_BESTOF_MAX`=32) fired a single
  `[idea-vm] WARN (telemetry/EXHAUSTED): best-of table near capacity: 26/32
  (81%)` and the report showed `best-of 32/32 (peak 32)`, `errors=1`.

## Next (for 005)

GC quality: reachability/barrier review and collection pacing, surfacing GC
pressure through `vm_telemetry` counters and typed results — continuing the
observable-and-controllable arc toward the consolidated v010.

## Sleela OOD manifest

`SLVmVersion004.sleela` (this folder) is the loadable manifest that lets a user
**select this VM** on system start or in a conference: it declares the native
dir + `main`, the hardening units composed through v004, and the default config
profile, and `build()` returns a configured `SLVmRuntime`. The shared OOD
classes it uses live in `lib/vm/src/sleela` (see its README). Selection routes through
the `OP_VM_SELECT` (148) opcode.

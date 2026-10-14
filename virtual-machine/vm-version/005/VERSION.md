# VM Version 005 — GC quality (pressure, pacing, observability)

*Idea System(TM) / Idea(TM). From Author; distributive.*

## Change (built on 004)
Added `vm_gc.{h,c}` + test: a quality layer over the repo's generational
mark-sweep collector. From GC stat deltas it derives a condition
(HEALTHY/BUSY/PRESSURE/THRASH) via reclaim/alloc ratio + collections-per-1k,
gives young-collection pacing advice, feeds v004 telemetry, and edge-triggers a
typed v003 result on worsening (+ a recovery notice).

## Rationale
GC was a black box: a user couldn't tell healthy churn from thrashing. v005
makes reclamation vs. allocation legible and gives the runtime a pacing hint —
observability + control, consistent with the arc.

## Carried forward
v001–v004 (71 core files + config/report/status/memory/result/telemetry).

## Verification
Builds clean (`-Wall -Wextra -pthread`); classified HEALTHY→PRESSURE(collect
_young=1)→THRASH→BUSY across snapshots, with PRESSURE→WARN and THRASH→ERROR, and
telemetry showing allocs=4500 bridge=199.

## Next (006)
Hot-path dispatch profiler: per-opcode-family execution counters feeding
telemetry, so the user sees where VM time goes.

## Sleela OOD manifest

`SLVmVersion005.sleela` (this folder) is the loadable manifest that lets a user
**select this VM** on system start or in a conference: it declares the native
dir + `main`, the hardening units composed through v005, and the default config
profile, and `build()` returns a configured `SLVmRuntime`. The shared OOD
classes it uses live in `lib/vm/src/sleela` (see its README). Selection routes through
the `OP_VM_SELECT` (148) opcode.

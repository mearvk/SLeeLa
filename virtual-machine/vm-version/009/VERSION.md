# VM Version 009 — Structured audit trail

*Idea System(TM) / Idea(TM). From Author; distributive.*

## Change (built on 008)
Added `vm_audit.{h,c}` + test: a fixed-capacity (256) ring of structured events
(CONFIG/DENIAL/PRESSURE/SATURATE/PLATFORM/ERROR/INFO), each with a monotonic
sequence number and a running FNV-1a **chain hash** over (prev, seq, kind,
subsystem, message) — ordered and tamper-evident. `vm_audit_result()` folds a
typed v003 result straight into the trail; every event is also mirrored live to
v001's `vm_report`.

## Rationale
The prior layers each reported to the console transiently. 009 gives the VM one
durable, queryable, integrity-chained history tying config changes, denials,
pressure, saturation, and platform mismatches together — the backbone v010's
facade exposes.

## Carried forward
v001–v008.

## Verification
Builds clean (`-Wall -Wextra`). Recorded CONFIG/DENIAL/PRESSURE/SATURATE events
with a stable chain hash and correct report; flooding past capacity kept
`retained=256` while `total` rose to 310 (ring overwrite correct).

## Next (010)
Consolidation: a single `vm_runtime` facade that wires config→platform→
capability→memory→gc→telemetry→profile→audit into one init/step/report/shutdown
surface — the finished "eventual program".

## Sleela OOD manifest

`SLVmVersion009.sleela` (this folder) is the loadable manifest that lets a user
**select this VM** on system start or in a conference: it declares the native
dir + `main`, the hardening units composed through v009, and the default config
profile, and `build()` returns a configured `SLVmRuntime`. The shared OOD
classes it uses live in `lib/vm/src` (see its README). Selection routes through
the `OP_VM_SELECT` (148) opcode.

# VM Version 002 — Memory hardening (pressure + watermarks)

*Idea System(TM) / Idea(TM). From Author; distributive.*

## Change (built on 001)

Added `vm_memory.{h,c}` + `vm_memory_test.c` — a pressure layer over the core
memory manager (`slmm_*`):
- WARNING / CRITICAL watermarks (fractions of the hard limit; default 0.80 /
  0.95, configurable via `vm_memory_set_watermarks`).
- a polled condition `NORMAL / PRESSURE / CRITICAL / LIMIT`.
- **edge-triggered** reporting through v001's `vm_report`: one legible notice
  per watermark crossing and a single recovery notice when usage returns to
  normal — not a message per allocation.
- a one-line `vm_memory_report` for `vm_status`-style readouts.

## Rationale

v001 let a user *set* a memory limit but only told them *after* an allocation
was refused. v002 makes approaching memory exhaustion observable and actionable:
the user is warned at a watermark, sees severity escalate, and gets a clear LIMIT
error — all in the uniform v001 report format. This is real hardening at the
user level, not internal churn.

## Carried forward

All of v001: the 71 core files + the ground floor (`vm_config`, `vm_report`,
`vm_status`).

## Verification

Built and run memmgr-wired (`-DVM_HAVE_MEMMGR` + `slmm`/memory/thread, pthread):
driving 16 MiB allocations against a 256 MiB limit fired PRESSURE at 50%,
CRITICAL at ~81%, and LIMIT at 100% — each exactly once (edge-triggered), with
the per-step condition tracking usage. Builds clean (`-Wall -Wextra`).

Honest note: `condition` can read `LIMIT` after freeing because `slmm`'s
`refused` counter is cumulative (by design). A later round could expose a
"refusals since reset" vs. "currently over limit" distinction; for v002 the
cumulative signal is correct and intentional.

## Next (for 003)

Robust dispatch + diagnostics: a typed VM result (`vm_result`) so bridges return
structured, user-legible outcomes (code + subsystem + message) instead of raw
status ints, reported through `vm_report`.

## Sleela OOD manifest

`SLVmVersion002.sleela` (this folder) is the loadable manifest that lets a user
**select this VM** on system start or in a conference: it declares the native
dir + `main`, the hardening units composed through v002, and the default config
profile, and `build()` returns a configured `SLVmRuntime`. The shared OOD
classes it uses live in `lib/vm/src/sleela` (see its README). Selection routes through
the `OP_VM_SELECT` (148) opcode.

# VM Version 003 — Typed results (structured, legible dispatch outcomes)

*Idea System(TM) / Idea(TM). From Author; distributive.*

## Change (built on 002)

Added `vm_result.{h,c}` + `vm_result_test.c` — one typed result the whole VM
shares, replacing per-family raw status ints:
- `VmResult` = stable `VmResultCode` (OK/INVALID/LIMIT/NOT_FOUND/UNAVAILABLE/
  MALFORMED/EXHAUSTED/DENIED/INTERNAL) + subsystem tag + formatted message.
- `vm_ok()` / `vm_fail(code, subsystem, fmt, …)` constructors.
- `vm_result_report()` prints through v001's `vm_report` at the severity implied
  by the code (LIMIT/DENIED/INTERNAL → ERROR; others → WARN).
- `vm_result_from_status()` maps the existing sleela_* ABI convention
  (0 ok / negative error, as in `idea_canvas_status`) onto the stable categories,
  so current bridges adopt typed results without changing their return values.

## Rationale

A user (and a calling tool) could not tell what `-2` meant from one bridge vs.
another. Typed results give every failure path a stable, branchable code and a
legible message in the uniform format — the dispatch-level counterpart to v002's
memory legibility. It is additive: bridges can return `VmResult` directly or
wrap a legacy status via `vm_result_from_status`.

## Carried forward

All of v001 + v002: the 71 core files, `vm_config`, `vm_report`, `vm_status`,
`vm_memory`.

## Verification

Builds clean (`-Wall -Wextra`) and runs: OK vs. failure results classified
correctly; `vm_result_report` emits WARN for INVALID and ERROR for LIMIT; the
legacy mapping yields `0→OK, -2→LIMIT, -3→UNAVAILABLE, -99→INTERNAL`.

## Next (for 004)

Thread-safety + observability: a `vm_telemetry` layer with atomic counters and a
saturation watch over the bounded handle tables (SL_BESTOF_MAX=32,
SL_SYNCHRO_MAX=64, SL_MUNCTION_MAX=64, SL_AI_MAX), reporting typed results
through v003 when a table nears exhaustion.

## Sleela OOD manifest

`SLVmVersion003.sleela` (this folder) is the loadable manifest that lets a user
**select this VM** on system start or in a conference: it declares the native
dir + `main`, the hardening units composed through v003, and the default config
profile, and `build()` returns a configured `SLVmRuntime`. The shared OOD
classes it uses live in `lib/vm/src` (see its README). Selection routes through
the `OP_VM_SELECT` (148) opcode.

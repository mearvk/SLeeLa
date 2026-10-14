# VM Version 010 — Consolidation (the eventual program)

*Idea System(TM) / Idea(TM). From Author; distributive.*

## Change (built on 009 — the capstone)

Added `vm_runtime.{h,c}` + `vm_runtime_demo.c`: a single facade that wires the
whole think-tank into four calls —
`vm_runtime_init / poll / permit / report / shutdown` — delegating to each layer
in correct dependency order:

```
config -> platform(+parity audit) -> capability -> memory -> gc
       -> telemetry -> profiler -> audit
```

`vm_runtime_report()` emits ONE consolidated status block (version · config ·
platform parity · memory · gc · telemetry · dispatch profile · recent audit).
Significant events (denials, memory/GC pressure, saturation, platform
mismatches) are routed into the v009 audit trail automatically.

## The arc, 001 → 010

| v | Improvement |
|---|---|
| 001 | Ground floor: config, uniform reporting, status |
| 002 | Memory hardening: pressure watermarks |
| 003 | Typed results: structured, legible outcomes |
| 004 | Thread-safe telemetry + handle-table saturation watch |
| 005 | GC quality: pressure, pacing, observability |
| 006 | Hot-path dispatch profiler |
| 007 | Capability enforcement (closed v001's gap) |
| 008 | Platform-parity readout (Windows 10+/macOS/Linux) |
| 009 | Structured, chain-hashed audit trail |
| 010 | Consolidated `vm_runtime` facade |

Every version carries all prior layers forward over the 71-file baseline core.

## Rationale

The improvements were real but separate; v010 makes the hardened VM usable as a
whole through one surface, with correct ordering done once in the facade — the
"eventual program" the chain was building toward.

## Carried forward

All of v001–v009.

## Verification

Builds clean (`-Wall -Wextra`) and runs **memmgr- and platform-wired**
(`-DVM_HAVE_MEMMGR -DVM_HAVE_PLATFORM` + the real `sleela_memmgr/memory/thread/
platform`): init applied a 256 MiB cap and denied `process`; a guarded spawn was
DENIED; the best-of table (real `SL_BESTOF_MAX`=32) was watched to saturation;
GC pressure classified; 2000 arith dispatches profiled; and one consolidated
report rendered config + real Linux platform parity + memory + gc + telemetry +
dispatch profile + a chain-hashed audit trail.

## Status

**Done.** v010 is the consolidated eventual program of this think-tank. Each
layer remains usable directly; the facade is the recommended entry point.

# lib/vm/src — Sleela OOD equivalents of the VM C/C++

*Idea System(TM) / Idea(TM). From Author; distributive.*

This directory holds the **Sleela object-oriented equivalents** of the native
C/C++ VM code, so a user can **select a VM other than the Native VM in `/impl`**.
On system start — or during a conference with the user — the native VM loads
these `.sleela` into memory, lets the user pick a version, and then loads that
version's C/C++ `main`.

## The two sides

| Native C/C++ | Sleela OOD equivalent (here) | Mirrors |
|---|---|---|
| `vm_config.{h,c}` (v001) | `SLVmConfig.sleela` | memory/thread/verbosity/capability tuning |
| `vm_report.{h,c}` (v001) | `SLVmReport.sleela` | uniform `[idea-vm] SEV (sub/code): msg` |
| `vm_status.{h,c}` (v001) | `SLVmStatus.sleela` | the "what am I running" banner |
| `vm_memory.{h,c}` (v002) | `SLVmMemory.sleela` | pressure watermarks |
| `vm_result.{h,c}` (v003) | `SLVmResult.sleela` | typed results + legacy-status map |
| `vm_telemetry.{h,c}` (v004) | `SLVmTelemetry.sleela` | counters + table saturation |
| `vm_gc.{h,c}` (v005) | `SLVmGc.sleela` | GC pressure/pacing quality |
| `vm_profile.{h,c}` (v006) | `SLVmProfile.sleela` | dispatch profiler |
| `vm_capability.{h,c}` (v007) | `SLVmCapability.sleela` | capability enforcement |
| `vm_platform.{h,c}` (v008) | `SLVmPlatform.sleela` | platform parity |
| `vm_audit.{h,c}` (v009) | `SLVmAudit.sleela` | hash-chained audit trail |
| `vm_runtime.{h,c}` (v010) | `SLVmRuntime.sleela` | the consolidated facade |
| `lib/vm/src/sleela_vm_source.c` | `SLVmSource.sleela` | VM source-requirement validation |

Plus the selection layer:

- `SLVmVersion.sleela` — base manifest a selectable version extends.
- `virtual-machine/vm-version/NNN/SLVmVersionNNN.sleela` — the per-version
  manifest (id, label, native dir + `main`, composed units, config profile).
- `virtual-machine/vm-version/SLVmRegistry.sleela` — the catalogue; `list()`,
  `select(n)`, `boot(n)`.

## Selecting a VM (start / conference / terminal)

```
SLVmRegistry reg = new SLVmRegistry();
reg.load();                 // load all ten manifests into memory
reg.list();                 // enumerate for the user
SLVmRuntime rt = reg.boot(10);   // choose v010 (consolidated); builds its runtime
```

Conceptually: `sleela vm list` / `sleela vm select <NNN>` from the terminal, or a
setup/conference choice. The chosen manifest's `build()` returns a configured
`SLVmRuntime`; its `nativeMain` names the C/C++ entry the native loader then
builds and boots.

## Opcode seam

VM selection routes through the **`OP_VM_SELECT` (148)** service opcode — the
Sleela-side `SLVMNativeBinding` VM-exit the host services by loading the chosen
version's native main. The source ISA (`lib/vm/InstructionSet.sleela`), the
native enum (`impl/core/sleela_core.h` + its copies), the native dispatch
(`impl/core/sleela_core.c`), and `OPCODE-MAP.md` were all updated together per
the opcode maintenance rule — this change also closed the prior `OP_IC_*`
(126–147) source-ISA lag.

## Design notes

- Authored once here (shared classes), with a thin self-describing manifest in
  each of the ten `vm-version/NNN/` folders — the OOD-respectful design, not ten
  copies of identical class bodies.
- Match the lib/vm Sleela conventions: `#sleela 1.3`, the five-field doc-header,
  `configure()`-style init (no constructors), `int` state machines, and numbered
  fields instead of arrays (the surface has no arrays).
- Every file is grammar-clean through the real `sleela` compiler. Two method
  names were adjusted because `record` is contextual after `.` in call position
  (`SLVmAudit.log`/`logResult`, `SLVmProfile.attribute`).

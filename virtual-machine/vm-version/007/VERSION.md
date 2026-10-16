# VM Version 007 — Capability enforcement

*Idea System(TM) / Idea(TM). From Author; distributive.*

## Change (built on 006)
Added `vm_capability.{h,c}` + test: a single choke point that turns v001's
capability mask into real decisions. `vm_cap_check(cap, subsystem, op, &r)`
returns 1 to proceed or fills a typed VM_E_DENIED result (reported ERROR,
counted in v004 telemetry) and returns 0. Bridges adopt it with one guard line.

## Rationale
v001 recorded disabled capabilities but did not enforce them — explicitly noted
there as a later round. 007 closes that gap with uniform, observable denials, so
a user who disables networking (or file-io, process, …) actually gets it blocked
and sees why.

## Carried forward
v001–v006.

## Verification
Builds clean (`-Wall -Wextra -pthread`). With NETWORK denied, a guarded
`net.listen` returned DENIED (ERROR reported, denials=1, telemetry errors=1);
re-enabling NETWORK let the same op return OK.

## Next (008)
Portability/platform parity readout: a `vm_platform` surface reporting the
active backend (Win32/POSIX/Apple) and per-capability platform support, so a
user knows what their OS actually provides.

## Sleela OOD manifest

`SLVmVersion007.sleela` (this folder) is the loadable manifest that lets a user
**select this VM** on system start or in a conference: it declares the native
dir + `main`, the hardening units composed through v007, and the default config
profile, and `build()` returns a configured `SLVmRuntime`. The shared OOD
classes it uses live in `lib/vm/src/sleela` (see its README). Selection routes through
the `OP_VM_SELECT` (148) opcode.

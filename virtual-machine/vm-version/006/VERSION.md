# VM Version 006 — Hot-path dispatch profiler

*Idea System(TM) / Idea(TM). From Author; distributive.*

## Change (built on 005)
Added `vm_profile.{h,c}` + test: per-opcode-family execution counts and
accumulated nanoseconds via a cheap scoped timer (`vm_profile_begin/end`), a
`tick` fast path, a "hot family" query, and a ranked report. Feeds v004's STEPS
(and BRIDGE) counters so totals stay consistent.

## Rationale
Users tuning a program need to know where VM time goes. 006 attributes dispatch
time to arith/control/loadstore/call/struct/bridge/print, turning performance
into something observable rather than guessed.

## Carried forward
v001–v005.

## Verification
Builds clean (`-Wall -Wextra -pthread`). A simulated dispatch loop ranked arith
hottest by total time and bridge highest per-call (avg 4591 ns), and the STEPS
counter (7050) matched the sum of all dispatches.

## Next (007)
Capability enforcement: make v001's capability mask actually gate the native
bridges, returning a typed DENIED result when a program uses a disabled
capability.

## Sleela OOD manifest

`SLVmVersion006.sleela` (this folder) is the loadable manifest that lets a user
**select this VM** on system start or in a conference: it declares the native
dir + `main`, the hardening units composed through v006, and the default config
profile, and `build()` returns a configured `SLVmRuntime`. The shared OOD
classes it uses live in `lib/vm/src/sleela` (see its README). Selection routes through
the `OP_VM_SELECT` (148) opcode.

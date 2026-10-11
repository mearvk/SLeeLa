# SLeeLa VM examples — novice → very senior

Compilable, runnable examples that build up the SLeeLa VM model across four
experience levels. Each is a single self-contained file; each **compiles**
(`sleela check`) and **runs** (`sleela run`) on the toolchain today.

| Level | File | Teaches |
|---|---|---|
| **Novice** | [`01-novice/novice-vm.sleela`](01-novice/novice-vm.sleela) | the VM lifecycle: configure → start → load a module → stop (mirrors `SLVM`) |
| **Mid** | [`02-mid/mid-vm-config.sleela`](02-mid/mid-vm-config.sleela) | composing a VM from option objects (memory/CPU/execution), feature bits, a validity gate, and a build plan |
| **Senior** | [`03-senior/senior-vm-managers.sleela`](03-senior/senior-vm-managers.sleela) | orchestrating the subsystem managers: ordered boot, a managed heap with allocation accounting, security admit, linker resolution, and a GC reclaim cycle |
| **Very senior** | [`04-very-senior/very-senior-vm.sleela`](04-very-senior/very-senior-vm.sleela) | the production arc — resilient component acquisition (primary → official → college/edu → archive fallback), an OS syscall bridge gated by a feature bit, a dynamic memory guard, a self-verifying challenge pass, and emitting the C config only once VERIFIED |

## Running

```sh
./impl/build/sleela run lib/vm/examples/01-novice/novice-vm.sleela
./impl/build/sleela run lib/vm/examples/02-mid/mid-vm-config.sleela
./impl/build/sleela run lib/vm/examples/03-senior/senior-vm-managers.sleela
./impl/build/sleela run lib/vm/examples/04-very-senior/very-senior-vm.sleela

# validate without running:
./impl/build/sleela check lib/vm/examples/01-novice/novice-vm.sleela
```

## The progression

Each level adds exactly one or two new ideas on top of the last, so a reader can
climb:

1. **Novice** — one class, the lifecycle, `print`. No composition.
2. **Mid** — object composition (class-typed fields), a `valid()` gate before
   acting, feature flags as a small bitset, passing objects to a planner.
3. **Senior** — many collaborating subsystems brought up in order with a
   failure gate; stateful accounting (heap live/limit); cross-object method
   calls; a reclaim cycle. Honest limits (an over-limit alloc is *refused*, not
   crashed).
4. **Very senior** — the full "assemble → verify → emit" pipeline, including the
   resilient fallback acquisition pattern (the same tiering as
   `SLVMComponentFetcher` + `lib/os/SLSupportDownloader`), a capability bridge
   gated by features, a challenge/report verdict, and generated C that only
   emits once verification passes.

## Issues and notes (read before extending)

These mirror the `lib/os/examples` notes and are deliberate, not shortcuts:

1. **Each example is a self-contained single file — required, not a shortcut.**
   `sleela run`/`check` compiles **one file as a single compilation unit**. The
   split `lib/vm` classes (`SLVM`, `SleelaVMSource`, the managers, …) are not
   visible to a program that only references them: a bare
   `SleelaVMSource source;` fails with `unknown type 'SleelaVMSource'`. So each
   example inlines the compact classes it needs, exactly like the runnable
   generators do elsewhere in the tree.

2. **Most `lib/vm/*.sleela` files are declaration contracts, not runnable.**
   Many use the `uintNN`/`function` declaration vocabulary (e.g.
   `SleelaVMMemoryManagementSimple`), and a few use the `native sleela_str_*`
   intrinsics (`SLVMModuleLoader`, `InstructionSet`, `SleelaVMSystemCallBridge`),
   which the single-file compiler rejects. The pre-existing
   `lib/vm/tutorials/examples/*.sleela` are illustrative pseudo-Sleela and do
   **not** compile (`unknown type 'SleelaVMSource'`, `function` keyword). These
   `examples/` files are the compilable counterpart; they avoid `native` and the
   declaration-only vocabulary and build with ordinary `+`/`if`/`while`.

3. **The examples model behaviour, not the native VM.** They demonstrate the
   shape, composition, and control flow of each VM concern (lifecycle, options,
   managers, resilient fetch, verification). The real execution core is the
   C/C++ VM under `impl/`; these are the SLeeLa-side teaching slices.

4. **`assert` is reserved** — the very-senior example uses `require(...)` for its
   challenge checks. `#sleela 1.11` is required (the current surface).

## Relationship to the real VM classes

| Example piece | Real `lib/vm` object(s) |
|---|---|
| `Vm` (novice) | `SLVM` |
| option objects + `BuildPlan` (mid) | `SleelaVMMemoryOptions`/`CpuOptions`/`ExecutionOptions`, `SleelaVMFeatureBits`, `SleelaVMBuildPlan` |
| the managers (senior) | `SleelaVMMemoryManagement*`, `SleelaVMSecurityManagement*`, `SleelaVMLinkingManager*`, `SLVMHeap`, `GarbageCollector` |
| fetcher + bridge + challenge + emit (very senior) | `SLVMComponentFetcher` (+ `lib/os/SLSupportDownloader`), `SleelaVMSystemCallBridge`, `SleelaVMDynamicMemoryGuard`, `SleelaVMChallengeManagerAdvanced`, `SleelaVMReportsManager`, `SleelaVMBuildPlan` |

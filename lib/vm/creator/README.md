<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa VM Creator Source Package

The /lib/vm/creator package is the SLeeLa source-side VM Creator. It gives each SLVM generation a stable formal name and source-level construction contract while keeping /impl as the standard native Core implementation.

## Formal VM names

| Path | Formal Name | Source |
|---|---|---|
| /impl | Core | SleelaVMCore.sleela |
| /1 | Foundation | SLVMFoundation.sleela |
| /2 | Operator | SLVMOperator.sleela |
| /3 | Specialist | SLVMSpecialist.sleela |
| /4 | Supervisor | SLVMSupervisor.sleela |
| /5 | Manager | SLVMManager.sleela |
| /6 | Director | SLVMDirector.sleela |
| /7 | Administrator | SLVMAdministrator.sleela |
| /8 | Executive | SLVMExecutive.sleela |
| /9 | Authority | SLVMAuthority.sleela |
| /10 | Principal | SLVMPrincipal.sleela |
| /11 | Sovereign | SLVMSovereign.sleela |

These names are descriptive identifiers, not permissions by themselves. Selecting a higher-numbered VM never implicitly grants OS, filesystem, network, storage, security, or administrative capability.

## Creator entry points

- SleelaVMCreator.sleela — VM construction coordinator (**implemented**): selects a
  generation, builds and **boots a live SLVM**, hosts a Sleela CPU as the
  executor, and runs a C/C++/Java/Sleela program through it.
- SleelaVMGenerationCatalog.sleela — canonical version-to-name/path catalog (**implemented**).
- SleelaVMCore.sleela — /impl Core contract. SLVMFoundation .. SLVMSovereign — the
  per-generation build-side contracts (`/1` .. `/11`).

The creator reads the canonical VM configuration, selects one generation, resolves its source modules, builds a SleelaVMBuildPlan, invokes the compiler, verifies the artifact, and packages the result. `SleelaVMCreator` additionally instantiates the result as a running `SLVM` (lib/vm/SLVM.sleela) so the VM can be used immediately, not just packaged.

## Construction model

configuration -> generation catalog -> named VM source -> architecture/options -> build plan -> compiler -> C/C++ native boundary -> VM artifact -> verification -> package

All generations use the common source models under /lib/vm. A generation class adds the responsibilities appropriate to its documented VM architecture.

## Running the full stack: Creator → SLVM → Sleela CPU → program

The Creator Edition's purpose is to produce a VM you can then *run guest code on*.
The executor is a **Sleela CPU model** (any `SLCPURuntime` / `SL<ARCH>CPU` in
`/lib/cpu`), and the guest code is a **C, C++, Java, or Sleela** program:

```
SleelaVMCreator  →  SLVM (Secondary VM, on the Native Sleela VM substrate)
                 →  hostCpu(SL<ARCH>CPU)   (the executor, nested on the SLVM substrate)
                 →  runGuestWorkload(C | C++ | Java | Sleela program)
```

The whole chain resolves in one call:

```
SLCPUResource grant = new SLCPUResource(); grant.configure(4096, 100000, grant.PRIORITY_NORMAL);
SL6502CPU cpu = new SL6502CPU(); cpu.configure();
SLWorkload job = new SLWorkload(); job.configure("app", "examples/hello.c", job.LANG_C, grant);

SleelaVMCreator creator = new SleelaVMCreator(); creator.configure();
int out = creator.run(5, cpu, job, 1000000);   // generation 5 "Manager"; returns the program's output
```

How the layers connect (the bridge):

- `SleelaVMCreator.create()` builds and boots the `SLVM`.
- `SLVM.hostCpu(cpu)` lends the CPU the VM's `SLSleelaVM` substrate, and the CPU
  calls `attachSubstrate(...)` so it lowers its program onto **that** substrate —
  the CPU runs *nested* on the Secondary VM, not on a private engine.
- `SLVM.runGuestWorkload(job, budget)` has the CPU compile the program
  (`SLCompilerDriver` → `SLProgram`, language-independent) and map it onto the
  shared substrate (`SLOpcodeMap`), then SLVM drives the opcode stream and
  services I/O VM-exits, returning the observable result.

See `examples/creator-full-stack.sleela` for a runnable demonstration (C and
Java programs through a 6502 executor on a Manager-generation SLVM).

## Build options

Request these on the creator before `create()`/`run()`; they flow into the built
VM and its hosted CPU:

- `requestDMA()` — direct memory-access engine (bulk transfers without the CPU
  copying word by word).
- `requestGPU()` — native GPU access (offload compute kernels to a host GPU over
  the bridge, or a modelled SIMT engine when none is present).
- `requestNativeRam()` — run the VM's linear memory on **real host RAM**
  (`SLNativeMemory`) instead of the simulated `SLRAM` store, for running outside
  simulation for procedural layment.

```
SleelaVMCreator creator = new SleelaVMCreator(); creator.configure();
creator.requestNativeRam();   // run on real host RAM
creator.requestDMA();         // direct DMA
creator.requestGPU();         // native GPU
int out = creator.run(5, cpu, job, 1000000);
```

These correspond to the `DMA`, `GPU`, and `NATIVE_RAM` capability bits in
`../SleelaVMFeatureBits.sleela`. See `examples/native-ram-vm.sleela`.

## Source authority

The .sleela definitions describe what the VM Creator is constructing. Native C/C++ implements required low-level services. The creator must not silently invent modules, capabilities, memory limits, or host authority.
# Running a Sleela-sourced CPU on top of the Secondary Sleela VM

This note documents the layer added so that a CPU written in SleeLa can run as a
real CPU **and** as a model for further `.sleela` inputs, on top of the secondary
Sleela VM (`lib/vm`), which in turn runs on the Native Sleela VM substrate
(`lib/cpu/SLSleelaVM`).

## The execution stack

```
  hardware model      gates -> SLALU -> SLCore -> SLCPU        (lib/cpu)
        |
  Native Sleela VM    SLSleelaVM: the 124 canonical opcodes    (lib/cpu/SLSleelaVM.sleela)
        |
  Secondary Sleela VM SLVM: heap/frames/threads/classes/loader (lib/vm/SLVM.sleela)
        |
  Sleela-sourced CPU  SLCPURuntime + SL<ARCH>CPU               (lib/cpu/SLCPURuntime.sleela)
```

Every layer ultimately executes the **same** canonical opcode stream the native
engine interprets, which is what makes the layering 1:1 in Turing effect
(`SLTuringBridge`).

## What was missing, and what was added

### Secondary VM runtime (`lib/vm`)

The core object model used to be 13 identical empty stubs. They are now real:

| Class | Role |
| --- | --- |
| `SLVMValue` | tagged (tag,payload) runtime value |
| `SLVMHeap` | handle-based object store over `SLRAM` (header + field slots, GC marks) |
| `SLVMMemory` | flat word/byte-addressable linear memory |
| `SLVMObject` | typed handle for reading/writing an instance |
| `SLVMField` / `SLVMMethod` / `SLVMClass` | type metadata, field slots, method code windows |
| `SLVMFrame` | activation record: locals + operand stack + return ip |
| `SLVMThread` | call stack of frames + run state (for `OP_SPAWN`/`OP_JOINALL`) |
| `SLVMModule` | id→class table + the shared `SLSleelaVM` substrate |
| `SLVMModuleLoader` | package lifecycle: discover → define classes → load |
| `SLVMNativeBinding` | audited bridge: parks a VM-exit the host services |
| `SLVM` | the interpreter: owns heap/memory/loader/threads and drives the substrate one opcode at a time, routing CALL/RET and service VM-exits |

A coder can now design and control a Sleela-sourced VM end to end:

```
SLVM vm = new SLVM(); vm.configure("app"); vm.boot(65536, 65536);
SLVMModule m = vm.loader().beginModule("app", 4096, 4096, 1024);
m.emit(1, 2);   // OP_CONST 2
m.emit(1, 3);   // OP_CONST 3
m.emit(8, 0);   // OP_ADD
m.emit(27, 0);  // OP_PRINT
m.emit(48, 0);  // OP_HALT
vm.install(m);
vm.run(1000);   // prints 5 via the native substrate
```

### CPU base layer (`lib/cpu`)

- `SLCPUModel` and `SLCPUArchitecture` were bare name lists; they now carry real
  fields plus a working `validate()` / `addressSpaceWords()`.
- `SLCPURuntime` is new: the runnable base every `SL<ARCH>CPU` extends. It
  supplies register storage (over `SLRAM`), a memory interface, an `SLALU`,
  flags, a fetch–`preFetch`–decode–execute loop, and the lowering surface
  (`emitConst`/`emitAdd`/`emitPrint`/…`runLowered`) that lets a CPU execute **on
  top of the secondary VM** by translating its instructions to canonical opcodes.

A concrete CPU now only has to declare its registers and implement `decode()`:

```
class SL6502CPU extends SLCPURuntime {
  void configure() { bringUp("MOS 6502", 8, 16, 5, 65536); }
  void decode(int word) { /* map 6502 opcodes onto readReg/writeReg/aluOp/... */ }
}
```

## Resource assignment and multi-language workloads

Every CPU can now run a workload written in **C, C++, or Sleela** within an
explicit **resource grant**, via two classes and one inherited method:

- `SLCPUResource` — the grant: memory words, instruction budget (time-slice),
  priority, I/O and privilege permissions, core affinity, plus live accounting.
- `SLWorkload` — a unit of work: a source path + language id + its grant. Its
  `compile()` drives the shared multi-language pipeline (`SLCompilerDriver` →
  `SLProgram`, the language-independent register-ISA image).
- `SLCPURuntime.runWorkload(SLWorkload)` — admission control against the grant,
  then maps the compiled image onto *this CPU's* canonical-opcode substrate
  (`SLOpcodeMap`) and runs it up to the grant's instruction budget.

Because the register-ISA image is language-independent, the **same workload runs
identically whether its source was C, C++, or Sleela** — the 1:1 Turing-effect
guarantee. Example:

```
SLCPUResource grant = new SLCPUResource();
grant.configure(4096, 100000, grant.PRIORITY_NORMAL);   // 4K words, 100k-instr slice

SLWorkload job = new SLWorkload();
job.configure("hello", "examples/hello.c", job.LANG_C, grant);

SL6502CPU cpu = new SL6502CPU(); cpu.configure();
int out = cpu.runWorkload(job);     // compiles C -> maps onto substrate -> runs in grant
```

Admission is enforced: a workload whose memory request exceeds the CPU's address
space, or whose budget is non-positive, is rejected (`runWorkload` returns -1).

## The full nested stack: Creator → SLVM → Sleela CPU → program

`runWorkload()` above runs a CPU on its *own* substrate. The full product stack
nests the CPU inside the Secondary Sleela VM, built by the VM Creator Edition:

```
SleelaVMCreator (lib/vm/creator)
  → builds + boots a live SLVM (lib/vm, the Secondary Sleela VM)
      → which runs on the Native Sleela VM substrate (lib/cpu/SLSleelaVM)
          → hosts a Sleela CPU model (SLCPURuntime / SL<ARCH>CPU) as executor
              → runs a C / C++ / Java / Sleela program through that CPU
```

This is now wired in code (previously it existed only in this document). The
bridge is a **substrate-injection seam**:

- `SLVM.hostCpu(cpu)` creates the VM's `SLSleelaVM` substrate and calls
  `cpu.attachSubstrate(substrate)`.
- `SLCPURuntime.attachSubstrate(...)` records that substrate; `runWorkload()` and
  `mapWorkload()` then lower the program onto the **attached** substrate instead
  of a private `new SLSleelaVM()`. CPU and SLVM therefore share **one** engine —
  the CPU genuinely runs *on top of* the Secondary VM, not beside it.
- `SLVM.runGuestWorkload(job, budget)` has the CPU compile + map the program onto
  the shared substrate, then SLVM drives the opcode stream and services I/O
  VM-exits, surfacing the observable result.

One-call usage through the creator:

```
SLCPUResource grant = new SLCPUResource(); grant.configure(4096, 100000, grant.PRIORITY_NORMAL);
SL6502CPU cpu = new SL6502CPU(); cpu.configure();
SLWorkload job = new SLWorkload(); job.configure("app", "examples/hello.c", job.LANG_C, grant);

SleelaVMCreator creator = new SleelaVMCreator(); creator.configure();
int out = creator.run(5, cpu, job, 1000000);   // generation 5 "Manager"
```

The two feasibility stackings in `PERFECT.CONSEQUENCE.md` correspond exactly to
whether the CPU uses its own substrate (**direct**) or an SLVM-injected one
(**nested**); the nested column is this `hostCpu` path.

## CPU fleet status — all 99 folders runnable

Every architecture folder now carries a runnable `SL<ARCH>CPU` that
`extends SLCPURuntime`, declares a real register file, and implements `decode()`:

- **Hand-written models** (richer, arch-specific decoders): `6502`, `z80`,
  `motorola-68000`, `mips`, `riscv`, `arm`, `x86`, and `pdp8` (12-bit AC/LINK
  semantics). `x86` includes `lowerAddAndPrint()` as a worked lowering example.
- **Generated models** (`tools/generate-cpu-models.py`): the remaining ~90
  folders — the 680x0 family, DEC (Alpha, VAX, the full PDP-1..PDP-15 line),
  Intel (4004, i860, i960, iAPX 432, Itanium), IBM/POWER (801, ROMP, POWER,
  PowerPC, System/360-370-390, z/Architecture), RISC workstation (SPARC,
  PA-RISC, 88000, Am29000, Clipper, NS32000, OpenRISC), embedded/DSP (Cortex-M/R,
  SuperH, Xtensa, Transputer, DSP56000, dsPIC, TMS320, SHARC, Z8000), and all
  the **console SoCs** (Nintendo NES→Switch 2, Sega SG-1000→Dreamcast,
  PlayStation 1→5, Xbox→Series X, Atari 2600→VCS) each modelled on their real
  main CPU.

Each generated model uses a compact, arch-flavored instruction encoding
(load/move/arithmetic/compare/branch/halt) routed through the inherited `SLALU`
and memory. The per-arch bus/timing/register detail continues to live in each
folder's nine markdown docs; `tools/generate-cpu-models.py` is the source of
truth for the generated set and can be re-run or extended with richer decoders.

## Optional acceleration: direct DMA and native GPU access

Two opt-in capabilities span the whole stack (native VM → Secondary VM → CPU →
program). Both default **off** and are turned on only when a program needs them.

- **Direct DMA** (`SLDMAController`): a direct memory-access engine that moves a
  block of words between regions over `SLRAM` **without the CPU copying each
  word** in its fetch-decode-execute loop. Multiple channels, memory↔memory /
  memory↔device modes, overlap-safe copy, and `memcpy`/`fill` helpers.
- **Native GPU access** (`SLGPUDevice`): a compute GPU (compute units × SIMT
  lanes, a core clock, VRAM) the CPU can offload a kernel to. A kernel is a
  canonical-opcode program dispatched across a lane grid; the device uses a real
  host GPU over the bridge (`sleela_gpu_available` / `sleela_gpu_dispatch`) when
  present, else a modelled SIMT engine, so GPU code always runs.

They are exposed as **options** at every layer, and flow down when enabled:

| Layer | Enable | Access |
| --- | --- | --- |
| CPU (`SLCPURuntime`) | `enableDMA()` / `enableGPU()` | `dmaEngine()` / `gpuDevice()`, `dmaCopy(...)` |
| Secondary VM (`SLVM`) | `enableDMA()` / `enableGPU()` (before `hostCpu`) | hosted CPU inherits them; `dmaEngine()`/`gpuDevice()` |
| Creator (`SleelaVMCreator`) | `requestDMA()` / `requestGPU()` | applied to the built VM and its hosted CPU |
| Build (`SLExecutorBuild`) | `enableDMA()` / `enableGPU()` | on the build's CPU |
| Options model (`SleelaVMFeatureBits`) | `DMA` / `GPU` capability bits | feature-mask gating |

Example (Creator grants both; the program uses them):

```
SleelaVMCreator creator = new SleelaVMCreator(); creator.configure();
creator.requestDMA(); creator.requestGPU();
creator.selectGeneration(5); creator.create(); creator.installExecutor(cpu);

cpu.dmaEngine().memcpy(0, 100, 4);     // direct block move, no CPU loop
SLGPUDevice gpu = cpu.gpuDevice();     // offload a compute kernel
gpu.emitKernel(1, 2); gpu.emitKernel(1, 3); gpu.emitKernel(8, 0); gpu.emitKernel(27, 0); gpu.emitKernel(48, 0);
int out = gpu.dispatch(gpu.totalLanes(), 100);
```

See `examples/dma-and-gpu-options.sleela`. Because they are plain options, a
program that never needs them pays nothing; one that needs DMA or the GPU at any
point simply enables it.

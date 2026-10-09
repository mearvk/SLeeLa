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

## CPUs converted in this pass (representative set)

`6502`, `z80`, `motorola-68000`, `mips`, `riscv`, `arm`, `x86` — each upgraded
from a declarative skeleton to a runnable model with real registers, a decoder,
ALU-backed flags, and a memory interface. `x86` additionally includes
`lowerAddAndPrint()` as a worked example of a CPU running on the secondary VM
substrate.

The remaining per-architecture folders still carry their declarative skeleton +
9 markdown docs; they can be converted the same way (declare registers, extend
`SLCPURuntime`, implement `decode()`), general cases first and specific cases as
needed.

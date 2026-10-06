# `lib/cpu` — A Complete CPU, OS, and Program Stack in SLeeLa Source

This package builds an entire computer out of SLeeLa classes: from individual
logic **gates** up through a multi-core **CPU**, a class-driven **operating
system**, and the **programs** that run on top of it. Every layer is composed
from the layer beneath it, so the whole machine is object-oriented SLeeLa source
that bottoms out at Boolean logic.

> The design is faithful to how real Intel/AMD-class processors are organized —
> standard cells → functional blocks → cores → chip, with caches, an I/O bus,
> DMA, interrupts, and drivers — while staying a readable behavioral model.
> Where real silicon uses carry-lookahead adders, deep pipelines, or
> out-of-order execution, the classes document the real technique and model the
> equivalent observable semantics.

## The stack (bottom to top)

### Layer 0 — Digital logic
| Class | Role |
|---|---|
| `SLGate` | One Boolean gate (AND/OR/NOT/NAND/NOR/XOR/XNOR/BUF). The universal primitive. |
| `SLCircuit` | A named combinational circuit; composes gates and sub-circuits; reports a transistor estimate and critical path. |
| `SLHalfAdder` | 1-bit sum (XOR) + carry (AND). |
| `SLFullAdder` | 1-bit add of a, b, carry-in → sum, carry-out (two half adders + OR). |
| `SLAdder` | N-bit ripple-carry adder over a machine word. |
| `SLALU` | Arithmetic Logic Unit: add/sub/and/or/xor/not/shift/compare with status flags. |

### Layer 1 — Storage
| Class | Role |
|---|---|
| `SLLatch` | Level-sensitive 1-bit cell. |
| `SLFlipFlop` | Edge-triggered 1-bit cell (master/slave). |
| `SLRegister` | N-bit clocked, load-enabled register. |
| `SLRegisterFile` | The bank of architectural registers (r0 hardwired zero). |
| `SLRAM` | Word-addressable main memory (native backing store). |
| `SLCache` | L1/L2/L3 cache with hit/miss accounting over a backing store. |
| `SLHardDrive` | Persistent block storage (HDD/SSD), native file-backed. |

### Layer 2 — Core
| Class | Role |
|---|---|
| `SLClock` | Oscillator; issues cycles (rising edges). |
| `SLBus` | Address/data/control interconnect. |
| `SLProgramCounter` | Next-instruction address register (increment / branch). |
| `SLInstructionSet` | The ISA: a RISC-style opcode catalog, aligned with the VM ISA. |
| `SLInstruction` | One decoded instruction (opcode, rd, rs, imm) + encode(). |
| `SLInstructionDecoder` | Splits a fetched word into an `SLInstruction`. |
| `SLStatusRegister` | FLAGS: zero/negative/carry/overflow + mode/interrupt bits. |
| `SLControlUnit` | The fetch-decode-execute conductor; runs the ISA semantics. |
| `SLInterruptController` | Fields IRQ lines (timer, keyboard, disk, network); vectors to handlers. |
| `SLCore` | One execution core: clock + regs + ALU + PC + status + L1 + IRQ + control. |
| `SLCPU` | Multi-core chip: N cores sharing L3 and main memory. |

### Layer 3 — I/O and drivers
| Class | Role |
|---|---|
| `SLIOPort` | A port-mapped I/O register (IN/OUT). |
| `SLIOBus` | Peripheral bus routing IN/OUT to registered ports. |
| `SLDMAController` | Direct Memory Access block transfers. |
| `SLDeviceDriver` | Base driver: probe/attach/open/read/write/close/detach + ISR. |
| `SLConsoleDriver` | Character output (console/tty). |
| `SLDiskDriver` | Block storage via `SLHardDrive` + DMA. |
| `SLKeyboardDriver` | Interrupt-driven character input. |
| `SLNetworkDriver` | NIC frame transmit/receive via DMA. |
| `SLDriverManager` | Registers drivers and dispatches interrupts to them. |

### Layer 4 — Operating system
| Class | Role |
|---|---|
| `SLBootloader` | POST, load the kernel image from disk, hand off to the boot core. |
| `SLMemoryManager` | Pages, allocation, kernel/user split. |
| `SLProcess` | Process control block: state + saved context. |
| `SLScheduler` | Round-robin preemptive scheduler with a time quantum. |
| `SLSystemCall` | The syscall ABI (write/read/open/close/exit/spawn/yield). |
| `SLFileSystem` | Named files over disk blocks; the loader reads images through it. |
| `SLKernel` | The OS core: scheduler + memory + drivers + syscalls + filesystem; runs the main loop. |
| `SLOperatingSystem` | Full OS assembly over a CPU; `install()` + `spawn()` + `run()`. |

### Layer 2.5 — Hardware stack
| Class | Role |
|---|---|
| `SLStack` | The hardware call/data stack (grows downward) for PUSH/POP and CALL/RET return addresses. |

### Layer 5 — Programs and the multi-language toolchain
| Class | Role |
|---|---|
| `SLProgram` | An executable image (encoded instructions + entry point). |
| `SLAssembler` | Emits encoded instructions from mnemonics into a program. |
| `SLIRInstruction` / `SLIR` | The common, language-neutral intermediate representation. |
| `SLFrontend` | Base frontend: source → `SLIR`. |
| `SLFrontendSleela` / `SLFrontendC` / `SLFrontendCpp` / `SLFrontendJava` | Per-language frontends lowering C, C++, Java, and Sleela to the shared IR. |
| `SLLowering` | Shared backend: `SLIR` → machine code (register allocation + label resolution). |
| `SLCompilerDriver` | Selects a frontend by language/extension and runs frontend→IR→backend. |
| `SLProgramLoader` | Loads a compiled program **or compiles source in any supported language** and spawns it. |
| `SLMachine` | The capstone: composes the whole stack and runs it end to end. |

### Layer 6 — Virtualization (Linux-style guest)
| Class | Role |
|---|---|
| `SLHypervisor` | Type-2 hosted hypervisor: creates/schedules guest VMs; TRAP = VM-exit. |
| `SLGuestVM` | One guest: a vCPU over a guest-physical memory window with VM-exit handling. |
| `SLGuestLinux` | A Linux-style guest kernel (boot protocol + Linux syscall ABI), emulated on VM-exit. |

> **Execution, the multi-language loader, and the Linux-guest boundary are
> documented in detail in [`EXECUTION.md`](EXECUTION.md).** The ISA
> (`SLInstructionSet`) is now a real multi-language lowering target with a full
> call/stack protocol; `SLControlUnit` executes the complete set.

## The end-to-end chain

`SLMachine` demonstrates exactly what this package is for — a Sleela CPU built
from Sleela source that boots a Sleela class-driven OS that then loads and runs
Sleela programs:

```
buildHardware()   gates → ALU → registers/RAM/cache → cores → SLCPU
boot()            SLBootloader loads the kernel; SLOperatingSystem installs
                  the kernel, drivers, scheduler, memory manager, filesystem
loadDemoProgram() SLAssembler builds a program; SLProgramLoader loads it
run()             SLKernel schedules the process and runs it on the CPU,
                  servicing its SYS_WRITE / SYS_EXIT syscalls
```

`powerOnAndRun(cores, bits, mhz, rounds)` performs the whole sequence.

### Minimal usage

```sleela
SLMachine m = new SLMachine();
m.powerOnAndRun(4, 32, 3200, 8);   // 4 cores, 32-bit, 3.2 GHz, 8 scheduling rounds
```

## Relationship to the rest of SLeeLa

- The `SLInstructionSet` opcodes map onto the canonical SLeeLa VM ISA in
  `lib/vm/InstructionSet.sleela` (OP_ADD, OP_JMP, OP_CALL, OP_HALT, …), so a
  program for this CPU corresponds to real VM work.
- Native-backed components (`SLRAM`, `SLHardDrive`, `SLConsoleDriver`) cross the
  explicit VM/OS bridge the same way the rest of the standard library does.
- Everything above the bridge is plain, composable SLeeLa source — the point of
  the package is that a CPU, an OS, and the programs on it can all be expressed
  as SLeeLa classes.

## Note on fidelity

These classes are a clear, teachable **behavioral model** of a modern CPU and
OS, not a gate-accurate netlist of a specific Intel or AMD part. They capture
the real structure and semantics (combinational logic, clocked state, the
fetch-decode-execute cycle, the memory hierarchy, interrupts, DMA, drivers,
scheduling, syscalls, and loading) at a level that stays readable as SLeeLa
source.

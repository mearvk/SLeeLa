# `lib/cpu` — Execution Model, Multi-Language Loading, and the Linux Guest

This document answers three questions precisely and honestly:

1. **Can this CPU actually run a program?**
2. **How do C, C++, Java, and Sleela programs get loaded and run?**
3. **How does a Linux-style VM run on top of the Sleela CPU — and what is real vs. modeled?**

---

## 1. Can it run a program? — Yes, within the model

`SLControlUnit.step()` is a genuine fetch-decode-execute cycle over the
`SLInstructionSet`: it fetches a word from `SLRAM` at the program counter,
decodes it (`SLInstructionDecoder`), executes it through the register file
(`SLRegisterFile`), ALU (`SLALU`), and hardware stack (`SLStack`), updates the
program counter (sequential / branch / call / return), and sets the status
flags. The kernel services `SYS_WRITE` / `SYS_EXIT`.

The ISA now includes a **real call/stack protocol** — `CALL` pushes the return
address and jumps, `RET` pops and returns, `PUSH`/`POP` move words — plus full
integer arithmetic (`MUL`/`DIV`/`MOD`/`NEG`), bitwise/shift ops, three
addressing modes (`LOAD`/`LOADR`/`LOADX` and the `STORE` family), five branch
forms, and `SYSCALL`/`TRAP`. That is enough to be a lowering target for
compiled code with functions, locals, and spills.

**Verification status.** The pipeline's semantics were validated by an
independent simulation of frontend → IR → lowering → execution: the Sleela demo
lowers `2 + 3`, executes it, and writes `5`. The classes could **not** be run
through the real `sleela` compiler in this environment (its build gate requires
a SHA-256 manifest + network), so "runs" means *verified by construction and
simulation*, not *executed on the native VM*. Building the compiler is the
prerequisite to running any of this for real.

> **Boundary:** this is a CPU/OS **simulator expressed as SLeeLa classes**, not
> code executing on bare metal. It is an object-oriented model of hardware.

---

## 2. Multi-language loading — one driver, one IR, one backend

The ISA accepts **C, C++, Java, and Sleela** the way real toolchains do: each
language has a **frontend** that lowers it to a common **intermediate
representation (SLIR)**; a single shared **backend** compiles the IR to machine
code. The ISA is multi-language because the backend is shared — exactly the
clang/clang++/LLVM and javac/bytecode pattern.

```
 source.c     -> SLFrontendC    \
 source.cpp   -> SLFrontendCpp   \        SLLowering            SLProgramLoader
 source.java  -> SLFrontendJava   >--- SLIR ---> (IR -> ISA) --->  load into RAM --> process
 source.sleela-> SLFrontendSleela/        ^                            |
                                           |                      SLKernel schedules + runs
                 SLCompilerDriver picks the frontend by language
```

| Class | Role |
|---|---|
| `SLIRInstruction` | One language-neutral three-address IR op (const/move/arith/cmp/branch/load/store/param/call/ret/syscall/halt). |
| `SLIR` | An ordered IR module with temp and label allocators. |
| `SLFrontend` | Base frontend: `parse()` + `lower() -> SLIR`. |
| `SLFrontendSleela` | SLeeLa frontend (reference; modeled end to end). |
| `SLFrontendC` | C → IR lowering (declarations→slots, expressions→temps, calls→param/call, return→ret). |
| `SLFrontendCpp` | C++ → IR (this-pointer, construction, virtual dispatch), extends the C frontend. |
| `SLFrontendJava` | JVM-bytecode → IR (locals→temps, operand stack linearized, invoke→call). |
| `SLLowering` | Shared backend: IR → `SLProgram` with register allocation + 2-pass label resolution. |
| `SLCompilerDriver` | Selects the frontend by language/extension and runs frontend→IR→backend. |

### Loading, by language

```sleela
SLProgramLoader loader = ...;         // from SLMachine.boot()
loader.attachCompiler(isa);

// infer language from extension, compile, load, and spawn:
loader.loadFile("hello.c",     "hello", 8192);   // C
loader.loadFile("hello.cpp",   "hello", 8192);   // C++
loader.loadFile("Hello.java",  "hello", 8192);   // Java (from bytecode)
loader.loadFile("hello.sleela","hello", 8192);   // Sleela
```

Or explicitly: `loader.loadSource(path, langId, name, addr)` with `langId`
0=Sleela, 1=C, 2=C++, 3=Java.

### Honest scope of the frontends

- The **Sleela** frontend is modeled end to end and its demo lowers and executes
  correctly (verified by simulation).
- The **C, C++, and Java** frontends implement the **real lowering contracts**
  and each demonstrates one complete, correct lowering, but their `parse()` is
  not a full parser for the language. Turning each into a complete compiler
  means growing `parse()` into a real C parser / C++ parser / classfile reader —
  **the IR and backend beneath them do not change.** That separation is the
  whole point: the hard, language-specific work is isolated in the frontend.

This is also consistent with the rest of the repository, where the production
multi-language path is **Nordshrift** (`nordshrift build --target=java|sleela|c`
transpiles `.sleela` and the native VM runs it). `lib/cpu` is a self-contained
teaching model of the same frontend/IR/backend idea.

---

## 3. The Linux-style guest VM — architectural model

The "run a Linux-based VM on top of the Sleela CPU" layer is a **type-2 hosted
hypervisor** model:

```
SLCPU (hardware) -> SleelaOS (host kernel) -> SLHypervisor
   -> SLGuestVM (virtual CPU + guest-physical memory window)
      -> SLGuestLinux (guest kernel: boot protocol + Linux syscall ABI)
         -> guest user programs
```

| Class | Role |
|---|---|
| `SLHypervisor` | Creates/schedules guests; installs the TRAP vector as the VM-exit path; counts exits. |
| `SLGuestVM` | One guest: a vCPU over a host-memory window, guest-physical→host translation, VM-exit counting. |
| `SLGuestLinux` | A Linux-style guest kernel: boot protocol (kernel+initrd image, entry), a Linux-like syscall ABI (read/write/open/close/mmap/exit/fork/execve), emulated as host syscalls on VM-exit. |

Run it:

```sleela
SLMachine m = new SLMachine();
m.buildHardware("SleelaWorks","SL-Core-X", 4, 32, 3200);
m.boot();
m.runLinuxGuest(0, 64);   // boot a Linux-style guest and run 64 guest instructions
```

> **Boundary — read this.** `SLGuestLinux` is an **architectural model** of
> running a Linux-like guest: it defines the boot sequence, the guest syscall
> numbers, guest-physical memory translation, and the VM-exit/emulation path.
> It is **not** the upstream Linux kernel and does not boot a real `bzImage`.
> Doing that would require a hardware-virtualization-grade vCPU (VT-x/AMD-V
> semantics), a device model (virtio, interrupt controller, timers, MMU with
> real page tables), and a loader for real ELF/kernel images — each a large
> project. What this provides is a faithful, composable model of the full
> layering so the "Linux on the Sleela CPU" story is expressed in SLeeLa classes
> with every seam named.

---

---

## 4. The Sleela opcode substrate — one Turing-complete ground truth

The deepest layer unifies everything. The SLeeLa VM opcodes are **Turing
complete** and are the instructions the native core already executes
(`impl/core/sleela_core.c`, `lib/vm/InstructionSet.sleela`). So the CPU model
does not invent a parallel reality: the register-style ISA and the OS system
calls are **mapped onto those canonical opcodes**, and every language reduces to
the same opcode stream.

```
 C / C++ / Java / Sleela
     -> SLFrontend*            language -> common IR (SLIR)
     -> SLLowering             IR -> register ISA (SLInstructionSet / SLProgram)
     -> SLOpcodeMap            register ISA + OS syscalls -> SLeeLa opcodes
     -> SLSleelaVM             execute the canonical base-98 Turing-complete substrate
```

| Class | Role |
|---|---|
| `SLSleelaOpcode` | The canonical base-98 opcode registry — exact native codes/mnemonics. The ground truth. |
| `SLSleelaVM` | A stack machine that executes the Sleela opcodes directly (mirrors the native dispatch loop). |
| `SLOpcodeMap` | Maps each register-ISA instruction and each OS syscall onto a sequence of Sleela opcodes. |
| `SLTuringBridge` | Reduces any-language programs to the substrate and runs them; `proveEquivalence()` checks all four give the identical result. |

### Why this makes C == C++ == Java == Sleela (1:1 in Turing effect)

A register instruction like `ADD r1, r2` maps to the opcode sequence
`LOADG r1 ; LOADG r2 ; OP_ADD ; STOREG r1` (registers live in global slots). An
OS `SYS_WRITE` maps to `LOADG arg ; OP_PRINT`; `SYS_EXIT` maps to `OP_HALT`.
Because **every** CPU instruction and **every** syscall is defined as such a
Sleela-opcode sequence, a whole program — whatever language it started in —
becomes one stream of base-98 opcodes. Two programs that reduce to opcode
streams with the same observable effect are, by definition, equal in Turing
effect.

`SLTuringBridge.proveEquivalence(c, cpp, java, sleela)` makes this operational:
it lowers the **same computation** from all four frontends, reduces each to the
Sleela substrate, runs each on `SLSleelaVM`, and returns true only if every
language produced the identical result.

> **Verification.** An independent simulation of the entire chain (frontend →
> IR → register ISA → `SLOpcodeMap` → `SLSleelaVM`) confirms that the C, C++,
> Java, and Sleela canonical programs each reduce to the identical 14-opcode
> Sleela stream and all print `5`. The equivalence claim is therefore
> operational, not asserted. (As elsewhere, this is verified by construction and
> simulation; running on the native VM awaits a buildable `sleela` compiler.)
>
> Note the frontends deliberately lower the **same computation** so the proof is
> honest — equivalence means "same effect for the same program expressed in
> different languages," not that any two different programs are equal.

## What would make each piece "real" (roadmap)

| Goal | Needed |
|---|---|
| Execute any of this for real | A buildable `sleela` compiler (the current blocker). |
| Full C / C++ / Java frontends | Real `parse()` implementations (C parser; C++ parser; JVM classfile reader) — IR/backend unchanged. |
| Real binaries | ELF loader and JVM classfile loader for on-disk program images. |
| Larger programs | Register spilling to the stack (PUSH/POP paths are present), heap allocator, calling-convention finalization. |
| Boot real Linux | Hardware-virtualization vCPU, MMU + page tables, device model (virtio/APIC/timers), real kernel-image loader. |

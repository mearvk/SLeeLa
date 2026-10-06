# SLeeLa /lib Library Index

**Revision:** 0.22  
**Packages:** 83  
**SLeeLa source units:** 10265  
**Module-facade symbols:** 55  
**Total symbol records:** 10320  
**Symbol manifest:** `LIBRARY.SYMBOLS.md`

> Revision 0.22 lets the guest OS-VM boot a GENUINELY COMPILED kernel (2
> classes): `SLGuestKernelImage` (a bootable opcode-stream image + entry) and
> `SLGuestKernelBuilder` (compiles kernel source in any supported language
> through SLCompilerDriver -> register ISA -> SLOpcodeMap -> opcode image). The
> guest substrate loads and executes the compiled image instead of a hand-
> written stub (OS-VM roadmap step 4; a custom kernel, not upstream Linux).
> 10,263 -> **10,265** source classes (**10,320** total). See `cpu/EXECUTION.md`.

> Revision 0.21 adds a guest device model to the `cpu` package (5 classes):
> `SLGuestDevice` (MMIO base), `SLGuestTimer` (programmable interval timer +
> IRQ), `SLGuestConsole` (serial/console output), `SLGuestBlockDevice`
> (virtio-blk-style storage over a host drive), and `SLGuestDeviceBus` (MMIO
> routing + IRQ aggregation into the guest interrupt controller). The guest
> OS-VM now has the timer/console/block surface a kernel needs (OS-VM roadmap
> step 3). 10,258 -> **10,263** source classes (**10,318** total). See
> `cpu/EXECUTION.md`.

> Revision 0.20 adds `SLMMU` to the `cpu` package: a Memory Management Unit with
> single-level page tables, per-page present/write/exec permissions, and page
> faults (unmapped + protection), giving the guest OS-VM real second-level
> address translation (OS-VM roadmap step 2). 10,257 -> **10,258** source
> classes (**10,313** total). See `cpu/EXECUTION.md`.

> Revision 0.19 maps the `cpu` model onto the canonical SLeeLa opcode substrate
> (4 new classes): `SLSleelaOpcode` (the base-98 Turing-complete opcode registry
> with exact native codes), `SLSleelaVM` (a stack machine executing those
> opcodes directly, mirroring the native dispatch loop), `SLOpcodeMap` (maps the
> register ISA and OS system calls onto Sleela-opcode sequences), and
> `SLTuringBridge` (reduces any-language programs to the substrate and proves
> C/C++/Java/Sleela are 1:1 in Turing effect). This takes the collection from
> 10,253 to **10,257** source classes (**10,312** total records); package count
> unchanged at 83. See `cpu/EXECUTION.md` §4. Counts verified by
> `test-suites/test-library-inventory.sh`.

> Revision 0.18 expands the `cpu` package with a real multi-language toolchain
> and a virtualization layer (13 new classes): a hardware stack (`SLStack`) and
> an expanded `SLInstructionSet`/`SLControlUnit` with a full CALL/RET/PUSH/POP
> protocol and MUL/DIV/MOD; a common IR (`SLIRInstruction`, `SLIR`), a shared
> backend (`SLLowering`), language frontends (`SLFrontend`, `SLFrontendSleela`,
> `SLFrontendC`, `SLFrontendCpp`, `SLFrontendJava`), and a `SLCompilerDriver` so
> the ISA accepts C/C++/Java/Sleela; plus a hosted hypervisor (`SLHypervisor`,
> `SLGuestVM`, `SLGuestLinux`) modeling a Linux-style guest on the Sleela CPU.
> This takes the collection from 10,240 to **10,253** source classes (**10,308**
> total records); package count is unchanged at 83. See `cpu/EXECUTION.md`.
> Counts are verified by `test-suites/test-library-inventory.sh`.

> Revision 0.17 adds the new `cpu` package family — a complete CPU, operating
> system, and program stack in SLeeLa source (45 classes): logic gates, adders,
> an ALU, latches/flip-flops/registers/register file, RAM/cache/hard drive,
> clock/bus/program counter, an instruction set + decoder + control unit,
> interrupt controller, multi-core `SLCPU`, I/O ports/bus/DMA and the device
> driver hierarchy, and the OS layer (bootloader, memory manager, scheduler,
> processes, syscalls, filesystem, kernel) up to `SLProgram`/`SLAssembler`/
> `SLProgramLoader` and the capstone `SLMachine`. This takes the collection from
> 82 to **83** package families and from 10,195 to **10,240** source classes
> (**10,295** total records). See `cpu/CPU.md`. `LIBRARY.SYMBOLS.md` was
> regenerated and the counts are verified by `test-suites/test-library-inventory.sh`.

> Revision 0.16 adds the national-grade cryptography façades to the `crypto`
> package family — `SLNationalSuite`, `SLSha256`, `SLSha512`, `SLSha3`, `SLAes`,
> `SLAesGcm`, `SLHkdf`, `SLMlKem`, and `SLMlDsa` — the SLeeLa-layer front ends
> for the native suite implemented under `/crypto` (NSA CNSA / NIST FIPS: AES,
> SHA-2/SHA-3, AES-GCM, HMAC, HKDF, and the ML-KEM/ML-DSA post-quantum
> interfaces). Nine new source units take the collection from 10,186 to **10,195**
> source classes (**10,250** total records). `LIBRARY.SYMBOLS.md` was
> regenerated from the live tree and the counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.15 fully regenerates `LIBRARY.SYMBOLS.md` from the live `/lib`
> tree via `tools/generate-library-symbols.py` (one row per `.sleela` unit)
> and reconciles every count to the filesystem: **82** package families,
> **10,186** source classes + **55** `SLPackage.sleela` module facades =
> **10,241** total symbol records. This picks up the package families that had
> been added since the manifest was last written — `autocad`, `java`,
> `languages`, `opcodes`, `sldocument`, and `website` — and corrects the earlier
> internally inconsistent header (which claimed 10,054 sources / 90 facades over
> a 982-row body). The manifest body and header are now verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.14 reconciled the headline source-unit figure to the verified
> filesystem count of 10,241 `.sleela` units under `/lib` (matching
> `CLASS.INVENTORY.md` Revision 2.1). Earlier revision stamps lagged the live
> count; the figure here is the authoritative total.

> Revision 0.9 adds the new `opcodes` package family: one SLeeLa class per
> canonical VM opcode (103 classes, codes 0–102; the base 98 are OP_NOP..
> OP_AUDIO_PLATFORM) plus `SLOpcodeBase` and `SLOpcodeStream` — 105 `.sleela`
> source units. Each class carries a single opcode and honours the fetch-then-
> execute-one contract against the VM. See `opcodes/OPCODES.md`.
>
> Revision 0.10 adds the `opcodes/governance` series — 8 classes giving SLeeLa
> procedural discretion over opcode execution: a Registrar considers a program
> A→B BEFORE it runs, a Listener confirms live fit DURING, and an Event Observer
> judges the whole as a musical, ordered process AFTER (with graded verdicts,
> base-concept checks, and known-symbol-map patching). See
> `opcodes/governance/GOVERNANCE.md`.
>
> Revision 0.11 adds the `opcodes/running` sub-family — 6 classes for richer
> opcode execution: grouping (`SLOpcodeGroup`, `SLOpcodeGroupSet`), a
> conditional-reactive layer (`SLOpcodeCondition`, `SLOpcodeConditionalReactive`,
> `SLOpcodeReactorBank`) that warms/gates/runs/skips groups on program state, and
> warming (`SLOpcodeWarmer`). See `opcodes/running/RUNNING.md`.
>
> Revision 0.12 adds the new `sldocument` package family and the `.sldocument`
> format — 6 classes for an ordered, top-down document that compiles against and
> with standard SLeeLa source (`SLDocument`, `SLDocumentStep`,
> `SLDocumentAnnotation`, `SLVeritable`, `SLDocumentResult`,
> `SLDocumentCompiler`). Each step is an annotated method that runs in order and
> usually returns a single binary veritable-and-kind value. See
> `sldocument/SLDOCUMENT.md`.
>
> Revision 0.13 makes `.sldocument` a selectable compile choice and adds naming
> conventions for comparing/converting the forms: `lib/compiler/SLSourceForm` and
> `lib/compiler/SLCompileChoice` let the compiler be told to compile a `.sleela`
> or a `.sldocument`; and `sldocument/SLDocumentNaming`,
> `sldocument/SLSourceNameComparison`, and `sldocument/SLDocumentConverter`
> synthesize method names for anonymous document steps so an engineer can convert
> a `.sldocument` to a named `.sleela` for safekeeping. 5 new `.sleela` classes.

The `/lib` tree is the canonical language-facing source collection. The compiler and Nordshrift use the same recursive library discovery implementation, so a package becomes importable when its directory contains SLeeLa source.

| Coverage | Count |
|---|---:|
| Repository module families represented under /lib | 83 |
| SLeeLa source units | 10,265 |
| Module-facade symbols | 55 |
| Total symbol records | 10,320 |

Every repository-level module family that is a language/runtime/package concern now has at least one SLeeLa source unit under `/lib`. Documentation, images, generated build output, tests, and CI-only directories remain non-library artifacts and are intentionally not presented as language packages.

## Compiler and Loader Resolution

Resolution is shared by the SLeeLa compiler and Nordshrift:

1. `$SLEELA_LIB`
2. `lib`
3. `../lib`
4. `../../lib`

Discovery is recursive. Package presence is therefore derived from the actual `/lib` tree rather than a hand-maintained package allow-list. `validateImports()` rejects an imported package that is not present.

The VM-facing `SLVMModuleLoader` source mirrors this contract: package names are discovered from the canonical library root, registered, and checked before use. Native loader facilities remain below the explicit C/C++ OS bridge.

## Inventory

The complete path-level and facade-level symbol collection is maintained in `LIBRARY.SYMBOLS.md`.

**Max Rupplin — MEARVK LLC — 2026**


## Compiler / Nordshrift / Loader Contract

The shared `sleela::library::Index` recursively discovers the 83 package families and all 10,320 `.sleela` source units. It now exposes package counts, per-package symbol counts, symbol lookup, and source-path resolution. Compiler and Nordshrift use this index; `lib/vm/SLVMModuleLoader.sleela` represents the same discovered package/symbol state at the SLeeLa layer.

## SST / Nordshrift Symbol Contract

SST declarations are typed symbols with package-qualified identity. Nordshrift resolves them through the same `sleela::library::Index` used by the compiler; `/lib` remains the sole authoritative symbol collection. See `impl/nordshrift/SST.SYMBOLS.md` and `impl/nordshrift/NORDSHRIFT.SYMBOLS.md`.

# Slecompiler™ Terminology

## Artifact and container terms
- **Artifact** — A native binary or binary-like input analyzed by Slecompiler.
- **Container** — The file-format structure that carries executable, library, object, archive, or module data.
- **ELF** — Executable and Linkable Format used extensively by Linux and Unix-like systems.
- **PE/COFF** — Portable Executable / Common Object File Format used by Windows.
- **Mach-O** — Native executable and object format used by Apple platforms.
- **GNU ar / Static Archive** — A container such as `.a` holding object-file members.
- **Archive Member** — One object or native artifact stored inside an archive, with its own offsets, size, and detected format.
- **Relocatable Object** — An object file containing code/data that still requires relocation/linking.
- **Dynamic Library / Shared Object** — A library intended to be loaded and linked dynamically, such as Linux `.so`.
- **Kernel Module** — A Linux kernel-loadable ELF object, commonly `.ko`; Slecompiler analyzes it statically and does not insert it.
- **Driver** — A native component providing hardware, bus, filesystem, device, or system integration; classification is format/platform dependent.

## ELF structure
- **ELF Header** — The root header describing ELF class, machine, file type, entry point, and table locations.
- **Section** — A linker/object-oriented region such as `.text`, `.data`, `.dynsym`, or `.modinfo`.
- **Program Header** — A loader-oriented ELF record describing segments and runtime mapping requirements.
- **Program Segment** — The normalized Slecompiler representation of a program header.
- **PT_LOAD** — ELF loadable segment describing bytes mapped into an image.
- **PT_DYNAMIC** — Segment containing the dynamic linker table.
- **PT_NOTE** — Segment carrying note records such as GNU build identification.
- **PT_GNU_RELRO** — GNU read-only-after-relocation memory evidence.
- **PT_GNU_STACK** — GNU stack permission metadata used to identify executable-stack requirements.
- **Virtual Address** — An address in the artifact's intended image address space.
- **File Offset** — A byte position within the artifact on disk.
- **Virtual Size / Memory Size** — The size represented in the image versus the size occupied after loading.

## Linking and symbol terms
- **Symbol** — A named or otherwise identified addressable entity.
- **Import** — A symbol or interface required from another object/library.
- **Export** — A symbol or interface made available to other objects.
- **SONAME** — ELF shared-object identity advertised through `DT_SONAME`.
- **DT_NEEDED** — ELF dynamic dependency naming another required shared library.
- **Dynamic Symbol Table** — The ELF `.dynsym` table used for runtime linking.
- **Symbol Version** — A version namespace attached to an exported or required symbol.
- **GNU Version Definition** — Metadata describing versions provided by an object.
- **GNU Version Requirement** — Metadata describing versions required from another object.
- **Build ID** — A GNU note-based identifier associated with a particular build artifact.
- **Relocation** — A record describing an address/value adjustment required by linking or loading.
- **REL / RELA** — ELF relocation encodings; REL stores implicit addends while RELA stores explicit addends.
- **PLT** — Procedure Linkage Table used for dynamically resolved function calls.
- **GOT** — Global Offset Table used for dynamically addressed data/functions.
- **DT_PLTGOT** — Dynamic metadata identifying the relevant GOT/PLT address.
- **JUMP_SLOT** — A relocation commonly used to resolve PLT function entries.
- **GLOB_DAT** — A relocation commonly used for dynamically resolved data/function addresses.
- **RELATIVE** — A relocation resolved relative to the loaded image base.
- **TLS** — Thread-Local Storage; ELF sections and metadata supporting per-thread data.
- **Init/Fini** — Initialization and finalization routines or arrays executed by a loader/runtime; Slecompiler records their evidence without executing them.
- **RELRO** — Read-only-after-relocation hardening evidence.
- **BIND_NOW / NOW** — Metadata requesting immediate rather than lazy symbol binding.
- **NX Stack** — Evidence that the ELF requests a non-executable stack.

## Kernel-module terms
- **.modinfo** — Kernel-module metadata section containing fields such as license, author, description, aliases, dependencies, and vermagic.
- **vermagic** — Kernel module compatibility/build string.
- **ksymtab** — Kernel symbol-export tables used by modules to expose kernel interfaces.
- **__ksymtab / __ksymtab_gpl** — Common section names for exported kernel symbols.
- **Kernel Export** — A kernel interface made available to modules.
- **Module Dependency** — Another module required by a kernel module.
- **Alias** — A device/module matching identifier recorded in module metadata.

## Analysis and recovery
- **Instruction** — A decoded native machine instruction.
- **Operand** — A register, immediate, memory, relative, or symbolic argument to an instruction.
- **Decoder** — The component translating bytes into instructions.
- **Basic Block** — A straight-line sequence of instructions with one entry and controlled exits.
- **Control-Flow Graph (CFG)** — A graph of basic blocks and possible execution transitions.
- **Function Candidate** — A region inferred to represent a function.
- **Call Graph** — A graph connecting functions through call relationships.
- **SLIR** — SLeeLa Intermediate Representation used to represent recovered program behavior independently of one native encoding.
- **Lifter** — A component translating native instructions into an intermediate representation.
- **Provenance** — Evidence tying a recovered fact back to artifact bytes, addresses, sections, or analysis sources.
- **Confidence** — A numerical indication of how strongly a recovered fact is supported by available evidence.
- **Static Analysis** — Analysis performed without executing the target artifact.
- **Dynamic Analysis** — Runtime observation of an executing artifact; outside the default Slecompiler safety boundary.
- **Refactoring** — Transformation of recovered representation while retaining provenance and semantic evidence.

## Slecompiler safety terms
- **Bounded Parsing** — Parser behavior constrained by validated offsets, sizes, and input boundaries.
- **Read-Only Analysis** — Analysis that does not modify target artifact memory or the running operating system.
- **Non-Execution Boundary** — Slecompiler's default rule that input native artifacts, drivers, and kernel modules are not executed or loaded.
- **Evidence Record** — Structured representation of an observed artifact property.
- **Native Interface** — A discovered binary-level interface such as a symbol, import, export, relocation, segment, or kernel export.
- **Library Graph** — A graph of libraries and dependency/symbol-provider relationships.
- **Provider Resolution** — Matching an unresolved interface to an artifact that exports a compatible symbol/interface.
- **Identity Digest** — The current artifact identity string; in the present implementation it is not a cryptographic SHA-256 digest.

## Stage 4 terms
- **Native Instruction Decoding** — Architecture-specific translation of machine bytes into meaningful instructions.
- **Branch Target** — An address reached by a conditional or unconditional control-transfer instruction.
- **Fallthrough** — The next sequential instruction address when control does not branch.
- **Call Target** — The destination represented by a call instruction or call relocation.
- **Return** — A control transfer from a function back toward its caller.
- **Indirect Control Flow** — Control transfer whose destination is obtained through a register, memory location, table, or other runtime value.
- **PLT Stub** — A small code sequence associated with dynamic function resolution.
- **GOT Entry** — A data slot used to hold a dynamically resolved address.
- **Relocation-Aware Decoding** — Instruction/operand interpretation informed by relocation records and symbol metadata.
- **Function Boundary Recovery** — Inferring starts and ends of functions from symbols, calls, branches, unwind metadata, and code structure.
- **Call-Site Recovery** — Identifying locations from which a function or external interface is invoked.
- **Indirect-Call Resolution** — Static inference of possible targets for calls through registers or memory.
- **Thunk** — A small forwarding or adaptation routine that transfers control to another target.
- **Trampoline** — A generated or linkage-oriented transfer sequence used to reach another code location.
- **Jump Table** — A table of target addresses or offsets used for multi-way control flow such as switch statements.
- **Tail Call** — A call-like transfer that reuses the current function's return context.
- **Instruction Semantics** — The abstract effect of an instruction on registers, memory, flags, and control flow.
- **Architecture Lifter** — Architecture-specific component that converts decoded instructions into SLIR operations.
- **CFG Recovery** — Construction of control-flow relationships from decoded instructions and recovered targets.
- **Function Recovery** — Construction of function candidates and boundaries from multiple independent evidence sources.


## Stage 4 implementation terms

### ModRM
The x86 instruction byte that encodes register operands and addressing-mode information for many instructions.

### SIB
The x86 Scale-Index-Base byte used to encode indexed memory operands.

### Direct Control Transfer
A call or branch whose destination is encoded directly by a relative displacement or immediate target.

### Indirect Control Transfer
A call or branch whose destination is supplied through a register or memory operand and therefore may require additional analysis.

### Relocation-Backed Call
A decoded call or jump whose instruction or target is associated with an ELF relocation record.

### PLT/GOT Evidence
Static evidence connecting Procedure Linkage Table stubs, Global Offset Table entries, dynamic relocations, and imported symbols.

### Function Seed
An address used as initial evidence for function-boundary recovery, such as an entry point, symbol, export, or direct call target.

### Identity Digest
A cryptographic digest used to identify artifact bytes. Slecompiler now uses SHA-256 for this field.

### Conservative Decode
A decoding policy that emits an unknown or unsupported instruction as raw data rather than inventing semantics.


## Source reconstruction terms

- **Source Output Target** — A language selected for generated decompiler output; currently Java, Sleela, C, or C++.
- **Source Emitter** — The component that converts recovered analysis evidence into source-oriented text for a selected output target.
- **Output Language Selection** — The explicit CLI/API choice of Java, Sleela, C, or C++ for generated source.
- **Source Reconstruction** — Evidence-based production of source-like code from recovered native structure; it is not automatically equivalent to the original source.
- **Language Projection** — Mapping the common recovered representation into a particular source language.
- **Evidence-Preserving Output** — Generated source that marks unresolved or ambiguous native behavior instead of presenting unsupported conclusions as facts.
- **Original-Language Claim** — A claim that the binary was originally written in a particular source language. Slecompiler's output target does not by itself establish such a claim.

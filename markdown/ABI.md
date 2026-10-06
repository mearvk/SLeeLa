# SLeeLa ABI

## Purpose

The Application Binary Interface (ABI) is the binary-level contract that allows separately compiled, linked, loaded, interpreted, or communicating components to agree on the meaning and representation of code, values, memory, symbols, resources, and execution state.

For SLeeLa, ABI is broader than a single native calling convention. It covers the boundaries among:

1. SLeeLa source and compiled representations.
2. The SLeeLa compiler, IR, and VM.
3. SLeeLa and native C/C++ implementations.
4. SLeeLa and operating-system/platform adapters.
5. Modules, libraries, plugins, and executable artifacts.
6. Processes, IPC, files, and network protocols.
7. Compiler, linker, loader, runtime, and operating-system interfaces.
8. Version, architecture, capability, resource, and security compatibility.

**Max Rupplin — MEARVK LLC — 2026**

---

## 1. API vs ABI

An API primarily describes what software exposes:

- functions;
- arguments;
- return values;
- types;
- behavior;
- errors.

An ABI additionally defines how those concepts exist at the binary boundary:

- registers;
- stack;
- calling convention;
- parameter classification;
- return-value representation;
- type sizes;
- alignment;
- structure offsets;
- padding;
- symbol names;
- visibility;
- object formats;
- relocations;
- runtime libraries;
- unwind information;
- thread-local storage;
- ownership;
- binary versioning.

A matching API does not prove binary compatibility.

Example:

    API:
        int calculate(int a, int b);

    ABI questions:
        size and signedness of int
        argument registers or stack locations
        return register
        stack alignment
        symbol name
        calling convention
        object format
        runtime dependencies

---

## 2. The SLeeLa ABI Layers

SLeeLa treats ABI as a layered contract.

### 2.1 Language ABI

The language ABI describes the binary consequences of SLeeLa language constructs.

It may include:

- primitive values;
- arrays;
- records;
- structures;
- classes;
- references;
- handles;
- functions;
- methods;
- modules;
- constants;
- visibility;
- errors/results;
- synchronization objects.

### 2.2 Core ABI

The core ABI defines the boundary between compiled SLeeLa execution and the SLeeLa execution core.

The currently documented exchange entry point is:

    slcore_exchange(SLVM* vm, SLExchangeOp op, SLExchangeArg* arg)

The operation set covers VM reset, constants, globals, functions, emission, patching, entry selection, execution, and result retrieval.

The exact structure definitions, operation values, and binary layouts remain implementation/header contracts and must be versioned.

### 2.3 Runtime Artifact ABI

The persistent .sleela artifact format has a binary contract independent of source-language syntax.

The current project documentation identifies:

    SLEELA_VM_ABI_MAJOR
    SLEELA_VM_ABI_MINOR
    SLEELA_ARTIFACT_FORMAT_VERSION

The runtime validates, at minimum:

- opcode values;
- code references;
- function metadata;
- globals;
- constants;
- structure metadata;
- synchronization operands;
- entry-point information.

The documented non-executing validation command is:

    sleela validate-artifact <file.sleela>

Artifact validation is not execution.

### 2.4 Native ABI

The native ABI connects SLeeLa to native implementations.

SLeeLa should prefer a stable C-compatible boundary when cross-compiler interoperability is required.

C++ may implement higher-level internals, but C++ ABI details can vary by compiler and runtime. These include:

- name mangling;
- class layout;
- virtual tables;
- RTTI;
- exception handling;
- templates;
- standard-library object layout.

A stable SLeeLa C ABI should not silently become a compiler-specific C++ ABI.

### 2.5 Platform ABI

The platform ABI combines:

- architecture;
- operating system;
- object format;
- compiler conventions;
- runtime library;
- system interface conventions.

Examples include Microsoft x64 on Windows and System V-family x86-64 conventions on Linux and related Unix-like targets.

### 2.6 Protocol ABI

A protocol ABI applies when binary information crosses a file, process, IPC, or network boundary.

It must define:

- field sizes;
- field order;
- byte order;
- framing;
- lengths;
- versions;
- optional fields;
- errors;
- capability negotiation;
- authentication/integrity where required.

A native C structure is not automatically a portable wire format.

---

## 3. What an ABI Can Define

A complete ABI may define:

1. Calling conventions.
2. Parameter passing.
3. Return values.
4. Register preservation.
5. Stack layout.
6. Stack alignment.
7. Shadow/home space where applicable.
8. Red-zone rules where applicable.
9. Data sizes.
10. Data alignment.
11. Structure layout.
12. Padding.
13. Bit-field representation.
14. Pointer representation.
15. Endianness.
16. Floating-point representation.
17. Vector/SIMD representation.
18. Function-pointer representation.
19. Symbol naming.
20. Name mangling.
21. Symbol visibility.
22. Linkage.
23. Object formats.
24. Relocations.
25. Dynamic linking.
26. Loader behavior.
27. Thread-local storage.
28. Exception handling.
29. Stack unwinding.
30. Debug information.
31. Runtime initialization.
32. Runtime finalization.
33. Resource ownership.
34. Allocation and deallocation.
35. Threading and synchronization.
36. Error/status conventions.
37. Version negotiation.
38. Plugin contracts.
39. File layouts.
40. Protocol framing.
41. Capability boundaries.
42. Security constraints.

Not every ABI defines every item.

---

## 4. Architecture Is Part of ABI

ABI analysis starts with the target architecture.

Important properties include:

- instruction set;
- register width;
- address width;
- pointer width;
- alignment;
- endianness;
- floating-point model;
- vector registers;
- atomic instructions;
- executable-memory requirements;
- instruction encoding;
- call/branch mechanisms.

Potential SLeeLa targets include architectures and managed targets supported by the corresponding SLeeLa release. The exact supported set must be taken from the compiler and VM implementation rather than inferred from this overview.

Architecture compatibility is necessary but not sufficient for ABI compatibility.

---

## 5. Calling Conventions

A calling convention defines how one function invokes another.

It can specify:

- argument order;
- argument registers;
- floating-point argument registers;
- stack arguments;
- stack alignment;
- return registers;
- hidden parameters;
- caller-saved registers;
- callee-saved registers;
- variadic behavior;
- aggregate passing;
- aggregate return;
- function pointers;
- unwind requirements.

A mismatch can produce:

- corrupted arguments;
- corrupted return values;
- stack corruption;
- register corruption;
- invalid unwind state;
- failures far from the original call.

### Caller and callee

For a call such as:

    result = function(a, b)

the caller prepares the call according to the target ABI. The callee receives the arguments according to that same ABI and preserves everything it is required to preserve.

---

## 6. Register Volatility

ABIs normally classify registers as:

- caller-saved / volatile;
- callee-saved / nonvolatile;
- argument registers;
- return-value registers;
- stack/frame registers;
- special-purpose registers.

The classification is architecture and platform dependent.

SLeeLa native code generation must use the target ABI rather than assuming one universal register policy.

---

## 7. Microsoft x64

Microsoft documents Windows x64 as using four register parameter positions.

Integer and pointer arguments use:

- RCX;
- RDX;
- R8;
- R9.

Floating-point arguments use:

- XMM0;
- XMM1;
- XMM2;
- XMM3.

Additional arguments use the stack, and the caller reserves shadow/home space for the register arguments.

Microsoft also documents volatile and nonvolatile registers, stack alignment, prolog/epilog requirements, and unwind metadata.

Source:
https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/x64-architecture

Calling convention:
https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention

---

## 8. x86-64 System V

The System V x86-64 ABI defines its own:

- argument classification;
- register use;
- stack rules;
- return rules;
- preserved registers;
- floating-point/vector conventions.

It is not the same ABI as Microsoft x64.

The System V ABI documentation identifies registers such as RBX, RBP, and R12–R15 as callee-preserved under the documented x86-64 calling sequence.

Reference:
https://refspecs.linuxfoundation.org/elf/x86_64-abi-0.99.pdf

SLeeLa Linux native code must follow the actual target ABI instead of copying Windows x64 assumptions.

---

## 9. ARM64 / AArch64

AArch64 has its own procedure-call and data-layout conventions.

Relevant ABI properties include:

- general-purpose registers;
- SIMD/floating-point registers;
- stack alignment;
- parameter classification;
- return values;
- preserved registers;
- floating-point state;
- exception/unwind information.

Windows ARM64 generally follows the AArch64 EABI with Microsoft-specific platform requirements.

Reference:
https://learn.microsoft.com/en-us/cpp/build/arm64-windows-abi-conventions

SLeeLa must distinguish generic AArch64 rules from OS-specific extensions.

---

## 10. 32-bit x86

32-bit x86 historically has several calling conventions, including:

- cdecl;
- stdcall;
- fastcall;
- thiscall;
- vectorcall-related conventions.

The exact rules can differ by compiler and platform.

Microsoft documents these conventions and notes architecture-specific applicability.

Reference:
https://learn.microsoft.com/en-us/cpp/build/reference/gd-gr-gv-gz-calling-convention

SLeeLa must never infer that a 32-bit x86 convention is interchangeable with x64.

---

## 11. Parameter Passing

Parameter classification can depend on:

- scalar size;
- integer vs floating-point type;
- vector type;
- aggregate size;
- aggregate members;
- alignment;
- by-value vs by-reference semantics;
- variadic status;
- architecture;
- calling convention.

Large or complex values may be passed indirectly.

The compiler must implement the target ABI's classification rules rather than treating every value as an integer of arbitrary size.

---

## 12. Return Values

Return values may be:

- placed in integer registers;
- placed in floating-point/vector registers;
- written through memory;
- returned through a hidden structure-return pointer;
- represented as an explicit SLeeLa result object;
- returned as an opaque handle.

Return representation is part of the ABI.

---

## 13. Data Representation

Every value crossing a stable binary boundary needs a defined representation.

At minimum consider:

- width;
- signedness;
- alignment;
- padding;
- offsets;
- initialization;
- ownership;
- lifetime;
- validity.

For stable binary contracts, fixed-width types such as uint8, uint16, uint32, and uint64 are generally preferable to implementation-dependent types whose size can differ between ABIs.

---

## 14. Structure Layout

Structure compatibility includes:

- member order;
- member offsets;
- member alignment;
- padding;
- total size;
- total alignment;
- packing rules;
- bit-field representation.

A structure with identical source declarations can have different binary layout under different ABI/compiler rules.

SLeeLa should not write a native structure directly to disk or the network unless its native layout is explicitly the specified file/wire ABI.

---

## 15. Alignment

Alignment applies to:

- stack pointers;
- heap allocations;
- structures;
- fields;
- vector values;
- atomics;
- memory-mapped interfaces.

ABI implementations must define alignment where required.

Misalignment can result in:

- performance degradation;
- hardware faults on some targets;
- invalid atomic operations;
- ABI violations.

---

## 16. Endianness

Endianness determines byte order.

Common forms include:

- little-endian;
- big-endian.

Native memory representation may inherit the target architecture.

Portable files and protocols should explicitly define byte order.

Therefore:

    native memory representation != portable serialization format

---

## 17. Pointer ABI

A pointer is not merely an integer address.

Pointer compatibility can depend on:

- width;
- address space;
- alignment;
- validity;
- lifetime;
- provenance;
- executable/data permissions;
- platform pointer conventions.

SLeeLa should prefer opaque handles when the underlying resource is owned by a runtime or subsystem.

---

## 18. Handles

A handle can represent:

- file;
- socket;
- VM object;
- thread;
- process;
- compiler object;
- resolver object;
- cryptographic context;
- memory region;
- plugin;
- module;
- device.

A handle ABI must define whether it is:

- process-local;
- VM-local;
- transferable;
- reference-counted;
- invalidated on close;
- safe across threads.

---

## 19. Ownership and Lifetime

Every native resource should define:

1. Creator.
2. Owner.
3. Borrow/retain rules.
4. Release operation.
5. Lifetime.
6. Failure behavior.
7. Thread-safety rules.

Possible models include:

- caller-owned;
- callee-owned;
- borrowed;
- retained;
- reference-counted;
- arena-owned;
- VM-owned;
- transferred;
- immutable/shared.

A pointer without an ownership contract is not a complete ABI.

---

## 20. Allocation ABI

If one side allocates memory, the ABI must specify who can release it.

Unsafe assumption:

    sleela_alloc(...) -> native free(...)

Safe models explicitly pair allocation and release, for example:

    sleela_alloc <-> sleela_free

or define an explicit ownership transfer.

Cross-runtime memory must never be released by an unrelated allocator unless the ABI says that is valid.

---

## 21. Strings

A string ABI should define:

- encoding;
- character width;
- termination;
- length;
- capacity where applicable;
- ownership;
- mutability;
- lifetime.

Possible representations include:

- UTF-8 + byte length;
- UTF-16 + code-unit length;
- UTF-32;
- null-terminated byte strings;
- SLeeLa string objects.

An encoding must never be inferred solely from a pointer.

---

## 22. Arrays and Buffers

A stable buffer ABI should define:

    pointer
    length
    capacity, if applicable
    element size
    alignment
    ownership
    mutability
    lifetime

A pointer alone does not establish how many elements exist.

---

## 23. Function Pointers

Function-pointer compatibility requires agreement on:

- calling convention;
- parameter ABI;
- return ABI;
- pointer representation;
- executable-address validity;
- lifetime;
- security restrictions.

A function pointer using one calling convention must not be invoked using another.

---

## 24. Variadic Functions

Variadic functions are particularly ABI-sensitive because caller and callee must agree on:

- fixed parameters;
- default promotions;
- register/stack classification;
- floating-point handling;
- format semantics.

Where possible, SLeeLa stable internal interfaces should prefer explicitly typed request/result structures.

---

## 25. Symbol Names and Mangling

Source names do not necessarily equal binary symbol names.

C++ mangling can encode:

- namespaces;
- classes;
- overloads;
- templates;
- qualifiers.

SLeeLa tooling should track:

- source symbol;
- binary symbol;
- linkage name;
- visibility;
- version;
- owning module.

C linkage can provide a smaller and more stable native boundary where appropriate.

---

## 26. Symbol Visibility

Symbols can be:

- local;
- global;
- weak;
- hidden;
- exported;
- imported;
- versioned.

Visibility affects:

- linking;
- dynamic loading;
- symbol interposition;
- plugin interfaces;
- debugging;
- attack surface.

SLeeLa libraries should expose only intended ABI surfaces.

---

## 27. Object and Executable Formats

ABI analysis may involve:

- ELF;
- PE;
- COFF;
- Mach-O;
- a.out;
- static archives;
- WebAssembly modules;
- JVM class/JAR artifacts;
- .NET assemblies;
- BEAM artifacts;
- LLVM bitcode;
- raw machine-code artifacts.

These formats can contain:

- executable sections;
- data sections;
- symbols;
- relocations;
- imports;
- exports;
- debug information;
- unwind information;
- TLS information;
- architecture identifiers.

Format recognition is not execution authorization.

---

## 28. Relocations

Relocations describe references whose final address is established later.

They can represent:

- absolute addresses;
- PC-relative references;
- function calls;
- data references;
- TLS;
- imported symbols;
- dynamic-link structures.

The compiler, linker, loader, and runtime must agree on relocation semantics.

SLeeLa decompiler tooling may use relocation information as evidence without executing the artifact.

---

## 29. Static and Dynamic Linking

### Static linking

Object code is incorporated during linking.

ABI concerns include:

- object compatibility;
- symbols;
- relocations;
- runtime initialization;
- duplicate symbols.

### Dynamic linking

External symbols are resolved at runtime.

ABI concerns include:

- library identity;
- symbol identity;
- symbol version;
- calling convention;
- data layout;
- loader behavior;
- initialization/finalization.

---

## 30. Loader ABI

A loader may establish:

- architecture;
- address mappings;
- relocations;
- imports;
- exports;
- TLS;
- entry point;
- unwind metadata;
- memory permissions;
- runtime initialization.

SLeeLa VM loading must remain distinct from unrestricted native process loading.

---

## 31. Exception and Unwind ABI

Exception/unwind ABI can define:

- stack-frame information;
- saved registers;
- return addresses;
- cleanup information;
- personality/handler data;
- exception representation;
- runtime ownership.

C++ exception ABIs are not universally interchangeable.

A SLeeLa error/result contract should not depend on crossing arbitrary compiler-specific C++ exception boundaries.

---

## 32. Error ABI

An ABI should explicitly define how failures cross the boundary.

Possible models:

- integer status;
- enumeration;
- boolean + output;
- result structure;
- error handle;
- exception;
- thread-local error state.

SLeeLa should distinguish:

- success;
- invalid input;
- unsupported ABI;
- unavailable capability;
- resource exhaustion;
- cancellation;
- timeout;
- security denial;
- fatal runtime condition.

A failure status must not be ambiguous with a valid data result.

---

## 33. Thread ABI

Thread-related ABI concerns include:

- thread entry function;
- thread identity;
- creation;
- join;
- detach;
- cancellation;
- TLS;
- synchronization;
- scheduler behavior.

A callback used as a thread entry point must use the calling convention expected by the thread implementation.

---

## 34. Thread-Local Storage

TLS can be implemented differently by:

- ELF;
- PE;
- Mach-O;
- compilers;
- runtimes;
- VM implementations.

A TLS pointer or offset from one platform cannot be assumed portable to another.

---

## 35. Atomics and Memory Ordering

Shared ABI objects may require:

- atomicity;
- acquire;
- release;
- acquire-release;
- relaxed ordering;
- sequential consistency;
- fences;
- lock-free guarantees;
- alignment.

Structural compatibility without synchronization compatibility is not safe ABI compatibility.

---

## 36. SLeeLa VM ABI

The VM creates an abstraction above the host platform ABI.

Conceptually:

    SLeeLa Source
        ↓
    SLeeLa Compiler
        ↓
    SLeeLa Artifact / VM Representation
        ↓
    SLeeLa VM ABI
        ↓
    Native C ABI
        ↓
    Platform ABI
        ↓
    Operating System / Hardware

The VM can therefore provide a stable logical contract while adapting native implementation details to different host ABIs.

---

## 37. VM Value ABI

VM values may include:

- integer;
- floating-point;
- boolean;
- character;
- string;
- array;
- object;
- structure;
- reference;
- handle;
- function;
- module;
- result/error;
- synchronization object.

Each native crossing must define:

- representation;
- size;
- alignment;
- tag;
- ownership;
- mutability;
- lifetime;
- conversion rules.

---

## 38. VM Context ABI

A VM context may contain:

- execution state;
- virtual registers;
- stack;
- heap;
- module graph;
- scheduler;
- memory manager;
- capability state;
- resolver state;
- I/O state;
- diagnostics;
- security state.

Private VM internals should not become stable ABI merely because native code can see their memory.

Opaque context handles are preferred for stable boundaries.

---

## 39. Startup ABI

The SLeeLa startup path is conceptually:

    SLeeLa System
        ↓
    Startup Module
        ↓
    System Harness
        ↓
    compiled SLeeLa C ABI + C++ runtime
        OR
    compatible SLeeLa VM module
        ↓
    bounded bootstrap calls
        ↓
    VM READY
        ↓
    normal execution

Startup ABI concerns include:

- startup-module identity;
- harness connection;
- execution mode;
- readiness state;
- bootstrap capabilities;
- queued calls;
- execution dispatch;
- shutdown.

Startup does not inherently grant unrestricted operating-system authority.

---

## 40. Dynamic Memory Guard

The SLeeLa VM Dynamic Memory Guard is a runtime policy layer rather than a host ABI replacement.

The documented modes are:

- HARD — never exceed the configured hard limit.
- SLOW_CAREFUL — bounded growth with the possibility of deferred growth.
- AGGRESSIVE — prompt bounded growth subject to configured and physical limits.

Conceptually:

    allocation request
        ↓
    dynamic memory guard
        ↓
    within limit / deferred / growth allowed / limit reached
        ↓
    memory manager
        ↓
    allocation result

The physical/resource limit remains authoritative.

---

## 41. C ABI Design Rules

For stable SLeeLa native interfaces, prefer:

- fixed-width integers;
- explicit result/status types;
- opaque handles;
- explicit lengths;
- explicit ownership;
- explicit alignment;
- explicit initialization;
- explicit version fields;
- caller-provided buffers where appropriate;
- reserved fields;
- C linkage;
- documented thread-safety.

Avoid exposing as stable cross-compiler ABI:

- compiler-private structures;
- C++ standard-library containers;
- C++ exceptions;
- private class layouts;
- compiler-specific RTTI;
- ownership-ambiguous pointers;
- variable-size structures without size/version fields.

---

## 42. Versioned ABI Structures

A robust extensible ABI structure can conceptually contain:

    uint32 size
    uint32 abi_major
    uint32 abi_minor
    uint32 flags
    versioned fields

The size field can allow a consumer to determine which fields are present.

Reserved fields can permit future extension.

A version number does not automatically make an ABI compatible; compatibility rules must define permitted changes.

---

## 43. ABI Versioning

SLeeLa should keep separate version identifiers for separate contracts:

- language version;
- compiler version;
- runtime version;
- VM ABI major/minor;
- artifact format;
- native C ABI;
- platform ABI;
- protocol version;
- plugin ABI.

One global project version is insufficient to describe all binary compatibility.

A compatibility record can contain:

    language
    compiler
    artifact_format
    vm_abi
    native_abi
    architecture
    platform
    object_format
    endianness
    pointer_width

---

## 44. Major vs Minor ABI Changes

Potential major ABI changes include:

- changed calling convention;
- changed structure layout;
- changed parameter size;
- changed value representation;
- changed required initialization;
- changed ownership semantics;
- removed exported symbols.

Potentially additive changes include:

- new symbols;
- optional fields;
- new optional capabilities;
- new negotiated protocol extensions.

Whether a change is actually compatible must be determined from the complete ABI contract.

---

## 45. Architecture/OS/ABI Matrix

ABI analysis should use a matrix rather than a single platform label.

| Architecture | OS | Object Format | Native ABI |
|---|---|---|---|
| x86-64 | Linux | ELF | System V family |
| x86-64 | Windows | PE/COFF | Microsoft x64 |
| x86-64 | macOS | Mach-O | Apple platform conventions |
| ARM64 | Linux | ELF | AArch64 platform ABI |
| ARM64 | Windows | PE/COFF | Windows ARM64 ABI |
| ARM64 | macOS | Mach-O | Apple ARM64 conventions |

This is an orientation matrix, not a substitute for the target platform's exact specification.

---

## 46. Compiler ABI

Different compilers can implement the same source language with different binary behavior.

ABI differences can involve:

- structure layout;
- enum representation;
- vector ABI;
- name mangling;
- exception handling;
- RTTI;
- TLS;
- runtime library;
- standard-library ABI.

SLeeLa binary metadata should retain toolchain information when it materially affects compatibility.

---

## 47. Runtime Library ABI

The runtime library can itself be an ABI dependency.

Examples include:

- libc;
- libstdc++;
- libc++;
- MSVC runtime;
- platform SDK runtime;
- JVM runtime;
- .NET runtime.

Matching architecture and object format does not prove runtime compatibility.

---

## 48. Debug Information

Debug information is not normally the execution ABI, but it is important ABI evidence.

It may provide:

- source file;
- line;
- function;
- variable;
- type;
- address range;
- register mapping;
- inline call information;
- stack information.

Missing debug information is not proof that a source construct did not exist.

---

## 49. Unwind ABI

Unwind information can support:

- exceptions;
- crash diagnostics;
- debuggers;
- profilers;
- stack walking;
- signal handling.

SLeeLa native code should preserve target-platform unwind requirements when generating or embedding native code.

---

## 50. Security ABI

An ABI can also be a security boundary.

Security-related ABI concerns include:

- capability identifiers;
- permission checks;
- opaque handles;
- module identity;
- memory boundaries;
- syscall mediation;
- resource quotas;
- cryptographic context ownership;
- secure cleanup.

A function being callable does not mean the caller is authorized to perform the operation.

---

## 51. ABI and Resolver

The SLeeLa resolver may encounter ABI information while resolving:

- libraries;
- symbols;
- modules;
- platform variants;
- architecture-specific artifacts.

A library should not be selected solely because its filename matches.

The resolver should consider:

    identity
    version
    architecture
    OS
    object format
    ABI
    runtime
    capabilities
    security policy

---

## 52. ABI and Compiler

The compiler should establish:

1. Target architecture.
2. Target operating system.
3. Object format.
4. Native calling convention.
5. Data-layout model.
6. Alignment.
7. Runtime dependencies.
8. Exception/unwind model.
9. TLS model.
10. Linkage/export requirements.
11. SLeeLa VM ABI.
12. Artifact format.
13. Required capabilities.
14. Security/resource constraints.

The compiler should reject a target when required ABI properties cannot be established.

---

## 53. ABI and Decompiler

The decompiler should treat ABI detection as evidence-based analysis.

Useful evidence includes:

- executable/object headers;
- architecture markers;
- import/export tables;
- relocation records;
- calling patterns;
- register usage;
- stack alignment;
- unwind metadata;
- symbol naming;
- debug information;
- runtime metadata;
- compiler fingerprints.

A single magic number is only initial evidence.

Conflicting evidence should be preserved rather than silently discarded.

---

## 54. ABI and Binary Safety

ABI compatibility is not a safety guarantee.

A binary can be:

- ABI-compatible;
- structurally valid;
- correctly linked;

and still be malicious or unsafe.

Therefore:

    ABI compatibility
        != trust
        != authorization
        != safe execution

SLeeLa inspection should remain non-executing until the appropriate validation, security, capability, provenance, and resource gates are satisfied.

---

## 55. ABI Evidence Levels

SLeeLa tooling can classify evidence as:

### Direct

Explicit ABI metadata or authoritative manifest.

### Strong

Consistent architecture, object-format, runtime, and ABI evidence.

### Supporting

Symbols, relocations, debug data, compiler fingerprints, calling patterns, and section information.

### Weak

Filename, extension, isolated byte pattern, or heuristic guess.

Weak evidence should not override strong contradictory evidence.

---

## 56. ABI Fingerprints

A useful ABI fingerprint can contain:

    architecture
    pointer_width
    endianness
    os
    object_format
    compiler_family
    compiler_version
    c_runtime
    cpp_runtime
    calling_convention
    data_model
    stack_alignment
    unwind_model
    tls_model
    sleela_vm_abi
    artifact_format

A fingerprint is evidence for compatibility analysis, not automatic execution authorization.

---

## 57. ABI Metadata

A future SLeeLa artifact/module manifest can explicitly identify:

    artifact:
        format
        version

    target:
        architecture
        os
        abi
        object_format
        endianness
        pointer_width

    runtime:
        vm_abi_major
        vm_abi_minor

    native:
        language
        compiler
        runtime

    security:
        required_capabilities
        signature
        provenance

Explicit metadata is preferable to guessing when the producer can provide it.

---

## 58. Protocol and Network ABI

A protocol ABI should define:

- version;
- frame;
- header;
- length;
- encoding;
- message type;
- request ID;
- response ID;
- error;
- timeout;
- cancellation;
- authentication;
- integrity;
- maximum sizes.

Protocol compatibility and native ABI compatibility are separate dimensions.

---

## 59. HTTP and SLeeLa

HTTP is a protocol-level contract, not a CPU calling convention.

SLeeLa HTTP implementations therefore have protocol concerns such as:

- message framing;
- headers;
- payload encoding;
- stream state;
- request/response semantics;
- protocol version;
- connection state.

The native HTTP implementation still has to conform to the host platform ABI underneath.

---

## 60. File ABI

A persistent file format is an ABI-like contract.

It should define:

- magic/signature;
- version;
- header size;
- architecture/target;
- byte order;
- lengths;
- offsets;
- checksums where required;
- optional sections;
- required sections;
- unknown-section behavior;
- corruption behavior.

Unknown fields should not be interpreted as known fields without a versioned rule.

---

## 61. Plugin ABI

A plugin ABI should define:

- entry point;
- ABI version;
- metadata;
- initialization;
- shutdown;
- allocator;
- error handling;
- threading;
- callbacks;
- ownership;
- capabilities;
- exported interface.

Plugins should not rely on private VM structure layout.

---

## 62. Allocator ABI

Memory allocation is a critical ABI boundary.

If one side allocates memory, the other side must use the release mechanism specified by the ABI.

This prevents:

- incompatible heaps;
- runtime mismatch;
- double-free;
- invalid free;
- use-after-free;
- allocator corruption.

---

## 63. FFI

Foreign Function Interface integration must map:

- functions;
- types;
- strings;
- arrays;
- structures;
- callbacks;
- errors;
- resources;
- threads;
- supported exception/result boundaries.

FFI must use the target binary ABI, not merely matching source-language type names.

---

## 64. Managed Runtime ABIs

Managed runtimes have their own binary contracts.

Examples include:

- JVM;
- .NET;
- WebAssembly;
- BEAM.

Managed ABI concerns may include:

- module/class format;
- metadata;
- method descriptors;
- bytecode;
- verification;
- object representation;
- garbage collection;
- exceptions;
- host functions.

SLeeLa must distinguish managed-runtime ABI from native machine-code ABI.

---

## 65. WebAssembly

WebAssembly provides a portable execution model, but applications still need an interface contract for:

- imports;
- exports;
- linear memory;
- tables;
- host functions;
- WASI or equivalent host interfaces;
- component/interface contracts.

A WebAssembly module is not an ELF executable merely because it contains executable logic.

---

## 66. Garbage-Collected Runtime ABI

Garbage-collected runtimes add concerns such as:

- moving objects;
- roots;
- handles;
- pinning;
- write barriers;
- safepoints;
- thread state.

A native pointer into a moving managed heap cannot automatically be treated as a permanent stable pointer.

SLeeLa should use handles or explicit pinning where required.

---

## 67. CPU Feature ABI

ABI-compatible code may still require CPU features such as:

- SSE;
- SSE2;
- AVX;
- AVX2;
- AVX-512;
- NEON;
- SVE;
- atomic extensions.

Therefore:

    ABI compatible
        +
    CPU feature compatible

are separate checks.

---

## 68. Security Features

Platform security features can affect executable compatibility and behavior.

Examples include:

- NX/DEP;
- W^X;
- ASLR;
- control-flow protection;
- code signing;
- hardened runtime rules;
- pointer authentication;
- sandboxing.

SLeeLa should model these as target/runtime constraints rather than assuming executable memory or unrestricted system access.

---

## 69. Syscall ABI

A system-call interface is a binary boundary, but it is not automatically the same as a user-space C ABI.

A typical path may be:

    SLeeLa/native call
        ↓
    C runtime
        ↓
    platform runtime
        ↓
    system call
        ↓
    kernel

SLeeLa should not assume that libc calling conventions and kernel syscall conventions are interchangeable.

---

## 70. Kernel Module ABI

Kernel modules can depend on:

- kernel version;
- kernel configuration;
- architecture;
- compiler;
- exported kernel symbols;
- internal structures;
- module format;
- signing policy.

SLeeLa tooling may inspect such artifacts, but inspection is distinct from loading or executing them.

---

## 71. Cross-Compilation ABI

The host ABI and target ABI can differ.

For example:

    host:
        x86-64 / Linux

    target:
        ARM64 / Windows

The compiler must generate according to the target:

- architecture;
- object format;
- calling convention;
- data layout;
- runtime;
- linker;
- loader assumptions.

The host process running the compiler does not define the target ABI.

---

## 72. ABI and Reproducibility

ABI-sensitive builds should record:

- compiler;
- compiler version;
- linker;
- assembler;
- target triple;
- SDK;
- runtime;
- build flags;
- architecture features;
- ABI version;
- source revision.

This makes ABI failures diagnosable rather than speculative.

---

## 73. ABI Diagnostics

SLeeLa diagnostics should identify the ABI layer when possible.

Useful categories include:

    ABI_ARCHITECTURE_MISMATCH
    ABI_OS_MISMATCH
    ABI_OBJECT_FORMAT_MISMATCH
    ABI_VERSION_MISMATCH
    ABI_CALLING_CONVENTION_MISMATCH
    ABI_DATA_LAYOUT_MISMATCH
    ABI_ALIGNMENT_MISMATCH
    ABI_SYMBOL_MISMATCH
    ABI_RUNTIME_MISMATCH
    ABI_CAPABILITY_MISMATCH
    ABI_RESOURCE_MISMATCH
    ABI_UNKNOWN

Exact numeric error-code assignments remain release-specific implementation contracts.

---

## 74. ABI Compatibility States

Compatibility can be represented as separate states:

- UNKNOWN;
- IDENTIFIED;
- STRUCTURALLY_COMPATIBLE;
- ABI_COMPATIBLE;
- RUNTIME_COMPATIBLE;
- CAPABILITY_COMPATIBLE;
- RESOURCE_COMPATIBLE;
- SECURITY_APPROVED;
- EXECUTION_AUTHORIZED;
- INCOMPATIBLE.

These should not be collapsed into one boolean.

---

## 75. ABI and Security Authority

A valid ABI call is not automatically an authorized operation.

Conceptually:

    native function exists
        !=
    caller is authorized

SLeeLa's VM, resolver, memory manager, certificate/security systems, capability system, and other policy boundaries remain authoritative.

---

## 76. ABI and Port Authority

Network port operations are resource-control operations.

A port ABI should define:

- requested port;
- protocol;
- address family;
- binding scope;
- ownership;
- lifecycle;
- pause behavior;
- shutdown behavior;
- authorization;
- conflict behavior.

Representing a port operation in an ABI does not itself authorize opening a listener.

---

## 77. ABI and Startup Configuration

SLeeLa startup configuration can distinguish:

    system.startup
    server.startup
    http.1.startup
    ...
    http.9.startup
    vm.edition
    port-authority.startup
    port-authority.pause
    port-authority.shutdown

These are startup-policy settings.

They do not independently grant network, filesystem, process, or operating-system authority.

---

## 78. ABI and Shutdown

Shutdown is part of a resource contract.

A robust shutdown path should be able to:

1. Stop accepting new work.
2. Signal active operations.
3. Flush or cancel queued work.
4. Release resources.
5. Close handles.
6. Stop worker threads.
7. Unregister callbacks.
8. Release modules.
9. Disconnect the system harness.
10. Report final state.

Shutdown should also define behavior after partial initialization.

---

## 79. Partial Initialization

ABI implementations must define cleanup after partial failure.

Example:

    allocate A
    allocate B
    initialize C
    C fails

The implementation must specify how A and B are released before failure returns.

This matters for:

- plugins;
- VM startup;
- cryptographic contexts;
- network servers;
- devices;
- memory managers.

---

## 80. Callbacks and Reentrancy

A callback ABI should define:

- callback calling convention;
- arguments;
- user-data handle;
- lifetime;
- execution thread;
- reentrancy;
- cancellation;
- shutdown behavior.

It should also define whether a function is:

- reentrant;
- thread-safe;
- thread-compatible;
- single-threaded;
- callback-reentrant;
- VM-thread-only.

A valid calling convention does not guarantee safe concurrent use.

---

## 81. Time ABI

Time crossing an ABI should define:

- clock;
- epoch;
- unit;
- width;
- signedness;
- precision;
- monotonicity;
- overflow behavior.

A duration and a wall-clock timestamp are different ABI types even if both happen to use a 64-bit integer.

---

## 82. OS Handles

Operating-system handles differ by platform.

Examples include:

- Unix file descriptors;
- Windows HANDLE values;
- sockets;
- Mach ports;
- device handles.

A portable SLeeLa interface should generally use opaque SLeeLa handles rather than assuming that an integer representation is portable.

---

## 83. Device ABI

Device interfaces can require:

- device identity;
- command structures;
- buffer layout;
- DMA alignment;
- interrupt/callback behavior;
- ownership;
- synchronization;
- versioning;
- hardware protocol.

SLeeLa driver work should distinguish:

    SLeeLa user-space ABI
    kernel/device ABI
    hardware protocol

---

## 84. Cryptographic ABI

A cryptographic ABI should define:

- algorithm identifier;
- key ownership;
- context lifetime;
- input/output lengths;
- nonce/IV representation;
- error behavior;
- secure cleanup;
- thread-safety;
- provider/module version.

Private cryptographic context layout should remain opaque unless explicitly stabilized.

---

## 85. ABI and Reports/Diagnostics

SLeeLa reporting infrastructure may observe authorized:

- input;
- output;
- messages;
- system records;
- VM state;
- build records;
- ABI validation results.

Observation should not silently become authority to mutate the observed resource.

---

## 86. ABI and Memory Management

A memory ABI should distinguish:

- allocate;
- deallocate;
- resize;
- ownership transfer;
- alignment;
- zeroing;
- protection;
- accounting.

The Dynamic Memory Guard is policy layered above these mechanics.

---

## 87. ABI and Scheduler

Scheduler interfaces may define:

- task identity;
- thread identity;
- priority;
- state;
- wake/sleep;
- cancellation;
- join;
- synchronization;
- CPU affinity.

A callback ABI should specify which execution context invokes it.

---

## 88. ABI and Modules

A module ABI should identify:

- module name;
- module version;
- ABI version;
- dependencies;
- exports;
- capabilities;
- initialization;
- shutdown;
- entry points;
- resource ownership.

Source version and ABI version are distinct.

---

## 89. ABI and Dependency Resolution

A dependency resolver should evaluate:

    module identity
    version
    architecture
    OS
    ABI
    runtime
    capabilities
    security policy

Selecting the newest dependency is not sufficient if its ABI is incompatible.

---

## 90. ABI and Packages

A package can contain multiple ABI variants:

    package
      ├── linux-x86_64
      ├── linux-arm64
      ├── windows-x86_64
      └── macos-arm64

Each variant should identify its intended ABI.

---

## 91. ABI and Installation

Installation should not copy a binary merely because its filename matches.

ABI-aware deployment can verify:

- target OS;
- architecture;
- artifact format;
- runtime ABI;
- dependencies;
- required capabilities;
- provenance/signature where required.

The Quick and Safe SLeeLa installer is a deployment/configuration mechanism; ABI validation remains a distinct concern.

---

## 92. ABI and PATH

PATH is an operating-system process/environment convention, not a SLeeLa binary ABI.

Changing PATH can select a different executable, but it does not establish binary compatibility.

PATH configuration and ABI validation should therefore remain separate stages.

---

## 93. SLeeLa Source Authority

SLeeLa source definitions remain authoritative for SLeeLa-defined behavior.

Native C/C++ is an implementation/support layer where explicitly defined.

Conceptually:

    .sleela source contract
        ↓
    compiler/semantic contract
        ↓
    VM/artifact contract
        ↓
    native C/C++ implementation
        ↓
    platform ABI

Native implementation details must not silently redefine the SLeeLa source contract.

---

## 94. Compiler and Decompiler Symmetry

The compiler asks:

    What ABI must this artifact satisfy?

The decompiler asks:

    What ABI does this artifact appear to satisfy?

Compiler direction:

    Source → target ABI → artifact

Decompiler direction:

    Artifact → evidence → probable ABI → reconstructed representation

Neither workflow should invent missing evidence.

---

## 95. ABI and Reverse Engineering

ABI evidence can help reconstruct:

- function boundaries;
- argument counts;
- register use;
- stack layout;
- structure accesses;
- imported APIs;
- calling conventions.

It cannot necessarily recover:

- original variable names;
- comments;
- exact source formatting;
- exact original types;
- original class hierarchy;
- original build configuration.

SLeeLa decompiler should preserve uncertainty.

---

## 96. ABI Test Requirements

ABI testing should include:

### Function tests

- parameter passing;
- return values;
- function pointers;
- callbacks;
- variadic interfaces where supported.

### Data tests

- sizeof;
- alignment;
- offsets;
- padding;
- endianness;
- serialization.

### Resource tests

- ownership;
- retain/release;
- lifetime;
- failure cleanup.

### Runtime tests

- startup;
- shutdown;
- threading;
- TLS;
- unwind;
- error boundaries.

### Artifact tests

- headers;
- versions;
- opcodes;
- relocations;
- symbols;
- entry points;
- dependencies.

### Cross-platform tests

Every supported architecture, OS, and toolchain combination claimed by a release should have an ABI test profile.

---

## 97. ABI Conformance Tests

A conformance test should verify the contract, not merely observe that one application happened to run.

Examples:

    ABI version = expected
    sizeof(type) = expected
    alignof(type) = expected
    offsetof(field) = expected
    symbol exists = expected
    calling convention = expected
    return representation = expected
    ownership behavior = expected
    error behavior = expected

SLeeLa can maintain target-specific ABI manifests for these checks.

---

## 98. ABI Compatibility Checklist

Before accepting a native artifact, tooling should ask:

### Identity

- What artifact is this?
- What version?
- What producer?

### Target

- What architecture?
- What OS?
- What object format?
- What endianness?
- What pointer width?

### Calling

- What calling convention?
- What parameter rules?
- What return rules?
- What register preservation?

### Layout

- What type sizes?
- What alignment?
- What structure offsets?
- What packing?

### Runtime

- What C runtime?
- What C++ runtime?
- What unwind model?
- What TLS model?

### Linking

- What imports?
- What exports?
- What symbol versions?
- What relocations?

### SLeeLa

- What VM ABI?
- What artifact format?
- What VM edition?
- What capabilities?

### Security

- What provenance?
- What signature/checksum policy?
- What privileges?
- What resource limits?

If essential facts are unknown, compatibility should remain indeterminate rather than guessed.

---

## 99. What ABI Compatibility Does Not Guarantee

ABI compatibility does not automatically guarantee:

- correctness;
- security;
- trustworthiness;
- source recovery;
- performance;
- determinism;
- feature availability;
- capability authorization;
- sufficient memory;
- CPU feature support;
- compatible cryptographic policy;
- compatible network policy.

ABI compatibility is one property of a binary integration, not the entire execution decision.

---

## 100. Current SLeeLa Runtime Artifact Contract

The current project documentation identifies:

- VM ABI major/minor identifiers;
- artifact format version;
- opcode validation;
- code-reference validation;
- function metadata validation;
- globals/constants validation;
- structure metadata validation;
- synchronization operand validation;
- entry-point validation;
- non-executing artifact validation.

The validation command is:

    sleela validate-artifact <file.sleela>

The authoritative implementation and headers remain the source for exact field definitions and release-specific values.

---

## 101. ABI Authority Order

When resolving an ABI question, prefer:

1. Exact SLeeLa ABI headers and implementation for the release.
2. SLeeLa artifact/VM version contract.
3. SLeeLa compiler/VM/decompiler documentation.
4. Target operating-system ABI specification.
5. Target architecture ABI specification.
6. Compiler/toolchain ABI documentation.
7. Observed binary evidence.
8. Heuristics.

Observed behavior should not silently replace an explicitly documented contract.

---

## 102. External ABI References

Useful platform references include:

- System V x86-64 ABI:
  https://refspecs.linuxfoundation.org/elf/x86_64-abi-0.99.pdf
- Microsoft x64 architecture:
  https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/x64-architecture
- Microsoft x64 software conventions:
  https://learn.microsoft.com/en-us/cpp/build/x64-software-conventions
- Microsoft x64 calling convention:
  https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention
- Microsoft ARM64 ABI conventions:
  https://learn.microsoft.com/en-us/cpp/build/arm64-windows-abi-conventions
- Microsoft calling-convention options:
  https://learn.microsoft.com/en-us/cpp/build/reference/gd-gr-gv-gz-calling-convention
- Itanium C++ ABI:
  https://itanium-cxx-abi.github.io/cxx-abi/

These references describe external platform/compiler contracts. They do not override SLeeLa's own ABI contract.

---

## 103. Final Principle

The SLeeLa ABI is the collection of binary contracts that make independently produced components agree on what bytes, registers, memory, symbols, resources, messages, and execution states mean.

The project-wide principle is:

    Define the boundary.
    Version the boundary.
    Validate the boundary.
    Respect ownership.
    Respect the target ABI.
    Respect SLeeLa capability policy.
    Do not execute merely to discover compatibility.

An ABI is successful when independently produced components can cross the boundary without guessing.

**Max Rupplin — MEARVK LLC — 2026**

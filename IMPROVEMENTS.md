# Slecompiler Improvements

This document is the active engineering backlog and implementation plan for Slecompiler™ in `/decompiler`.

The forty improvement areas remain the authoritative engineering register. **Phase 2** now defines the next implementation cycle: move from foundation hardening into the address-space, native decoding, control-flow, function-recovery, and semantic-lifting core while preparing the later binary-format and source-reconstruction work.

## Implementation status

- **Phase 1 — Foundation:** initial error handling, CLI hardening, static-analysis boundary, and regression scaffolding.
- **Phase 2 — Native analysis core:** Items 1–10, with Items 24–30 and 34–40 providing the reliability/provenance controls required by the core.
- **Phase 3 — Binary formats and ABI evidence:** Items 11–20.
- **Phase 4 — Recovery, reconstruction, and difficult artifacts:** Items 21–30.
- **Phase 5 — Production validation and security:** Items 31–40.
- Percentages in this document are engineering targets unless backed by measured test/corpus evidence.

## 1–40 Improvement Register

1. **Native instruction decoding** — Complete x86/x86-64 decoding first; add ARM/ARM64 and later RISC-V, including prefixes, VEX/EVEX, SIMD, floating point, atomics, system instructions, operand widths and memory operands.
2. **Address-space model** — Explicitly distinguish file offsets, section offsets, virtual addresses and runtime/load addresses; provide checked translations between them.
3. **Control-flow recovery** — Recover direct and indirect branches, calls, returns, jump tables, switches, tail calls, thunks, exception edges, PLT/GOT paths and multiple CFG hypotheses where ambiguity exists.
4. **Function recovery** — Combine symbols, entry points, calls, prologues/epilogues, tail calls, thunks, exception handlers and compiler/runtime patterns; attach confidence and provenance.
5. **Semantic lifting / SLIR** — Represent registers, flags, memory, constants, pointers, loads/stores, arithmetic, comparisons, calls, returns, branches, PHI/merge values, SSA, exceptions, volatile and atomic operations.
6. **Data-flow analysis** — Add def-use/use-def, reaching definitions, liveness, constant/copy propagation, dead-code analysis, value sets, pointer tracking, stack/register variables and alias analysis.
7. **Type recovery** — Recover widths, signedness, pointers, arrays, structs/unions/enums, function pointers, qualifiers, floating/vector types and ABI-derived parameter/return types.
8. **Source reconstruction** — Turn CFG and SLIR into actual expressions, assignments, conditionals, loops, switches, calls and data structures instead of function shells.
9. **Java backend** — Add evidence-based classes, methods, fields, packages, arrays, references, exceptions and JVM-compatible types.
10. **Sleela backend** — Define a compiler-valid output grammar and emit modules, declarations, functions, types, control flow, native interop and explicit unknown/evidence constructs.
11. **C/C++ backends** — Recover declarations, prototypes, globals, structs/unions/enums, typedefs, namespaces, classes, methods, function pointers and ABI attributes where evidence supports them.
12. **PE/COFF analysis** — Implement headers, RVA mapping, sections, imports/exports, delay imports, relocations, TLS, unwind metadata, PDB references, resources, load-config and Windows CFG/SEH metadata.
13. **Mach-O analysis** — Implement 32/64-bit headers, load commands, segments/sections, symbols, dyld exports/fixups, relocations, Obj-C/Swift metadata, code signatures and universal binaries.
14. **GNU archive analysis** — Recursively analyze members, symbol indexes, duplicate/weak/COMDAT symbols and cross-member relationships.
15. **Dynamic/shared library analysis** — Handle PLT/GOT, symbol versions, IFUNC, TLS, constructors/destructors, DT_NEEDED, RPATH/RUNPATH, interposition and weak symbols.
16. **Debug information** — Preserve DWARF, PDB/CodeView, dSYM and other debug evidence including files, lines, scopes, inline functions, variables and types.
17. **Exception/unwind analysis** — Parse DWARF CFI, .eh_frame/.debug_frame, Windows unwind/SEH and language-specific exception tables.
18. **Calling conventions / ABI** — Model System V AMD64, Microsoft x64, cdecl/stdcall/fastcall/thiscall/vectorcall, AAPCS32/AAPCS64 and relevant RISC-V conventions.
19. **Global/data analysis** — Recover strings, constants, arrays, jump tables, vtables, RTTI, globals, TLS, function-pointer tables and relocation-backed objects.
20. **Compiler/runtime fingerprinting** — Identify common GCC, Clang/LLVM, MSVC, ICC/ICX, Rust, Go, Swift, Delphi and runtime patterns without treating fingerprints as proof.
21. **Optimization-aware recovery** — Handle O0–O3/Os/Ofast, LTO, ThinLTO, PGO, inlining, vectorization and tail-call optimization.
22. **Stripped-binary strategy** — Explicitly classify symbol/debug availability and adapt function/type recovery to stripped, partially stripped and debug-stripped artifacts.
23. **Packed/obfuscated artifacts** — Detect and report packing, encryption, opaque predicates, control-flow flattening, indirect dispatch and self-modifying behavior without presenting guesses as facts.
24. **Structured error/exception model** — Replace generic runtime errors with typed, stage-aware failures such as InvalidArtifact, TruncatedArtifact, UnsupportedFormat, InvalidAddress, DecodeFailure, AmbiguousDecode, AnalysisFailure, OutputFailure and ResourceLimit.
25. **Fault tolerance** — Continue analysis around recoverable malformed or unsupported regions; record per-region status and coverage.
26. **Resource limits** — Add limits for memory, instructions, functions, recursion/analysis depth, time, output size and CFG nodes.
27. **Cancellation and progress** — Expose cancellation, phase, progress and percentage through library and CLI APIs.
28. **Deterministic output** — Same artifact/configuration must produce stable analysis, source, JSON and hashes.
29. **Machine-readable output** — Add text, JSON, SARIF and YAML representations with artifact, architecture, sections, functions, CFG, evidence, confidence, errors and coverage.
30. **Source-to-instruction provenance** — Map generated source back through SLIR to instruction addresses and binary offsets.
31. **Regression corpus** — Build a cross-language corpus covering optimized, stripped, exception-heavy, template, virtual, threaded, library and architecture-specific samples.
32. **Sanitizer/UB testing** — Gate CI with ASan, UBSan, LeakSan and ThreadSan where supported.
33. **Parser/decoder fuzzing** — Fuzz ELF, PE, Mach-O, AR, debug metadata, decoder, SLIR and CLI paths for crashes, hangs, OOB access and pathological allocation.
34. **CLI correctness** — Stable help/version behavior, validated numeric arguments, explicit options, predictable output files and stable exit codes.
35. **Architecture selection** — Permit explicit architecture selection and reject contradictions with detected artifact architecture.
36. **Base/load address handling** — Model preferred image base, actual load base, PIE and ASLR without conflating file, virtual and runtime addresses.
37. **Thread-safe API** — Permit independent artifact analyses in parallel without shared mutable global state.
38. **Caching** — Add content-addressed caching for artifact, decoding, CFG, SLIR and generated source.
39. **Measured coverage** — Report measured format, instruction, CFG, function, symbol, type, semantic, source and unknown/unsupported/ambiguous coverage.
40. **Security boundary** — Keep static analysis non-executing by default; never run target instructions, constructors, scripts or kernel modules. Any future emulation must be explicitly isolated.

# Phase 2 — Native Analysis Core

## Purpose

Phase 2 moves Slecompiler™ from a hardened foundation into a real native-analysis pipeline. The central rule is:

**Artifact bytes → address model → instruction decoder → CFG → function recovery → SLIR → data flow/types → source backends.**

The four requested source targets—**Java, Sleela, C, and C++**—must ultimately consume the same recovered semantic representation. They should not each invent independent interpretations of the binary.

## Phase 2A — Address-space foundation

Primary items: **2, 35, 36, 24, 25, 26, 28, 30, 39, 40**.

Implement explicit checked address types/concepts for:

- file offset;
- section-relative offset;
- virtual address;
- preferred image address;
- actual load address;
- runtime address;
- instruction address;
- relocation target address.

Required behavior:

- no implicit conversion between address domains;
- checked file-offset ↔ virtual-address translation using section/segment mappings;
- explicit PIE/load-base handling;
- relocation calculations use the correct address domain;
- invalid or unmapped translations produce structured errors;
- every recovered instruction/function can retain both binary location and virtual-address provenance;
- deterministic ordering of sections, functions, blocks, instructions, and emitted artifacts.

**Phase 2 exit evidence:** address translation tests cover valid mappings, section boundaries, unmapped gaps, overflow, relocation targets, PIE/load-base cases, and malformed artifacts.

## Phase 2B — Native decoder expansion

Primary item: **1**, supported by **24–26, 33, 39**.

The first production decoder target is x86/x86-64. It must move beyond the current limited branch/call/mov subset.

Required decoder layers:

1. prefix decoding;
2. opcode-map decoding;
3. ModR/M and SIB decoding;
4. displacement/immediate decoding;
5. operand-size/address-size handling;
6. register and memory operand modeling;
7. VEX/EVEX and SIMD coverage;
8. floating-point/vector operations;
9. atomic/lock semantics;
10. system and control instructions;
11. instruction-length and boundary validation;
12. explicit unsupported/ambiguous decode states.

Unknown bytes must not silently become proven instructions. Decoder results should carry status and evidence.

ARM/ARM64 follows the x86/x86-64 foundation. RISC-V follows after the first cross-architecture semantic interface is stable.

## Phase 2C — CFG and function recovery

Primary items: **3 and 4**, supported by **17–20, 21–23, 30, 39**.

Build a real control-flow graph with:

- basic blocks;
- fall-through edges;
- direct branch edges;
- conditional edges;
- call edges;
- return edges;
- indirect branch candidates;
- jump-table/switch recovery;
- tail-call edges;
- thunk detection;
- PLT/GOT-aware edges when binary-format support is available;
- exception/unwind edges when evidence is available;
- ambiguous edges represented as hypotheses rather than forced facts.

Function recovery should combine:

- explicit symbols;
- known entry points;
- call targets;
- relocation evidence;
- prologue/epilogue patterns;
- tail calls;
- thunks;
- runtime metadata;
- exception handlers;
- compiler/runtime fingerprints.

Every function and edge should carry provenance and a confidence/evidence record.

## Phase 2D — SLIR semantic model

Primary item: **5**, supported by **6, 7, 18, 30**.

Define a stable Slecompiler Low-level Intermediate Representation (**SLIR**) capable of representing:

- registers;
- flags;
- constants;
- memory reads/writes;
- pointer arithmetic;
- arithmetic and logical operations;
- comparisons;
- branches;
- calls and returns;
- stack/frame objects;
- volatile operations;
- atomic operations;
- exceptions;
- merge/PHI values;
- SSA relationships;
- instruction-to-SLIR provenance.

The SLIR must preserve uncertainty. An unknown or partially understood instruction should produce an explicit unknown/evidence node rather than fabricated semantics.

## Phase 2E — Data flow and type evidence

Primary items: **6 and 7**, supported by **18, 19, 20, 21, 22, 30, 39**.

Add:

- def-use/use-def chains;
- reaching definitions;
- liveness;
- constant and copy propagation;
- value-set tracking;
- stack-variable recovery;
- register-variable recovery;
- pointer/reference tracking;
- conservative alias analysis;
- integer width/signedness evidence;
- pointer types;
- arrays;
- structs/unions/enums;
- function-pointer evidence;
- floating/vector types;
- ABI-derived argument/return evidence.

Types must remain evidence-based. When evidence is insufficient, emit an unknown/opaque type rather than inventing a specific type.

## Phase 2F — Source reconstruction contract

Primary items: **8, 9, 10, 11**, supported by **5–7 and 30**.

Phase 2 does not claim that Java, Sleela, C, or C++ reconstruction is complete. Instead, it establishes the shared semantic contract required for those backends.

The source reconstruction pipeline becomes:

**binary → instructions → CFG/functions → SLIR → data flow/types → language-specific AST/IR → source.**

The four backends must consume the same evidence and preserve unknowns where necessary.

Target responsibilities:

- **C:** procedural expressions, declarations, globals, structs/unions/enums, prototypes and function pointers.
- **C++:** C reconstruction plus classes, methods, namespaces, templates/ABI evidence where recoverable, and C++-specific object-model evidence.
- **Sleela:** compiler-valid modules, types, functions, control flow, native interop, and explicit unknown/evidence constructs.
- **Java:** classes, methods, fields, arrays, references, exceptions, packages, and JVM-compatible type mappings where evidence supports them.

## Phase 2G — Reliability requirements

Phase 2 is not complete merely because the decoder produces more instructions.

The phase must include:

- regression tests for every newly supported instruction family;
- malformed/truncated artifact tests;
- decoder fuzz targets;
- deterministic-output tests;
- resource-limit tests;
- cancellation-safe analysis boundaries where the API supports them;
- address/provenance assertions;
- coverage accounting for decoded, unsupported, ambiguous, and unknown regions;
- security tests proving analyzed artifacts are never executed.

## Phase 2 Definition of Done

Phase 2 is complete only when the repository can demonstrate, with tests or corpus evidence:

1. explicit address-domain separation;
2. checked address translation;
3. substantially expanded x86/x86-64 decoding;
4. real basic-block CFG construction;
5. evidence-based function recovery;
6. a reusable SLIR semantic model;
7. initial data-flow analysis;
8. initial type recovery;
9. source reconstruction driven by SLIR rather than function shells;
10. shared semantics feeding Java, Sleela, C, and C++;
11. structured errors for malformed/unsupported/ambiguous regions;
12. deterministic output;
13. measured coverage for the Phase 2 corpus;
14. no target execution.

Phase 2 should **not** be marked complete based on line count, number of files, or a claimed percentage. Completion requires reproducible technical evidence.

## Phase 2 Work Sequence

1. Address-domain types and translation.
2. Load-base/PIE/relocation correctness.
3. Decoder operand model.
4. x86/x86-64 opcode and prefix expansion.
5. Basic-block construction.
6. Branch/call/return edge recovery.
7. Function discovery and provenance.
8. SLIR instruction semantics.
9. Def-use and liveness.
10. Initial type recovery.
11. CFG/SLIR-driven source reconstruction.
12. Java/Sleela/C/C++ backend integration.
13. Regression corpus and fuzzing.
14. Coverage measurement and deterministic-output verification.
15. Phase 2 review and documented evidence.

## Phase 2 Non-Goals

The following remain outside the completion claim for Phase 2 unless separately demonstrated:

- complete ARM/ARM64 decoding;
- complete RISC-V support;
- complete PE/Mach-O/DWARF/PDB support;
- complete optimization-aware recovery;
- reliable deobfuscation;
- perfect type recovery;
- perfect source reconstruction;
- execution or emulation of target programs.

## Iteration Policy

Each implementation iteration should:

1. inspect the current implementation;
2. implement the next safe subset in order;
3. add regression tests;
4. update this document with what is actually implemented;
5. avoid claiming support that is not measured;
6. preserve the non-executing security boundary;
7. keep `main` and `master` synchronized in the changes made.

## Current Phase 2 Starting Point

Phase 1 has already established:

- structured `DecompilerError` / `ErrorCode` handling with stage, file-offset, and virtual-address context;
- stricter CLI length parsing;
- explicit help/version behavior;
- output-file validation;
- regression coverage for the error object and four source-output paths;
- the non-executing static-analysis boundary.

The immediate Phase 2 implementation target is **the explicit address-space model**, followed by decoder expansion and CFG/function recovery. The existing source emitters remain output shells until SLIR and reconstruction provide semantic content.

**Known limitation:** local compilation/CTest execution is environment-dependent. Repository changes should not be described as build-verified unless CI or a real build result is available.


# Phase 3 — Binary Formats, ABI, and Native Evidence

## Purpose

Phase 3 takes the native-analysis core established by Phase 2 and makes it understand the major binary containers, linking models, debug information, unwind metadata, calling conventions, and runtime evidence needed for serious cross-platform decompilation.

The governing architecture remains:

**artifact → container/segment model → address map → native instructions → CFG/functions → ABI/runtime evidence → SLIR → data flow/types → source.**

Phase 3 must not create isolated format-specific analyzers that bypass the shared semantic model. ELF, PE/COFF, Mach-O, archives, shared libraries, debug information, exception metadata, and ABI information all become evidence sources feeding the same analysis graph.

## Phase 3A — ELF and Unix/Linux binary evidence

Primary items: **12, 14, 15, 16, 17, 18, 19, 20**.

Expand ELF-oriented analysis to include:

- ELF32 and ELF64 headers;
- program headers and section headers;
- PT_LOAD segment mappings;
- section/segment relationships;
- symbol tables and string tables;
- relocations;
- dynamic sections;
- DT_NEEDED dependencies;
- PLT/GOT relationships;
- symbol versioning;
- IFUNC resolution evidence;
- TLS;
- constructors/destructors;
- weak symbols;
- GNU symbol/version extensions;
- PIE/shared-object characteristics;
- GNU build/runtime notes where relevant.

The loader model must distinguish what is physically present in the file from what the runtime loader may construct or relocate.

## Phase 3B — PE/COFF and Windows evidence

Primary item: **12**, supported by **16–20 and 36**.

Implement a complete PE/COFF evidence layer covering:

- DOS/PE headers;
- COFF headers;
- optional headers;
- section table;
- RVA/file-offset translation;
- imports and exports;
- delay imports;
- base relocations;
- TLS;
- resources;
- load configuration;
- exception/unwind information;
- Windows CFG-related metadata;
- SEH-related metadata;
- debug-directory references;
- PDB identity/reference information;
- executable, DLL, and object-file distinctions.

PE parsing must explicitly validate sizes, RVAs, section boundaries, alignment assumptions, and integer arithmetic before using metadata.

## Phase 3C — Mach-O and Apple-platform evidence

Primary item: **13**, supported by **16–20 and 36**.

Implement:

- 32-bit and 64-bit Mach-O headers;
- load commands;
- segments and sections;
- symbol tables;
- dynamic linking information;
- dyld exports;
- chained fixups where applicable;
- relocations;
- TLS;
- Objective-C metadata;
- Swift metadata;
- code-signing metadata;
- fat/universal binaries;
- architecture slices;
- slice selection and explicit architecture validation.

Universal binaries must be represented as multiple artifacts/slices rather than silently treating one slice as the entire file.

## Phase 3D — Static archives and object collections

Primary item: **14**.

Support GNU/Unix archive analysis as a container of independently analyzable members.

Required behavior:

- parse archive headers safely;
- enumerate members deterministically;
- recognize symbol indexes;
- recursively analyze supported object members;
- preserve member-to-symbol relationships;
- represent weak/duplicate/COMDAT evidence;
- identify cross-member call/data relationships;
- prevent archive metadata from being mistaken for executable instructions.

Archive analysis should produce a unified project-level evidence graph while retaining member provenance.

## Phase 3E — Dynamic/shared libraries and linking

Primary item: **15**, supported by **3, 4, 18, 19, 20**.

Model the effects of dynamic linking without executing the loader:

- imports/exports;
- PLT/GOT;
- symbol versions;
- IFUNC;
- weak/interposable symbols;
- TLS;
- constructors/destructors;
- dependency graphs;
- RPATH/RUNPATH;
- platform-specific dynamic-loader metadata;
- lazy versus non-lazy binding evidence where available.

A dynamic symbol reference should be represented as a relationship with evidence, not automatically treated as the implementation of the target function.

## Phase 3F — Debug information and provenance

Primary item: **16**, supported by **30 and 39**.

Create a common debug-evidence abstraction for:

- DWARF;
- CodeView/PDB;
- dSYM;
- source file names;
- line tables;
- lexical scopes;
- variables;
- types;
- inline functions;
- function boundaries;
- declaration/definition relationships.

Debug evidence must be preserved with provenance and confidence. Conflicting debug evidence must remain visible rather than being silently flattened.

Source-to-instruction provenance should become bidirectional where possible:

**source location ↔ recovered entity ↔ SLIR operation ↔ instruction ↔ binary location.**

## Phase 3G — Exception and unwind metadata

Primary item: **17**, supported by **3, 4, 5, 18, 30**.

Implement evidence extraction for:

- DWARF CFI;
- .eh_frame;
- .debug_frame;
- Windows unwind metadata;
- SEH structures;
- language-specific exception tables;
- cleanup/finally paths;
- landing pads;
- personality/runtime references.

Unwind data must influence function and stack analysis without being treated as executable code.

## Phase 3H — ABI and calling-convention model

Primary item: **18**.

Create a reusable ABI model for:

- System V AMD64;
- Microsoft x64;
- x86 cdecl;
- stdcall;
- fastcall;
- thiscall;
- vectorcall;
- AAPCS32;
- AAPCS64;
- relevant RISC-V conventions;
- platform-specific return/register conventions.

The model should describe:

- argument locations;
- return-value locations;
- caller/callee-saved registers;
- stack alignment;
- shadow/home space;
- aggregate passing;
- floating/vector argument rules;
- variadic behavior;
- name decoration where relevant;
- unwind/frame implications.

ABI conclusions remain evidence-based and can be marked ambiguous when multiple conventions fit.

## Phase 3I — Global/data/object-model recovery

Primary item: **19**, supported by **5–7, 15–20**.

Recover and classify:

- strings;
- scalar constants;
- arrays;
- relocation-backed objects;
- global variables;
- TLS variables;
- jump tables;
- vtables;
- RTTI;
- function-pointer tables;
- dispatch tables;
- read-only versus writable data;
- probable object layouts.

Data classification must not confuse arbitrary bytes with typed objects. Every classification should have an evidence path.

## Phase 3J — Compiler and runtime fingerprints

Primary item: **20**, supported by **21–23**.

Build fingerprints for common compiler/runtime families including:

- GCC;
- Clang/LLVM;
- MSVC;
- ICC/ICX;
- Rust;
- Go;
- Swift;
- Delphi;
- common language runtimes and standard libraries.

Fingerprints may guide hypotheses, decoder choices, ABI selection, function recovery, and source reconstruction, but **a fingerprint is never proof of compiler, language, or source construct**.

## Phase 3K — Cross-format evidence graph

Phase 3 should introduce a common evidence graph connecting:

- artifact;
- slice/member;
- segment/section;
- symbol;
- relocation;
- instruction;
- basic block;
- function;
- type;
- global;
- debug entity;
- ABI fact;
- runtime fingerprint;
- exception/unwind entity;
- source location.

Every relationship should retain provenance and, where appropriate:

- confidence;
- source format;
- file offset;
- virtual address;
- architecture;
- evidence kind;
- conflicting evidence.

This graph becomes the bridge between binary formats and SLIR.

## Phase 3L — Phase 3 reliability requirements

Every new parser must include:

- truncated-input tests;
- malformed-header tests;
- integer-overflow tests;
- invalid-offset/RVA tests;
- section/segment boundary tests;
- duplicate/contradictory metadata tests;
- deterministic enumeration tests;
- unsupported-feature reporting;
- resource-limit enforcement;
- fuzzing targets where practical;
- regression fixtures from multiple architectures/platforms.

No parser should trust lengths, offsets, counts, or addresses merely because they occur in a binary header.

## Phase 3 Definition of Done

Phase 3 is complete only when reproducible tests or corpus evidence demonstrate:

1. robust ELF evidence extraction;
2. robust PE/COFF evidence extraction;
3. robust Mach-O evidence extraction;
4. recursive archive/member analysis;
5. dynamic-linking evidence modeling;
6. common debug-information abstraction;
7. exception/unwind evidence extraction;
8. reusable ABI/calling-convention modeling;
9. global/data/object-model recovery;
10. compiler/runtime fingerprints represented as evidence;
11. all Phase 3 evidence connected to the common analysis graph;
12. address/provenance preserved through format parsing;
13. malformed metadata handled without unsafe memory access;
14. deterministic analysis results;
15. measured coverage for the Phase 3 corpus;
16. no target execution.

Phase 3 is not complete because a parser accepts a sample file. Completion requires cross-format regression evidence, malformed-input coverage, and integration with the shared native-analysis pipeline.

## Phase 3 Work Sequence

1. Common container/segment interfaces.
2. ELF headers, sections, segments, symbols, and relocations.
3. ELF dynamic linking and PLT/GOT evidence.
4. PE/COFF headers, sections, imports/exports, relocations, and TLS.
5. PE unwind/debug/load-config evidence.
6. Mach-O headers, load commands, slices, symbols, and fixups.
7. Archive/member/symbol-index analysis.
8. Shared-library/dependency graph.
9. DWARF/PDB/CodeView/dSYM evidence abstraction.
10. Exception/unwind integration.
11. ABI/calling-convention model.
12. Global/data/vtable/RTTI recovery.
13. Compiler/runtime fingerprint evidence.
14. Cross-format evidence graph integration.
15. Fuzzing, regression corpus, coverage measurement, and Phase 3 review.

## Phase 3 Non-Goals

Phase 3 does not claim:

- perfect source reconstruction;
- complete deobfuscation;
- universal compiler identification;
- perfect type recovery;
- execution of analyzed programs;
- automatic correctness of every inferred function boundary;
- complete support for every historical binary-format extension.

Those remain subject to measured evidence and later phases.

## Phase 3 Relationship to the 1–40 Register

Phase 3 primarily advances **Items 11–20**, while strengthening:

- **Item 2:** address translation;
- **Item 3:** control-flow evidence;
- **Item 4:** function recovery;
- **Item 5:** SLIR provenance;
- **Items 6–7:** data-flow/type evidence;
- **Item 22:** stripped-binary strategy;
- **Item 28:** deterministic output;
- **Item 30:** provenance;
- **Item 33:** parser fuzzing;
- **Items 35–36:** architecture/load-address correctness;
- **Item 39:** measured coverage;
- **Item 40:** non-executing security boundary.

## Current Phase 3 Starting Point

Phase 2 remains the prerequisite for Phase 3. Phase 3 should begin with the common container/segment/address interfaces rather than immediately adding independent format parsers.

The implementation target is therefore:

**Phase 2 native core → common binary-container model → ELF → PE/COFF → Mach-O → archives/dynamic linking → debug/unwind → ABI → global/data evidence → unified evidence graph.**

As with Phase 2, Phase 3 completion must be documented from actual repository tests and corpus measurements, not from source-file count or an aspirational percentage.

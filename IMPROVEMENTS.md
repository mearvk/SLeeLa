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

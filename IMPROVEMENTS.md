# Slecompiler Improvements

This document is the active engineering backlog, architecture plan, and evidence standard for Slecompiler™ in `/decompiler`.

The **1–40 register is authoritative**. Phases are implementation groupings over that register; they do not replace it. A feature is not considered complete because a parser, emitter, or stub exists. Completion requires reproducible implementation evidence, regression coverage, and measured behavior.

## Implementation Status

- **Phase 1 — Foundation:** structured errors, CLI hardening, static-analysis boundary, initial regression scaffolding.
- **Phase 2 — Native Analysis Core:** address domains, instruction decoding, CFG/function recovery, SLIR, data flow, type evidence, and shared source reconstruction.
- **Phase 3 — Binary Formats, ABI, and Native Evidence:** ELF, PE/COFF, Mach-O, archives, dynamic linking, debug/unwind metadata, ABI, and evidence graph.
- **Phase 4 — Recovery, Reconstruction, and Difficult Artifacts:** optimization-aware recovery, stripped binaries, packed/obfuscated artifacts, provenance-driven reconstruction, semantic reconciliation, and difficult native patterns.
- **Phase 5 — Production Validation and Security:** regression corpus, sanitizers, fuzzing, CLI/API stability, concurrency, caching, measured coverage, and security boundary.
- Percentages in documentation are **targets unless backed by measured corpus/test evidence**.

---

# 1–40 Improvement Register

## 1. Native instruction decoding

Build a production-quality instruction decoder beginning with x86/x86-64 and then extending the same semantic interface to ARM/ARM64 and RISC-V.

The decoder must cover prefixes, opcode maps, ModR/M, SIB, displacement and immediates, operand/address widths, register classes, memory operands, VEX/EVEX, SIMD, floating point, atomics, fences, system/control instructions, privileged instructions where representable, and instruction-length validation.

Every decode result must identify whether it is **supported, unsupported, ambiguous, malformed, or unknown**. Unknown bytes must never silently become proven instructions.

## 2. Address-space model

Make file offset, section-relative offset, virtual address, preferred image address, actual load address, runtime address, instruction address, and relocation target address explicit concepts.

Translations must be checked and preserve the mapping source. PIE/load-base, segment permissions, gaps, overflow, relocation domains, and architecture-specific address rules must be modeled without implicit conversions.

## 3. Control-flow recovery

Recover basic blocks and edges for fall-through, direct/conditional branches, calls, returns, indirect branches, jump tables, switches, tail calls, thunks, PLT/GOT paths, exception edges, and runtime-generated relationships.

Ambiguous indirect targets must be represented as hypotheses with evidence instead of being forced into a single CFG.

## 4. Function recovery

Combine symbols, entry points, relocation evidence, call targets, prologue/epilogue patterns, tail calls, thunks, exception handlers, unwind data, runtime metadata, compiler fingerprints, and data-flow boundaries.

Function identity, boundary, calling convention, and confidence must remain independently represented because each can have different evidence.

## 5. Semantic lifting / SLIR

Create a stable Slecompiler Low-level Intermediate Representation (SLIR) for registers, flags, constants, memory, pointers, arithmetic, logical operations, comparisons, loads/stores, branches, calls, returns, stack/frame state, atomics, volatile operations, exceptions, merges/PHI nodes, SSA relationships, and provenance.

SLIR is the shared semantic contract for all later source backends.

## 6. Data-flow analysis

Implement def-use/use-def chains, reaching definitions, liveness, constant propagation, copy propagation, value sets, stack/register variable recovery, pointer/reference tracking, conservative alias analysis, memory-state reasoning, and dead-code evidence.

Analysis must be path-aware where practical and must preserve uncertainty when multiple interpretations remain possible.

## 7. Type recovery

Recover type evidence from instruction widths, signedness, pointer arithmetic, memory access patterns, ABI rules, debug information, data structures, function signatures, RTTI/vtables, and cross-reference behavior.

Support primitive types, pointers, arrays, structs/unions/enums, function pointers, qualifiers, floating/vector types, opaque types, and ABI-derived parameter/return types.

## 8. Source reconstruction

Replace function-shell output with structured reconstruction from CFG + SLIR + data flow + types.

Recover expressions, assignments, declarations, conditionals, loops, switches, calls, returns, memory objects, data structures, and control-flow structure while preserving irreducible or unknown regions explicitly.

## 9. Java backend

Generate evidence-based Java classes, methods, fields, packages, arrays, references, exceptions, primitive/reference mappings, and method signatures.

Do not claim Java semantics that are not supported by the recovered native evidence; native-specific behavior may require explicit annotations or reconstruction markers.

## 10. Sleela backend

Define and enforce a compiler-valid Sleela output grammar for modules, declarations, types, functions, control flow, native interop, unknown regions, and evidence annotations.

The Sleela backend must consume the shared SLIR/reconstruction model rather than independently interpreting machine instructions.

## 11. C/C++ backends

Recover C declarations, prototypes, globals, structs/unions/enums, typedefs, expressions, control flow, and function pointers.

Extend C++ reconstruction with namespaces, classes, methods, constructors/destructors where evidenced, inheritance/object-model information, templates where recoverable, RTTI/vtable evidence, and ABI attributes.

## 12. PE/COFF analysis

Implement DOS/PE/COFF headers, optional headers, sections, RVA/file-offset mapping, imports/exports, delay imports, relocations, TLS, resources, load configuration, CFG-related metadata, SEH/unwind metadata, debug-directory/PDB references, and executable/DLL/object distinctions.

Validate every count, size, offset, RVA, alignment assumption, and arithmetic operation before using metadata.

## 13. Mach-O analysis

Implement 32/64-bit headers, load commands, segments/sections, symbols, dyld exports, chained fixups, relocations, TLS, Objective-C metadata, Swift metadata, code-signing metadata, and fat/universal binaries.

Universal binaries must be represented as multiple architecture slices with explicit slice selection.

## 14. GNU archive analysis

Treat archives as containers of independently analyzable members.

Safely parse archive headers, deterministic member order, symbol indexes, weak/duplicate/COMDAT relationships, member-to-symbol links, and cross-member references without mistaking archive metadata for executable code.

## 15. Dynamic/shared-library analysis

Model imports, exports, PLT/GOT, symbol versions, IFUNC, TLS, constructors/destructors, DT_NEEDED dependencies, RPATH/RUNPATH, weak/interposable symbols, and lazy/non-lazy binding evidence.

A dynamic reference is a relationship and evidence record; it is not automatically the implementation of the referenced function.

## 16. Debug information

Build a common abstraction for DWARF, CodeView/PDB, dSYM, source files, line tables, scopes, variables, types, inline functions, declaration/definition relationships, and function boundaries.

Conflicting or incomplete debug evidence must remain visible and provenance-preserving.

## 17. Exception/unwind analysis

Parse DWARF CFI, `.eh_frame`, `.debug_frame`, Windows unwind information, SEH structures, language-specific exception tables, landing pads, cleanup/finally paths, and personality/runtime references.

Unwind metadata must improve function/stack recovery without being mistaken for executable instructions.

## 18. Calling conventions / ABI

Model System V AMD64, Microsoft x64, x86 cdecl/stdcall/fastcall/thiscall/vectorcall, AAPCS32/AAPCS64, and relevant RISC-V conventions.

Represent argument locations, returns, caller/callee-saved registers, stack alignment, shadow/home space, aggregate passing, floating/vector arguments, variadic behavior, name decoration, and frame/unwind implications.

## 19. Global/data analysis

Recover strings, scalar constants, arrays, relocation-backed objects, globals, TLS variables, jump tables, vtables, RTTI, function-pointer tables, dispatch tables, and probable object layouts.

Typed classifications require evidence; arbitrary bytes must not be presented as confirmed objects.

## 20. Compiler/runtime fingerprinting

Build evidence fingerprints for GCC, Clang/LLVM, MSVC, ICC/ICX, Rust, Go, Swift, Delphi, standard libraries, language runtimes, and common generated-code idioms.

Fingerprints can guide hypotheses but are never proof of compiler, language, source construct, or function identity.

## 21. Optimization-aware recovery

Handle optimized artifacts rather than assuming source-like instruction structure.

Account for O0–O3/Os/Ofast behavior, inlining, tail-call optimization, common subexpression elimination, register allocation, instruction scheduling, vectorization, constant folding, dead-code elimination, LTO, ThinLTO, PGO, outlining, identical-code folding, and linker optimization.

The analysis must distinguish **optimized-away source concepts** from merely unrecovered concepts.

## 22. Stripped-binary strategy

Classify symbol/debug availability before analysis and adapt recovery for fully stripped, partially stripped, debug-stripped, symbol-only, and mixed artifacts.

Use call targets, relocation evidence, ABI behavior, code/data boundaries, unwind information, runtime metadata, and cross-references as alternate evidence sources.

## 23. Packed/obfuscated artifacts

Detect and report evidence of packing, compression, encryption, opaque predicates, flattened control flow, indirect dispatch, anti-analysis structures, and possible self-modification.

Do not silently unpack, execute, or assert a guessed original program. Static evidence, confidence, and limitations must remain explicit.

## 24. Structured error/exception model

Use typed, stage-aware errors including InvalidArtifact, TruncatedArtifact, UnsupportedFormat, UnsupportedArchitecture, MalformedHeader, InvalidSection, InvalidAddress, DecodeFailure, AmbiguousDecode, UnsupportedInstruction, AnalysisFailure, OutputFailure, and ResourceLimit.

Errors should retain file offset, virtual address, analysis stage, and useful diagnostic context where available.

## 25. Fault tolerance

Continue through recoverable malformed, unsupported, or ambiguous regions.

Record per-region status, skipped ranges, unknown instructions, unsupported features, partial functions, and coverage impact rather than failing the entire artifact unnecessarily.

## 26. Resource limits

Provide explicit limits for memory, input size, instruction count, function count, CFG nodes, recursion depth, analysis depth, wall-clock time, output size, cache size, and parallel work.

Limits must fail predictably and report which limit was reached.

## 27. Cancellation and progress

Provide library and CLI cancellation boundaries and structured progress information.

Progress should identify analysis phase, completed/total work where measurable, and whether the value is exact, estimated, or indeterminate.

## 28. Deterministic output

The same artifact, configuration, tool version, and deterministic mode must produce stable analysis ordering, source output, JSON, hashes, diagnostics, and cache keys.

Non-deterministic operations must be isolated or explicitly labeled.

## 29. Machine-readable output

Provide stable machine-readable output for analysis results, including JSON first and additional formats such as SARIF/YAML where justified.

Represent artifact identity, architecture, address maps, sections, functions, CFG, SLIR, evidence, confidence, errors, unknowns, unsupported regions, coverage, provenance, and tool/version metadata.

## 30. Source-to-instruction provenance

Maintain a trace from generated source to language AST/IR, SLIR, instruction addresses, file offsets, and originating binary artifacts.

Where possible, maintain the reverse relationship so a user can navigate from an instruction or evidence item back to generated source.

## 31. Regression corpus

Build a permanent cross-platform corpus covering C, C++, Rust, Go, Swift, Java/native interfaces, templates, virtual dispatch, exceptions, threads, libraries, stripped binaries, debug binaries, optimized builds, multiple ABIs, and architecture-specific constructs.

Corpus cases must have expected structural properties rather than relying only on exact source-text matching.

## 32. Sanitizer/UB testing

Use ASan, UBSan, LeakSan, ThreadSan, and other platform-appropriate instrumentation where supported.

Sanitizer findings must be treated as release-blocking defects when they demonstrate unsafe behavior in the analyzer.

## 33. Parser/decoder fuzzing

Fuzz binary containers, headers, address translation, instruction decoding, debug metadata, archive parsing, SLIR construction, source reconstruction, CLI parsing, and machine-readable output.

Track crashes, hangs, excessive memory use, integer overflow, OOB access, invalid state transitions, and pathological complexity.

## 34. CLI correctness

Maintain stable help/version behavior, strict argument validation, explicit output-language selection, predictable file handling, meaningful diagnostics, stable exit codes, and safe defaults.

CLI behavior must agree with the library API and documented examples.

## 35. Architecture selection

Detect artifact architecture and permit explicit user selection only when compatible.

Reject contradictions instead of silently decoding bytes under the wrong architecture. Multi-architecture artifacts must expose their architecture set and selected slice.

## 36. Base/load address handling

Model preferred image base, actual load base, PIE, ASLR, relocation-adjusted addresses, shared-library load bias, and runtime addresses without conflating them with file offsets.

Every conversion must identify its address domain and mapping evidence.

## 37. Thread-safe API

Permit independent analyses to run concurrently without shared mutable global state.

Caches, registries, diagnostics, configuration, and temporary analysis state must have explicit ownership and synchronization rules.

## 38. Caching

Add content-addressed caching for artifact parsing, instruction decoding, CFGs, function recovery, SLIR, data flow, type evidence, and generated source.

Cache entries must include artifact identity, relevant configuration, architecture, tool/schema version, and invalidation information.

## 39. Measured coverage

Measure format, architecture, instruction, byte, basic-block, CFG-edge, function, symbol, type, semantic, source, provenance, and unknown/unsupported/ambiguous coverage.

Coverage reports must distinguish **decoded** from **semantically understood**, and **reconstructed** from **proven equivalent to source**.

## 40. Security boundary

Slecompiler is a static-analysis/decompilation system and must not execute analyzed target code by default.

Do not run target instructions, constructors, scripts, embedded interpreters, kernel modules, or untrusted build steps during analysis. Any future emulation/sandboxing must be an explicitly isolated subsystem with separate policy, resource limits, and auditability.

---

# Phase 2 — Native Analysis Core

## Purpose

Phase 2 establishes the native semantic pipeline:

**artifact bytes → address model → instruction decoder → CFG → function recovery → SLIR → data flow/types → source backends.**

Java, Sleela, C, and C++ must consume the same recovered semantic representation.

## Phase 2A — Address-space foundation

Primary items: **2, 24–26, 28, 30, 35–36, 39–40**.

Implement explicit address-domain types and checked translations. Required evidence includes valid mappings, boundaries, unmapped gaps, overflow, relocation targets, PIE/load-base behavior, and malformed artifacts.

## Phase 2B — Native decoder expansion

Primary item: **1**, supported by **24–26, 33, 39**.

Expand x86/x86-64 decoding through prefix/opcode/ModR/M/SIB/operand layers, SIMD/VEX/EVEX, floating/vector, atomic, system/control, instruction boundaries, and explicit unsupported/ambiguous states.

ARM/ARM64 and later RISC-V must use the same semantic interface.

## Phase 2C — CFG and function recovery

Primary items: **3–4**.

Recover basic blocks, branches, calls, returns, indirect targets, jump tables, switches, tail calls, thunks, exception edges, and evidence-based function boundaries.

## Phase 2D — SLIR

Primary item: **5**, supported by **6–7, 18, 30**.

SLIR must represent machine semantics without inventing semantics for unknown instructions. Every SLIR operation should be traceable to instruction evidence where applicable.

## Phase 2E — Data flow and type evidence

Primary items: **6–7**.

Implement def-use, liveness, propagation, pointer tracking, conservative alias analysis, stack/register variables, primitive types, aggregates, function pointers, and ABI-derived signatures.

## Phase 2F — Shared source reconstruction

Primary items: **8–11**.

The pipeline is:

**binary → instructions → CFG/functions → SLIR → data flow/types → language AST/IR → source.**

The four backends must share evidence and preserve unknown regions.

## Phase 2G — Reliability

Every new decoder/analysis family requires regression tests, malformed-input tests, fuzz coverage where practical, deterministic-output tests, resource-limit tests, provenance assertions, and proof that target execution does not occur.

## Phase 2 Definition of Done

Phase 2 requires demonstrated address separation, expanded decoding, real CFGs, evidence-based functions, reusable SLIR, initial data flow/type recovery, SLIR-driven reconstruction, shared backends, structured errors, deterministic output, measured coverage, and no target execution.

---

# Phase 3 — Binary Formats, ABI, and Native Evidence

## Purpose

Phase 3 makes the native core understand major binary containers, link models, debug information, unwind metadata, ABI rules, and runtime evidence.

The architecture is:

**artifact → container/segment model → address map → instructions → CFG/functions → ABI/runtime evidence → SLIR → data flow/types → source.**

Format-specific parsers must feed a common evidence model.

## Phase 3A — ELF

Support ELF32/64 headers, program/section headers, PT_LOAD mappings, symbols, strings, relocations, dynamic sections, DT_NEEDED, PLT/GOT, symbol versions, IFUNC, TLS, constructors/destructors, weak symbols, PIE/shared objects, and relevant GNU metadata.

## Phase 3B — PE/COFF

Support DOS/PE/COFF headers, sections, RVA/file offsets, imports/exports, delay imports, relocations, TLS, resources, load configuration, CFG/SEH/unwind metadata, debug/PDB references, and object/executable/DLL distinctions.

## Phase 3C — Mach-O

Support headers, load commands, segments/sections, symbols, dyld exports, fixups, relocations, TLS, Objective-C/Swift metadata, code-signing metadata, and universal slices.

## Phase 3D — Archives

Safely enumerate members, symbol indexes, weak/duplicate/COMDAT relationships, and cross-member evidence while preserving member provenance.

## Phase 3E — Dynamic linking

Model imports/exports, PLT/GOT, versions, IFUNC, TLS, constructors/destructors, dependencies, RPATH/RUNPATH, interposition, and binding evidence without executing the loader.

## Phase 3F — Debug/provenance

Unify DWARF, PDB/CodeView, dSYM, source files, lines, scopes, variables, types, inline functions, and declarations/definitions into evidence-bearing entities.

## Phase 3G — Exception/unwind

Integrate CFI, .eh_frame/.debug_frame, Windows unwind/SEH, language exception tables, landing pads, cleanup paths, and personality/runtime references.

## Phase 3H — ABI

Provide reusable calling-convention models with argument/return locations, register preservation, stack rules, aggregate passing, floating/vector rules, variadic behavior, and frame/unwind implications.

## Phase 3I — Global/data/object model

Recover strings, constants, arrays, globals, TLS, jump tables, vtables, RTTI, function-pointer tables, and probable object layouts with evidence.

## Phase 3J — Fingerprints

Use compiler/runtime fingerprints as hypotheses only. Preserve the actual evidence supporting or contradicting each fingerprint.

## Phase 3K — Common evidence graph

Connect artifacts, slices/members, sections, symbols, relocations, instructions, blocks, functions, types, globals, debug entities, ABI facts, runtime fingerprints, unwind entities, and source locations.

Each relationship should preserve provenance, architecture, address domain, evidence kind, and conflicts.

## Phase 3L — Reliability

All parsers require malformed/truncated tests, integer-overflow tests, invalid offset/RVA tests, boundary tests, contradiction tests, deterministic enumeration, resource limits, fuzzing where practical, and multi-platform regression fixtures.

## Phase 3 Definition of Done

Phase 3 requires demonstrated ELF, PE/COFF, Mach-O, archive, dynamic-linking, debug, unwind, ABI, global/data, fingerprint, and evidence-graph integration, plus safe malformed-input behavior, deterministic results, measured coverage, and no target execution.

---

# Phase 4 — Recovery, Reconstruction, and Difficult Artifacts

## Purpose

Phase 4 takes the evidence-rich native analysis produced by Phases 2–3 and addresses the hardest part of decompilation: reconstructing useful higher-level program structure when compilation has destroyed, transformed, or obscured the original source structure.

The governing pipeline becomes:

**binary → format/address evidence → instructions → CFG/functions → ABI/runtime evidence → SLIR → SSA/data flow → memory/type model → structural recovery → language IR → source → provenance/validation.**

Phase 4 is explicitly **not** a promise of perfect original-source recovery. A native binary often does not contain enough information to uniquely recover the original source. The objective is a reproducible, evidence-preserving reconstruction that distinguishes known facts, strong inferences, weak hypotheses, and unknowns.

## Phase 4A — Optimization-aware normalization

Primary items: **21, 3–8, 18, 20, 30**.

Normalize optimized machine code into semantic forms that survive compiler transformations.

Handle:

- inlining;
- tail calls;
- register promotion;
- stack-slot elimination;
- constant folding;
- common-subexpression elimination;
- loop transformations;
- vectorization;
- instruction scheduling;
- dead-code elimination;
- function outlining;
- identical-code folding;
- LTO/ThinLTO;
- PGO-driven layout;
- linker relaxation and optimization.

The analyzer must not mistake an optimized-away variable or branch for an analysis failure.

## Phase 4B — SSA and value-set recovery

Primary items: **5–8**.

Build robust SSA/PHI relationships over recovered CFGs. Track scalar values, pointers, flags, memory state, stack slots, register lifetimes, and path-sensitive facts where practical.

Value recovery should distinguish:

- exact constants;
- bounded value sets;
- symbolic expressions;
- unknown values;
- conflicting path facts.

## Phase 4C — Memory and alias model

Primary items: **6–7, 19, 21–23**.

Develop a conservative memory model for stack, global, TLS, heap-like references, mapped regions, and unknown memory.

Track pointer arithmetic, object candidates, alias sets, escape behavior, loads/stores, volatile accesses, atomics, and possible overlapping objects.

The analyzer must prefer an opaque memory object over an unjustified concrete structure.

## Phase 4D — Structural control-flow recovery

Primary items: **3, 8, 21, 23**.

Recover high-level constructs from CFG/SLIR:

- if/else;
- nested conditionals;
- loops;
- do/while;
- switch/case;
- early exits;
- break/continue;
- irreducible regions;
- state-machine dispatch;
- tail-call structures;
- exception cleanup.

When a graph cannot be cleanly structured, preserve a lower-level representation rather than emitting misleading source.

## Phase 4E — Function signature and call reconstruction

Primary items: **4, 7, 18–20, 22**.

Combine ABI, call-site, data-flow, debug, symbol, and return-use evidence to reconstruct:

- parameter count;
- parameter locations;
- parameter types;
- return types;
- variadic behavior;
- calling convention;
- pointer/reference semantics;
- likely ownership/lifetime relationships where evidence supports them.

Signatures must record their evidence and unresolved alternatives.

## Phase 4F — Object model reconstruction

Primary items: **7, 11, 19–20**.

Recover C/C++-style objects from:

- vtables;
- RTTI;
- constructor/destructor patterns;
- virtual calls;
- field-offset access;
- inheritance evidence;
- object-size behavior;
- type metadata;
- cross-function field consistency.

Separate **observed layout** from **inferred language-level class structure**.

## Phase 4G — Template/generic and compiler-generated structures

Primary items: **11, 20–22**.

Recognize repeated/generated patterns associated with templates, generics, monomorphization, iterators, closures, lambdas, coroutine/state-machine code, compiler helper functions, thunks, and standard-library implementations.

Generated-code identification must remain evidence-based and must not manufacture source-level template names.

## Phase 4H — Exception and asynchronous control-flow reconstruction

Primary items: **3, 4, 17, 18**.

Integrate exception edges, landing pads, cleanup paths, personality routines, destructors, finally behavior, signal-like control transfers where statically visible, and asynchronous/thread-related evidence.

The result must distinguish normal control flow from exceptional or externally transferred control flow.

## Phase 4I — Stripped and partially stripped reconstruction

Primary item: **22**.

Create explicit analysis modes for:

- fully symbolized;
- symbols without debug;
- debug-stripped;
- partially stripped;
- fully stripped;
- dynamically linked;
- statically linked.

Recovery quality reports must identify which evidence sources remain available and how their absence affects reconstruction.

## Phase 4J — Packed/obfuscated evidence model

Primary item: **23**.

Detect likely packers, compressed regions, encrypted regions, opaque predicates, flattened CFGs, indirect dispatch, anti-analysis structures, and possible runtime unpacking.

Static analysis may identify and describe these structures but must not silently execute or bypass the security boundary.

## Phase 4K — Semantic reconciliation

Primary items: **5–8, 20–23, 28–30, 39**.

Create a reconciliation layer that compares competing evidence:

- debug type vs ABI type;
- symbol name vs recovered function boundary;
- CFG hypothesis A vs B;
- compiler fingerprint vs instruction pattern;
- object-layout hypotheses;
- source reconstruction alternatives.

Conflicts must be retained, explained, and resolved only when evidence supports the resolution.

## Phase 4L — Language-neutral reconstruction IR

Primary items: **5, 8–11, 29–30**.

Before emitting Java, Sleela, C, or C++, create a language-neutral reconstruction representation containing:

- declarations;
- expressions;
- statements;
- control structures;
- functions;
- data objects;
- types;
- calls;
- exceptions;
- unknown/opaque regions;
- provenance.

This prevents four source backends from developing four incompatible decompilation engines.

## Phase 4M — Source backend reconstruction

Primary items: **8–11, 30**.

The backends should translate the shared reconstruction IR:

- **C:** prioritize faithful procedural reconstruction and explicit low-level constructs.
- **C++:** add object model, methods, namespaces, templates/generic evidence, and ABI details where supported.
- **Sleela:** produce compiler-valid Sleela while retaining native-specific constructs explicitly.
- **Java:** produce JVM-compatible constructs only where the native evidence can support a meaningful mapping; preserve native operations that cannot be represented directly.

Each emitted construct should be traceable to evidence.

## Phase 4N — Reconstruction validation

Primary items: **28–30, 39–40**.

Validate reconstruction structurally rather than assuming source equality.

Required checks include:

- deterministic source;
- AST/IR consistency;
- balanced control structures;
- valid references;
- type consistency;
- provenance completeness;
- unknown-region accounting;
- no invented addresses;
- no invented symbols;
- no silent loss of unsupported instructions.

Where a generated backend can be compiled safely as a **generated artifact**, validation may compare compiler diagnostics or structural properties. This does not permit executing the analyzed target.

## Phase 4O — Difficult-artifact laboratory

Create a permanent test set for artifacts that commonly defeat simplistic decompilers:

- heavily optimized functions;
- recursive functions;
- indirect calls;
- jump-table dispatch;
- virtual dispatch;
- exception-heavy C++;
- templates;
- coroutines/state machines;
- mixed C/C++;
- static and dynamic libraries;
- stripped binaries;
- split/debug-linked artifacts;
- unusual ABIs;
- packed or partially obfuscated samples;
- malformed-but-parseable files;
- architecture-specific idioms.

Each case must document expected evidence and known reconstruction limitations.

## Phase 4P — Phase 4 reliability requirements

Every reconstruction pass must support:

- bounded resource use;
- deterministic ordering;
- explicit ambiguity;
- partial recovery;
- cancellation;
- provenance;
- stable machine-readable diagnostics;
- regression fixtures;
- fuzzable parser/IR boundaries;
- no target execution.

## Phase 4 Definition of Done

Phase 4 is complete only when reproducible corpus evidence demonstrates:

1. optimization-aware normalization on defined test classes;
2. SSA/value recovery over real CFGs;
3. conservative memory/alias analysis;
4. structured control-flow recovery;
5. evidence-based function signatures;
6. object-model recovery where evidence exists;
7. stripped-binary recovery modes;
8. explicit packed/obfuscated evidence handling;
9. semantic conflict/reconciliation support;
10. a shared language-neutral reconstruction IR;
11. Java/Sleela/C/C++ backends consuming that shared IR;
12. source-to-instruction provenance;
13. deterministic reconstruction;
14. measured unknown/unsupported/ambiguous coverage;
15. difficult-artifact regression fixtures;
16. no target execution.

Phase 4 must not be declared complete because generated source “looks right.” Completion requires measurable corpus behavior and explicit accounting for uncertainty.

## Phase 4 Work Sequence

1. Optimization normalization.
2. SSA and value-set infrastructure.
3. Memory/alias model.
4. Structured CFG recovery.
5. Signature/calling-convention reconciliation.
6. Object-model recovery.
7. Generated/template/runtime pattern handling.
8. Exception/cleanup reconstruction.
9. stripped-binary recovery modes.
10. packed/obfuscated evidence handling.
11. semantic conflict reconciliation.
12. language-neutral reconstruction IR.
13. Java/Sleela/C/C++ backend migration.
14. provenance and deterministic-source validation.
15. difficult-artifact corpus.
16. fuzzing, regression, coverage, and Phase 4 review.

## Phase 4 Non-Goals

Phase 4 does not claim:

- perfect original-source recovery;
- recovery of source comments or names without evidence;
- universal compiler/language identification;
- automatic deobfuscation;
- execution of target programs;
- correctness of every inferred type or function;
- exact recreation of proprietary build systems;
- equivalence proof for arbitrary binaries.

---

# Phase 5 — Production Validation, Compatibility, and Security

## Purpose

Phase 5 turns the analysis and reconstruction pipeline into a continuously validated production system.

Phase 5 primarily advances **Items 31–40**, while exercising every earlier item. It is not merely a test phase: it establishes the release discipline, compatibility matrix, reproducibility rules, performance/resource controls, API stability, and security evidence required for the project to make defensible production claims.

The governing production loop is:

**artifact → analysis → reconstruction → validation → measured result → regression corpus → release evidence.**

## Phase 5A — Corpus and compatibility matrix

Primary item: **31**.

Maintain a versioned corpus across:

- ELF/Linux;
- PE/COFF/Windows;
- Mach-O/macOS;
- x86/x86-64;
- ARM/ARM64;
- RISC-V when supported;
- debug and stripped artifacts;
- static and dynamic linking;
- optimized and unoptimized builds;
- C and C++;
- Rust, Go, Swift, and other supported language families;
- libraries, executables, object files, and archives.

Every corpus artifact should record:

- source language where known;
- compiler/toolchain where known;
- compiler version where known;
- target architecture;
- ABI;
- optimization mode;
- debug/symbol state;
- static/dynamic linkage;
- expected structural facts;
- expected unsupported/ambiguous regions;
- provenance of the fixture.

The corpus must test behavior, not merely file acceptance.

## Phase 5B — Golden analysis and regression evidence

Primary items: **28, 31, 39**.

Create stable golden expectations for:

- container identification;
- architecture;
- address mappings;
- decoded instruction families;
- basic-block counts/ranges where deterministic;
- function candidates;
- call/branch relationships;
- ABI facts;
- recovered types;
- evidence classes;
- unknown/unsupported/ambiguous regions;
- generated-source structure;
- machine-readable schema.

Golden tests should avoid requiring byte-for-byte identical generated source when multiple semantically equivalent reconstructions are valid.

Where exact output is required, canonicalize ordering and serialization first.

## Phase 5C — Sanitizers and undefined-behavior testing

Primary item: **32**.

Use platform-appropriate sanitizer configurations including:

- AddressSanitizer;
- UndefinedBehaviorSanitizer;
- LeakSanitizer where available;
- ThreadSanitizer where compatible;
- compiler warnings at strict production levels.

Sanitizer failures must identify:

- artifact/fixture;
- analysis stage;
- reproducible input;
- stack trace;
- relevant address/offset;
- whether the defect is parser, decoder, IR, reconstruction, CLI, cache, or concurrency related.

A sanitizer-discovered memory-safety defect must block a production-quality claim until resolved or explicitly isolated from the affected release surface.

## Phase 5D — Persistent fuzzing

Primary item: **33**.

Maintain fuzz targets for:

- ELF;
- PE/COFF;
- Mach-O;
- archives;
- address translation;
- instruction decoding;
- debug metadata;
- unwind metadata;
- SLIR construction;
- reconstruction IR;
- source serialization;
- JSON/SARIF/YAML serialization;
- CLI/configuration parsing.

Fuzzing must monitor:

- crashes;
- hangs;
- timeouts;
- excessive memory;
- integer overflow;
- out-of-bounds access;
- recursion exhaustion;
- pathological CFG growth;
- pathological allocation;
- invalid internal-state transitions.

Every fixed security/reliability bug should become a permanent regression fixture when practical.

## Phase 5E — CLI and library compatibility

Primary item: **34**, supported by **37**.

Define compatibility guarantees for:

- command names;
- help text structure;
- version reporting;
- exit codes;
- output-language selection;
- input/output behavior;
- diagnostics;
- configuration;
- library entry points;
- machine-readable schemas.

API and CLI changes must be classified as:

- compatible;
- additive;
- deprecated;
- breaking.

Breaking changes require documentation and versioning rather than silent semantic changes.

## Phase 5F — Architecture and platform matrix

Primary item: **35**.

The release matrix must explicitly identify:

- host platform;
- target platform;
- target architecture;
- binary format;
- ABI;
- supported decoder level;
- supported semantic level;
- source-backend availability.

A successful host build must not imply that every target architecture is supported.

Multi-architecture artifacts must expose their available slices and require deterministic selection rules.

## Phase 5G — Resource, cancellation, and denial-of-service controls

Primary items: **26–27, 33**.

Production analysis must enforce configurable limits for:

- input size;
- decompression/expansion;
- memory;
- instruction count;
- CFG nodes/edges;
- functions;
- analysis depth;
- recursion;
- output size;
- cache consumption;
- wall-clock time;
- parallel workers.

Cancellation must propagate through parser, decoder, CFG, SLIR, data-flow, reconstruction, and serialization layers.

A resource-limited result must be distinguishable from a successful complete analysis.

## Phase 5H — Determinism and reproducibility

Primary item: **28**.

For deterministic mode:

- ordering must be stable;
- hash inputs must be explicit;
- parallel scheduling must not change semantic results;
- serialization must be canonical;
- diagnostics must have stable identifiers;
- cache keys must be reproducible.

Record enough environment/tool metadata to distinguish an analyzer change from an input change.

## Phase 5I — Thread safety and parallel analysis

Primary item: **37**.

Permit independent artifacts to be analyzed concurrently.

Shared services must have explicit ownership:

- immutable configuration may be shared;
- caches require synchronization;
- diagnostics require per-analysis ownership or safe aggregation;
- temporary state must never leak between artifacts;
- architecture/ABI state must remain analysis-local.

Concurrency tests must intentionally vary scheduling and compare deterministic outputs.

## Phase 5J — Content-addressed caching

Primary item: **38**.

Cache stages independently where useful:

1. artifact identity/container parse;
2. address map;
3. instruction decoding;
4. CFG;
5. function recovery;
6. SLIR;
7. data flow;
8. type evidence;
9. reconstruction IR;
10. generated source.

Cache identity must include all inputs that can alter semantics, including artifact bytes, selected architecture/slice, configuration, analysis schema, tool version, and relevant feature flags.

Invalid or incompatible cache entries must be safely discarded rather than reused.

## Phase 5K — Machine-readable release contract

Primary item: **29**.

Define a versioned machine-readable schema for:

- artifact identity;
- container/slice/member;
- architecture;
- address maps;
- sections/segments;
- instructions;
- blocks;
- functions;
- symbols;
- ABI facts;
- types;
- globals;
- SLIR;
- reconstruction IR;
- provenance;
- evidence class;
- confidence;
- conflicts;
- unknown/unsupported regions;
- diagnostics;
- coverage;
- resource limits;
- analyzer version.

Schema evolution must be backward-aware and documented.

## Phase 5L — Measured coverage and release scorecard

Primary item: **39**.

Coverage reporting must separate at least:

- bytes inspected;
- bytes mapped;
- instructions decoded;
- instructions semantically modeled;
- basic blocks recovered;
- CFG edges recovered;
- functions identified;
- functions semantically lifted;
- symbols resolved;
- types recovered;
- source constructs reconstructed;
- provenance links established;
- unknown regions;
- unsupported regions;
- ambiguous regions.

Do not collapse these into one percentage.

A release scorecard should report measurements by artifact class, architecture, format, optimization level, and debug/symbol state.

## Phase 5M — Security boundary and hostile-input review

Primary item: **40**, supported by **24–26 and 33**.

The analyzer must remain non-executing by default.

Security review must verify that Slecompiler does not:

- execute target instructions;
- invoke target constructors/destructors;
- execute embedded scripts;
- load target kernel modules;
- invoke target interpreters;
- automatically execute generated source;
- follow attacker-controlled external commands merely because metadata references them;
- treat binary metadata as trusted memory boundaries.

Future emulation, if ever introduced, must be isolated behind an explicit subsystem boundary with its own resource limits, policy, audit trail, and test suite.

## Phase 5N — Supply-chain and build integrity

Production validation must also cover the analyzer itself.

Record:

- compiler/toolchain identity;
- source revision;
- dependency versions;
- build configuration;
- generated-code provenance;
- release artifact hashes;
- test corpus revision;
- sanitizer/fuzz configuration.

Release artifacts should be reproducible where practical and should never be represented as verified merely because they were successfully packaged.

## Phase 5O — Performance and scalability

Measure:

- startup time;
- parser throughput;
- decode throughput;
- CFG construction;
- function recovery;
- SLIR construction;
- data-flow analysis;
- type recovery;
- reconstruction;
- serialization;
- cache effectiveness;
- peak memory;
- parallel scaling.

Performance claims must identify corpus, hardware/environment, configuration, and measurement method.

Correctness and safety take precedence over benchmark optimization.

## Phase 5P — Release gates

A production release should have explicit gates for:

1. compilation;
2. unit tests;
3. integration tests;
4. regression corpus;
5. sanitizer runs;
6. fuzz health;
7. deterministic-output tests;
8. concurrency tests;
9. resource-limit tests;
10. machine-readable schema validation;
11. coverage measurement;
12. security review;
13. documentation synchronization.

A release gate may be **not applicable** for a platform or feature, but it must not be silently omitted.

## Phase 5Q — Defect classification and remediation

Production defects should be classified as:

- correctness;
- memory safety;
- parser robustness;
- decoder correctness;
- CFG/function recovery;
- semantic/SLIR;
- type recovery;
- reconstruction;
- output/schema;
- determinism;
- concurrency;
- resource exhaustion;
- security boundary;
- documentation/contract.

Each defect should preserve a minimal reproducer where possible and become a regression test before closure.

## Phase 5R — Compatibility and deprecation policy

Document the support level for each:

- binary format;
- architecture;
- ABI;
- compiler/runtime family;
- debug format;
- source backend;
- machine-readable schema.

Unsupported combinations must fail clearly and must not silently downgrade into an apparently successful but misleading analysis.

## Phase 5S — Phase 5 Definition of Done

Phase 5 is complete only when reproducible evidence demonstrates:

1. a versioned cross-platform regression corpus;
2. golden structural regression coverage;
3. sanitizer-tested builds;
4. persistent fuzz coverage for critical parser/decoder/IR paths;
5. stable CLI/API and output-schema contracts;
6. explicit architecture/platform compatibility reporting;
7. enforced resource and cancellation controls;
8. deterministic results;
9. thread-safe parallel analysis;
10. validated content-addressed caching;
11. machine-readable release schema;
12. multidimensional measured coverage;
13. auditable non-executing security boundary;
14. analyzer build/supply-chain provenance;
15. performance measurements with stated methodology;
16. explicit release gates;
17. reproducible defect remediation;
18. documented compatibility/deprecation policy.

Phase 5 is not complete because tests are numerous. It is complete when the project can show reproducible evidence that the system behaves safely and consistently across the declared support matrix.

## Phase 5 Work Sequence

1. Version the corpus and compatibility matrix.
2. Establish golden structural expectations.
3. Enable sanitizer configurations.
4. Establish persistent fuzz targets.
5. Stabilize CLI/library contracts.
6. Formalize architecture/platform reporting.
7. Enforce resource and cancellation controls.
8. Establish deterministic-mode verification.
9. Validate concurrent analysis.
10. Implement content-addressed caching.
11. Version machine-readable schemas.
12. Build multidimensional coverage reports.
13. Perform hostile-input/security review.
14. Record analyzer build/supply-chain provenance.
15. Measure performance and scalability.
16. Implement release gates.
17. Formalize defect remediation.
18. Document compatibility/deprecation.
19. Perform Phase 5 review.

## Phase 5 Non-Goals

Phase 5 does not claim:

- universal architecture support;
- perfect decompilation;
- exact original-source recovery;
- safe execution of analyzed programs;
- automatic correctness of inferred semantics;
- that passing a test suite proves arbitrary binaries are handled correctly;
- that an aggregate percentage represents overall decompiler quality.

---

# Cross-Phase Architecture

The intended long-term architecture is:

**Artifact**
→ **Container/Slice/Member**
→ **Address Map**
→ **Instruction Decoder**
→ **CFG**
→ **Function Recovery**
→ **ABI/Runtime/Debug Evidence**
→ **SLIR**
→ **SSA/Data Flow**
→ **Memory/Type Model**
→ **Language-Neutral Reconstruction IR**
→ **Java / Sleela / C / C++**
→ **Provenance / Coverage / Validation**

No later phase should bypass the shared semantic model merely to obtain faster support for one output language.

## Evidence Classes

Every important recovered fact should be classifiable as one or more of:

- **Observed** — directly represented in artifact data.
- **Decoded** — produced by a validated decoder.
- **Derived** — mechanically derived from observed/decoded facts.
- **Inferred** — a reasoned interpretation supported by multiple facts.
- **Hypothesized** — a plausible alternative not sufficiently established.
- **Unknown** — insufficient evidence.
- **Unsupported** — the analyzer does not currently implement the needed capability.
- **Conflicting** — available evidence disagrees.

This vocabulary should appear in APIs, diagnostics, machine-readable output, and documentation.

## Completion Rule

The project should prefer:

**measured partial correctness + explicit uncertainty**

over:

**unmeasured completeness claims + fabricated certainty.**

The purpose of the 1–40 register and Phases 2–5 is to make that distinction operational in code, tests, documentation, and generated source.

## Current Starting Point

The existing repository has the Phase 1 foundation, including structured `DecompilerError` handling, stricter CLI parsing, help/version behavior, output-file validation, four source-output paths, and a non-executing analysis boundary.

The next implementation work should proceed from the actual codebase rather than treating this document as proof of implementation. In particular, the existing source emitters remain limited until SLIR, reconstruction IR, and semantic analysis are implemented.

**Known limitation:** local compilation/CTest execution is environment-dependent. Repository changes must not be described as build-verified unless CI or a real build result is available.

## Iteration Policy

Every implementation iteration should:

1. inspect the current code and tests;
2. implement a bounded, reviewable subset;
3. add or update regression tests;
4. measure the affected corpus;
5. update this document to match actual implementation status;
6. preserve evidence and uncertainty;
7. preserve the non-executing security boundary;
8. keep `main` and `master` synchronized for the changes made.

This document is a roadmap and engineering contract, not a substitute for tests.


## API Class Model Implementation

The repository now contains the first shared C/C++ API class-definition layer under `/api`:

- public C++20 header: `api/include/sleela/api.hpp`;
- integration translation unit: `api/src/api.cpp`;
- documentation: `api/README.md` and `decompiler/API_CLASS_MODEL.md`;
- CMake integration into the Slecompiler static library.

The model covers native-analysis context, reusable types and fields, function signatures and calling conventions, C structs/unions/enums, C++ classes/namespaces/methods, vtable/RTTI evidence, provenance, and a language-neutral `ReconstructionUnit`.

This directly advances items 7, 8, 10, and 11 of the 1–40 register: type recovery, source reconstruction, Sleela output, and C/C++ backends. It is an implementation foundation, not evidence of universal recovery. The analysis pipeline must populate these definitions from actual SLIR/data-flow/ABI/debug/runtime evidence and retain Unknown, Unsupported, Hypothesized, and Conflicting states.

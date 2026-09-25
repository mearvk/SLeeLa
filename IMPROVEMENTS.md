# Slecompiler Improvements

This document is the active engineering backlog and implementation order for Slecompiler™ in `/decompiler`.

The list below records the forty improvement areas identified during the current review. Items are ordered intentionally: parser and failure safety come before deeper semantic reconstruction, and semantic reconstruction comes before source-quality output.

## Implementation status

- **Pass 1 — Foundation:** Items 1–5 are the first implementation iteration.
- **Pass 2 — Native analysis:** Items 6–12.
- **Pass 3 — Binary formats and ABI evidence:** Items 13–20.
- **Pass 4 — Recovery and reconstruction:** Items 21–29.
- **Pass 5 — Reliability, tooling, and security:** Items 30–40.
- Status should describe measured implementation state, not aspirational percentage.

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

## Iteration policy

Each iteration should:
1. inspect the current implementation;
2. implement the next safe subset in order;
3. add regression tests;
4. update this document with what is actually implemented;
5. avoid claiming support that is not measured;
6. keep `main` and `master` synchronized.

## Current iteration

### Items 1–5: foundation started

The first implementation pass hardens the error boundary and CLI before deeper decoder/CFG/SLIR work. The next pass should introduce the explicit address-space model and checked address translation before expanding instruction coverage.

**Known limitation:** local compilation/CTest execution is environment-dependent. Repository changes should not be described as build-verified unless CI or a real build result is available.

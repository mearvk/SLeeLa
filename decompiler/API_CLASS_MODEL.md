# SLeeLa API Class Model

**Max Rupplin - MEARVK LLC - 2026**

The /api layer is now the shared C/C++ semantic class-model foundation intended to bridge native analysis, SLIR, reconstruction, and source emission.

## Core model

- Type, Field, Variable, FunctionSignature
- CallingConvention and Method
- CStruct, CUnion, CEnum
- CppClass and CppNamespace
- Artifact, Address, Instruction, BasicBlock, ControlFlowGraph
- ReconstructionUnit
- Provenance and EvidenceKind

## C++ recovery

CppClass can represent base classes, fields and field offsets, methods, virtual methods, vtable-address evidence, RTTI evidence, and per-object provenance. These objects represent recovered evidence or hypotheses; they do not silently assert original-source equivalence.

## C recovery

CStruct, CUnion, and CEnum provide reusable semantic declarations for C reconstruction. Shared Type, Field, signature, and provenance objects avoid maintaining incompatible C and C++ type systems.

## Pipeline rule

SLIR remains the lower-level semantic representation. The API class model is a higher-level reconstruction layer that consumes SLIR, data-flow, ABI, debug information, RTTI, vtable evidence, and related analysis results.

native artifact -> SLIR -> reconstruction model -> C/C++/Sleela/Java

The API remains read/analyze/reconstruct oriented and does not execute target artifacts.

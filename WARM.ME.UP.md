# WARM.ME.UP.md

**Max Rupplin - MEARVK LLC - 2026**

# SLeeLa API Class Model — Warm-Up and Implementation Brief

## Purpose

SLeeLa now has a shared C/C++ API class-definition foundation intended to connect native binary analysis, SLIR, reconstruction, and source-language backends.

This document is a concise implementation warm-up: it explains what was added, why it exists, how the pieces relate, and what remains before the API can be considered a complete world-class semantic reconstruction layer.

## Architecture

```
Native Artifact
      ↓
Instruction / CFG / Analysis
      ↓
SLIR
      ↓
Type / Data Flow / ABI / Debug / RTTI Evidence
      ↓
SLeeLa API Class Model
      ↓
ReconstructionUnit
      ↓
C / C++ / Sleela / Java
```

The API class model is a higher-level reconstruction layer. SLIR remains the lower-level semantic representation.

## Implemented API Foundation

The public C++20 header is:

`api/include/sleela/api.hpp`

The integration translation unit is:

`api/src/api.cpp`

The model currently provides:

### Native-analysis context

- `Artifact`
- `Address`
- `Instruction`
- `BasicBlock`
- `ControlFlowGraph`

### Type and variable model

- `Type`
- `Field`
- `Variable`
- `TypeKind`

### Function and ABI model

- `FunctionSignature`
- `CallingConvention`
- `Method`

### C model

- `CStruct`
- `CUnion`
- `CEnum`
- `CApi`

### C++ model

- `CppClass`
- `CppNamespace`
- `Method`
- inheritance relationships
- fields and field offsets
- virtual-method state
- vtable-address evidence
- RTTI type-name evidence
- provenance

### Reconstruction model

- `ReconstructionUnit`

The reconstruction unit is intended to provide a common semantic collection for source backends rather than allowing each backend to invent its own incompatible type model.

## Evidence Model

The API uses an explicit evidence vocabulary:

- **Observed** — directly represented by artifact data.
- **Decoded** — produced by a validated decoder.
- **Derived** — mechanically derived from existing evidence.
- **Inferred** — reasoned from multiple supporting facts.
- **Hypothesized** — plausible but insufficiently established.
- **Unknown** — insufficient evidence.
- **Unsupported** — the analyzer does not currently implement the required capability.
- **Conflicting** — available evidence disagrees.

Every important recovered class, field, method, type, signature, vtable, RTTI relationship, and address should ultimately be traceable to this evidence model.

## C++ Class Recovery

`CppClass` is designed to represent evidence-backed native C++ object structures.

It can contain:

- class name;
- base classes;
- fields;
- field offsets;
- methods;
- virtual methods;
- vtable address;
- RTTI type-name evidence;
- provenance.

The presence of a `CppClass` object does **not** mean that the original source is proven to have contained that exact class. The object represents recovered information, an inference, or a hypothesis depending on its evidence.

## C Recovery

C reconstruction uses the same semantic foundation through:

- `CStruct`;
- `CUnion`;
- `CEnum`;
- `Type`;
- `Field`;
- `FunctionSignature`;
- `Provenance`.

C and C++ therefore share the underlying semantic model wherever their native representations overlap.

## Intended Data Flow

A future complete implementation should populate the model from actual analysis evidence:

1. identify the artifact;
2. establish its address-space and format information;
3. decode native instructions;
4. recover control flow;
5. recover functions and calling conventions;
6. lift semantics into SLIR;
7. perform data-flow and memory analysis;
8. recover type evidence;
9. inspect ABI/debug/runtime/RTTI/vtable evidence;
10. construct the API class model;
11. construct the language-neutral reconstruction representation;
12. emit C, C++, Sleela, or Java;
13. retain provenance and coverage information.

## Security Boundary

The API is analysis and reconstruction oriented.

It must not execute:

- analyzed instructions;
- recovered constructors or destructors;
- target scripts;
- target kernel modules;
- recovered native code;
- generated source;
- untrusted build commands.

Generated source may eventually be compiled as a separate validation activity, but compilation or validation must remain separate from execution of the analyzed target.

## Relationship to Slecompiler™

Slecompiler™ remains the native static-analysis/decompilation component.

The API class model is not a replacement for the decoder, CFG, SLIR, data-flow engine, ABI analysis, or reconstruction engine.

Instead:

```
Slecompiler™
   ↓
Native Evidence
   ↓
SLIR
   ↓
SLeeLa API Class Model
   ↓
Language-Neutral Reconstruction
   ↓
Source Backends
```

This keeps the semantic work reusable across C, C++, Sleela, and Java output.

## Current Documentation

Related documentation includes:

- `api/README.md`
- `decompiler/API.md`
- `decompiler/API_CLASS_MODEL.md`
- `decompiler/TUTORIAL.md`
- `decompiler/TERMINOLOGY.md`
- `IMPROVEMENTS.md`

## What This Addition Accomplishes

This implementation establishes the first shared C/C++ class-definition API foundation rather than leaving C and C++ reconstruction as isolated source-emitter behavior.

It directly supports the larger objectives for:

- type recovery;
- source reconstruction;
- C output;
- C++ output;
- Sleela output;
- provenance;
- evidence classification;
- shared semantic reconstruction.

## What Still Needs to Be Built

The API definitions are a foundation, not the completion of binary reconstruction.

The next major implementation layers include:

1. connect recovered SLIR values to `Type`;
2. connect data-flow results to `Variable` and `Field`;
3. implement real ABI/calling-convention inference;
4. recover C/C++ structures from actual binary evidence;
5. recover vtables and RTTI;
6. recover inheritance relationships;
7. recover constructors/destructors;
8. recover virtual dispatch;
9. connect function recovery to `Method`;
10. build the language-neutral reconstruction IR;
11. make C/C++ emitters consume that reconstruction IR;
12. add regression fixtures for known C/C++ binaries;
13. add provenance and coverage to generated source;
14. test stripped and optimized binaries;
15. preserve explicit unknown and conflicting states.

## Engineering Rule

The project should prefer:

**measured partial correctness + explicit uncertainty**

over:

**unmeasured completeness claims + fabricated certainty.**

The API should therefore grow with the analysis evidence that can actually support it.

## Build Integration

The API source is integrated into the Slecompiler CMake target through the decompiler build system.

The implementation has been synchronized to both:

- `main`
- `master`

Build verification should only be reported after an actual CMake/CTest or CI result is available.

## Status

**API class-definition foundation: implemented.**

**Complete semantic C/C++ binary reconstruction: not yet complete.**

The next objective is to make the class model a real consumer of SLIR, data-flow, ABI, debug, runtime, RTTI, and vtable evidence rather than merely a collection of declarations.

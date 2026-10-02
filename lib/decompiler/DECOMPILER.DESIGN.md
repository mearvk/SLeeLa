# SLeeLa Decompiler Design

Max Rupplin - MEARVK LLC - 2026

## Design authority

The decompiler is SLeeLa-sourced and SLeeLa-driven. The .sleela classes define the request, analysis, module, evidence, fractional-input, OS-discernment, reconstruction, and output model. Native C/C++ implements services behind that contract.

The decompiler must not become a second language implementation or a generic opaque binary-to-text converter.

## Request model

Every request has explicit source language, source version, expected input, desired output, fractional-input policy, OS/ABI discernment policy, analysis/reconstruction profile, and optional language/format modules.

C, C++, and SLeeLa are first-class built-in families. Modules extend the same interface for additional languages.

## Pipeline

Artifact -> acquisition -> container/format detection -> architecture detection -> OS/ABI evidence -> decoding -> control-flow graph -> SLeeLa IR -> type/symbol/data-flow analysis -> reconstruction -> evidence-preserving output -> VM readiness validation.

## Fractional input

Partial, truncated, or corrupt input is a first-class state.

PRESERVE keeps unreadable or undecoded ranges as evidence. PARTIAL reconstructs valid regions and inserts explicit unresolved regions. REPORT_ONLY stops before reconstruction while retaining analysis evidence. STRICT rejects incomplete or contradictory input.

No fractional bytes are silently treated as source code.

## OS and ABI discernment

OS-specific reasoning is weighted because a binary can contain ambiguous, packed, cross-linked, or loader-generated evidence.

Evidence classes include magic/container headers, machine and ABI identifiers, imports and exports, relocation style, executable sections, calling conventions, runtime/compiler metadata, loader/linker records, and system-library references.

A candidate OS is not selected solely because one header resembles it. Conflicts are retained and exposed. Modules may contribute additional evidence.

## Reconstruction

Reconstruction distinguishes observed facts, decoded instructions/data, inferred types/control flow, reconstructed source, and unresolved regions. Inference is never presented as direct observation.

## VM-ready output

SLeeLa output is lowered through the same compiler/VM boundary used elsewhere in the repository. VM readiness means the generated model is structurally complete enough for VM validation and obeys capability/security constraints; it does not mean semantic equivalence has been proven.

## Profiles

BASIC_COMPLETE provides the complete practical path for a selected language/input/output combination.

ADVANCED_TOTAL adds deep architecture/ABI analysis, richer data-flow and type recovery, module resolution, conflict analysis, provenance, cross-reference reporting, and stricter VM/capability validation.

## Native boundary

C is the stable ABI. C++ provides orchestration and higher-level analysis. Neither may bypass SLeeLa-defined capability, security, resolver, memory, certificate, or VM boundaries.

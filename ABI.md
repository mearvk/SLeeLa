# SLeeLa ABI

## Purpose

The ABI defines the stable boundary between SLeeLa-generated code, the execution core, native libraries and platform adapters.

## Core ABI

The central exchange entry point is:

`slcore_exchange(SLVM* vm, SLExchangeOp op, SLExchangeArg* arg)`

The operation set covers VM reset, constants, globals, functions, emission, patching, entry selection, execution and result retrieval.

## ABI layers

1. Language ABI — source-level calling and type rules.
2. Core ABI — VM exchange and value representation.
3. Native ABI — C-compatible interoperability.
4. Platform ABI — OS/compiler-specific conventions.
5. Protocol ABI — stable wire/frame layouts.

## Stability

Each layer is versioned independently. Surface syntax changes do not automatically authorize core ABI changes.

## Data layout

Structured values crossing a native boundary require explicit size, alignment, ownership and lifetime rules. Network structures require explicit byte order and serialization contracts.

## Ownership

Every native resource must define creator, owner, release operation and failure behavior.

## Binary formats

SLeeLa tooling may inspect and integrate PE/COFF, Mach-O, ELF-related artifacts, static archives, dynamic libraries and Linux kernel-module artifacts. Inspection is distinct from execution.

## Compatibility

ABI tests must run on every supported platform and architecture. Breaking ABI changes require an explicit version transition.

**Max Rupplin — MEARVK LLC — 2026**


## Runtime Artifact ABI

The persistent `.sleela` artifact format is versioned independently from the source-language syntax. Runtime ABI version `1.0` is exposed by `SLEELA_VM_ABI_MAJOR` / `SLEELA_VM_ABI_MINOR`, while artifact format version `2` is exposed by `SLEELA_ARTIFACT_FORMAT_VERSION`. Before execution, the runtime validates opcode values, code references, function metadata, globals/constants, struct metadata, synchronization operands, and the entry point. `sleela validate-artifact <file.sleela>` performs the same non-executing validation gate.

**Max Rupplin — MEARVK LLC — 2026**


## Synchro protocol ABI

The repository-native Synchro integration ABI is a C11 boundary. Its packet contract is 16 bytes: ASCII `SYNC` magic at offset 0, a network-order uint32 sequence at offset 4, and a network-order uint64 monotonic send timestamp at offset 8. Invalid magic or packets shorter than 16 bytes are rejected.

`synchro_integration_prepare()` allocates the next sequence and encodes a packet. `synchro_integration_ack()` validates the sequence and records measured RTT; when a local send timestamp is not supplied, it uses the validated packet timestamp. `synchro_integration_timeout()` records a loss. The C++17 wrapper owns the same C ABI and does not alter its wire representation.

This integration ABI is distinct from the VM-facing `slsynchro_*` API in `impl/core/sleela_synchro.*`. The latter remains the native runtime probe implementation; the former provides a reusable packet/statistics boundary for language adapters.
   
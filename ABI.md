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

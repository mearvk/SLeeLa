# SLeeLa Decompiler Definitions

**Max Rupplin - MEARVK LLC - 2026**

## Artifact
The immutable input file under investigation. The engine records its SHA-256 digest before analysis.

## Container
The file-format layer, such as PE/COFF, ELF, Mach-O, or a raw binary image.

## Architecture
The target instruction set and execution mode, such as x86-64, x86, ARM64, or ARM32.

## Section / Segment
A mapped or stored region described by the input container. A section is a file-format concept; a segment is a load/memory-mapping concept.

## Instruction
A decoded machine instruction with address, length, bytes, mnemonic, operands, flags and references.

## Basic Block
A single-entry sequence of instructions with no internal control-flow entry and a terminating control-flow instruction or fall-through boundary.

## Function Candidate
A region inferred to represent a callable procedure. Candidate status is explicit because stripped binaries may not preserve authoritative function boundaries.

## Control-Flow Graph
A directed graph of basic blocks connected by observed or inferred execution edges.

## SLIR
SLeeLa Intermediate Representation. A stable, architecture-neutral representation used between decoding and higher-level reconstruction.

## Provenance
The chain connecting every recovered object to an input offset, virtual address, instruction, symbol, relocation or inference rule.

## Confidence
A bounded value describing evidentiary support. It is metadata, not a claim that recovered source is identical to original source.

## Driver
A native module intended to participate in operating-system or device infrastructure. SLeeLa analyzes driver files as data and does not load them.

## VM
The SLeeLa virtual machine executes **SLIR test programs**, not untrusted native binaries. Its purpose is deterministic validation of lifting and transformation passes.

# SLeeLa Decompiler

Max Rupplin - MEARVK LLC - 2026

The /lib/decompiler package is the SLeeLa-sourced and SLeeLa-driven decompiler design and implementation layer. It converts executable artifacts, object files, byte streams, and other supported binary inputs into evidence-preserving SLeeLa-oriented intermediate representations and VM-ready reconstruction artifacts.

## Authority

The authoritative model is:

input artifact -> acquisition -> format/OS evidence -> decoding -> CFG -> SLIR -> semantic analysis -> reconstruction -> SLeeLa/target output -> VM validation

The decompiler does not pretend that every binary can be perfectly reconstructed. Evidence, inference, reconstruction, and unresolved/fractional input are represented separately.

## Selection model

A decompilation request can declare source language family (C, C++, SLeeLa, or a loaded language module), source language version, expected input format and architecture, desired output, treatment of fractional/partial input, operating-system/ABI discernment policy, analysis depth, and additional language/format modules.

## Fractional input

Fractional input is never silently discarded. The request chooses PRESERVE, PARTIAL, REPORT_ONLY, or STRICT.

## OS discernment

OS identification is evidence-weighted, not assumed from a single signature. Format headers, ABI markers, imports/exports, relocation data, calling conventions, section names, runtime metadata, and executable evidence may contribute. Conflicting evidence remains visible in the report.

## VM readiness

The resulting SLeeLa model is validated by the same VM completeness/capability boundaries used by the compiler. Native C/C++ code provides implementation services; .sleela files define the source-level decompiler contract.

## Build

make -C lib/decompiler

or make decompiler

**SLeeLa — MEARVK LLC — 2026**

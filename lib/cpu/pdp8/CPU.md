# SLeeLa DEC PDP-8 CPU

**Path:** `/lib/cpu/pdp8/`  
**Scope:** Profile-driven model of the 12-bit DEC PDP-8 family.

## Overview

The PDP-8 is a 12-bit minicomputer architecture with a compact instruction format and memory-reference, operate, and I/O-transfer instruction groups. Implementations and options differ across the PDP-8 family, so the selected model profile controls memory size and optional facilities.

## Architectural core

- 12-bit accumulator (AC), link bit, and program counter.
- 12-bit words and 12-bit base addresses in the base profile.
- Page-relative and zero-page memory-reference addressing.
- Indirect addressing and auto-index locations where defined.
- Memory-reference, operate, and I/O-transfer instruction groups.
- Optional extended arithmetic and memory-extension facilities only when configured.

## Family boundaries

Do not assume all PDP-8 variants have the same memory capacity, arithmetic options, I/O devices, or memory-extension behavior. Model documented variants explicitly.

## Evidence policy

Label implementation-specific timing and options as `documented`, `derived`, `estimated`, or `unspecified`. This is an architectural foundation, not a complete cycle-accurate emulator.

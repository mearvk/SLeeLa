# SLeeLa IBM z/Architecture CPU Model

**Path:** `/lib/cpu/z-architecture/`  
**Scope:** Profile-aware IBM z/Architecture mainframe CPU foundation.

## Overview

IBM z/Architecture is a 64-bit mainframe architecture with a long evolutionary history. This SLeeLa model must select an architecture level and enabled facilities before execution. It must not treat every instruction or facility from every generation as universally available.

## Responsibilities

- 64-bit general-purpose state and architecture-defined condition code.
- Instruction decoding gated by architecture level and installed facilities.
- Program interruption and privileged-state modeling.
- Address translation and storage protection through a configured system model.
- Optional floating-point, vector, decimal, cryptographic and other facilities where enabled.
- Interruption, timing and I/O interfaces configured for the target machine.

## Model boundary

A CPU instruction-set model is not a complete mainframe system. Channel subsystem, I/O devices, storage hierarchy, firmware, operating system, and multiprocessor coordination are separate components or explicitly configured interfaces.

## Evidence policy

Use `documented`, `derived`, `estimated`, and `unspecified` to distinguish evidence quality. Do not invent facility bits, instruction availability, reset values, cycle counts, or system behavior.

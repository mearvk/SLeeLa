# SLeeLa IBM ESA/390 (System/390) CPU

**Path:** `/lib/cpu/system-390/`  
**Scope:** Profile-driven architectural model for IBM ESA/390-era systems.

## Overview

ESA/390 is an architecture level associated with IBM System/390 systems. This implementation models that level separately from System/370 and z/Architecture, with explicit controls for facilities and machine configuration.

## Responsibilities

- 32-bit and 64-bit addressing modes where defined by the selected ESA/390 profile.
- Architecture-appropriate general-purpose, control, access, and status state.
- Program status word and interruption processing.
- Dynamic address translation and storage protection where enabled.
- Facility-gated instruction decode and execution.
- Explicit system interfaces for storage, channels, timers, and devices.

## Compatibility boundaries

Do not enable z/Architecture 64-bit mode or later instructions merely because the host uses 64-bit values. Distinguish ESA/390 addressing modes, facility levels, and model-dependent behavior.

## Evidence policy

Use `documented`, `derived`, `estimated`, and `unspecified` for architecture and performance claims. Do not claim full instruction coverage or cycle accuracy until tested against an authoritative ESA/390 specification and test suite.

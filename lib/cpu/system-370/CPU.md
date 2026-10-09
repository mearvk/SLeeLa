# SLeeLa IBM System/370 CPU Family

**Path:** `/lib/cpu/system-370/`  
**Scope:** Profile-driven architectural model for IBM System/370 systems.

## Overview

System/370 evolved the System/360 lineage with architecture and system features that vary by model and generation. This implementation is separate from both System/360 and later ESA/390 or z/Architecture profiles.

## Responsibilities

- Sixteen 32-bit general-purpose registers and condition-code state.
- Model-appropriate program status word (PSW) and interruption handling.
- Instruction decoding gated by the configured architecture level and installed facilities.
- Address translation and virtual-storage behavior only when supported by the selected profile.
- Model-specific storage protection, timers, channel I/O, and optional facilities.
- Structured diagnostics for unavailable instructions and invalid machine configurations.

## Compatibility boundaries

Do not assume every System/370 model has the same DAT, extended-control, floating-point, or other optional facilities. Distinguish base architecture from model options and later architecture extensions.

## Evidence policy

Label claims as `documented`, `derived`, `estimated`, or `unspecified`. Timing and implementation-specific cache behavior must be tied to a named model.

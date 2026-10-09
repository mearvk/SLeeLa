# SLeeLa IBM System/360 CPU Family

**Path:** `/lib/cpu/system-360/`  
**Scope:** Profile-driven model of the original IBM System/360 architecture family.

## Overview

IBM System/360 established a compatible family of general-purpose computers with different implementations and performance levels. This SLeeLa model describes architectural behavior by selected System/360 model and feature profile; it does not equate the family with later System/370 or z/Architecture facilities.

## Core responsibilities

- Architecture-defined general-purpose registers and condition code.
- Program status word (PSW), interruption state, and privileged/problem-state distinction.
- Instruction decode and execution gated by the selected model and documented facilities.
- Storage addressing, protection, and access faults through a configured machine model.
- Floating-point, decimal, channel, and other features only where supported by the selected profile.
- Explicit system interfaces for I/O channels, storage, timers, and interruptions.

## Compatibility boundaries

System/360, System/370, ESA/390, and z/Architecture must remain distinct profiles. Later facilities must not be silently introduced into a System/360 configuration. Model-specific differences and optional features are documented rather than flattened into a single universal machine.

## Evidence policy

Use `documented`, `derived`, `estimated`, and `unspecified` to label evidence. Avoid invented cycle counts, model-specific instruction support, reset values, and hardware configuration.

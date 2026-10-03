<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Virtual Machine 10

SLVM/10 is the next-generation storage and execution-assurance layer after SLVM/9.

## What makes /10 noteworthy

SLVM/9 established native filesystem observation. SLVM/10 turns that observation into a **verified storage lifecycle**: identity is first-class, operations carry required capabilities and verification state, filesystem generations are revalidated, and quiesce/recovery becomes an explicit execution phase.

The VM therefore does not merely ask, "Can the operating system open this file?" It asks whether the storage identity, generation, adapter, policy, and requested operation still agree with the admitted SLeeLa artifact.

## Architecture

`SLeeLa Artifact -> SLVM/8 Admission -> SLVM/9 Native/FAL Observation -> SLVM/10 Verification -> Operation -> Native OS`

SLVM/10 does not replace SLVM/8 or /9. It composes them into a stronger verification boundary.

## Core additions

- Verified storage identity and generation.
- Explicit operation verification and capability requirements.
- Revalidation operations for long-lived handles.
- Quiesce and recovery lifecycle.
- Adapter qualification as an explicit contract.
- Fail-closed stale-generation behavior.
- Portable Linux, Windows, and macOS foundation.
- A clean profile boundary for advanced filesystems such as TAC3.

## Design principle

**Observation is not authority, and authority is not permanence.** A native filesystem may be visible to the VM, but SLeeLa still requires admission and policy. A successful operation is not automatically durable. A previously valid identity must be revalidated after a generation change, recovery, migration, or mount replacement.

Copyright (c) Max Rupplin - MEARVK LLC - 2026
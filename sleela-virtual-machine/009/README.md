<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Virtual Machine 9

SLVM/9 extends SLVM/8 with a portable, capability-controlled filesystem substrate and explicit operating-system adapters.

## Primary additions

- Filesystem Abstraction Layer (FAL).
- Filesystem capability negotiation and feature discovery.
- OS adapter contracts for Linux, Windows, and macOS.
- Explicit support for novel/custom filesystems such as TAC3 without embedding TAC3 assumptions into the VM core.
- Transactional filesystem operations.
- Mount/namespace identity and generation tracking.
- Filesystem integrity and durability capability reporting.
- Recovery-safe handle invalidation and re-acquisition.
- Build foundations for Linux, Windows, and macOS.

## TAC3 relationship

The design was informed by the TAC3 architecture in Ubuntu.Determinant.Beta.Restricted/tools/tac3, including its versioned on-disk format, contextual FILE layer, HEALTH/ADMIN/RECOVERY regions, device-class awareness, recovery metadata, system-pointer relationships, and explicit distinction between read-only reconstruction and durable write support.

SLVM/9 does not assume TAC3 is the only filesystem, and it does not copy TAC3 implementation into the VM. Instead, TAC3 is represented as a filesystem profile/adapter that can advertise its actual capabilities.

A new filesystem may have semantics that do not map cleanly to ordinary pathname + inode assumptions.

## Core rule

The VM speaks a stable filesystem capability contract. The OS/filesystem adapter speaks the native filesystem. No filesystem is permitted to silently claim durability, atomicity, identity, or recovery semantics that it cannot actually provide.

SLeeLa source remains authoritative. Filesystem behavior is reached through the SLeeLa capability/broker boundary.

Copyright (c) Max Rupplin - MEARVK LLC - 2026
## Native adapter layer

SLVM/9 now includes a host-native filesystem probe beneath the FAL. Linux uses statfs/statvfs/stat observations; macOS uses Darwin statfs/statvfs/stat observations; Windows uses native volume and file-handle APIs. The probe reports evidence and identity into the SLVM/9 contract but never grants SLeeLa authority.

This makes the filesystem boundary noteworthy: the VM can meet an unfamiliar modern filesystem through an adapter without rewriting the language runtime around that filesystem. TAC3 remains an advanced profile above the native probe, where TAC3-specific superblock, FILE, HEALTH, ADMIN, RECOVERY, contextual identity, and durability rules can be verified explicitly.

The control path is:

SLeeLa Artifact -> SLVM/8 Admission -> Policy -> Capability Lease -> SLVM/9 Supervisor -> Native Adapter -> Operating System
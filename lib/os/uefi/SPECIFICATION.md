# UEFI Platform Firmware — Specification (uefi)

UEFI (Unified Extensible Firmware Interface) is the firmware that runs first on
a modern machine, initialises hardware, and hands control to an EFI boot loader.
`lib/os/uefi` models the finite set of firmware implementations and the
attributes a machine description cares about.

## What UEFI provides

- **Boot services** — available before the OS takes over: memory allocation,
  device/protocol handles, image loading, the boot-device order.
- **Runtime services** — remain available after `ExitBootServices`: real-time
  clock, variable store (NVRAM), firmware reset.
- **EFI System Partition (ESP)** — a FAT partition holding `\EFI\...` loaders
  (`BOOTX64.EFI`, vendor loaders).
- **Secure Boot** — verifies loader signatures against enrolled keys (PK/KEK/db).
- **CSM** — the optional Compatibility Support Module that emulates legacy BIOS
  for non-EFI boot; increasingly removed on modern platforms.

## Specification revisions

The UEFI Forum publishes the specification in revisions (2.x). The classes carry
a representative revision per vendor (e.g. 2.10); the exact revision a board
ships varies.

## Firmware vendors

| Vendor | Product | Notes |
|---|---|---|
| AMI | Aptio V | the most widely shipped PC firmware |
| Insyde | InsydeH2O | common on laptops |
| Phoenix | SecureCore | long-standing BIOS/UEFI vendor |
| TianoCore | EDK II / OVMF | the open-source reference; OVMF is the QEMU firmware |
| coreboot | coreboot + Tianocore payload | open firmware with a UEFI payload |

`SLUEFIFirmware` exposes each by name. **OVMF** (TianoCore) is the firmware the
`lib/os` installers invoke under QEMU (`-bios OVMF.fd`) for ISO verification.

## Relationship to the OS model

`SLUEFIFirmware.canLaunch(loader)` connects the hardware firmware to the OS-side
`SLBootloaderSpec`: UEFI firmware can launch any EFI loader (GRUB, BOOTMGR,
boot.efi, systemd-boot); a legacy loader requires the CSM. This is the firmware
→ boot handoff edge of the firmware → boot → kernel → userspace → desktop chain.

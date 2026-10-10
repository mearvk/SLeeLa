# Solid-State Drives — Specification (ssd)

A solid-state drive stores data in NAND flash with no moving parts. `lib/os/ssd`
models the finite set of variants by the dimensions that distinguish real
drives: host interface, form factor, NAND cell type, capacity, and throughput.

## Host interface

| Interface | Link | Representative seq. read | Representative seq. write |
|---|---|---:|---:|
| SATA III | 6 Gb/s | ~560 MB/s | ~530 MB/s |
| NVMe PCIe 3.0 x4 | ~3.9 GB/s | ~3,500 MB/s | ~3,000 MB/s |
| NVMe PCIe 4.0 x4 | ~7.9 GB/s | ~7,000 MB/s | ~5,500 MB/s |
| NVMe PCIe 5.0 x4 | ~15.8 GB/s | ~14,000 MB/s | ~12,000 MB/s |

SATA throughput is capped by the 6 Gb/s link; NVMe runs the NVM Express command
set directly over PCIe lanes. Values are representative flagship figures the
SLeeLa classes carry as defaults; refine with `setThroughput`.

## Form factors

- **2.5"** — SATA drives in a laptop/desktop bay.
- **M.2 2280** — the common NVMe gumstick (22 mm × 80 mm).
- **U.2 / U.3** — enterprise 2.5" NVMe.
- **AIC** — PCIe add-in card.

## NAND cell type

| Type | Bits/cell | Endurance | Use |
|---|---:|---|---|
| SLC | 1 | highest | cache / enterprise write-heavy |
| MLC | 2 | high | legacy high-end |
| TLC | 3 | moderate | mainstream (most consumer drives) |
| QLC | 4 | lower | high-capacity, read-centric |

## Manufacturers

NAND fabs and SSD brands include **Samsung**, **SK Hynix** (incl. **Solidigm**),
**Micron** (consumer brand **Crucial**), **Western Digital/SanDisk**,
**Kingston**, **Seagate**, **Intel**, **Corsair**, and **Sabrent**.
`SLSSDCatalog` exposes representative models from each by name (e.g. Samsung
`990 PRO`, WD `WD_BLACK SN850X`, Crucial `T700`).

## Mapping into the Sleela VM

`SLSSD.capacityBlocks()` converts capacity + logical block size into the VM
block count. When an SSD is selected as the boot disk, `SLMachineModel` passes
that to the VM's block store (the same block interface `lib/cpu/SLHardDrive`
uses), with `MEDIA_SSD` media so the machine boots from solid-state storage.

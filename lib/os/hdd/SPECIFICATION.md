# Hard Disk Drives — Specification (hdd)

A hard disk drive stores data on rotating magnetic platters read by moving
heads. `lib/os/hdd` models the finite set of HDD variants by form factor,
spindle speed, interface, recording technology, cache, and capacity.

## Spindle speed and performance

| RPM | Typical use | Representative sustained transfer |
|---:|---|---:|
| 5400 | laptop / low-power / archival | ~180 MB/s |
| 7200 | desktop / NAS / mainstream | ~260 MB/s |
| 10000 | performance / enterprise | ~250 MB/s |
| 15000 | enterprise transactional | ~300 MB/s |

Rotational drives are bandwidth-competitive for sequential transfer but have
millisecond-scale seek latency, which is the defining difference from an SSD.

## Interfaces

- **SATA III** — 6 Gb/s, desktop/NAS.
- **SAS** — 12 Gb/s, dual-port, enterprise.

## Recording technology

- **CMR** (conventional magnetic recording) — non-overlapping tracks; preferred
  for predictable random-write performance.
- **SMR** (shingled magnetic recording) — overlapping tracks for higher areal
  density, at the cost of slower random rewrites.

## Form factors

- **3.5"** — desktop/NAS/enterprise capacity drives.
- **2.5"** — laptop and some enterprise 10k/15k drives.

## Manufacturers

The HDD market has consolidated to three makers: **Seagate** (IronWolf, Exos),
**Western Digital** (WD Red/Gold/Blue), and **Toshiba** (N300, MG). All three are
exposed by `SLHardDiskCatalog`.

## Mapping into the Sleela VM

`SLHardDisk.capacityBlocks()` converts capacity + block size into the VM block
count. When an HDD is the selected boot/bulk device, `SLMachineModel` hands that
to the VM block store with `MEDIA_HDD` media, so the emulated machine uses
rotational storage.

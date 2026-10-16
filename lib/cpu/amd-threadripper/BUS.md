# AMD Ryzen Threadripper 9000 (Zen 5 x86-64) Bus

## External buses

| Bus | Width | Direction |
|---|---:|---|
| Address | 48 | CPU output |
| Data | 64 | bidirectional |
| R/W control | 1 | CPU output/control |
| Clock | model clock | timing/control |

## Physical I/O (documented, not re-modeled per-cycle)

- Memory: DDR5-6400 — 4-channel (HEDT) or 8-channel (PRO WX).
- PCIe: Gen 5.0 — up to 88 lanes (HEDT) or up to 128 lanes (PRO WX).
- Socket: sTR5.

## Transaction model

A SLeeLa bus event records cycle number, phase, address, read/write, data, device/coupler, and completion state. Wait/ready behavior is permitted at the system boundary without asserting a specific hardware protocol beyond what is documented.

## Address formation

Addressing modes may require multiple transfers; indexed and indirect operations can produce additional observable bus cycles.

## DMA coupling

External DMA is represented as a system-level bus master; the base CPU does not instantiate a DMA engine.

## Sources

- https://en.wikipedia.org/wiki/Zen_5
- https://en.wikipedia.org/wiki/Ryzen_Threadripper
- https://en.wikipedia.org/wiki/List_of_AMD_Ryzen_processors

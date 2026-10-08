# Nintendo CPU Model Reference

The SLeeLa CPU library provides historical Nintendo console hardware profiles under lowercase `/nintendo-*` paths.

| Model | CPU / Architecture | Speed | Main/System RAM | US Launch MSRP |
|---|---|---:|---:|---:|
| `nes` | Ricoh 2A03/2A07, 6502-derived | ~1.79 / 1.66 MHz | 2 KB | $179.99 |
| `snes` | Ricoh 5A22, 65C816-derived | ~3.58 MHz | 128 KB | $199.99 |
| `nintendo-64` | NEC VR4300, MIPS III-derived | 93.75 MHz | 4 MB | $199.99 |
| `gamecube` | IBM Gekko, PowerPC-derived | 485 MHz | 24 MB + 16 MB | $199.99 |
| `wii` | IBM Broadway, PowerPC-derived | 729 MHz | 88 MB total | $249.99 |
| `wii-u` | IBM Espresso, 3-core PowerPC-derived | ~1.24 GHz | 2 GB DDR3 | $299.99 |
| `switch` | Custom NVIDIA Tegra, ARM-based | configurable | 4 GB LPDDR4 | $299.99 |
| `switch-2` | Custom NVIDIA processor, ARM-based | not publicly specified | 12 GB class LPDDR5X | $449.99 launch |

## Directory Standard

All model directory names are lowercase:

- `/lib/cpu/nintendo-nes`
- `/lib/cpu/nintendo-snes`
- `/lib/cpu/nintendo-nintendo-64`
- `/lib/cpu/nintendo-gamecube`
- `/lib/cpu/nintendo-wii`
- `/lib/cpu/nintendo-wii-u`
- `/lib/cpu/nintendo-switch`
- `/lib/cpu/nintendo-switch-2`

Each model contains a `CPU.md` reference document covering CPU architecture, clock/timing information, memory, launch date, cost, SLeeLa representation, and compatibility intent.

## Accuracy and Cost

These are historical hardware reference profiles for SLeeLa CPU and machine modeling. Launch prices are US launch MSRPs rather than current resale prices.

Nintendo's current public documentation identifies the original Switch as using a custom NVIDIA Tegra processor with 32 GB internal storage, while Switch 2 uses a custom NVIDIA processor and 256 GB UFS storage. Switch 2 launched at $449.99 in the US; Nintendo revised that US MSRP to $499.99 effective September 1, 2026. citeturn0search0turn0search2turn0search5turn0search1

The profiles intentionally avoid proprietary firmware, keys, or copyrighted system software.

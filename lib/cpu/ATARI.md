# Atari CPU Model Reference

The SLeeLa CPU library uses direct lowercase directories for major Atari console and handheld systems.

| Model | CPU | Speed | Memory |
|---|---|---:|---:|
| `atari-2600` | MOS 6507 | 1.19 MHz | 128 B |
| `atari-5200` | MOS 6502C | 1.79 MHz | 16 KB |
| `atari-7800` | SALLY / 6502C-derived | 1.79 MHz | 4 KB |
| `atari-xegs` | MOS 6502C | 1.79 MHz | 64 KB |
| `atari-lynx` | 65SC02-derived | ~4 MHz | 64 KB |
| `atari-jaguar` | Motorola 68000 + custom processors | 13.295 MHz host | 2 MB |
| `atari-jaguar-cd` | Motorola 68000 host | 13.295 MHz | 2 MB |

The library focuses on actual Atari hardware systems rather than every later licensed plug-and-play revision. Atari Flashback models are intentionally treated as a product family rather than separate original CPU architectures because later models use emulation or system-on-chip implementations. citeturn0search2turn0search8

The 2600, 5200, 7800, XEGS, Lynx, and Jaguar families are established Atari hardware platforms. citeturn0search3turn0search6
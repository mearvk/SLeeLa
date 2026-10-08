# SEGA CPU Model Reference

The SLeeLa CPU library uses direct lowercase directories for major SEGA hardware systems.

| Model | CPU | Speed |
|---|---|---:|
| `sega-sg-1000` | Zilog Z80A | 3.58 MHz |
| `sega-mark-iii` | Zilog Z80A | 3.58 MHz |
| `sega-master-system` | Zilog Z80A | 3.58 MHz |
| `sega-genesis` | Motorola 68000 + Z80 | ~7.6 MHz |
| `sega-mega-cd` | Motorola 68000 family | ~12.5 MHz |
| `sega-32x` | 2 × Hitachi SH-2 | 23 MHz |
| `sega-game-gear` | Z80-derived | ~3.58 MHz |
| `sega-nomad` | Motorola 68000 | ~7.67 MHz |
| `sega-pico` | Motorola 68000 | ~7.6 MHz |
| `sega-saturn` | 2 × Hitachi SH-2 | 28.6 MHz |
| `sega-dreamcast` | Hitachi SH-4 | 200 MHz |

The library keeps major add-ons and distinct handheld/educational platforms as separate CPU descriptors where they represent meaningful hardware configurations. Sega's official historical archive covers SG-1000 through Dreamcast-era hardware. citeturn0search0
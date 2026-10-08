# Nintendo Entertainment System (NES) — CPU Reference

## Hardware Profile

| Field | Reference |
|---|---|
| System | Nintendo Entertainment System (NES) |
| CPU | Ricoh 2A03 / 2A07, 8-bit 6502-derived |
| CPU clock | ~1.79 MHz NTSC / ~1.66 MHz PAL |
| Main RAM | 2 KB |
| Video RAM | 2 KB internal PPU VRAM |
| Launch | 1983 Japan; 1985 North America |
| Launch MSRP | $179.99 US |

## SLeeLa Representation

The NES profile is a historical hardware reference for `/lib/cpu`. It is intended to describe CPU, memory, timing, bus, instruction, and device behavior without including proprietary Nintendo firmware.

## Compatibility Intent

The profile can be used by SLeeLa CPU and guest-machine components when modeling an NES-class 6502-derived system, including its constrained memory and cycle-oriented execution model.

## Cost Note

The price is the historical US launch MSRP, not current collector or resale value.

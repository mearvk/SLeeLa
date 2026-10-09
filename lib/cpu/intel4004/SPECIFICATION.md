# Intel 4004 Specification

| Property | SLeeLa profile |
|---|---|
| CPU | Intel 4004 |
| Arithmetic/data width | 4 bits |
| Instruction storage | 8-bit instruction bytes |
| Program counter | 12 bits |
| Register file | 16 × 4-bit registers, usable as eight pairs |
| Accumulator | 4 bits |
| Carry | 1-bit state |
| Return stack | Three 12-bit return-address levels |
| External components | 4001 ROM/I/O and 4002 RAM/I/O family models, where configured |
| Cache | None |
| Timing | Profile metadata required for cycle-accurate mode |

Keep instruction decoding, chipset mapping, and execution semantics model-specific. Mark undocumented or configuration-dependent peripheral behavior as unspecified rather than assuming a modern memory map.
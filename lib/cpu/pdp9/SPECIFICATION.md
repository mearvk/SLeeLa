# PDP-9 Specification

| Property | SLeeLa profile |
|---|---|
| Family | DEC PDP-9 |
| Word width | 18 bits |
| Architecture style | Accumulator-oriented stored-program minicomputer |
| Addressing / memory capacity | Defined by selected machine profile |
| Instruction groups | Memory, arithmetic/logical, control-flow, I/O |
| Peripheral handling | Configurable device/function map |
| Cache | No modern cache assumed |
| DMA | No generic DMA assumed by baseline |
| Timing | Requires explicit timing metadata for timed mode |

Evidence labels: **documented**, **derived**, **estimated**, and **unspecified**. Use PDP-9-specific opcode and register definitions; do not copy PDP-4 or PDP-7 decoding tables without validation.
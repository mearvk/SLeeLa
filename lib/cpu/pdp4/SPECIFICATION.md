# PDP-4 Specification

| Property | SLeeLa profile |
|---|---|
| Family | DEC PDP-4 |
| Word width | 18 bits |
| Architecture style | Accumulator-oriented, stored-program minicomputer |
| Address width / memory capacity | Configurable by machine profile; do not infer solely from word width |
| Instruction groups | Memory operations, arithmetic/logical operations, control flow, I/O |
| I/O | Configurable device/function map |
| Cache | None assumed |
| DMA | No generic DMA assumed |
| Timing | Unspecified unless supplied by a reliable PDP-4 timing profile |

Evidence labels: **documented**, **derived**, **estimated**, and **unspecified**. Never reuse PDP-5/PDP-8 12-bit assumptions or later PDP-7/PDP-9/PDP-15 extensions without explicit evidence.
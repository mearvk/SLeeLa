# PDP-15 Specification

| Property | SLeeLa profile |
|---|---|
| Family | DEC PDP-15 |
| Word width | 18 bits |
| Execution style | Accumulator-oriented |
| Memory size/addressing | Selected machine configuration; no universal capacity assumed |
| Instruction groups | Memory, arithmetic/logical, control flow, I/O |
| Optional instructions | Explicitly enabled by profile |
| Peripherals | Configurable device map |
| Cache | No modern cache hierarchy assumed |
| Timing | Functional by default; timed mode requires sourced metadata |

Record hardware facts as **documented**, implementation consequences as **derived**, approximations as **estimated**, and unavailable facts as **unspecified**. Do not treat PDP-4/PDP-7/PDP-9 behavior as identical merely because they share an 18-bit lineage.
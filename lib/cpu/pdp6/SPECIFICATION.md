# PDP-6 Specification

| Property | SLeeLa profile |
|---|---|
| Family | DEC PDP-6 |
| Word width | 36 bits |
| Instruction model | PDP-6-specific instruction formats; halfword interpretation is decoder-defined |
| Address width / memory capacity | Machine-profile configured; validate against the selected hardware model |
| Register file | Profile-defined; do not assume PDP-10 register semantics without verification |
| Instruction groups | Memory reference, arithmetic/logical, control transfer, I/O |
| Cache | No modern cache hierarchy assumed |
| DMA | Device-specific only when explicitly configured |
| Timing | Unspecified unless supplied by a reliable PDP-6 timing profile |

Evidence labels: **documented**, **derived**, **estimated**, and **unspecified**. Do not reuse PDP-10 or later KL/KI processor extensions as PDP-6 behavior.
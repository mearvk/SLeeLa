# LINC-8 Specification

| Property | SLeeLa profile |
|---|---|
| Machine | DEC LINC-8 hybrid computer |
| Word width | 12 bits |
| Execution personalities | LINC mode and PDP-8 mode |
| Decoder | Separate mode-specific decoders |
| Memory size and mapping | Profile-defined |
| Peripheral map | Profile-defined, including LINC-specific devices |
| Cache | No modern cache hierarchy assumed |
| DMA | No generic DMA assumed |
| Timing | Unspecified unless supplied by a verified machine profile |

Treat LINC-8 as a specific hybrid system, not as a generic PDP-8 with a cosmetic mode flag. Distinguish documented facts from derived implementation details and unspecified hardware behavior.
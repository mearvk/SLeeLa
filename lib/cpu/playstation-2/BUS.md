# PlayStation 2 (Emotion Engine / MIPS) Bus

## External buses

| Bus | Width | Direction |
|---|---:|---|
| Address | 32 | CPU output |
| Data | 64 | bidirectional |
| R/W control | 1 | CPU output/control |
| Clock | model clock | timing/control |

## Transaction model

A SLeeLa bus event records cycle number, phase, address, read/write, data, device/coupler, and completion state. Wait/ready behavior is permitted at the system boundary without asserting a specific hardware protocol beyond what is documented.

## Address formation

Addressing modes may require multiple transfers; indexed and indirect operations can produce additional observable bus cycles.

## DMA coupling

This part integrates a DMA engine; DMA transfers are modeled as bus-master cycles arbitrated against the CPU.

## Sources

- https://en.wikipedia.org/wiki/Emotion_Engine
- https://en.wikipedia.org/wiki/Comparison_of_PlayStation_consoles

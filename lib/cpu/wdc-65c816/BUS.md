# WDC 65C816 Bus

## External buses

| Bus | Width | Direction |
|---|---:|---|
| Address | 24 | CPU output |
| Data | 16 | bidirectional |
| R/W control | 1 | CPU output/control |
| Clock | model clock | timing/control |

## Transaction model

A SLeeLa bus event records cycle number, phase, address, read/write, data, device/coupler, and completion state. Wait/ready behavior is permitted at the system boundary without asserting a specific hardware protocol beyond what is documented.

## Address formation

Addressing modes may require multiple transfers; indexed and indirect operations can produce additional observable bus cycles.

## DMA coupling

External DMA is represented as a system-level bus master; the base CPU does not instantiate a DMA engine.

## Sources

- https://wikiwand.com/en/articles/65C816

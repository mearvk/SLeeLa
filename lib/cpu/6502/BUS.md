# 6502 Bus

## External buses

| Bus | Width | Direction |
|---|---:|---|
| Address | 16 | CPU output |
| Data | 8 | bidirectional |
| R/W control | 1 | CPU output/control |
| Clock phases | 2-phase | timing/control |

## Transaction model

A SLeeLa bus event contains:

- cycle number;
- phase;
- address;
- read/write;
- data;
- device/coupler;
- completion state.

The model permits wait/ready behavior at the system boundary without asserting that the original CPU contained a modern bus protocol.

## Address formation

Addressing modes may require multiple byte transfers. Indexed and indirect operations can therefore produce additional observable bus cycles.

## DMA coupling

External DMA can be represented as a system-level bus master. The base CPU itself does not instantiate a DMA engine.

Source: https://github.com/mamedev/mame/blob/master/docs/source/techspecs/m6502.rst

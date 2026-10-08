# CPU DMA Specification

DMA is modeled independently from the CPU execution core.

A DMA channel may contain:

- source address
- destination address
- transfer length
- width
- increment/fixed addressing
- burst size
- descriptor/list pointer
- completion state
- interrupt routing
- priority/arbitration
- cache coherency requirements

Intel's current published DMA register documentation demonstrates source/destination address registers, linked-list pointers and control/status state as concrete DMA programming concepts. citeturn0search7turn0search13
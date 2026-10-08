# IBM POWER Circuit Model

## Functional scope

This is an architectural/functional circuit model, not a transistor reconstruction.

## Blocks

1. Instruction fetch.
2. Instruction decode.
3. Branch processor.
4. General register file.
5. Fixed-point execution.
6. Floating-point execution.
7. Condition/control registers.
8. Storage/load-store interface.
9. Cache interface.
10. Exception/interrupt control.
11. Completion/control.
12. System interconnect.

## POWER2 profile

POWER2 adds execution resources represented by explicit profile components rather than being hidden in the generic POWER unit.

## Couplers

SLCoupler models:

- fetch -> branch/decode;
- decode -> register file;
- registers -> fixed-point;
- registers -> floating-point;
- branch -> control/PC;
- storage -> cache/memory;
- cache -> system interconnect;
- exception -> control state;
- DMA -> bus arbitration.

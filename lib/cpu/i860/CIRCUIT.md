# Intel i860 Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Decoder and pairing logic.
3. Integer register file.
4. Integer/address execution.
5. Floating-point register file.
6. Floating-point/vector execution.
7. Load/store path.
8. MMU/TLB where implemented.
9. Instruction/data cache.
10. Branch/control logic.
11. Bus interface.
12. Clock and sequencing.

## Couplers

SLCoupler models:

- fetch -> decoder;
- decoder -> pairing;
- register files -> execution units;
- execution -> memory;
- cache/MMU -> bus;
- control transfer -> fetch;
- execution -> completion;
- DMA -> bus arbitration.

## Accuracy boundary

The i860's exposed VLIW/dual-operation behavior is modeled without inventing undocumented internal transistor topology.

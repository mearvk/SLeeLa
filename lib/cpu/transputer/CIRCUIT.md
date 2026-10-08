# Inmos Transputer Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Decoder.
3. Operand/workspace evaluation.
4. Integer execution.
5. Optional floating-point execution.
6. Process scheduler.
7. Channel controller.
8. Serial link interfaces.
9. Local-memory interface.
10. Timer/event logic.
11. Interrupt/control logic.
12. Clock/sequencing.

## Couplers

SLCoupler models:

- fetch -> decoder;
- decoder -> operand evaluation;
- execution -> process state;
- scheduler -> process execution;
- channels -> links;
- links -> external interconnect;
- memory -> execution;
- DMA -> system arbitration where present.

## Accuracy boundary

Transputer concurrency and communication behavior are modeled explicitly without inventing undocumented internal transistor topology.

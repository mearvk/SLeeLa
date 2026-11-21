# Motorola 68030 Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction prefetch/cache.
2. Decoder.
3. Effective-address generator.
4. Data/address register files.
5. Integer/shift unit.
6. Branch/exception controller.
7. Instruction cache.
8. Data cache.
9. MMU.
10. ATC.
11. Burst bus interface.
12. Coprocessor interface.
13. Interrupt controller.
14. Clock/pipeline control.

## Couplers

SLCoupler models:

- prefetch -> I-cache/decode;
- decode -> register files;
- address generation -> MMU;
- MMU/ATC -> caches;
- ALU -> condition codes;
- D-cache -> bus;
- exception -> supervisor state;
- coprocessor -> execution/control;
- DMA -> bus arbitration.

## Accuracy boundary

No 68040 dual-MMU, 4-KB-cache, multiple-concurrent-execution topology is projected backward onto the 68030. citeturn0search5

## Executable circuit model

`SL68030Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SL68030CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.

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

## Executable circuit model

`SLI860Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLI860CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.

# Intel iAPX 432 Circuit Model

## Functional scope

Architectural/functional model only; this is not a transistor or microcode-ROM reconstruction.

## Major blocks

1. Instruction/control interpreter.
2. Object/descriptor unit.
3. Protection and capability checks.
4. Operand/reference unit.
5. Execution unit.
6. Memory-management interface.
7. Fault/interrupt controller.
8. System interface.
9. Clock/sequencing.

## Couplers

SLCoupler models:

- control -> object resolution;
- object resolution -> operand access;
- protection -> memory transaction;
- execution -> architectural state;
- fault -> control state;
- DMA -> system arbitration.

The model intentionally preserves the unusual 432 architecture instead of translating it into a generic x86 register machine.

## Executable circuit model

`SLiAPX432Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLiAPX432CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.

# SHARC DSP Functional Circuit Model

This is a functional block model, not a transistor-level netlist. Configurable blocks include instruction fetch/decode, register file, address-generation units, ALU, multiplier/MAC, floating-point units, loop controller, program/data memory interfaces, interrupt controller, DMA, peripherals, clock, and profile-specific pipeline stages. SLCoupler models instruction flow, operand delivery, execution-to-register writeback, memory transactions, DMA arbitration, and interrupt signaling.

## Executable circuit model

`SLSHARCCircuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLSHARCCPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.

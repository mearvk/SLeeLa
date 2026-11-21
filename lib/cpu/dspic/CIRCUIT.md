# dsPIC Functional Circuit Model

Functional block model, not a transistor-level netlist. Blocks may include instruction fetch/decode, working register file, MCU ALU, DSP multiplier/MAC and accumulators, address-generation unit, loop controller, program flash interface, data RAM interface, interrupt controller, timers, serial peripherals, ADC, DMA, and clock. Instantiate only blocks supported by the selected device profile. SLCoupler models operand delivery, MAC writeback, memory/peripheral transactions, interrupt requests, and DMA arbitration.

## Executable circuit model

`SLdsPICCircuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLdsPICCPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.

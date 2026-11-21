# OpenRISC 1000 Functional Circuit Model

Functional block model, not a transistor-level netlist. Core blocks include instruction fetch/decode, GPR file, integer ALU, branch/control unit, load/store unit, exception/interrupt control, and SPR access. Optional blocks include FPU, DSP, atomic support, debug, MMU/TLB, and caches. SLCoupler models decode-to-execution, register writeback, memory traffic, exception transitions, and optional extension connections.

## Executable circuit model

`SLOpenRISCCircuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLOpenRISCCPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.

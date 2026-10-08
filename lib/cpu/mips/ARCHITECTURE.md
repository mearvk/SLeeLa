# MIPS Architecture

## Topology

SLMIPSCPU
-> SLClock
-> SLInstructionFetch
-> SLPipeline
-> SLRegisterFile
-> SLExecutionUnit
-> SLIOBus
-> SLInterruptController
-> SLBusArbiter
-> SLCoupler

Optional implementation layers:
SLMMU -> SLTLB
SLCache -> SLL1/SLL2/SLL3

## Register datapath

The register file supplies two source operands and receives a destination result. GPR 0 is hardwired to zero at the architectural boundary.

## Five-stage abstraction

### IF
Fetch the fixed-width instruction and advance sequential PC state.

### ID
Decode opcode/function fields, read registers, and generate immediate/control information.

### EX
Perform ALU operations, address calculation, comparisons, shifts, and branch target calculation.

### MEM
Perform load/store bus transactions.

### WB
Commit register results.

## Hazards

A concrete MIPS implementation may resolve hazards with forwarding, interlocks, stalls, or scheduling. SLeeLa records the chosen mechanism in the implementation rather than assuming all MIPS CPUs behave identically.

## Exceptions

CP0 and exception control are separate from ordinary ALU execution. Exception entry saves the implementation-defined return state and transfers control through the documented exception mechanism.

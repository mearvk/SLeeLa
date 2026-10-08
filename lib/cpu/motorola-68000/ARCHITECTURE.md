# Motorola 68000 Architecture

## Topology

SL68000CPU -> SLClock -> SLRegisterFile -> SLPipeline -> SLInstructionFetch -> SLExecutionUnit -> SLIOBus -> SLIOTiming -> SLInterruptController -> SLBusArbiter

## Register organization

The programmer model contains eight 32-bit data registers and eight 32-bit address registers, with A7 serving as stack pointer. The PC and SR complete the principal control state.

## Execution organization

The SLeeLa model separates instruction fetch, instruction decode, effective-address calculation, operand transfer, ALU/logic operation, memory/I/O transfer, and exception or interrupt processing.

This preserves the 68000's rich addressing behavior without pretending it is a modern superscalar pipeline.

## Bus

The external interface is modeled independently from internal operand width. The original processor uses a 16-bit data bus and a 24-bit address space.

Bus arbitration is modeled through SLBusArbiter, including bus request/grant behavior.

## Circuit boundary

Functional datapath and control blocks are modeled. Exact transistor-level claims require a validated die-level source and are not inferred from the programmer's manual.

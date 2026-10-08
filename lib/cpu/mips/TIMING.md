# MIPS Timing

## Pipeline timing

The classic SLeeLa abstraction uses IF, ID, EX, MEM, and WB stages. This gives a useful cycle-level model while avoiding the false claim that every MIPS processor has identical physical pipeline timing.

## Branches

Classic MIPS has a branch delay slot. The instruction immediately following a branch/jump is architecturally executed in the classic model. citeturn0search20

## Loads

A load has address-generation, memory-access, and register-writeback timing. Concrete implementations may add stalls or forwarding constraints.

## Exceptions

Exceptions and interrupts can redirect control flow and alter normal pipeline progression. CP0 state participates in that transition.

## Timing evidence

Record stage, instruction PC, operand dependencies, memory request/response, stall/forward decision, branch-delay state, and exception state.

# Z80 Circuit Model

## Scope

This is a functional circuit model, not an asserted transistor-level reconstruction.

## Major blocks

1. Register file and alternate register bank.
2. ALU and flag generation.
3. Instruction register/decode/control.
4. Address generation and index logic.
5. Internal data movement.
6. Memory/I/O bus interface.
7. Interrupt control.
8. Refresh control.
9. Clock and T/M cycle sequencing.

## Couplers

SLeeLa uses SLCoupler objects between these blocks so the architecture can expose register-to-ALU movement, address-to-bus movement, data-bus-to-register movement, interrupt acknowledge, external WAIT, and bus request/acknowledge.

The Z80's internal transistor organization is not claimed here unless supported by a validated die-level source.

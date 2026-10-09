# PDP-8 Logical Circuit Abstraction

## Scope

This is a logical educational/simulation abstraction, not a transistor-level DEC PDP-8 schematic.

## Functional blocks

- 12-bit instruction fetch and decode.
- 12-bit accumulator and separate link bit.
- 12-bit program counter and address selection.
- Memory-reference effective-address logic.
- Operate-instruction micro-operation control.
- Optional EAE arithmetic block.
- IOT dispatch and interrupt interface.
- Word-addressed memory interface.

## Variant control

Add memory-extension logic, arithmetic options, and device interfaces only when documented for the selected model. Do not infer gate counts, physical layout, power draw, or clock speed.

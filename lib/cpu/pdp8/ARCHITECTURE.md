# DEC PDP-8 Architecture

## Instruction groups

The base instruction set is organized around memory-reference instructions, operate instructions, and I/O-transfer (IOT) instructions. Decode the 12-bit instruction using the selected profile's opcode and field definitions.

## Memory-reference addressing

Memory-reference instructions use page selection and indirect addressing. The current-page form derives its page from the instruction location; the zero-page form uses the base page. Indirect references read an address word from memory, with auto-index behavior at architecturally defined locations.

## Arithmetic

The accumulator is 12 bits and the link is a separate one-bit state. Implement carry/link and rotate behavior according to the exact instruction definition, not host-language overflow.

## Operate instructions

Operate instructions combine documented micro-operations. Preserve ordering and interaction among clear, complement, increment, rotate, and skip operations according to the architecture.

## I/O and options

IOT instructions delegate device-specific behavior to configured handlers. EAE and memory-extension instructions are enabled only for profiles that support them.

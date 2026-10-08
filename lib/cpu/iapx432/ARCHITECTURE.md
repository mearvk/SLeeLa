# Intel iAPX 432 Architecture

## Architectural path

SLiAPX432CPU
-> instruction/control interpretation
-> object/descriptor resolution
-> operand access
-> execution
-> protection checks
-> memory access
-> completion

## Object model

The SLeeLa implementation provides explicit object and descriptor abstractions. Capability/protection information travels with the architectural reference rather than being reduced to a conventional flat pointer.

## Profiles

The original iAPX 432 architecture and its processor/interface components remain distinct from later Intel x86 designs.

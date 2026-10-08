# Intel Itanium Registers

## General registers

- GR0-GR127
- 64-bit architectural width
- rotating register regions where enabled

## Floating point

- FR0-FR127
- extended precision architectural storage
- integer/FP conversion and FP execution support

## Predicates

- PR0-PR63
- predicate-controlled instruction execution

## Branch registers

- BR0-BR7
- indirect/return/control-flow support

## Application/control state

IA-64 provides dedicated application and control registers for:

- instruction pointers;
- processor status;
- register-stack state;
- memory translation;
- interruption state;
- performance/debug facilities.

These are kept separate from general registers.

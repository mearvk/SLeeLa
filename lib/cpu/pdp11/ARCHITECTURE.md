# DEC PDP-11 Architecture

## Core

SLPDP11CPU
-> instruction fetch
-> decode
-> operand/address calculation
-> execute
-> memory/I/O
-> condition-code update

## Addressing

The PDP-11 provides a rich set of register, deferred, indexed, and autoincrement/autodecrement addressing modes. SLeeLa models effective-address generation explicitly.

## Generational profiles

PDP-11/20 through later LSI and J-11 implementations remain separate profiles. Later MMU, cache, and processor features are not projected onto the earliest machines.

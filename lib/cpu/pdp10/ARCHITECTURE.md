# DEC PDP-10 Architecture

## Execution path

SLPDP10CPU
-> instruction fetch
-> decode
-> accumulator/operand access
-> effective-address and byte-pointer processing
-> execution
-> memory or I/O
-> completion

## Addressing model

The model preserves 36-bit words, classic instruction fields, accumulator semantics, and byte pointers. It does not reinterpret PDP-10 word addressing as a modern byte-addressed 64-bit ISA.

## Profiles

PDP-6 lineage, KA/KI/KL/KL10, KS10, and later compatible systems are represented as distinct profiles where their system and implementation behavior differs.

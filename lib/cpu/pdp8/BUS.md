# PDP-8 Memory and I/O Interfaces

## Memory interface

Use a word-addressed storage interface for 12-bit words. Validate addresses against installed memory and the active memory field before each access.

## I/O interface

IOT instructions dispatch to configured device handlers. Device status, data transfers, interrupt requests, and device-specific timing are owned by the system/device model.

## Interrupts

Model interrupt enable and interrupt-entry behavior according to the selected PDP-8 variant. Do not assume every system has identical devices or interrupt sources.

## Fault handling

Out-of-range memory references, absent devices, and unsupported IOT operations must produce explicit simulator diagnostics or modeled outcomes. Avoid unchecked host-memory access.

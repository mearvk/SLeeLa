# PDP-8 Memory and Performance Model

## Memory hierarchy

The architectural memory model is word-addressed, with 12-bit words in the base profile. Installed memory capacity and memory-extension behavior depend on the selected machine variant.

## Cache policy

Do not assume a modern CPU cache hierarchy. Represent core memory access and any documented implementation-specific buffering only when supported by evidence for the chosen model.

## Timing

Memory access and instruction timing may vary by machine. Keep documented timing separate from a functional execution model.

## Evidence

Use `documented`, `derived`, `estimated`, or `unspecified` labels for timing and hardware details. Do not infer cache size or cycle counts from architecture alone.

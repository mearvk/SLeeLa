# VM Source Symbols

SLVM/7 consumes the common SLeeLa VM source symbol contract in `/lib/vm/COMPILER-SYMBOLS.md`.

It retains the SLVM/6 source vocabulary and common management symbols, including the Memory Management and Security Management profiles, Linking Manager, Challenge Manager, Reports Manager, and Compiler Manager contracts.

SLVM/7 additionally requires manager-aware fitment for:

- Log Manager
- Memory Manager
- Health Manager
- Recovery Manager
- Checkpoint Manager
- Resource Manager
- Attestation Manager
- Lineage Manager

Required options that cannot be implemented by this VM generation are rejected. Optional options may be omitted only according to the source declaration and runtime policy.

Manager fitment must be dependency-safe: memory pressure depends on memory accounting; recovery depends on valid checkpoints; migration depends on compatible manager state; security evidence depends on healthy logging and attestation.

Copyright (c) Max Rupplin - MEARVK LLC - 2026

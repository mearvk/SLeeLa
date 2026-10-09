# OpenRISC 1000 Architecture

Functional path: instruction fetch/decode -> general-purpose register file -> integer ALU/branch or enabled extension -> load/store interface -> architectural writeback.

The profile defines ORBIS revision, endianness, exception and interrupt behavior, supervisor/user state, special-purpose registers, and optional units. Privileged operations require access checks. Unsupported or reserved encodings raise a modeled illegal-instruction exception rather than being guessed. The core ISA is distinct from SoC-specific peripherals.

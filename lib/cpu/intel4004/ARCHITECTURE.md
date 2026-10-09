# Intel 4004 Architecture

The 4004 is a 4-bit accumulator-oriented processor with separate program and data paths.

1. Fetch an 8-bit instruction byte from the configured program store using the 12-bit PC.
2. Decode the instruction, including two-byte instructions and immediate data where applicable.
3. Execute accumulator, register-pair, branch/call, test, or external-chip operation.
4. Update accumulator, carry, registers, PC, and the three-level return stack as specified.
5. Route RAM/ROM/I/O operations to the configured 4001/4002-style chipset devices.

Do not flatten all instructions into 8-bit opcodes: some operations use an additional byte. Program-store and data-chip behavior should be separate interfaces. The profile is functional unless verified timing metadata is provided.
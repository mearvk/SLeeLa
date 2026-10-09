# Intel 4004 CPU

SLeeLa profile for the Intel 4004, an early commercial single-chip microprocessor introduced in 1971.

- **Status:** profile-driven functional foundation; not a cycle-accurate emulator.
- **Data width:** 4-bit arithmetic/data path.
- **Program storage:** instruction bytes are 8 bits; program address is 12 bits.
- **Core state:** accumulator, carry flag, sixteen 4-bit registers arranged as eight register pairs, and a three-level hardware return stack.
- **External system:** ROM, RAM, and I/O are modeled as chipset components, not as modern unified memory.

The 4004's instruction encoding and peripheral behavior must remain distinct from later Intel 8008/8080/x86 architectures.
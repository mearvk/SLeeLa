# PDP-12 Architecture

Model the PDP-12 as a configured machine with two instruction environments.

1. Fetch the next instruction using the active environment's PC and memory rules.
2. Dispatch decoding to either the PDP-8-family decoder or the LINC-compatible decoder.
3. Execute only semantics defined for that environment and selected machine profile.
4. Preserve environment-specific registers, flags, and instruction sequencing.
5. Route peripheral operations through the configured device map.
6. Handle environment changes only through documented control mechanisms or explicit emulator configuration.

Shared physical memory may be modeled centrally, but instruction decoding must not be merged into a single synthetic opcode table. Unsupported operations produce diagnostics and structured results.
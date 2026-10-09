# PDP-14 Control Architecture

The PDP-14 profile is organized around a deterministic control scan.

1. Acquire the configured input image.
2. Fetch and decode the next control instruction.
3. Evaluate logic and sequencing operations against input and internal state.
4. Update internal control state and the output image.
5. Commit outputs according to the selected execution policy.
6. Emit diagnostics for invalid instructions, unmapped points, or unsupported functions.

The exact scan semantics, point addressing, and instruction encoding must be supplied by the selected PDP-14 hardware/profile documentation. Do not silently substitute another DEC CPU's instruction decoder.
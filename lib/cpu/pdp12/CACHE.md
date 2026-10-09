# PDP-12 Cache Policy

The baseline PDP-12 profile assumes no modern L1/L2/L3 cache hierarchy.

- Both instruction environments access the configured memory abstraction.
- Any host-side caching is an invisible emulator optimization, not guest hardware.
- Writes must invalidate cached host-side values.
- Preserve memory and I/O ordering across mode changes.
- Do not add cache timing to guest execution unless a specific machine model explicitly requires it.

This is emulator policy, not a claim about all later systems or peripherals used with the PDP-12.
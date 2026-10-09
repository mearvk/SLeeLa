# PDP-4 Cache Policy

The baseline PDP-4 model assumes **no modern CPU cache hierarchy**.

- Memory operations pass through the configured memory abstraction.
- Host-side optimization is permitted only if invisible to guest behavior.
- Writes must invalidate any host-side memoized data.
- Memory and I/O ordering must remain deterministic.

This describes SLeeLa emulator policy, not a claim about every peripheral or later machine derived from the PDP-4.
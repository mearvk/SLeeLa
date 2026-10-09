# TX-0 Cache Policy

The SLeeLa TX-0 profile does not assume a modern CPU cache hierarchy.

- Guest memory accesses pass through the configured memory model.
- Host-side caching is allowed only as an invisible optimization.
- Writes must invalidate cached host data.
- Preserve deterministic instruction and I/O ordering.

This is an emulator implementation policy, not a claim about unmodeled experimental hardware details.
# Intel 4004 Cache Policy

The Intel 4004 profile has **no CPU cache hierarchy**.

- Instructions are fetched from the configured program store.
- Data and I/O requests go through the external chipset model.
- Optional host-side memoization is permitted only if guest-visible behavior is unchanged.
- Writes and external I/O effects must invalidate any affected host-side cache.
- Do not introduce modern L1/L2/L3 structures as emulated 4004 hardware.
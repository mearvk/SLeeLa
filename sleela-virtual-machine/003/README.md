<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Virtual Machine 3

SLVM/3 advances SLVM/2 toward verified execution, stronger isolation, deterministic policy, and supply-chain integrity.

It preserves the authoritative SLeeLa compiler and Output Symbol contract and does not introduce a parallel language implementation.

## Improvements over /2

- signed and versioned execution manifests;
- artifact hashes and ABI verification;
- explicit policy snapshots;
- sandbox/isolation profiles;
- resource quotas and lifetime controls;
- deterministic security decisions;
- cryptographic provider identity and algorithm policy;
- certificate pinning/trust hooks;
- tamper-evident observability;
- reproducible execution metadata;
- stronger runtime-selection validation.

Runtime selection is configured in `config/slvm-runtime.conf`. Configuration selects execution policy and adapter behavior; it does not grant capabilities.

The C interface remains the stable ABI. C++ may provide higher-level verification, policy, orchestration, and platform integration without bypassing the C security boundary.

Copyright (c) Max Rupplin - MEARVK LLC - 2026
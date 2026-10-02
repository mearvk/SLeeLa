<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Virtual Machine 3

SLVM/3 advances the SLVM/2 security architecture toward verified execution, stronger isolation, deterministic policy, and supply-chain integrity.

SLVM/3 uses /2 as its specification baseline. It does not introduce a parallel SLeeLa language or compiler.

## Primary Improvements

- signed and versioned execution manifests;
- stronger artifact/link verification;
- explicit policy snapshots;
- sandbox/isolation profiles;
- resource quotas and lifetime controls;
- deterministic security decisions;
- cryptographic provider identity and algorithm policy;
- certificate pinning/trust policy hooks;
- tamper-evident observability;
- reproducible execution metadata;
- stronger runtime selection validation.

## Runtime Selection

Use `config/slvm-runtime.conf` to select VM generation and runtime handling. The configuration selects policy and adapter behavior; it never grants capabilities.

## C and C++

The C interface remains the stable ABI. C++ may implement higher-level policy, verification, orchestration, and platform integration without bypassing the C security boundary.

Copyright (c) Max Rupplin - MEARVK LLC - 2026
# SLeeLa Virtual Machine 2

SLVM/2 is the second VM generation and the security-oriented evolution of SLVM/1.

## Base Specification

SLVM/2 starts from the execution contract established by `../1`: SLeeLa source and compiler output remain authoritative, SLVM remains the execution engine, and capability/runtime adapters remain the OS boundary. SLVM/2 does not introduce a second SLeeLa language.

## Obvious Improvements over SLVM/1

- explicit runtime-selection configuration;
- stronger security policy separation from execution;
- cryptographic service abstraction using vetted platform/provider implementations;
- signed module/link manifests and explicit link policy;
- structured observability and audit events;
- certificate/attestation hooks;
- government/compliance profiles as policy bundles, never as hard-coded political rules;
- versioned VM identity and compatibility metadata;
- fail-closed handling for security-sensitive linking, credentials, and certificate validation.

## Runtime Selection

The runtime can be selected by a user or deployment through `config/slvm-runtime.conf`. Environment and command-line overrides may be supported by the eventual CLI, but a lower-trust source must not silently override a stronger deployment policy.

Example:

```
vm.instance = 2
runtime.target = auto
runtime.adapter = auto
security.profile = secure-default
crypto.provider = platform
link.policy = signed
observer.mode = audit
certificate.policy = required
compliance.profile = baseline
```

The configuration selects runtime handling; it does not grant capabilities. Capability authority remains explicit.

## Security Model

SLVM/2 preserves the /1 memory, security, I/O heuristic, capability, broker, and observer layers while adding policy boundaries around linking, cryptography, certificates, and audit.

## C and C++

The stable VM ABI remains C. C++ may provide higher-level policy, linking, certificate, and orchestration facilities, but must not bypass the C security/capability boundary.

Copyright (c) Max Rupplin - MEARVK LLC - 2026

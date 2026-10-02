# VM Tutorial 05 — Security Management

## Simple

`SleelaVMSecurityManagementSimple` establishes policy, capabilities, isolation, and audit.

## Managed

`SleelaVMSecurityManagementManaged` adds cryptographic identity, certificates, replay protection, and resolver-aware decisions.

## Advanced

`SleelaVMSecurityManagementAdvanced` adds attestation, scoped capability delegation and revocation, provenance continuity, and recovery.

The intended lowering is:

`.sleela SM class -> compiler security plan -> C/C++ security module -> SLVM/SLJVM executable component`.

No security class bypasses the capability model. Unsupported security requirements are compile-time fitment failures rather than silent downgrades.

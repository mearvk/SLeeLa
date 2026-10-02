<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa VM Source Classes

The `/lib/vm` model defines SLeeLa source-level classes for constructing SLVM and SLJVM pieces.

## Memory and Security Management

MM and SM are explicit source-level architectures with three progressive forms each:

| Family | Simple / Complete | Managed / Secure | Advanced / Enterprise |
|---|---|---|---|
| Memory Management | bounded heap/stack/object budget, cleanup, zeroization | GC, guards, quarantine, scrubbing | reservation, integrity, checkpoint, migration, sealing/encryption policy |
| Security Management | policy, capabilities, isolation, audit | crypto identity, certificates, replay, resolver-aware decisions | attestation, delegation/revocation, provenance, recovery |

These classes lower through the same authoritative compiler into C/C++ VM modules and then SLVM/SLJVM executable parts.

## Fitment rules

1. Required options that cannot fit target architecture or physical limits are rejected.
2. Optional features may be disabled only when source marks them optional.
3. Capabilities never silently grant OS authority.
4. MM checkpointing requires integrity; MM migration requires checkpointing.
5. SM replay protection and certificates require cryptographic support; attestation requires certificates.
6. The compiler records MM/SM plans, selected options, resource plan, ABI, and module set.

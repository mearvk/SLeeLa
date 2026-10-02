# VM Memory Management and Security Management Architecture

SLeeLa models Memory Management (MM) and Security Management (SM) as explicit source classes. They are compiler inputs, not an alternate runtime language.

## MM progression
1. **Simple / Complete** — bounded heap and stack, object limit, explicit release and zeroization.
2. **Managed / Secure** — GC plus guard, quarantine, and scrub controls.
3. **Advanced / Enterprise** — reservation budgets, integrity-aware checkpointing, migration, sealing, and encryption policy.

## SM progression
1. **Simple / Complete** — policy, capabilities, isolation, audit.
2. **Managed / Secure** — cryptographic identity, certificates, replay protection, resolver-aware decisions.
3. **Advanced / Enterprise** — attestation, scoped capability delegation/revocation, provenance, recovery.

All forms lower through the authoritative compiler into C/C++ VM modules and then SLVM/SLJVM executable parts. Security remains behind explicit capability checks.
# VM Memory Management and Security Management Architecture

SLeeLa now models Memory Management (MM) and Security Management (SM) as explicit source classes. They are compiler inputs, not an alternate runtime language.

## MM progression

1. **Simple / Complete** — bounded heap and stack, object limit, alignment/page discipline, explicit release and zeroization.
2. **Managed / Secure** — garbage collection plus guard/quarantine/scrub controls for ordinary managed execution.
3. **Advanced / Enterprise** — reservation budgets, integrity-aware checkpointing, migration, sealing, memory encryption policy, and verified restore.

The C ABI is in `include/sleela_vm_management.h`; C++ orchestration is in `include/sleela_vm_management.hpp`. The compiler can select only the modules supported by the target's physical limits and declared capabilities.

## SM progression

1. **Simple / Complete** — policy, capabilities, isolation, and audit.
2. **Managed / Secure** — adds cryptographic identity, certificates, replay protection, and resolver-aware policy.
3. **Advanced / Enterprise** — adds attestation, capability delegation/revocation, provenance, and recovery.

Security features never silently grant authority. A resolver, certificate, or cryptographic module remains behind the VM capability boundary.

## Classic secure principles

- Least privilege.
- Explicit resource ceilings.
- Zeroization of released sensitive memory.
- W^X-style separation where the target supports it.
- Immutable/sealed regions for security-critical state.
- Capability checks before privileged operations.
- Authentication before trust-dependent operations.
- Replay/freshness checks for distributed state.
- Audit evidence for delegation and recovery.
- Physical limits and hardware features are treated as constraints, not assumptions.

Modern systems also demonstrate the value of memory sealing and hardware-backed confidential-memory mechanisms; SLeeLa keeps those as target capabilities rather than pretending every host supplies them. citeturn0search1turn0search2

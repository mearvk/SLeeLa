# Linking Manager Architecture

The SLeeLa Linking Manager provides a controlled local-terminal observation link to a known SLVM version. It is an observation boundary, not an execution bypass: the terminal must identify the target VM version, pass linking-plan validation, and request only observations permitted by that plan.

## Editions
1. Basic — known-version link plus bounded observation of memory, certificates, and transaction records.
2. Moderate — adds resolver state and audit records.
3. Advanced — adds attestation, capabilities, provenance, and checkpoint observations.
4. Government — adds immutable audit, dual-control, least-privilege, and retention controls.
5. Military — adds tamper evidence, isolated-link policy, mission partitioning, and emergency certificate revocation controls.

Government and Military are module policy profiles, not certification claims.

## Security boundary
The manager never grants the terminal authority to mutate VM memory, certificates, transaction records, capabilities, or execution state merely because an observation link exists. Observation records are capability-scoped and auditable. Known-version matching is exact on major and minor version in the current ABI.

## Lowering
SLeeLa Linking Manager source -> compiler linking plan -> C stable ABI -> C++ orchestration -> SLVM/SLJVM observation module.

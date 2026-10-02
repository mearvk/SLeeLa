# Tutorial 06: Linking Manager

A Linking Manager class describes a local-terminal observation connection to a specific SLVM version.

## Basic
Use Basic for bounded runtime observation: memory state, certificate metadata, and transaction records. The link must match the requested VM major/minor version.

## Moderate
Add resolver and audit observation when runtime diagnostics need name/address resolution context and audit history.

## Advanced
Add attestation, capability, provenance, and checkpoint evidence for higher-assurance runtime inspection.

## Government and Military
Government adds immutable audit, dual control, least privilege, and retention controls. Military adds tamper evidence, isolated-link policy, mission partitioning, and emergency revocation. These are implementation profiles and are not certification claims.

## C/C++ path
The C ABI validates the plan and checks version and observation permissions. The C++ header provides the orchestration wrapper. Both remain below the authoritative SLeeLa compiler and VM capability/security boundary.

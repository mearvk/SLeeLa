# SLVM/7 Migration

Migration retains the SLVM/6 validation chain and adds manager-set compatibility.

Required checks include:

- checkpoint integrity and replay safety
- artifact and manifest identity
- policy compatibility
- ABI/runtime compatibility
- capability compatibility
- certificate and attestation evidence
- resolver provenance continuity
- resource quotas
- manager identities and dependency state
- recovery state

After transfer, the destination runtime revalidates the complete state before execution resumes. Native OS handles are re-established through the destination capability broker rather than copied implicitly.

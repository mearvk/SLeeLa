# SLVM/2 Security

SLVM/2 extends /1 with explicit security controls for identity, linking, cryptography, certificates, and audit.

1. Capabilities remain the authority boundary.
2. Memory/security/I/O policy runs before sensitive operations.
3. Credentials and private key material are references or protected handles, not ordinary observer values.
4. Secret observer fields are redacted by default.
5. Link manifests are validated before code becomes executable.
6. Certificate chains are validated before trust is established.
7. Security failures fail closed for protected operations.
8. Cryptography is provider-backed; SLVM/2 does not invent cryptographic primitives.
9. Replay, freshness, and identity checks belong to authenticated protocol layers.
10. Compliance profiles can strengthen policy but cannot grant capabilities.

The design is defensive infrastructure. It does not claim to classify malware or determine political legitimacy.

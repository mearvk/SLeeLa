# SLVM/2 Security

SLVM/2 carries forward /1 memory security, runtime security, I/O heuristics, capabilities, broker security, and observer controls.

New /2 controls:
- fail-closed protected linking;
- signed module manifests;
- provider-backed cryptography;
- certificate/trust-store policy;
- runtime identity and attestation;
- security/audit observer events;
- configurable compliance policy.

Private keys, passwords, tokens, and other credentials are references/protected handles, not ordinary observer values. Observer output redacts secrets by default.

The VM must never silently turn a failed certificate, signature, capability, or security-policy decision into an allow decision.

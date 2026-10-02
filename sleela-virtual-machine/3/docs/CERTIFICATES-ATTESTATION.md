# SLVM/3 Certificates and Attestation

SLVM/3 binds trust decisions to execution identity.

Attestation metadata should include:
- VM generation and build identity;
- artifact/content hash;
- manifest hash;
- runtime adapter;
- cryptographic provider identity;
- certificate policy;
- isolation policy;
- capability policy summary;
- observer mode;
- compliance profile.

Private keys, passwords, tokens, and secret payloads are excluded.

Certificate pinning is policy-controlled. Trust failures must be visible to the observer and must fail closed when required by configuration.

Attestation is evidence about execution state. It is not, by itself, legal or governmental certification.

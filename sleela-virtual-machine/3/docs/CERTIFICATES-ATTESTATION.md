# SLVM/3 Certificates and Attestation

Attestation metadata should include VM generation/build identity, artifact/content hash, manifest hash, runtime adapter, cryptographic provider identity, certificate policy, isolation policy, capability-policy summary, observer mode, and compliance profile.

Private keys, passwords, tokens, and secret payloads are excluded.

Certificate pinning is policy-controlled. Required trust failures are visible to observers and fail closed.

Attestation is evidence about execution state; it is not itself legal or governmental certification.

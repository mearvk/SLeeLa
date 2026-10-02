# Certificates, Attestation, and Compliance

SLVM/2 treats certificates and compliance as explicit technical policy.

## Certificates

The certificate layer may authenticate:

- VM/runtime identity;
- signed module identity;
- broker peers;
- deployment trust roots;
- attestation signing keys.

Trust stores are selected by deployment policy. A certificate is not itself permission to access an OS capability.

## Attestation

An attestation report should identify:

- VM generation and build identity;
- loaded module hashes;
- enabled security policy;
- runtime adapter;
- crypto provider identity;
- certificate policy;
- observer/audit mode.

Sensitive credentials and private keys must never be emitted into an attestation report.

## Compliance Profiles

A compliance profile is a machine-readable policy set. Examples include:

- baseline;
- enterprise;
- regulated-environment;
- government-deployment.

These names describe deployment policy, not a claim that a VM is legally certified. Actual certification requires the applicable authority, assessment process, evidence, and jurisdiction-specific requirements.

Profiles can require stronger cryptography, signed modules, certificate validation, audit events, retention controls, and restricted runtime adapters.

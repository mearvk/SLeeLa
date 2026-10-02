# SLVM/2 Architecture

SLVM/2 is the security-oriented evolution of SLVM/1. It preserves the authoritative SLeeLa compiler and Output Symbol contract.

SLeeLa source -> authoritative compiler -> Output Symbols/Core -> SLVM/2 -> capability broker -> OS runtime adapter.

SLVM/2 adds explicit runtime selection, security policy composition, provider-backed cryptography, signed linking, expanded observability, certificates/attestation, and declarative compliance profiles.

Runtime selection chooses execution handling; it never grants capabilities. Capabilities remain the authority boundary.

Cryptography is provider-backed and must use established/vetted implementations rather than VM-specific cryptographic inventions.

Linking validates module identity, version, hashes, signatures, ABI requirements, and requested capabilities before executable modules are accepted.

Observer events include VM, security, crypto, link, certificate, and runtime events. Secret values are redacted by default.

Certificates authenticate identities and trust relationships. Attestation records the runtime/module/policy state without exposing credentials.

Compliance profiles are technical policy bundles for deployment requirements. A profile may strengthen controls but cannot itself grant authority or imply legal certification.

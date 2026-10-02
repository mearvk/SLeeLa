# SLVM/2 Architecture

SLVM/2 is an incremental VM generation built from the SLVM/1 specification.

```
SLeeLa source
   |
authoritative SLeeLa compiler
   |
SLeeLa Output Symbols / Core representation
   |
SLVM/2 execution
   |
+-- memory security + GC
+-- runtime security
+-- I/O heuristic
+-- capability broker
+-- crypto provider
+-- signed linker
+-- observer/audit
+-- certificate/attestation
   |
OS-specific runtime adapter
   |
Linux / Windows / macOS
```

### Runtime Selection

The runtime-selection configuration identifies the VM instance and preferred runtime adapter. Selection is policy input, not privilege.

### Security

Every security-sensitive boundary must validate policy before execution. Security state must be observable without exposing secret values.

### Cryptography

SLVM/2 exposes cryptographic operations through a provider abstraction. It must not implement new cryptographic algorithms merely for convenience. Production providers should delegate to vetted platform or established cryptographic libraries.

### Linking

Modules are linked through explicit manifests. The linker records module identity, version, hashes, requested capabilities, ABI requirements, and signatures. Signed-link policy can reject unsigned or incompatible modules before execution.

### Observability

Observer events cover VM lifecycle, function entry/return, parameters, object activity, memory, I/O, broker activity, linking, certificates, and security decisions. Secret values are redacted by default.

### Certificates and Attestation

Certificates authenticate module/runtime identities and trust relationships. Attestation reports describe what was loaded and which policy was applied. Certificate validation must occur before a trust decision is accepted.

### Government / Compliance

Compliance profiles are declarative policy bundles for organizational, contractual, regulatory, or government deployment requirements. The VM engine remains politically neutral: profiles describe technical controls such as approved algorithms, audit retention, certificate authorities, logging requirements, and module-signing requirements.

### Compatibility

SLVM/2 should reject an artifact when its required execution contract cannot be satisfied. It must not silently reinterpret an unknown Output Symbol.

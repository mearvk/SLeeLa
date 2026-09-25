# SLeeLa Version

## Current Development Version

**Version:** 0.1.0-dev  
**Edition:** SLeeLa Complete / Native Foundation  
**Status:** Active Development  
**Repository:** mearkv/SLeeLa

## Versioning Policy

SLeeLa uses semantic versioning:

**MAJOR.MINOR.PATCH**

- **MAJOR** — incompatible language, ABI, API, runtime, or artifact changes.
- **MINOR** — backward-compatible capabilities, modules, APIs, classes, or platform support.
- **PATCH** — backward-compatible fixes, corrections, hardening, documentation, and test improvements.
- Development releases use the `-dev` suffix until the corresponding release gate is satisfied.

## Current 0.1 Development Scope

The current development line contains foundations for:

- SLeeLa-Complete application authoring
- Nordshrift Complete
- Native memory management
- Native reflection
- Native runtime primitives
- Native networking
- Native security primitives
- HTTP and server foundations
- Telephony/Skya and VoIP foundations
- Cross-platform implementation contracts
- C/C++ native integration
- Build, test, packaging, and verification infrastructure

## Native Foundation Status

### Implemented Foundations

- Memory manager with guarded allocation and accounting
- Runtime EventLoop
- Runtime WorkQueue
- CancellationToken
- FutureResult
- TCP/UDP socket abstraction
- DNS-backed endpoint resolution
- IPv4/IPv6 family abstraction
- SHA-256
- Secure memory zeroization
- Constant-time byte comparison
- Credential secret cleanup
- Native smoke-test integration

### Deliberately Not Claimed Complete

The version number does **not** mean the entire SLeeLa platform is release-complete.

The following remain development gates:

- TLS implementation and certificate validation
- Secure random/key management
- Windows socket backend
- macOS socket backend
- Full nonblocking network integration
- Timeout/deadline and retry policies
- Connection pooling
- Production cryptographic backend policy
- Full VoIP media/signaling implementation
- Complete HTTP implementation and interoperability testing
- Hardware-driver certification
- Cross-platform release testing
- Comprehensive integration and conformance suites
- Reproducible release packaging and signing
- Final production release audit

## Release Rule

A version becomes a production release only after the corresponding source, build, runtime, platform, security, testing, packaging, and verification gates are evidenced.

Documentation describing a capability is not itself evidence that the capability is production-complete.

## Branch Policy

The primary development branches are:

- `main`
- `master`

For synchronized development milestones, version documentation and release-critical source should be kept consistent across both branches.

## Version History

### 0.1.0-dev

Initial consolidated development version covering the SLeeLa-Complete architecture and native foundation work.

The development line is intentionally pre-release and subject to API, ABI, source, build-system, and architectural changes.

---

**SLeeLa — MEARVK LLC — 2026**

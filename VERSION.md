# SLeeLa Version

## Current Development Version

**SLeeLa:** 0.2.0-dev  
**Sleela-Complete:** 0.2.0-dev  
**Nordshrift Complete:** 0.2.0-dev  
**Native Foundation:** 0.2.0-dev  
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

## 0.2 Development Line

The 0.2 development line records the transition from the initial consolidated 0.1 native-foundation line into an explicit application build/lifecycle and engineering-verification line.

This includes:

- SLeeLa-Complete application authoring
- Nordshrift Complete
- native runtime foundations
- native memory and reflection foundations
- networking and security foundations
- HTTP/server foundations
- Telephony/Skya and VoIP foundations
- cross-platform implementation contracts
- C/C++ native integration
- application-level build lifecycle tooling
- build provenance and artifact hashing
- dependency-lock/provenance foundation
- numerical top-down completion tracking

## Current Implemented Foundations

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
- Application-level build lifecycle driver (`tools/sleela-build.py`)
- `init/check/build/test/run/package/install/clean/doctor/version` lifecycle commands
- Build provenance recording
- SHA-256 artifact recording
- Initial build-tool lock/provenance format

## Deliberately Not Claimed Complete

The 0.2.0-dev version does **not** mean the SLeeLa platform is release-complete.

Remaining gates include:

- exact third-party dependency resolution and locking
- deterministic and reproducible builds
- complete SST → executable compiler path
- full runtime lifecycle verification
- TLS/certificate/key/random/security completion
- complete HTTP interoperability
- full VoIP media/signaling
- hardware-driver certification
- Windows 10+ and macOS production verification
- comprehensive integration/conformance suites
- fuzzing, sanitizers, stress and performance evidence
- reproducible release packaging and signing
- final production release audit

## Release Rule

A version becomes a production release only after the corresponding source, build, runtime, platform, security, testing, packaging, and verification gates are evidenced.

Documentation describing a capability is not itself evidence that the capability is production-complete.

## Branch Policy

The primary development branches are:

- `main`
- `master`

Version documentation and release-critical source should remain consistent across both branches.

## Version History

### 0.2.0-dev

Current development line. Establishes the build/lifecycle and verification phase following the initial 0.1 native-foundation consolidation.

### 0.1.0-dev

Initial consolidated development version covering the SLeeLa-Complete architecture and native foundation work.

---

**SLeeLa — MEARVK LLC — 2026**

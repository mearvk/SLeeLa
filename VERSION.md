# SLeeLa Version

## Current Development Version

**SLeeLa:** 0.3.0-dev  
**Sleela-Complete:** 0.3.0-dev  
**Nordshrift Complete:** 2.7-dev  
**Native Foundation:** 0.3.0-dev  
**Sleela Language Syntax:** 1.3 (supported range 1.3 .. 1.3)
**Compiler Compatibility Gate:** 2.7-dev  
**Edition:** SLeeLa Complete / Native Foundation  
**Standard Library Collection:** 0.7-dev — 74 packages / 953 source units / 1,041 symbol records  
**Status:** Active Development  
**Repository:** mearkv/SLeeLa

## Complete Version Registry

The repository contains multiple independently versioned layers. This registry distinguishes the active SLeeLa development line from stable specifications and separately released subprojects.

| Component | Current version | Status / scope |
|---|---|---|
| SLeeLa | **0.3.0-dev** | Active platform development |
| Sleela-Complete | **0.3.0-dev** | Active application/complete edition |
| Native Foundation | **0.3.0-dev** | Active C/C++ runtime foundation |
| Sleelvac compiler/toolchain | **0.3.0-dev** | Active compiler implementation |
| Sleela language syntax | **1.3** | Supported range **1.3 .. 1.3** |
| Nordshrift Complete | **2.7-dev** | Active SST compiler/transpiler development |
| Compiler Compatibility Gate | **2.7-dev** | Library-aware compiler/loader inventory gate |
| NS-SST-0001 | **1.0.0** | Normative SST specification |
| SL-META-0001 | **1.0.0** | Pre-Normative language metadocument |
| Sleela VM ABI | **1.0** | Runtime artifact ABI major/minor |
| Sleela artifact format | **2** | Persistent .sleela artifact format |
| SleelaTerminal | **1.0.0** | Independent released terminal component |
| API Server | **1.0.1** | Independent server component |
| Server Edition | **1.0.0** | Independent server-edition line |
| Server Edition Moral/2 | **2.0.1** | Independent server edition |
| Server Edition Moral/3 | **3.0.1** | Independent server edition |
| Port Awareness | **1.0.0** | Independent server component |

Independent component versions are not automatically bumped when the SLeeLa compiler or Nordshrift development line advances. Their own VERSION files or specification sources remain authoritative.

## Versioning Policy

SLeeLa uses semantic versioning:

**MAJOR.MINOR.PATCH**

- **MAJOR** — incompatible language, ABI, API, runtime, or artifact changes.
- **MINOR** — backward-compatible capabilities, modules, APIs, classes, or platform support.
- **PATCH** — backward-compatible fixes, corrections, hardening, documentation, and test improvements.
- Development releases use the `-dev` suffix until the corresponding release gate is satisfied.

## 0.3 Development Line

The 0.3 SLeeLa development line records the transition from the initial consolidated 0.1 native-foundation line into an explicit application build/lifecycle and engineering-verification line.

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
- Nordshrift source-validation gate and negative compiler evidence
- Nordshrift shared target-neutral lowering IR and emitter gate
- Nordshrift 2.3-dev target-neutral type/semantic analysis and negative evidence
- Nordshrift 2.4-dev end-to-end SST compilation and runnable artifact execution proof
- Nordshrift 2.5-dev runtime artifact ABI validation gate — executable validation and rejection test
- Nordshrift 2.6-dev cross-version compiler compatibility gate
- Planned next gate: Nordshrift 2.7-dev compiler fuzzing and malformed-input gate
- Planned next gate: Nordshrift 2.8-dev deterministic compiler-output gate
- Sleela language syntax 1.3 support and compiler range synchronization

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

The 0.3.0-dev version does **not** mean the SLeeLa platform is release-complete.

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

### 0.3.0-dev

Current development line. Extends the build/lifecycle and verification phase through Nordshrift 2.6 cross-version compiler compatibility evidence.

Completed Nordshrift verification milestones currently represented by executable/documented evidence:

- 2.1-dev source validation
- 2.2-dev target-neutral lowering
- 2.3-dev semantic analysis
- 2.4-dev end-to-end SST compilation and runnable artifact execution
- 2.5-dev runtime artifact ABI validation
- 2.6-dev cross-version compiler compatibility

The next planned compiler verification milestones are 2.7-dev malformed-input/fuzzing coverage and 2.8-dev deterministic compiler-output verification. They are roadmap items, not completed release gates.

### 0.1.0-dev

Initial consolidated development version covering the SLeeLa-Complete architecture and native foundation work.

---

**SLeeLa — MEARVK LLC — 2026**


## 2.7 Development Gate

Nordshrift 2.7-dev adds the canonical `/lib` package and symbol inventory to the compiler/Nordshrift/loader verification surface. The current collection is 74 packages, 953 SLeeLa source units, 88 module-facade symbols, and 1,041 total symbol records. The shared library index now exposes per-package counts and symbol-to-source resolution.

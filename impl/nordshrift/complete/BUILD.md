# Nordshrift Complete Build Contract

Required pipeline:

1. Discover the .sst control sheet.
2. Parse and validate the SST model.
3. Resolve canonical Sleela-Complete class bindings.
4. Resolve source imports and platform capabilities.
5. Lower to the shared Sleela source/core-artifact path.
6. Compile native/C/C++ implementation dependencies.
7. Link the selected runtime and adapters.
8. Run unit and integration tests.
9. Produce a package manifest containing target, dependencies and digests.
10. Sign and verify the package according to repository policy.

Nordshrift Complete must not introduce a parallel Sleela compiler.

Intended command surface: sleela init, check, doctor, build, test, run, package, install and version.

Build manifests should record source revision, target triple, compiler/toolchain identity, runtime revision, native libraries, platform adapters, generated artifacts, cryptographic digests and test results.

A build must fail closed when required source, class binding, native dependency, security material or platform adapter is absent. Optional capabilities may be explicitly disabled in the manifest.

**Max Rupplin — MEARVK LLC — 2026**

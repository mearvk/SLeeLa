# SLeeLa Daily Engineering TODO — Top-Down Numerical Drilldown

**Project:** SLeeLa  
**Version line:** 0.3.0-dev  
**Method:** Top-down numerical engineering closure  
**Rule:** A requirement advances only when implementation is followed by executable evidence.

## 0. Daily Control Loop
- [ ] Synchronize `main` and `master`.
- [ ] Confirm clean/known working tree state.
- [ ] Confirm current version.
- [ ] Select one primary drilldown target.
- [ ] Verify source → build → runtime → platform → security → test → package → verification.
- [ ] Record failures honestly.
- [ ] Update the relevant audit row only when evidence exists.
- [ ] Commit and synchronize both branches.

# 1. Build System

## 1.1 Lifecycle
- [x] Native Makefile exists.
- [x] Build lifecycle documentation exists.
- [x] Lifecycle driver: `tools/sleela-build.py`.
- [x] `init/check/build/test/run/package/install/clean/doctor/version` entry points.
- [x] Build provenance record.
- [x] Artifact SHA-256 recording.
- [x] Toolchain version recording.
- [ ] Execute and verify every lifecycle command on supported hosts.
- [ ] Integrate lifecycle driver into CI.

## 1.2 Dependencies
- [x] Define `sleela.lock.json` generation.
- [ ] Record exact resolved dependency versions.
- [ ] Reject unexpected dependency changes.
- [ ] Add dependency integrity verification.
- [ ] Add dependency update procedure.

## 1.3 Incremental / Reproducible Builds
- [ ] Implement incremental build policy.
- [ ] Implement deterministic build inputs.
- [ ] Record target triple.
- [ ] Record source revision.
- [ ] Record environment affecting output.
- [ ] Compare two clean builds.
- [ ] Document reproducibility result.

## 1.4 Diagnostics
- [x] `doctor` diagnostic report.
- [x] Build provenance report.
- [ ] Add compiler/linker diagnostics capture.
- [ ] Add dependency diagnostics.
- [ ] Add failed-build diagnostic bundle.
- [ ] Add CI artifact retention.

# 2. Compiler / Nordshrift
- [x] SST semantic validation at sheet/source-check boundary.
- [x] Target-neutral type/semantic analysis gate.
- [x] Negative semantic-analysis test.
- [x] End-to-end SST → Sleela artifact → runtime execution proof.
- [x] Shared lowering / target-neutral IR gate.
- [x] SST source → shared Sleela lexer/parser validation during `nordshrift check`.
- [x] Negative source-validation test.
- [ ] End-to-end SST compilation.
- [ ] Native linking.
- [ ] Runtime loading.
- [x] Version compatibility tests.
- [x] Runtime artifact ABI compatibility validation.
- [ ] Negative compiler tests.
- [x] Invalid-source diagnostics.
- [x] Cross-version fixtures.
- [ ] SST → executable → execution proof.

# 3. Runtime
- [ ] Event loop.
- [ ] Work queue.
- [ ] Future/promise.
- [ ] Cancellation.
- [ ] Structured shutdown.
- [ ] Crash handling.
- [ ] Runtime dependency management.
- [ ] Runtime diagnostics.
- [ ] Drain.
- [ ] Failure transitions.
- [ ] Graceful shutdown.
- [ ] Resource cleanup.

# 4. Memory Manager
- [ ] Arenas/domains.
- [ ] Snapshots.
- [ ] Leak reporting.
- [ ] Quarantine.
- [ ] Guard pages.
- [ ] Zeroization.
- [ ] Telemetry callbacks.
- [ ] Concurrent-destroy contract.
- [ ] Production test suite.

# 5. Reflection
- [ ] Enum values.
- [ ] Generated registration.
- [ ] Serialization adapters.
- [ ] Safe invocation adapters.
- [ ] ABI compatibility validation.
- [ ] Snapshot API.
- [ ] Runtime integration.

# 6. Networking
- [ ] Native socket backend evidence.
- [ ] IPv4/IPv6 evidence.
- [ ] Connection lifecycle.
- [ ] Timeouts/deadlines.
- [ ] Retry policy.
- [ ] Back-pressure.
- [ ] MTU/path handling.
- [ ] TLS integration.
- [ ] Streaming/multipart.
- [ ] Header/body limits.
- [ ] Connection pooling.
- [ ] Malformed-input tests.
- [ ] Linux verification.
- [ ] Windows 10+ verification.
- [ ] macOS verification.

# 7. HTTP
- [ ] HTTP/1.x tests.
- [ ] HTTP/2 tests.
- [ ] HTTP/3/QUIC tests.
- [ ] HTTP/4 experimental architecture tests.
- [ ] Streaming.
- [ ] Multipart.
- [ ] Header limits.
- [ ] Body limits.
- [ ] Connection pooling.
- [ ] Interoperability suite.
- [ ] Malformed-input suite.
- [ ] Security suite.

# 8. Security
- [ ] TLS implementation.
- [ ] Certificate management.
- [ ] Key management.
- [ ] Secure random.
- [ ] Secret storage.
- [ ] Memory zeroization.
- [ ] Package signature verification.
- [ ] Dependency integrity.
- [ ] Security policy.
- [ ] Fail-closed verification.
- [ ] Security regression suite.

# 9. VoIP / Skya
- [ ] SIP transactions.
- [ ] SIP dialogs.
- [ ] Registration/authentication.
- [ ] SIP transport.
- [ ] SDP offer/answer.
- [ ] ICE/STUN/TURN.
- [ ] RTP timing/jitter.
- [ ] SRTP/SRTCP.
- [ ] Codec implementations.
- [ ] Echo/AGC/noise suppression.
- [ ] Device enumeration.
- [ ] Call state machine.
- [ ] Recovery/reconnection.
- [ ] End-to-end VoIP proof.

# 10. Drivers / Platforms
For Linux, Windows 10+, and macOS:
- [ ] Native build.
- [ ] Runtime.
- [ ] Networking.
- [ ] Security.
- [ ] Driver adapters.
- [ ] UI/media.
- [ ] Device lifecycle.
- [ ] Hot-plug recovery.
- [ ] Hardware validation.
- [ ] Integration tests.
- [ ] Packaging.

# 11. Database / Email / XML
## 11.1 Database
- [ ] Implementation.
- [ ] Security.
- [ ] Connection lifecycle.
- [ ] Queries.
- [ ] Transactions.
- [ ] Failure tests.
- [ ] Integration tests.

## 11.2 Email
- [ ] SMTP implementation.
- [ ] Authentication.
- [ ] TLS.
- [ ] Message construction.
- [ ] Attachments.
- [ ] Failure handling.
- [ ] Integration tests.

## 11.3 XML/Data
- [ ] Parser.
- [ ] Serializer.
- [ ] Validation.
- [ ] Security.
- [ ] Malformed-input tests.
- [ ] Integration tests.

# 12. UI / Media / AI / Science
For every subsystem:
- [ ] Runtime implementation.
- [ ] Platform implementation.
- [ ] Unit tests.
- [ ] Failure tests.
- [ ] Integration test.
- [ ] End-to-end executable example.

# 13. Application Lifecycle
- [ ] Create.
- [ ] Configure.
- [ ] Initialize.
- [ ] Start.
- [ ] Run.
- [ ] Stop.
- [ ] Drain.
- [ ] Cancel.
- [ ] Destroy.
- [ ] Failure transitions.
- [ ] Resource cleanup.
- [ ] Graceful shutdown.

# 14. Observability
- [ ] Structured logging.
- [ ] Metrics.
- [ ] Tracing.
- [ ] Memory diagnostics.
- [ ] Network diagnostics.
- [ ] Driver diagnostics.
- [ ] Build diagnostics.
- [ ] Crash reports.
- [ ] Diagnostic bundle.

# 15. Packaging / Provenance
- [ ] Dependency manifests.
- [ ] Dependency locking.
- [ ] Artifact verification.
- [ ] Signing.
- [ ] Signature verification.
- [ ] Debug packages.
- [ ] Release packages.
- [ ] Linux package.
- [ ] Windows package.
- [ ] macOS package.
- [ ] SBOM.
- [ ] Upgrade.
- [ ] Rollback.
- [ ] Reproducible release.

# 16. Testing
- [ ] Compiler.
- [ ] Runtime.
- [ ] Networking.
- [ ] HTTP.
- [ ] Security.
- [ ] VoIP.
- [ ] Drivers.
- [ ] Database.
- [ ] Email.
- [ ] XML security.
- [ ] Cross-platform.
- [ ] Failure paths.
- [ ] Fuzz.
- [ ] Sanitizers.
- [ ] Performance.
- [ ] Stress.

# 17. End-to-End Applications
- [ ] Hello.
- [ ] CLI.
- [ ] HTTP server.
- [ ] Network service.
- [ ] Database service.
- [ ] Email service.
- [ ] Desktop.
- [ ] XML service.
- [ ] Scientific.
- [ ] Analytics.
- [ ] AI.
- [ ] VoIP.
- [ ] Driver.
- [ ] Full SST → executable proof.

# 18. Documentation / Operations
- [ ] Driver documentation.
- [ ] VoIP documentation.
- [ ] Security documentation.
- [ ] Deployment documentation.
- [ ] Operations guide.
- [ ] Disaster/recovery guide.
- [ ] Upgrade guide.
- [ ] Rollback guide.
- [ ] Diagnostic procedures.

# 19. Release Closure
- [ ] Required APIs implemented.
- [ ] Native implementations complete.
- [ ] Runtime verified.
- [ ] Linux verified.
- [ ] Windows 10+ verified.
- [ ] macOS verified.
- [ ] Security verified.
- [ ] Networking/HTTP verified.
- [ ] VoIP verified.
- [ ] Drivers verified.
- [ ] Memory/Reflection production tests pass.
- [ ] Database/Email/XML pass.
- [ ] UI/Media/AI/Science pass.
- [ ] End-to-end applications pass.
- [ ] Failure/stress/sanitizer/fuzz coverage passes where applicable.
- [ ] Cross-platform CI passes.
- [ ] Packages verify.
- [ ] SHA-256/signatures verify.
- [ ] Dependencies/provenance recorded.
- [ ] Install/upgrade/rollback verify.
- [ ] Documentation matches implementation.
- [ ] No required item remains ❌, 🟨, ⬜, or 🔒.

**Engineering rule:** implementation first, evidence second, status change third.

# Java converter branch ledger — 2026-11-19

Central record of every remote branch related to the Java to SLeeLa converter, compared against
`master` (`630121e5` when written). Ahead/behind counts come from the GitHub compare API.
"Ahead" counts commits, so squash-merged PRs still show as ahead; classification below is by file
content and merged PRs, not by commit count. No build or test was run to produce this ledger.

Source map: `.kiro/steering/java-converter-source-map.md`. Status: `JAVA.CONVERSION.STATUS.md`.

## A. Fully contained in master (ahead = 0). Safe to delete.

| Branch | Behind |
|---|---|
| work/java-converter-constructs-transactional | 473 |
| work/java-generic-overload-resolution | 520 |
| work/java-generic-project-resolution-next (PR #373) | 507 |
| work/jdk28-signature-index-generic-validation (PR #375) | 139 |
| work/regression-status-nov18-session-update (PR #377) | 126 |
| work/java-utf16-integration | 1788 |
| feature/java-host-capabilities, feature/java-jar-class-launch, fix/java-launch-manifest-verification | 3544-3576 |

## B. Converter work already landed on master by a different commit (squash or re-application)

| Branch | Landed via | Evidence on master |
|---|---|---|
| work/java-package-identity-import-resolution | PR #374 | `tests/reject/{AnnotationType,ModuleDeclaration,SwitchPattern}.java` present |
| work/java-lower-bound-inference-project-resolution | PR #372 content ported by PR #379 | `tests/reject/{LowerBoundWildcardConflict,BoundedClassTypeConflict}.java` present |
| work/java-qualified-type-fail-closed-2026-11-18 | PR #380 | merged 2026-11-19 |
| work/java-converter-mapping-pass-01 (2026-11-15) | earlier re-application | `CONVERTER-IDEMPOTENCE.md`, `CONVERTER-MAPPING-AUDIT.md`, `IDENTIFIER-COLLISIONS.md`, `tests/java/{IdentifierParity,OverloadWidening,ReservedIdentifiers}.java`, `tests/reject/AmbiguousWideningOverload.java` present |

Master is newer for the files these branches touch (`java_to_sleela.cpp`, `java_to_sleela.h`,
`conversion.model`, `tests/javaconv_differential_test.sh`). Do not merge these branches wholesale;
it would regress master. Not verified line by line that every hunk landed.

## C. Java string / UTF-16 VM line: integrated

Branches: work/java-string-{api-completion,source-utf16,utf16-storage,utf16-storage-master-sync,
utf16-vm,vm-integration,vm-integration-final,vm-integration-master-sync}.
Master has `impl/core/sleela_java_string.{c,h}` and UTF-16 handling in `impl/core/sleela_core.*`.
Treat as superseded. Not verified hunk by hunk.

## D. Open PR

| Branch | Content |
|---|---|
| work/nov18-production-validation (PR #378) | Workflow-only: one line added to each of java-convert.yml, jdk28-validation.yml, regression-set.yml, root-build-install.yml. 4 ahead, 122 behind. |

## E. Java library / interop workstream (not converter code)

### work/java-api-dependency-closure: already on master

Master moved `lib/java/tools/` to `lib/java/jdk-25/tools/` and `lib/java/implementations/` to
`lib/java/jdk-25/implementations/`. At the new paths, the branch's `java_api_dependency_graph.py`,
`java_api_member_dependencies.py`, `java_api_visibility_constructor_audit.py`,
`JavaObjectIdentity.sleela` and `tests/java_source_implementation_audit.py` are byte-identical on master;
`JavaStringValue.sleela`, `impl/core/sleela_utf16.c`, `tests/java_string_native_suite.py` and
`tests/Makefile` exist on master in a newer form. Nothing to port.

### work/java-interoperability-hardening: PORTED to master 2026-11-19

Ported per file against merge base `fe98a72b` (not merged):

- New: SLJIP v1 protocol (`lib/java/protocol/SLJIP-v1.{json,md}`), `lib/java/RUNTIME-INTEGRATION.md`,
  `impl/JAVA-INTEROP-LOWERING.md`, `runtime/java_interop_protocol.{c,h}`,
  `connector/java/.../SleelaJdkProtocol{Server,Fixture}.java`, and tests
  `test-suites/{java-interop-protocol-codec.sh,java-interop-protocol-endpoint.sh,java-interop-protocol.py,java-library-integrity.py,test-java-protocol-codec.c,test-java-protocol.py}`.
- Taken from the branch (master unchanged since base): `SleelaClassLoader.java`, `SleelaJdkBridge.java`,
  `SleelaJdkConformance.java`, `connector/CLASS_LOADER.md`, `lib/java/JDK28_API_SYNC.md`.
- Re-targeted to master layout: `jdk28_sync.py` and `jdk28_signature_sync.py` applied at
  `lib/java/jdk-25/tools/`; path references in `java-library-integrity.py` and `JDK28_API_SYNC.md` updated.
- Reconciled: `lib/java/validate.py` takes the branch's exact-count validator; `MANIFEST.json` keeps
  master's content, adds `RUNTIME-INTEGRATION.md` as a control file, and records the exact count 15,093;
  `lib/java/README.md` merged by hand; `connector/Makefile` auto-merged.
- Not taken: the branch linked `java_interop_protocol.o` into `sleela`/`sleelvac` and added an unused
  include to `impl/frontend/driver.cpp`. Nothing calls the codec, and master has since moved Java handling
  out of the driver, so master's driver and link lines are kept. The docs now say the codec is tested
  standalone and not yet linked. `impl/Makefile` `test-java-runtime` runs the two protocol tests.
- Bugs fixed while porting: `test-java-protocol.py` read stderr eagerly as an assert message, deadlocking
  every request (5 of 7 tests hung); `connector/Makefile` used `$tmp` instead of `$$tmp`;
  `SleelaJdkConformance.java` (also broken on master) called `add` on an `Object`.

Verified in this sandbox (JDK 25.0.5, gcc): `python3 lib/java/validate.py` PASS (15,093 exact);
`java-interop-protocol-codec.sh` PASS; `java-interop-protocol-endpoint.sh` 7/7 OK;
`java-library-integrity.py` 7/7 OK; `make -C connector protocol-test protocol-codec-test` PASS;
`SleelaJdkConformance` PASS (compiled directly; `make conformance` needs the Jakarta Servlet jar, which
the sandbox could not download). The full `impl` runtime build was not run.

Still pre-existing on master, not fixed: `SleelaHttpConnector.java` declares `maxResponseBytes` twice;
`tests/java_api_*_suite.py` and `MANIFEST.json` `validation` still point at `lib/java/tools/` paths that
moved to `lib/java/jdk-25/tools/`.

### fix/java-launcher-main-sync (171/3588)

Java launcher/runtime probe (`runtime/java_runtime_probe.c`, `runtime/JAVA-HOST-CAPABILITIES.md`,
`markdown/NATIVE.JAVA.md`); master already has `runtime/java_runtime_probe.c` and
`test-suites/java-runtime-{bridge,probe}.sh`, so likely superseded. Compare listing hit an API rate
limit; not fully inspected.

## F. Sleelactl native-math line

| Branch | Note |
|---|---|
| work/native-math-kernel-sleelactl (10/1760), pi-20000-sleelactl-native-math (11/1747) | Edit `bin/sleelactl`, `registry.conf`, `SLEELACTL.md`. Master registers a `math` subject (registry.conf line 79); whether these branches add more is not compared. Do not merge wholesale: master carries the 2.0 contract rules. |

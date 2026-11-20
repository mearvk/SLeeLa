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

## E. Genuinely not on master (separate Java library/interop workstream, not converter code)

Checked by file presence on master; none of the names below exist there.

| Branch (ahead/behind) | Unmerged content |
|---|---|
| work/java-interoperability-hardening (98/2096) | SLJIP-v1 protocol (`lib/java/protocol/SLJIP-v1.{json,md}`), `connector/java/.../SleelaJdkProtocol{Fixture,Server}.java`, `lib/java/tools/jdk28_signature_sync.py`, `impl/JAVA-INTEROP-LOWERING.md`, `lib/java/RUNTIME-INTEGRATION.md`, `test-suites/{test-java-protocol.py,test-java-protocol-codec.c,java-library-integrity.py,java-interop-protocol.py}` |
| work/java-api-dependency-closure (83/2096) | `lib/java/tools/java_api_{dependency_graph,member_dependencies,visibility_constructor_audit}.py`, `lib/java/implementations/java/lang/{JavaObjectIdentity,JavaStringValue}.sleela`, `tests/java_string_native*` |
| fix/java-launcher-main-sync (171/3588) | Java launcher/runtime probe (`runtime/java_runtime_probe.c`, `runtime/JAVA-HOST-CAPABILITIES.md`, `markdown/NATIVE.JAVA.md`); master already has `test-suites/java-runtime-{bridge,probe}.sh`, so likely partly superseded. Compare listing hit an API rate limit; not fully inspected. |

Porting rules if these are taken (see repo learnings): port per file set, not by merge. Both branches
are about 2000 commits behind and edit `lib/java/MANIFEST.json`, `README.md`, and `validate.py`, which
master has since changed. Any `lib/**` change needs `security/sha256-manifest.json`, `lib/LIBRARY.SYMBOLS.md`,
the inventory test counts, and `lib/LIBRARY.INDEX.md` regenerated.

## F. Sleelactl native-math line

| Branch | Note |
|---|---|
| work/native-math-kernel-sleelactl (10/1760), pi-20000-sleelactl-native-math (11/1747) | Edit `bin/sleelactl`, `registry.conf`, `SLEELACTL.md`. Master registers a `math` subject (registry.conf line 79); whether these branches add more is not compared. Do not merge wholesale: master carries the 2.0 contract rules. |

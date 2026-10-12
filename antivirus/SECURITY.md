# Sleelavirin Security and Release Checklist

## Security posture

Sleelavirin is a signature-matching and scanner-coordination component. A no-match result is not proof that a file is safe. The native SHA-256 index imported from compatible extracted ClamAV .hsb records is only a subset of ClamAV's coverage; it is not a CVD/CLD decoder or a replacement for ClamAV's own engine.

## Signature and code integrity

- Obtain ClamAV databases from trusted official or explicitly approved internal sources.
- Verify official ClamAV database authenticity using supported ClamAV tools before extracting/importing data. A locally calculated SHA-256 checksum detects later changes only when the expected digest is itself trusted.
- Treat extracted text, labels, source URLs, and source version fields as untrusted input until the source is verified.
- Import to a staged candidate; reject malformed or empty candidates; preserve the previous index if publication fails.
- Verify an index and its provenance manifest together:
  `python3 tools/sleelavirin_import_clamav.py --check signatures/native-sha256.txt --check-manifest signatures/native-sha256.txt.manifest.json`
- Keep scanner binaries, databases, configuration, reports, and quarantine storage writable only by the intended administrator/service identity.
- Protect release branches and require review for scanner, importer, build, CI, and remediation changes. Publish checksums and signed artifacts where available.

The manifest binds a recorded SHA-256 digest and signature count to exact index bytes. It is not a digital signature: an attacker able to replace both index and manifest can forge both. Store a trusted expected digest or sign release metadata when authenticity against a privileged attacker is required.

## Scan execution and filesystem safety

- Use least privilege. Do not run the scanner as root/Administrator unless required for a specific approved scan.
- The coordinator refuses symlinks by default, checks file metadata around scans, bounds concurrency, and supports file-size/depth/archive limits.
- Engine subprocesses have a 120-second per-file timeout by default. Set `--scan-timeout N`; a value of `0` disables the timeout and should be an explicit operational decision.
- Metadata checks reduce race risk but do not guarantee the engine read an immutable snapshot. For high-assurance scans, scan a filesystem snapshot or isolated copy.
- Quarantine is a response mechanism, not a perfect containment boundary. Use a directory outside all scan roots with restrictive permissions and do not execute quarantined content.
- Never silently treat unreadable, skipped, oversized, timed-out, or otherwise errored files as clean. Exit status 2 means error/incomplete coverage; status 1 means detection; status 0 means no detection and no known errors, not that content is safe.

## Adversarial and regression testing

Before a release, test at least:

- malformed, empty, truncated, and tampered signature indexes;
- manifest digest/count/schema mismatch;
- same-size file rewrites and file mutation during scan;
- missing and unreadable paths, traversal depth, symlink loops, and symlink targets;
- destination collisions and quarantine destinations inside scan roots;
- engine timeout, missing executable, malformed engine output, and nonzero error exit;
- archive bombs and deeply nested archives against configured engine limits;
- large file counts, large files, and interruption during index publication.

Tests must use harmless fixtures; do not commit live malware samples into the repository.

## CI and release gate

The antivirus workflow runs the build and unit tests on Linux, macOS, and Windows, plus Python compilation and Bandit static analysis on Ubuntu. A release candidate is not security-verified until these jobs pass on the exact commit being released and a maintainer has reviewed the results.

Release checklist:

1. Review the diff for unsafe process execution, path handling, privilege changes, and secret leakage.
2. Run `make -C antivirus clean test` on a supported Unix host; run the Windows workflow as well.
3. Run the manifest integrity check when a manifest is distributed with the index.
4. Confirm the source and provenance of the active signature index.
5. Verify report exit codes for clean, detected, and incomplete/error fixtures.
6. Confirm quarantine and recovery behavior on the target filesystem.
7. Record commit SHA, CI run, compiler/Python versions, signature source/version, and release artifact hashes.

## Current limitations

- The native engine is whole-file SHA-256 matching, not behavioral analysis or full archive inspection.
- The imported ClamAV subset does not carry all ClamAV signature families.
- Before/after metadata checks cannot eliminate all time-of-check/time-of-use races.
- A manifest checksum without an independently trusted expected value does not authenticate code or index origin.
- CI static analysis and unit tests reduce risk but are not a substitute for an independent security review or platform-specific penetration testing.


## ClamAV database acquisition and precious-file integrity

- Retrieve updates with the supported `freshclam` client; do not treat a direct HTTP download or a checksum published beside an untrusted file as sufficient authentication.
- The SLeeLa updater stages database files separately, requires FreshClam to exit successfully, and requires `clamscan` to load and scan using the staged database before publishing it.
- Every stored `.cvd`/`.cld` file receives a SHA-256 digest and byte length in `manifest.json`. The snapshot ID is derived from the canonical database-entry list. The `current.json` pointer is replaced atomically only after validation, and older snapshots remain available.
- Keep the cache out of version control and restrict write access to the updater account. For stronger rollback protection, store the expected snapshot ID or a signed manifest in a separate trusted location. A SHA-256 value is an integrity fingerprint, not a publisher signature; an attacker who can replace both a database and its manifest can recompute the hash.
- Run `python3 antivirus/tools/sleelavirin_update_clamav.py --check PATH_TO_SNAPSHOT` to verify the snapshot against its manifest. This checks local integrity; it does not replace upstream CVD signature verification.
- Native SHA-256 index imports are only compatible subsets of supported extracted `.hsb` records. Keep the full ClamAV database for the ClamAV engine; do not describe the native subset as equivalent coverage.


## Additional operational and adversarial controls

- **Fail-safe completion:** worker exceptions, engine errors, timeouts, inaccessible files, size/depth exclusions, and stop-after-detection are not clean results. Exit code 2 means error/incomplete; code 1 means a detection and may coexist with errors. Consume the structured `--report-json` summary rather than treating a single log line as the whole outcome.
- **Archive/resource limits:** keep a finite directory depth, maximum file size, ClamAV scan-size ceiling, recursion ceiling, bounded worker count, and per-file timeout. Use `--scan-timeout 0` only for deliberate exceptions. Do not describe a file skipped due to a limit as clean.
- **Quarantine and restore:** retain restrictive permissions and collision-safe writes; require hash verification and a safe destination when restoring. Keep quarantine outside scan roots, monitor disk capacity, and document retention and operator approval. Do not automatically execute restored files.
- **Signed artifacts:** `tools/sleelavirin_verify_signature.py` verifies detached SHA-256 signatures using OpenSSL. It is only meaningful when the public key is trusted independently. Establish key custody, signing access controls, revocation/rotation, and signed release manifests before relying on it in production. Never store private signing keys in the repository or CI logs.
- **Auditability:** preserve JSON scan reports and updater provenance with restricted access and retention. Protect reports because file paths may disclose sensitive information. Escape control characters when rendering untrusted paths to terminals or dashboards.
- **EICAR and adversarial tests:** use the harmless EICAR test pattern only to validate antivirus plumbing; never use live malware as a routine CI fixture. Test corrupted archives, deep nesting, decompression bombs, symlink and file-replacement races, disk-full conditions, interrupted updates, signature failure, worker exceptions, and missing databases.
- **Platform matrix:** run unit/static checks on supported operating systems; test quarantine semantics and permission handling on the actual target filesystem. A green Linux job alone does not certify Windows/macOS behavior.


## Source-tree and release-file SHA-256 inventory

Use `make -C antivirus integrity-manifest` to inventory the source tree, then verify with `make -C antivirus integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json`. The manifest includes ordinary source and documentation files as well as signature-index files, but excludes generated build/dist output and the mutable ClamAV cache, whose snapshots have their own validated manifests.

Platform release archives include an `INTEGRITY-MANIFEST.json`, a `SHA256SUMS` file, and a separate `.sha256` sidecar for the ZIP. CI verifies the package manifest before creating the archive. Keep an independently trusted copy of the archive digest or use a detached signature whose public key was obtained independently; hashes stored next to the files they describe do not provide publisher authentication.

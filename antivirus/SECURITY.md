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

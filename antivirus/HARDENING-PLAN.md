# Sleelavirin hardening plan

This document tracks the six hardening workstreams for the antivirus subsystem. It is a security checklist, not a claim that all controls are implemented or independently audited.

## 1. Authenticated release metadata

- Treat SHA-256 manifests as integrity checks only; they do not authenticate publisher identity.
- Verify detached signatures with a public key obtained through an independent trusted channel.
- Keep private keys out of the repository and ordinary CI logs.
- Maintain documented key custody, rotation, revocation, and overlap rules before relying on signed releases.
- Bind signatures to versioned release metadata that identifies the commit, artifact hashes, signature-index provenance, and schema version.
- Fail closed if a signature is missing, invalid, or verified against an untrusted key. Do not accept a checksum sidecar as a substitute for a signature.

## 2. Quarantine and restore safety

- Keep quarantine outside all scanned roots; restrict it to the scanner/service identity and administrators.
- Never auto-delete or execute quarantined content.
- Before quarantine, verify the source still matches the recorded scan metadata; refuse symlink sources and destination collisions.
- Verify the quarantined copy's SHA-256 before removing the source; on copy, fsync, or verification failure, preserve the source and report an error.
- A restore operation, when implemented, must require an explicit operator action, safe destination, collision refusal, and hash verification after writing. It must not execute the restored file.
- Test disk-full, permission denial, interrupted copy, source replacement, and recovery paths on each supported filesystem.

## 3. Adversarial regression suite

Use only synthetic or harmless fixtures. Cover malformed/empty/truncated signature indexes, invalid manifest schemas and duplicate paths, path traversal, symlink loops, unreadable roots, file replacement during scan, timeouts, malformed engine output, worker exceptions, quarantine collisions, interrupted update publication, disk-full behavior, and control-character paths. Test archive/decompression limits using generated benign archives rather than malware samples.

## 4. SLeeLa execution policy

- `NO_MATCH` is not a clean bill of health; it means only that the selected engine found no match.
- A missing engine or signature database, invalid signature, detection, timeout, worker exception, inaccessible path, skipped file, or incomplete traversal must never authorize execution where scanning is required by policy.
- Require a complete, policy-approved result and trusted release verification before execution/install decisions.
- Keep SLeeLa host-bridge methods marked `NOT_CONNECTED` / `NOT_IMPLEMENTED` until the filesystem walker, safe process bridge (argument arrays, no shell interpolation), cancellation, result parser, and OS-specific event/service integration are implemented and tested.
- Do not claim real-time protection until event monitoring and its failure modes are tested end to end.

## 5. Cross-platform release gates

A release candidate must pass the exact commit's CI on Linux x86-64, Windows x86-64, macOS x86-64, and macOS arm64. Include compiler/build tests, Python compilation and unit tests, package-manifest verification, native binary smoke tests, JSON report/exit-code checks, signature verification negative cases, and quarantine behavior. Record commit SHA, workflow run, toolchain versions, artifact hashes, and signature-index provenance. A single green Linux job is not cross-platform certification.

## 6. Coverage and resource controls

- Report the selected engine(s), engine version, signature/database version and age where available, scanned/eligible/skipped/error counts, and why coverage is incomplete.
- Keep finite defaults for file size, directory depth, archive recursion/expansion, scan size, worker count, queue size, and per-file timeout.
- Archive inspection and YARA are optional extensions, not assumed capabilities of the native SHA-256 matcher. Add them only with explicit licensing/deployment review, strict resource limits, and regression tests.
- A skipped or over-limit archive/file is incomplete coverage, never a clean result.

## Release checklist

- [ ] Trusted release signing key and rotation/revocation process documented.
- [ ] Quarantine copy/verification/failure recovery tests pass on target platforms.
- [ ] Adversarial regression suite passes on the exact release commit.
- [ ] SLeeLa execution policy rejects detection, errors, and incomplete scans.
- [ ] Linux, Windows, macOS Intel, and macOS arm64 CI all pass.
- [ ] Engine/database provenance and skipped-file coverage appear in reports.
- [ ] No YARA/archive capability is claimed unless implemented and tested.

**Status:** planning and release criteria only. Mark each item complete only after code, tests, and CI evidence support it.


## Implementation status for this change set

| Workstream | Change included | Still required before calling complete |
|---|---|---|
| 1. Authenticated releases | Added sleelavirin_verify_release.py to verify a detached signature over release metadata, then verify artifact paths, sizes, and SHA-256 values. Added synthetic positive/negative tests. | Configure a real release process, independently distribute the trusted public key, and document/key-operate rotation and revocation. |
| 2. Quarantine/restore | Existing quarantine code copies with exclusive destination creation, verifies copied bytes, and refuses changed/symlink sources; existing tests cover collisions and changed files. Requirements added for restore. | No restore command is claimed by this change. Add explicit restore workflow and platform-specific failure/recovery tests before enabling it. |
| 3. Adversarial tests | Added manifest traversal and symlink regression tests, plus signed-release tamper/path/signature tests. | Run the complete suite and extend mutation, disk-full, interrupted update, and archive-limit tests where missing. |
| 4. SLeeLa execution policy | Updated the component contract and documentation to keep NO_MATCH distinct from a policy authorization. | The native SLeeLa host bridge and actual execution gate remain unimplemented; this is not yet an end-to-end runtime control. |
| 5. Platform gates | Added a four-target GitHub Actions workflow for Linux x86-64, Windows x86-64, macOS x86-64, and macOS arm64. | The workflow must run successfully on the exact PR/release commit. The new workflow has not yet produced CI evidence in this change set. |
| 6. Coverage/resources | Documented engine/version/provenance reporting, finite limits, skipped-file semantics, and optional archive/YARA scope. | Do not claim archive inspection or YARA detection unless an engine integration and its tests are added. |

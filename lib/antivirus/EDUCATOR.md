# Antivirus Component Educator

This guide explains the component boundaries for developers integrating the SLeeLa antivirus framework.

## Scan flow

1. A caller creates an `SLScanRequest`.
2. `SLScanPolicy` validates scope, target intent, resource limits, and quarantine defaults.
3. `SLScannerEngine` sends the request through the ClamAV adapter/host bridge.
4. The adapter maps the engine response to one of the explicit verdicts in `SLScanResult`.
5. The caller displays the result and logs the audit record. It must not turn errors or skipped files into a clean verdict.
6. If detection is reported, quarantine remains a separate, policy-controlled action.

## Local versus system-wide

- **Local:** scan a selected file or directory as the current user. No privilege escalation.
- **System:** scan only administrator-approved roots. The implementation must check privileges and avoid following symlinks outside approved roots.
- **On-access:** separate, optional platform capability. A scheduled scan is not real-time protection.

## ClamAV choices

- `clamdscan`: client for an already-running `clamd`; daemon configuration and signatures determine scanning behavior.
- `clamscan`: standalone one-shot scan; it loads the signature database for each invocation.
- `clamonacc`: Linux-only on-access client supported by ClamAV under its documented prerequisites.

Do not build a shell command by concatenating untrusted paths. The native bridge should launch a process with an argument vector, bounded output, a timeout, cancellation support, and a controlled environment. Prefer a local Unix socket or a protected platform-specific endpoint; do not expose an unauthenticated daemon TCP listener.

## Suggested validation stages

1. Unit-test request/policy validation and every verdict mapping.
2. Test clean, infected, engine-unavailable, stale-database, permission-denied, timeout, and cancelled cases.
3. Test paths with spaces, Unicode, leading dashes, symlinks, large files, and archive bombs.
4. Use the official EICAR test file only in an isolated test directory and follow the ClamAV testing guidance; do not use live malware.
5. Verify quarantine permissions, integrity checks, audit logs, restoration procedure, and failure recovery.
6. Run platform integration tests on Debian, Ubuntu, Fedora, Windows, and macOS before advertising platform support.

## Current status

These files establish the SLeeLa component contracts and platform layout. Until the native bridge and integration tests exist, the framework must report engine/host integration as unavailable and must not claim that files were scanned.

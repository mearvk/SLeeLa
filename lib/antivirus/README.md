# SLeeLa Antivirus Components

**Status: component framework / integration contracts.** This directory defines the SLeeLa-side components for local and system-wide scanning and the ClamAV adapter boundary. It is not yet a complete, active antivirus product: the SLeeLa runtime still needs a trusted filesystem walker, process/daemon bridge, OS event monitor, and platform-specific service installer wired to these contracts.

## Goals

- Local scans: a user-selected file or directory, without elevated privileges by default.
- System scans: explicit administrator-authorized roots, with protected OS and application paths handled by policy.
- ClamAV integration: prefer `clamdscan` with a configured local `clamd` daemon; allow `clamscan` as a one-shot fallback.
- Platform profiles organized by OS brand and name.
- Auditable scan results, bounded resource use, exclusions, and an opt-in quarantine workflow.
- No shell-string execution, automatic deletion, or claims of real-time protection until the host bridge is implemented and tested.

## Components

| File | Responsibility |
| --- | --- |
| `SLScanRequest.sleela` | Request target, scope, recursion, and limits |
| `SLScanResult.sleela` | Structured verdict/result |
| `SLScanPolicy.sleela` | Scan and safety policy |
| `SLClamAVAdapter.sleela` | ClamAV executable/daemon integration contract |
| `SLQuarantineManager.sleela` | Quarantine workflow contract; disabled by default |
| `SLScannerEngine.sleela` | Orchestrates request validation and engine handoff |
| `SLLocalScanner.sleela` | User-scoped entry point |
| `SLSystemScanner.sleela` | Administrator-scoped entry point |
| `platforms/` | OS-brand and OS-name profiles |

## OS directory convention

- `platforms/GNU/Linux/Debian/`
- `platforms/GNU/Linux/Ubuntu/`
- `platforms/GNU/Linux/Fedora/`
- `platforms/Microsoft/Windows/`
- `platforms/Apple/macOS/`

These profiles provide paths and capability hints; they must not infer elevated permissions or silently enable blocking behavior. Package names, daemon sockets, service names, and install locations can differ by release and local configuration, so deployment code should detect and validate them.

## ClamAV operating model

1. Prefer a local `clamd` daemon with `clamdscan` for repeated scans.
2. If no daemon is available, optionally use `clamscan` for a one-shot scan after checking that the signature database is present and current.
3. Parse engine exit status and structured output into `SLScanResult`; distinguish clean, detected, error, timeout, and unsupported outcomes.
4. Keep daemon sockets local and access-controlled. Never expose ClamAV's unauthenticated TCP socket to untrusted networks.
5. Treat missing signatures, engine errors, permission errors, and timeouts as **unknown/error**, never as clean.
6. Linux on-access support may be supplied by ClamAV's `clamonacc` where supported and correctly configured. This is a separate optional integration, not a cross-platform feature.

References:
- [ClamAV scanning documentation](https://docs.clamav.net/manual/Usage/Scanning.html)
- [ClamAV on-access scanning guide](https://docs.clamav.net/manual/OnAccess.html)

## Safety defaults

- Local scanning is the default scope.
- System-wide scans require explicit administrative authorization and an explicit root list.
- Quarantine is disabled unless the administrator enables it.
- Never automatically delete detected files. Quarantine should copy to a private, access-controlled location, verify the copy and metadata, record an audit event, then only optionally isolate the original under an explicit policy.
- Exclude scanner-owned working directories and daemon event sources to avoid recursive scan loops.
- Enforce maximum file size, scan timeout, recursion depth, archive expansion limits, and queue limits in the host implementation.
- Avoid scanning virtual filesystems, device nodes, sockets, and other non-regular files unless a platform adapter explicitly supports them.
- Report that a scan is incomplete when any target was skipped or could not be read.

## Implementation boundary

The current `.sleela` files are baseline-syntax component definitions and contracts. Methods that return `NOT_CONNECTED`, `NOT_IMPLEMENTED`, or equivalent status are deliberate: they prevent a stub from being misrepresented as an operating scanner. The next implementation step is a tested native host bridge for filesystem enumeration, process execution without shell interpolation, result parsing, cancellation, and OS-specific service/event integration.

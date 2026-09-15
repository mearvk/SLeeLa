# AE6E66 Test Plan

**Status:** Required before production designation

## 1. Integrity gate

- Verify a known-good pinned release manifest.
- Verify the release/manifest provenance independently; SHA-256 alone does not authenticate the manifest.
- Confirm SHA-256 mismatch causes a hard failure.
- Confirm missing manifest causes a hard failure.
- Confirm malformed manifest paths are rejected.
- Confirm absolute and parent-traversal paths are rejected.
- Confirm symlinked manifest and target files are rejected.
- Confirm verification precedes diagnostics and operational execution.

## 2. Collection safety

- Reject non-HTTPS sources when HTTPS is required.
- Enforce connection and response timeouts.
- Enforce maximum response size.
- Enforce bounded concurrency.
- Enforce minimum inter-request delay.
- Verify redirects remain within approved policy.
- Treat HTML, images, and downloaded content as untrusted input.
- Verify authoritative indexes/APIs are preferred where available.
- Confirm robots, terms, access controls, and rate limits are respected.

## 3. Data quality

- Parse CSV records with a standards-compliant parser.
- Reject malformed physical records.
- Validate email syntax.
- Normalize phone values without inventing missing digits.
- Decode HTML entities only where the source format requires it.
- Require authoritative source URL and retrieval timestamp for operational records.
- Reject records that do not conform to `contact-schema.json`.
- Confirm `legacy-unverified` and `legacy-public` records cannot silently become delivery targets.
- Verify retention and correction/deletion behavior.
- Verify the checked-in historical dataset is not represented as a current authoritative directory.

## 4. Message safety

- Reject CR/LF header injection in recipients and header values.
- Enforce maximum message and attachment sizes.
- Validate recipient syntax and approved recipient policy.
- Require an explicit sender identity.
- Confirm dry-run produces no delivery.
- Confirm delivery requires explicit operator enablement.
- Hash exact message bytes with SHA-256 before delivery.
- Confirm credentials and full message bodies are not written to ordinary logs.
- Verify rate limits and bounded concurrency for bulk delivery.

## 5. MTA security

- Verify `reject_unauth_destination` or equivalent anti-open-relay protection.
- Reject wildcard binding unless explicitly approved.
- Verify HELO enforcement where required by the host policy.
- Verify SMTP TLS policy is configured.
- Verify DKIM uses a protected local Unix socket.
- Verify DKIM private keys are outside Git and have restrictive permissions.
- Verify SPF and DMARC match the actual deployment identity.
- Verify queue, bounce, abuse, and authentication monitoring.

## 6. Database security

- Verify the application account exists with minimum privileges.
- Verify actual grants, not only intended configuration.
- Verify database access is local-only where possible.
- Verify TLS when a network connection is unavoidable.
- Verify prepared statements are used by the application.
- Verify schema migrations are explicit and auditable.
- Verify credentials never enter the repository.
- Test backup and restore procedures.

## 7. Platform tests

For each supported platform, record OS version, runtime version, prerequisites, test command, result, and date. A script's existence is not platform-support evidence.

## 8. Negative testing

Every security control must have at least one negative test demonstrating that an unsafe condition stops execution. Examples include corrupted files, invalid recipients, malformed remote content, open relay configuration, wildcard listener, missing TLS, missing DKIM socket, excessive database privileges, missing secrets, unsafe manifest paths, and symlinked integrity targets.

## 9. Release gate

AE6E66 may be called **production-ready** only when:

1. application source exists;
2. the application passes functional tests;
3. all applicable negative security tests pass;
4. the pinned release manifest verifies successfully;
5. the release/manifest provenance is independently authenticated;
6. deployment-specific MTA/database controls pass;
7. supported-platform evidence is recorded; and
8. no unresolved high-severity security finding remains.

Until then, the module remains a hardened specification/data component and must not be represented as a complete production application.

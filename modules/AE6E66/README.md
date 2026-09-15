# AE6E66 — UK Parliament Contact Module

**Version:** 2.2 hardened baseline  
**Author:** Max Rupplin — MEARVK LLC  
**Status:** Security-hardened design; production deployment requires implementation and host-specific verification.

AE6E66 collects publicly available UK Parliament contact information for a declared operational purpose. It is designed around **least privilege, explicit operator control, provenance, and fail-closed integrity**.

## Security position

AE6E66 does not install or expose a mail server, alter DNS, alter firewall rules, generate repository-tracked credentials, or auto-restore corrupted files.

See [`SECURITY.md`](SECURITY.md), [`DEPLOYMENT.md`](DEPLOYMENT.md), [`SOURCE-STATUS.md`](SOURCE-STATUS.md), and [`TEST-PLAN.md`](TEST-PLAN.md).

## Structure

```text
modules/AE6E66/
├── configuration/
│   └── ae6e66-config.example.xml
├── scripts/
│   ├── verify-integrity.sh
│   ├── install-postfix-dovecot.sh   # MTA preflight only
│   ├── setup-dkim.sh                # DKIM/MTA preflight only
│   └── setup-mysql.sh               # DB preflight only
├── contacts.csv                     # Controlled legacy/public-source reference data
├── contact-schema.json              # Machine-readable record schema
├── SECURITY.md
├── DEPLOYMENT.md
├── SOURCE-STATUS.md
├── TEST-PLAN.md
├── AE6E66.RDRS
└── README.md
```

## Collection controls

- HTTPS is required.
- Request timeouts and response-size limits are required.
- Concurrency is bounded.
- A minimum inter-request delay is required.
- An authoritative index/API should be preferred over identifier brute force when available.
- Collection must respect applicable terms, access controls, robots guidance, and rate limits.
- HTML, redirects, images, and remote content are untrusted input.
- Each operational record must retain source URL and retrieval timestamp.
- Only fields necessary for the declared purpose should be retained.
- Re-crawling is explicit and governed by a configurable freshness period.

The old `0..999` member loop is not an authorization mechanism or preferred collection method.

## Contact-data controls

`contacts.csv` is a controlled public-source reference dataset, not a secret database or automatic delivery list. It now uses a fixed CSV schema and explicit status classification. Legacy records lacking verified provenance or current retrieval timestamps are marked `legacy-unverified` or `legacy-public` and **must not be treated as current operational records until revalidated**.

The schema requires provenance and retrieval metadata for operational records. The checked-in historical dataset is not asserted to be a current authoritative UK Parliament directory. Revalidation must occur against an authoritative source before operational use.

Operational data must have provenance, normalization, freshness, retention, and correction controls. Do not add private information merely because it can be discovered elsewhere. Do not place authentication material in CSV files.

## Mail architecture

Preferred flow:

```text
AE6E66 -> local submission interface -> administrator-managed MTA -> recipient MX
```

AE6E66 should not listen on a public SMTP port. The host MTA is configured independently.

When an administrator operates an MTA, it must have authenticated/authorized submission or local-only submission, anti-open-relay protection, modern TLS, DKIM, SPF/DMARC alignment, explicit outbound rate limits, abuse monitoring, and controlled queue/bounce handling.

Historical deployment identities such as `mail.lauradei.us`, `lauradei.us`, `45.32.31.139`, and `contact@lauradei.us` are not module defaults.

## Message safety

Before any delivery action: verify integrity; validate recipients; reject CR/LF header injection; enforce message-size limits; use an explicit sender identity; run dry-run first; require explicit operator enablement; hash the exact message bytes with SHA-256; and log delivery status without credentials or unnecessary sensitive content.

Bulk delivery must never be enabled merely by installing the module.

## Database

Use a local-only database account with minimum application privileges. Database administration belongs to a separate administrator account. Credentials must come from an OS secret store or protected runtime file. Schema creation and migrations must be reviewed and run separately from application startup.

## Integrity

SHA-256 is the required integrity mechanism. Verification is mandatory before build, execution, crawl, message preparation, and diagnostics. A mismatch stops the operation. The verifier rejects unsafe paths and symlinked manifest/target files. MD5 is not used for security decisions. Remote auto-restore is disabled. Trusted source identity must be a pinned, administrator-approved commit/release rather than a mutable branch.

Run:

```bash
bash modules/AE6E66/scripts/verify-integrity.sh /path/to/approved/SHA256SUMS
```

The manifest itself must come from a trusted release process and must be cryptographically authenticated by that release process; SHA-256 verification alone does not prove manifest provenance.

## Platform status

Scripts for Linux, macOS, and Windows are not evidence of equivalent production support. A platform is production-supported only after its implementation, prerequisites, security controls, and tests have been verified on that platform.

## Production status

AE6E66 remains a hardened module specification and controlled data/configuration component until the application implementation, test suite, and platform evidence described in `SOURCE-STATUS.md` and `TEST-PLAN.md` are completed.

**If integrity, provenance, authorization, or security configuration cannot be established, AE6E66 stops rather than guessing.**

# AE6E66 — UK Parliament Contact Module

**Version:** 2.0 hardened baseline  
**Author:** Max Rupplin — MEARVK LLC  
**Status:** Security-hardened design; production deployment requires host-specific verification.

AE6E66 collects publicly available UK Parliament contact information for a declared operational purpose. It is designed around **least privilege, explicit operator control, provenance, and fail-closed integrity**.

## Security position

The previous module design mixed application logic with host-level mail-server installation, embedded deployment identity, automatic service changes, and non-blocking integrity behavior. Those assumptions have been removed from the security baseline.

**AE6E66 does not install or expose a mail server, alter DNS, alter firewall rules, generate repository-tracked credentials, or auto-restore corrupted files.**

See [`SECURITY.md`](SECURITY.md) and [`DEPLOYMENT.md`](DEPLOYMENT.md).

## Structure

```text
modules/AE6E66/
├── configuration/
│   └── ae6e66-config.example.xml   # Safe template; no secrets
├── scripts/
│   ├── verify-integrity.sh          # SHA-256 fail-closed verifier
│   ├── install-postfix-dovecot.sh   # MTA preflight only
│   ├── setup-dkim-lauradei.sh       # DKIM/MTA preflight only
│   └── setup-mysql.sh               # DB preflight only
├── contacts.csv                     # Controlled public contact dataset
├── SECURITY.md                      # Security and data-protection standard
├── DEPLOYMENT.md                    # Deployment architecture and controls
├── AE6E66.RDRS                      # Registry descriptor
└── README.md
```

Empty runtime directories may exist for application state, but mutable state should normally live outside the Git checkout.

## Collection controls

- HTTPS is required.
- Request timeouts and response-size limits are required.
- Concurrency is bounded.
- A minimum inter-request delay is required.
- An authoritative index/API should be preferred over identifier brute force when available.
- Collection must respect applicable terms, access controls, robots guidance, and rate limits.
- HTML, redirects, images, and remote content are untrusted input.
- Each stored record should retain source URL and retrieval timestamp.
- Only fields necessary for the declared purpose should be retained.
- Re-crawling is explicit and governed by a configurable freshness period.

The old `0..999` member loop is not itself an authorization mechanism and is no longer presented as the preferred collection method.

## Contact-data controls

`contacts.csv` is treated as controlled public-source data, not as a secret database.

Operational data should have provenance, normalization, freshness, retention, and correction controls. Do not add private information merely because it can be discovered elsewhere. Do not place authentication material in CSV files.

## Mail architecture

Preferred flow:

```text
AE6E66 -> local submission interface -> administrator-managed MTA -> recipient MX
```

AE6E66 should not listen on a public SMTP port. The host MTA is configured independently.

When an administrator operates an MTA, it must have:

- authenticated/authorized submission or local-only submission;
- `reject_unauth_destination` or equivalent anti-open-relay protection;
- modern TLS according to the current MTA/platform security policy;
- DKIM signing with protected private keys;
- SPF and DMARC aligned to the actual sending domain;
- explicit outbound rate limits and abuse monitoring;
- controlled queue and bounce handling.

The historical values `mail.lauradei.us`, `lauradei.us`, `45.32.31.139`, and `contact@lauradei.us` are **not module defaults**. Deployment identity belongs in administrator-controlled configuration.

## Message safety

Before any delivery action:

1. verify module integrity;
2. validate recipients;
3. reject CR/LF header injection;
4. enforce a message-size limit;
5. use an explicit sender identity;
6. run dry-run first;
7. require explicit operator enablement for delivery;
8. hash the exact message bytes with SHA-256;
9. log delivery status without logging credentials or unnecessary sensitive content.

Bulk delivery must never be enabled merely by installing the module.

## Database

Use a local-only database account with minimum application privileges. Database administration belongs to a separate administrator account.

Credentials must come from an OS secret store or protected runtime file. The historical pattern of generating `.db-credentials` inside the repository is deprecated.

Schema creation and migrations must be reviewed and run separately from application startup.

## Integrity

SHA-256 is the required integrity mechanism.

- Verification is mandatory before execution and diagnostics.
- A mismatch stops the operation.
- MD5 is not used for security decisions.
- Remote auto-restore is disabled.
- Trusted source identity must be a pinned, administrator-approved commit/release rather than a mutable branch.

Run:

```bash
bash modules/AE6E66/scripts/verify-integrity.sh /path/to/approved/SHA256SUMS
```

The manifest must itself come from a trusted release process.

## Platform status

Scripts for Linux, macOS, and Windows are not evidence of equivalent production support. A platform is production-supported only after its implementation, prerequisites, security controls, and tests have been verified on that platform.

## Operational rule

**If integrity, provenance, authorization, or security configuration cannot be established, AE6E66 stops rather than guessing.**

# AE6E66 Security Standard

**Status:** Hardened baseline
**Scope:** `modules/AE6E66`
**Principle:** fail closed, least privilege, explicit deployment, no embedded secrets.

## 1. Trust boundary

AE6E66 is a data-collection and message-preparation module. It is **not** a mail-server installer and must not silently change host networking, DNS, firewall rules, package state, service state, or credentials.

All privileged deployment work is external to the module and must be explicitly approved by an administrator.

## 2. Integrity

- SHA-256 is the required content-integrity algorithm.
- MD5 is not used for security decisions.
- A digest mismatch is a hard failure.
- No automatic restoration from an unverified remote source is permitted.
- Verification occurs before build, execution, crawl, message preparation, and diagnostics.
- Trusted Git commits must be pinned to an administrator-approved commit or release; a mutable branch is not an integrity anchor.
- Generated state is kept outside the source tree where practical.

Use `scripts/verify-integrity.sh` before operational use.

## 3. Secrets

No passwords, private keys, API tokens, SMTP credentials, or database credentials belong in Git.

The repository may contain only configuration examples. Runtime secrets must come from an operating-system secret store, protected environment, or administrator-managed file with restrictive permissions.

The historical `.db-credentials` approach is deprecated.

## 4. Mail security

AE6E66 does not provision a public mail server by default.

If an administrator separately operates Postfix or another MTA:

- bind only to the required interfaces;
- deny unauthenticated relay;
- require modern TLS according to the MTA's current supported policy;
- use DKIM signing with protected private keys;
- publish SPF and DMARC records appropriate to the actual sending domain;
- use `milter_default_action = accept` only when the operator explicitly accepts unsigned-mail fallback risk; for mandatory signing, use a fail-closed policy after testing;
- rate-limit outbound delivery;
- monitor queue growth, authentication failures, bounces, and abuse indicators;
- do not rewrite or delete forensic `Received` headers merely to hide infrastructure details.

The old hard-coded `lauradei.us` / `45.32.31.139` deployment is not part of the module security baseline.

## 5. Web collection

Crawling must:

- use HTTPS;
- identify the client responsibly;
- honor applicable robots, terms, access controls, and rate limits;
- use bounded concurrency and request timeouts;
- avoid brute-force enumeration where an authoritative index/API exists;
- store only fields necessary for the declared purpose;
- record source URL and retrieval timestamp;
- validate content before parsing;
- treat downloaded HTML, images, and redirects as untrusted input.

A member identifier range such as `0..999` is not an authorization mechanism.

## 6. Contact data

`contacts.csv` contains public-facing contact information and is treated as controlled data, not as a secret store.

Operational copies must have:

- provenance/source URL;
- retrieval timestamp;
- normalization status;
- retention policy;
- correction/deletion process;
- access controls appropriate to the local deployment.

Do not add private/personal information merely because it is discoverable elsewhere.

## 7. Message safety

Before delivery:

1. validate every recipient;
2. reject CR/LF header injection in address and header fields;
3. cap message size;
4. use a fixed, trusted envelope sender;
5. require an explicit operator-controlled send action;
6. support a dry-run mode;
7. record a SHA-256 digest of the exact message bytes;
8. never log credentials or full sensitive message bodies by default;
9. keep delivery logs append-only where feasible.

Bulk delivery is disabled unless an operator explicitly enables it and defines a rate limit.

## 8. Database

The database service account must be local-only and least-privileged. Schema migration must be explicit and auditable. Credentials must never be generated into a tracked file.

Recommended controls include encrypted storage, TLS where a network connection is unavoidable, prepared statements, transaction boundaries, backups, and tested restore procedures.

## 9. Privilege

Scripts must use `set -euo pipefail`, validate prerequisites, avoid `eval`, quote shell variables, use absolute/known command paths where practical, and never pipe untrusted content into a shell.

`sudo` is not a substitute for a security boundary. Privileged actions require explicit operator intent.

## 10. Incident response

On integrity failure, unexpected outbound traffic, credential exposure, or suspicious data modification:

1. stop the module;
2. preserve logs and the affected artifact;
3. revoke exposed credentials/keys;
4. verify the source repository and deployment host;
5. restore only from an independently verified known-good version;
6. document the event before resuming operation.

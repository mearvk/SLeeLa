# AE6E66 Implementation Status

## Current status

The repository currently does **not** contain the Java implementation historically referenced by older AE6E66 documentation (`AE6E66Main.java` and `EmailDistributor.java`).

Those files are therefore not claimed as implemented, tested, or production-ready.

## Current hardened component status

The module now contains a security-hardened specification, deployment configuration template, controlled legacy reference dataset, machine-readable contact schema, fail-closed integrity verifier, non-destructive MTA/DKIM/database preflights, and a production security test plan.

These components do not constitute a complete application.

## Required implementation before production use

A future application implementation must provide, at minimum:

- source retrieval with HTTPS and bounded timeouts;
- rate limiting and bounded concurrency;
- parser isolation for untrusted remote content;
- schema validation and canonical normalization;
- provenance and retrieval timestamps;
- explicit dry-run mode;
- recipient validation and header-injection protection;
- bounded message size and attachment handling;
- administrator-controlled mail submission;
- database access through prepared statements;
- secret-store integration without repository credentials;
- SHA-256 verification before execution;
- unit tests, integration tests, and negative security tests;
- platform-specific build/test evidence before a platform is called supported.

## Production designation

Until those requirements are implemented and verified, AE6E66 is a hardened module specification and data/configuration component, not a complete production application.

# SLeeLa Important-File SHA-256 Integrity Policy

## Purpose

SLeeLa maintains two integrity boundaries.

1. `security/sha256-manifest.json` protects the native verified-build source boundary.
2. `security/important-sha256-manifest.json` protects important source, API, server, security, workflow, and reference documents.

The second manifest is an integrity record. A matching digest does not by itself establish legal authority, authorship, or factual correctness.

## Coverage

The important-file manifest covers native source, API source and documentation, Server Edition material, security material, GitHub Actions workflow definitions, important root documentation, and the `international-criminal-court/` reference directory including binary documents.

The build manifest remains intentionally narrow and fail-closed for the compiled runtime. The important-file manifest is intentionally broader so changes to API contracts, deployment specifications, documentation, workflows, or reference material are detectable.

## Verification

Regenerate with:

```text
python3 tools/generate-important-sha256-manifest.py --root . --output security/important-sha256-manifest.json
```

The GitHub Actions workflow regenerates the manifest on `main` and `master` and commits only when recorded file bytes change.

SHA-256 protects byte-level integrity. It does not establish authorship, legal validity, or factual correctness.

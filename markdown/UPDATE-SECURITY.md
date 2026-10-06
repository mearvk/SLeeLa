# Update and Execution Security

SLeeLa uses a verification-first model for update and execution workflows.

## Required order

```text
update/source acquisition
        -> SHA-256 verification
        -> build
        -> diagnostics/tests
        -> execution/install
```

A candidate update must be staged before installation. Its exact staged bytes must be hashed and compared with the trusted SHA-256 manifest value. A mismatch is a hard failure: the candidate is not built, tested, installed, or executed.

## Build and runtime rule

Build tooling and runtime entry points must treat verification as a prerequisite, not as a post-build diagnostic. Unverified update inputs must never become compiler inputs or executable inputs.

For production update distribution, SHA-256 integrity checking should be paired with an authenticated publisher/signature check. SHA-256 establishes that the bytes match the expected digest; it does not by itself establish who published those bytes.

## Scope

This policy applies to update sources obtained from HTTPS endpoints and named local update directories. Existing local source builds that are not update operations remain subject to the normal compiler and test pipeline.

## Failure behavior

Any missing manifest entry, malformed digest, unreadable candidate, digest mismatch, or failed publisher verification stops the operation. There is no verification bypass in the update path.

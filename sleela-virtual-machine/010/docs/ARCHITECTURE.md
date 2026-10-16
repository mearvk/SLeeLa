# SLVM/10 Architecture

SLVM/10 introduces a verification layer above the native filesystem substrate of SLVM/9.

## Lifecycle

`NEW -> ADMITTED -> RUNNING -> QUIESCING -> RECOVERING -> RUNNING`

Security or identity failure may enter `QUARANTINED`; controlled shutdown enters `STOPPED`.

## Verification boundary

Before an operation is allowed, the system has to establish:

1. SLeeLa policy is valid.
2. The filesystem was observed and verified.
3. The OS/filesystem adapter is verified.
4. The storage identity is current.
5. The requested capability is represented by the operation.
6. The object generation is still current.

This is deliberately stronger than checking a native file descriptor or pathname alone.

## Long-lived resources

SLVM/10 treats filesystem identity as revocable evidence. After recovery, migration, mount replacement, or detected generation drift, callers should issue `REVALIDATE` rather than silently continuing with an old identity.

## Advanced filesystems

TAC3 and future filesystems can provide their own verification profile above the common storage contract. Their specialized metadata remains outside the generic VM until an adapter explicitly qualifies it.

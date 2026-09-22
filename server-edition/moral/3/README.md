# Discord-3™ — SLeeLa Server and Service — Service 3

Service 3 is the next Server Edition implementation layer after Service 2. It
introduces a remedial server model: controlled preflight, execution, recovery
state, and completion evidence.

`moral/2/` remains an independent implementation. Service 3 is additive.

## Technology and implementation style

1. Preflight validates the engine, source, configuration, state, and integrity
   inputs.
2. Server.sleela owns request-processing semantics.
3. Recovery records lifecycle state without rewriting the implementation.
4. Evidence records what was attempted without treating logs as proof of
   cryptographic integrity.

NIST guidance treats configuration validation, verification, change control,
and recording configuration state as explicit engineering activities.
citeturn0search12turn0search13

## Configuration

Service 3 supports engine, inbox, state, server_source, log, recovery_log,
mode, tick, foreground, preflight, sha256_manifest, and allow_network.

The default network policy remains false.

## Remedial behavior

A remedial run does not silently substitute another executable or server.
If preflight fails, Service 3 records the failure and stops. If execution
fails, it records the exit status and leaves source and configuration intact.

## Design Activity

Service 3 uses the shared six-dimension Design Activity model. Its additional
preflight and recovery controls provide implementation evidence; they do not
redefine the scientific domains or claim scientific measurement uncertainty.

## Selecting Service 3

The native launcher supports an explicit implementation-level selection:

`SLEELA_SERVER_LEVEL=3 sleelas`

The supported level is explicit rather than discovered from arbitrary paths.


## Programmatic theme

**Discord-3™** is the programmatic name and implementation theme assigned to
Service 3. It identifies the remedial implementation generation and does not
indicate sponsorship, endorsement, ownership, or technical affiliation with
Discord Inc.

No Discord logo, trade dress, or proprietary Discord asset is included by the
naming convention. Discord's published brand guidance governs use of its marks. citeturn0search3turn0search2

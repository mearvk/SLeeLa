# Sleelavirin™ Security Policy

## Default principles

1. Verify before trusting.
2. Prefer authenticated provenance over unsigned claims.
3. Bound file, archive, stream, and memory sizes.
4. Treat scanner errors as distinct from clean verdicts.
5. Do not silently disable an existing endpoint security product.
6. Do not grant elevated privileges merely to scan a package.
7. Preserve sufficient evidence to reproduce a security decision.
8. Keep remediation under explicit policy control.

## Verdict classes

- CLEAN
- MALICIOUS
- SUSPICIOUS
- POTENTIALLY_UNWANTED
- UNKNOWN
- SCAN_ERROR
- UNSUPPORTED

A policy may map these classes to actions, but the mapping is package- and deployment-specific.

## Fail-closed mode

A package may declare that installation or execution requires a successful integrity/security check. In that mode, UNKNOWN, SCAN_ERROR, and UNSUPPORTED are not equivalent to CLEAN.

# Sleelavirin™ Manifest

The manifest describes the security material shipped with a package.

Suggested fields:

``
name = Sleelavirin
version = 1.0
package = <package-id>
protected = <artifact-or-resource>
digest = SHA-256:<digest>
policy = POLICY.md
heuristics = HEURISTICS.md
signatures = SIGNATURES.md
scanners = SCANNERS.md
provenance = PROVENANCE.md
```

Implementations should use a machine-readable manifest in a future version while retaining this human-readable form for review.

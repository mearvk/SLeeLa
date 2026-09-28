# Sleelavirin™ Per-File Hash Map

Sleelavirin uses exact SHA-256 lookup followed by an optional provider-supplied feature fingerprint for candidate similarity. Cryptographic digest similarity is not treated as semantic similarity. A semi-match is only a candidate and must proceed to provider or heuristic inspection.

The result model distinguishes EXACT_MATCH, SEMI_MATCH, UNKNOWN, SCAN_ERROR, and UNSUPPORTED. Detection remains separate from policy and remediation.

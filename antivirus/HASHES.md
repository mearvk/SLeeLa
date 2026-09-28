# Sleelavirin™ Hashes

Cryptographic digests provide artifact identity and integrity evidence. SHA-256 is the initial required digest representation for production Sleelavirin metadata.

Example:

``
artifact = bin/example
algorithm = SHA-256
digest = <lowercase-hex-digest>
```

Hashes identify bytes; they do not by themselves establish that an artifact is benign. Pair them with provenance, signatures, scanner results, and policy where appropriate.

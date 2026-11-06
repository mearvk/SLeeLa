# Antivirus example (basic): run the integrity preflight

This is a real **repository-integrity check**, not a malware scan. It confirms
that the files covered by the manifest still match their recorded SHA-256
digests.

From the repository root:

```sh
python3 tools/verify-before-execution.py \
  --manifest security/sha256-manifest.json \
  --root .
```

- **PASS** means every covered file matches its recorded digest.
- A mismatch means a covered file changed; stop before you build or run.

A PASS does **not** mean the repository is malware-free, that every file is
covered, or that a live scanner ran. It only means "the covered files are
unchanged since the manifest was recorded." Understanding that scope is the
whole point of the basic example.

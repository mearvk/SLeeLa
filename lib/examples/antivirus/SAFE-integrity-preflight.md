# Safe integrity preflight

This is a real repository-integrity check, not a malware scan. The antivirus package README explicitly describes the scanner/ClamAV host bridge as an integration contract, not a complete active antivirus product.

From the repository root:

```sh
python3 tools/verify-before-execution.py \
  --manifest security/sha256-manifest.json \
  --root .
```

A PASS means the files covered by that manifest match their recorded digests. It does **not** mean the repository is malware-free, that every file is covered, or that a live scanner has run. Stop before building or executing if verification fails. For actual malware scanning, follow the documented ClamAV adapter setup in `lib/antivirus/README.md`; never treat an unavailable scanner or engine error as a clean verdict.

---
inclusion: fileMatch
fileMatchPattern: "{docs/binary-sets.json,docs/BINARY-SETS.md,**/Makefile,build/**}"
---

# Building OS-specific binary sets

When building, collecting or documenting a flat set of executables for one OS, follow `docs/BINARY-SETS.md` and use `docs/binary-sets.json` for the module list, commands and the folders each module needs.

Hard rules (full list in the guide):
- Executables only; every file gets a README row; add `SHA256SUMS`.
- Never start server binaries to document them (`skya` listens on 0.0.0.0:8443 at once).
- Never run installers or targets whose `make -n` writes to system paths.
- Do not regenerate the committed `security/sha256-manifest.json` just to pass the gate; generate a manifest outside the repo and say so.
- Only Linux x86-64 is verified.

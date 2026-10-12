# Sleelavirin™ Signatures

This document records exact or family-oriented detection signatures supplied by an author or referenced provider.

Signature records should include:

- identifier;
- family/category;
- signature version;
- matching method;
- protected artifact scope;
- source/provenance;
- publication date;
- expiry or replacement information;
- false-positive notes;
- licensing/distribution constraints.

Sleelavirin may reference external signature databases rather than copying them into a package.


## ClamAV-to-native compatibility

The native matcher compares the SHA-256 digest of an entire file against the local native index. The importer in `tools/sleelavirin_import_clamav.py` accepts only extracted plain-text `.hsb` records whose first field is a 64-character hexadecimal digest and whose remaining fields match a compatible hash/size/name record.

Do not treat all ClamAV definitions as whole-file SHA-256 hashes. Signed `.cvd` and updated `.cld` containers, MD5 `.hdb`, PE-section hashes, bytecode, logical/body signatures, and YARA rules are not interchangeable with this index. The imported index is a supplemental subset, not a replacement for ClamAV. Keep source databases intact, record source/version/date and counts in provenance, and verify redistribution terms before bundling definitions.

Example:

```sh
python3 antivirus/tools/sleelavirin_import_clamav.py --source ./clamav-extracted --output antivirus/signatures/native-sha256.txt
python3 antivirus/tools/sleelavirin_import_clamav.py --check antivirus/signatures/native-sha256.txt
```


## Import provenance and safe replacement

The ClamAV subset importer writes an adjacent `native-sha256.txt.manifest.json` by default. The manifest records the source version/identifier, optional source URL, import time, signature count, skipped record count, and SHA-256 digest of the resulting native index. Supply `--source-version` and `--source-url` so the origin can be audited.

The importer validates a temporary candidate before activation, stages the provenance manifest, and attempts to roll back the index if publication fails. Failed imports with no compatible records leave the active index unchanged. Verify both the index and manifest after any filesystem or power failure. This is an offline import workflow, not an automatic ClamAV database updater.

```sh
python3 antivirus/tools/sleelavirin_import_clamav.py \
  --source ./clamav-extracted \
  --source-version "daily-<database-version>" \
  --source-url "trusted-internal-artifact-id" \
  --output antivirus/signatures/native-sha256.txt
```

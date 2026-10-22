<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">






# Sleelavirin™ Antivirus Co-Package

## Purpose

**Sleelavirin™** is the SLeeLa antivirus/security co-package for authors who want to ship security-aware deliverables alongside their primary package.

An author may include an `/antivirus` directory in a package to declare one-off security concerns, scanning requirements, detection metadata, heuristic rules, verification material, and platform integration notes without making those concerns part of the application's ordinary runtime API.

Sleelavirin is a **security integration framework and package convention**, not a claim that every package contains a complete antivirus engine.

## Design goals

- Keep security material close to the deliverable it protects.
- Support static scanning, signatures, hashes, reputation inputs, and heuristic analysis.
- Permit integration with existing antivirus engines rather than requiring a replacement engine.
- Record provenance, versions, confidence, and intended response for every detection rule.
- Fail closed where a package explicitly requires verification before execution or installation.
- Separate detection from remediation so package authors do not silently delete or quarantine user content.
- Support Linux, Windows 10+, and macOS through platform adapters.
- Provide a common SLeeLa representation for scan requests and verdicts.

## Reference integrations

Sleelavirin can reference established antivirus interfaces and rule systems. For Windows, Microsoft documents AMSI as a vendor-agnostic application interface for file, memory, stream, and related antimalware scanning, including session correlation. https://learn.microsoft.com/en-us/windows/win32/amsi/antimalware-scan-interface-portal

YARA provides a rule language for identifying and classifying files or other samples through strings, binary patterns, and boolean conditions. Sleelavirin may carry or reference YARA-compatible rules where licensing and deployment requirements permit. https://yara.readthedocs.io/

## Package boundary

A normal SLeeLa package may contain:

```
package/
├── src/
├── api/
├── tests/
└── antivirus/
    ├── README.md
    ├── PACKAGE.md
    ├── POLICY.md
    ├── MANIFEST.md
    ├── HEURISTICS.md
    ├── SIGNATURES.md
    ├── HASHES.md
    ├── SCANNERS.md
    ├── VERDICTS.md
    ├── PROVENANCE.md
    ├── platform/
    ├── rules/
    ├── signatures/
    ├── manifests/
    └── tests/
```

The antivirus co-package is optional. Its presence means the author has supplied security metadata or integrations; it does not by itself mean that a complete AV engine is bundled.

## Security lifecycle

```
AUTHOR
  ↓
DECLARE
  ↓
HASH / SIGN / DESCRIBE
  ↓
STATIC SCAN
  ↓
HEURISTIC ANALYSIS
  ↓
OPTIONAL ENGINE INTEGRATION
  ↓
VERDICT
  ↓
POLICY DECISION
  ↓
INSTALL / EXECUTE / QUARANTINE / REJECT / REVIEW
```

## Build and test

The portable C fingerprint matcher and Trap-2 heuristic evaluator can be built without an external antivirus engine:

```sh
# From the repository root
make antivirus
make antivirus-test

# Or from this directory
make
make test
```

The local Makefile writes generated objects, the static library, and the self-test executable to `antivirus/build/`. Run `make -C antivirus clean` to remove these generated files.

**Component boundary:** the original portable C fingerprint-matcher library compares caller-supplied fingerprints and evaluates caller-supplied heuristic indicators; its unit test does not establish broad malware-detection coverage. The separate native SHA-256 scanner below reads files and matches their digests against a local index. The Python coordinator supplies traversal, limits, reporting, and response policy, while the optional ClamAV adapter invokes an installed ClamAV executable. These components have distinct responsibilities and none provides a guarantee that an undetected file is safe.

## Scanner backends

Sleelavirin exposes two explicit scan backends:

### Native SHA-256 signature scanner

The native C11 scanner reads each supplied file, computes SHA-256 using its bundled implementation, and compares the digest against a local text signature database. It has no third-party crypto dependency.

```sh
make -C antivirus
python3 antivirus/tools/sleelavirin_scan.py --engine native ./path/to/file
# Explicit local signature database:
python3 antivirus/tools/sleelavirin_scan.py --engine native --database antivirus/signatures/native-sha256.txt ./path/to/file
```

The signature file format is one 64-character SHA-256 hex digest per line, optionally followed by a human-readable name. Blank lines and lines beginning with `#` are ignored. The shipped database is intentionally empty: populate it only with trusted, provenance-documented signatures. A test-only signature fixture is kept under `antivirus/tests/` and must not be copied into production databases.

The native scanner reports `DETECTED`, `NO_MATCH`, or `ERROR`. `NO_MATCH` means only that the selected local database did not contain that file's digest; it does **not** mean the file is clean. The scan coordinator adds bounded directory traversal, exclusions, extension filters, file-size/depth limits, configurable native worker count, and report/quarantine/stop actions. Archive inspection, behavior monitoring, and real-time protection are not provided by the native matcher.

### Optional ClamAV backend

Install ClamAV and initialize/update its signature database according to the official ClamAV documentation. Then run:

```sh
python3 antivirus/tools/sleelavirin_scan.py --engine clamav ./path/to/file
python3 antivirus/tools/sleelavirin_scan.py --engine clamav ./path/to/directory
# If clamscan is not on PATH:
python3 antivirus/tools/sleelavirin_scan.py --engine clamav --clamscan /path/to/clamscan ./path/to/file
```

The adapter uses the installed `clamscan` executable and requests recursive scanning for supplied directories. It does not install ClamAV or download signatures. It maps ClamAV exit status 0 to no detection, 1 to detection, and other nonzero statuses to a scan error. No detection is not a guarantee that a file is safe. See the official [ClamAV scanning guide](https://docs.clamav.net/manual/Usage/Scanning.html).

Check the optional integration with `make -C antivirus clamav`, or from the repository root with `make antivirus-clamav`. This check requires `clamscan` to be installed. The default `make antivirus` build does not require ClamAV.

## Initial scope

The first Sleelavirin structural release covers:

1. package security metadata;
2. SHA-256 and future digest references;
3. known signatures and detection identifiers;
4. heuristic rules and rationale;
5. scanner/provider declarations;
6. platform adapters;
7. verdict normalization;
8. provenance and rule-version tracking;
9. negative and benign test samples represented safely as metadata;
10. documentation for integration with established AV systems.

## Non-goals

Sleelavirin does not initially attempt to reproduce the internals of commercial antivirus products, distribute malware samples, bypass endpoint protections, or prescribe a single antivirus vendor.

## Status

**Implementation status varies by component.** The native SHA-256 scanner, scan coordinator, optional ClamAV adapter and updater, compatible-index importer, integrity-manifest tool, and associated tests have source implementations. Treat platform packages as release-ready only after the corresponding CI build and smoke tests pass. Features not documented with implementation and tests remain architectural or planned; no-match is never a safety guarantee.

Max Rupplin — MEARVK LLC — 2026

### Native-first scan options

Use the coordinator for directory scanning and response policy:

```sh
# Native engine (default)
python3 antivirus/tools/sleelavirin_scan.py --engine native --jobs 4 --max-depth 8 /path/to/scan

# Native pass followed by ClamAV as a secondary engine
python3 antivirus/tools/sleelavirin_scan.py --engine both --jobs 4 --max-depth 6 --action report /path/to/scan

# Move detections to quarantine instead of deleting them
python3 antivirus/tools/sleelavirin_scan.py --engine both --action quarantine --quarantine-dir ./quarantine /path/to/scan

# Exclusions and extension filters can be repeated
python3 antivirus/tools/sleelavirin_scan.py --exclude '*.cache' --extension .exe --extension .dll /path/to/scan
```

Options include `--max-depth` (0 means unlimited), `--jobs` for native workers, `--max-file-size`, repeatable `--exclude` and `--extension`, opt-in `--follow-symlinks`, and `--action report|quarantine|stop`. Reporting is the default. Quarantine moves detected files; it does not automatically delete them. A skipped, unreadable, or over-limit file means incomplete coverage.

### Importing compatible ClamAV definitions into the native index

ClamAV definitions are not all whole-file hashes. Sleelavirin's importer accepts only extracted plain-text `.hsb` records with a 64-hex SHA-256 digest and compatible hash/size/name fields. It does not decode signed `.cvd` or `.cld` archives, nor can it convert every ClamAV signature family.

```sh
python3 antivirus/tools/sleelavirin_import_clamav.py --source ./clamav-extracted --output antivirus/signatures/native-sha256.txt
python3 antivirus/tools/sleelavirin_import_clamav.py --check antivirus/signatures/native-sha256.txt
```

Extract trusted database content with a compatible ClamAV `sigtool` installation; do not pass binary `.cvd` or `.cld` files directly to the importer. Record the database version and source, and verify redistribution terms before packaging imported definitions.


### Resource controls and scan result integrity

`--jobs` limits concurrent native scans. `--clamav-jobs` independently limits concurrent ClamAV processes; both default to one. ClamAV receives per-file size, total scan-size, and nested archive recursion limits through `--max-file-size`, `--clamav-max-scansize`, and `--clamav-max-recursion`. The default limits are 100 MiB per file, 400 MiB per ClamAV scan, and recursion depth 8. Tune these to the host's capacity and threat model.

The coordinator reports incomplete coverage and exits with status 2 when files cannot be enumerated or skipped/over-limit inputs prevent a complete scan. Exit status 1 indicates at least one detection; 0 means no detection and no known scan errors, not proof that files are safe. Files changed during scanning are treated as errors. Quarantine refuses a destination within the scanned directory tree and avoids overwriting existing names; this is defense in depth, not a substitute for isolating suspicious files.

Definition imports now produce a provenance manifest. Example:

```sh
python3 antivirus/tools/sleelavirin_import_clamav.py \
  --source ./clamav-extracted \
  --source-version "daily-<database-version>" \
  --source-url "trusted-internal-artifact-id" \
  --output antivirus/signatures/native-sha256.txt
```


## Security hardening and release checks

See [SECURITY.md](SECURITY.md) for the security model, signature-index integrity checks, quarantine expectations, adversarial test coverage, CI checks, and release checklist. To verify an imported index against its provenance manifest, run:

```sh
python3 antivirus/tools/sleelavirin_import_clamav.py \\
  --check antivirus/signatures/native-sha256.txt \\
  --check-manifest antivirus/signatures/native-sha256.txt.manifest.json
```

Per-file engine processes have a 120-second default timeout. Configure `--scan-timeout N`; use `0` only when intentionally disabling the timeout. A timeout is a scan error, never a clean verdict.


## Official ClamAV database acquisition and SHA-256 snapshots

Use the official ClamAV update client rather than downloading CVD files from arbitrary mirrors. FreshClam retrieves and verifies database updates using ClamAV's supported update protocol; the updater then asks the ClamAV engine to load the staged databases before activating them.

Official references:
- Database distribution: https://database.clamav.net/
- FreshClam guide: https://docs.clamav.net/faq/faq-freshclam.html
- Signature formats and verification: https://docs.clamav.net/manual/Signatures.html

Install ClamAV so both `freshclam` and `clamscan` are available, then run from the repository root:

```sh
python3 antivirus/tools/sleelavirin_update_clamav.py \
  --root antivirus/signatures/clamav-cache
```

The updater downloads into a separate staging directory, requires FreshClam success, validates the staged databases by running ClamAV against an empty directory, calculates SHA-256 for every `.cvd`/`.cld` file, stores an immutable snapshot under its content-derived ID, and atomically replaces `current.json` only after validation. Previous snapshots are retained for rollback. Generated databases are local artifacts and should not be committed to Git.

To check a snapshot's recorded file sizes and hashes:

```sh
python3 antivirus/tools/sleelavirin_update_clamav.py \
  --check antivirus/signatures/clamav-cache/snapshots/<SNAPSHOT_ID>
```

`current.json` identifies the active snapshot and its manifest. Protect that pointer, the manifest, and the expected snapshot ID with appropriate filesystem permissions or independent trusted storage. SHA-256 detects changes relative to a trusted expected value; a manifest and hash stored together are not an independent publisher signature. FreshClam and ClamAV's database validation establish the upstream trust check; SHA-256 then provides local integrity tracking.

This full ClamAV snapshot is for the secondary ClamAV engine. The native SLeeLa SHA-256 importer remains a limited subset importer: it does not turn all CVD/CLD signature families into native whole-file hash signatures.


## Scan reports, archive limits, and release integrity

For an auditable machine-readable result, use `--report-json`:

```sh
python3 antivirus/tools/sleelavirin_scan.py --engine both \
  --max-depth 8 --max-file-size 100M --clamav-max-scansize 400M \
  --clamav-max-recursion 8 --jobs 2 --clamav-jobs 1 \
  --report-json antivirus-report.json /path/to/scan
```

The report contains per-engine verdict records, eligible-file count, detections, errors, completion state, and an explicit limitation statement. Exit status 0 means no match and a complete scan; 1 means detection; 2 means an error or incomplete scan. A detection can coexist with errors; inspect both the report and stderr. `CLEAN_NO_MATCH` is not a guarantee of safety.

To verify a detached release signature, with OpenSSL installed and a trusted public key obtained independently:

```sh
python3 antivirus/tools/sleelavirin_verify_signature.py \
  --public-key release-public.pem \
  --signature native-sha256.txt.sig \
  --file antivirus/signatures/native-sha256.txt
```

Signatures must be created by the release process and the corresponding public key distributed through an independent trusted channel. Do not commit private signing keys. Signature verification is optional until a project release key and key-rotation policy are established.


## Standalone binary distributions

The `.github/workflows/antivirus-binaries.yml` workflow builds and smoke-tests platform-native packages for Linux x86-64, Windows x86-64, macOS Apple Silicon (arm64), and macOS Intel (x86-64). On pushes, the packages are available as GitHub Actions artifacts; pushing a version tag such as `v1.0.0` publishes each platform ZIP to the GitHub Release.

Each archive contains:

- `sleelavirin-native[.exe]`: the compiled native C scanner;
- `sleelavirin-scan[.exe]`: the standalone scan coordinator;
- `sleelavirin-clamav[.exe]`: the ClamAV adapter CLI;
- `sleelavirin-update-clamav[.exe]`: FreshClam-based database snapshot updater;
- `sleelavirin-import-clamav[.exe]`: compatible signature-index importer;
- `sleelavirin-verify-signature[.exe]`: detached release-signature verifier;
- `signatures/native-sha256.txt`: the shipped native index (empty by default);
- `SHA256SUMS`: hashes of the packaged files.

Extract the ZIP and run the coordinator from the extracted directory. For example, on Linux/macOS:

```sh
./sleelavirin-scan --engine native --report-json scan-report.json /path/to/scan
./sleelavirin-scan --engine both --report-json scan-report.json /path/to/scan
```

On Windows, use `sleelavirin-scan.exe` with the same arguments. The native binary and signature index are resolved relative to the extracted package. The ClamAV backend still requires a compatible ClamAV installation and usable signature database on the host; the package does not embed ClamAV, FreshClam, or third-party signature databases. The native index is intentionally empty and its no-match result is not proof of safety.

These are native per-platform builds, not one universal executable. Only artifacts produced and smoke-tested by the workflow should be distributed. The ZIP's SHA-256 file detects accidental changes but is not a publisher-authentication mechanism; use the detached-signature verifier only after a trusted release key and independent public-key distribution are established.


## SHA-256 integrity inventory

Generate a SHA-256 inventory of the antivirus source tree, including source files, tests, build metadata, documentation, and protected signature-index files:

```sh
make -C antivirus integrity-manifest
make -C antivirus integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

The manifest is written under `antivirus/build/` and intentionally excludes generated build/dist folders, Python caches, itself, and the mutable local ClamAV cache. The ClamAV updater's per-snapshot `manifest.json` independently records each downloaded `.cvd`/`.cld` database hash and byte length. The native `signatures/native-sha256.txt` index is included in the source inventory; imported indexes should also be checked against their provenance manifest when one exists.

Each release package now contains:
- `INTEGRITY-MANIFEST.json`: per-file SHA-256 and size records, including the packaged native index and binaries;
- `SHA256SUMS`: convenient checksum list for package files;
- a `.sha256` sidecar alongside the platform ZIP, containing the ZIP's SHA-256.

Verify a package manifest after extraction from its extracted directory:

```sh
python tools/sleelavirin_manifest.py --root . --verify INTEGRITY-MANIFEST.json
```

The checksum list and manifest are integrity checks, not independent publisher authentication. An attacker who can replace both files and their checksums can recompute them. For tamper evidence, compare the archive's SHA-256 with a trusted value obtained independently, or authenticate a release signature using a public key distributed through a separate trusted channel. The release workflow checks its manifest before packaging.

## Security hardening and release criteria

See [HARDENING-PLAN.md](HARDENING-PLAN.md) for the six-workstream plan covering authenticated release metadata, quarantine/restore, adversarial tests, SLeeLa execution policy, cross-platform release gates, and explicit coverage/resource reporting. The cross-platform hardening workflow is [antivirus-hardening.yml](../.github/workflows/antivirus-hardening.yml). It is a CI test gate, not a publisher-signing system and not a substitute for independent security review.

### Verify signed release metadata

The release verifier checks a detached OpenSSL SHA-256 signature over the exact release-manifest bytes, then checks each declared artifact's size and SHA-256. Obtain the public key through an independent trusted channel; a key shipped only inside the same archive does not establish trust.

```sh
python3 antivirus/tools/sleelavirin_verify_release.py \
  --public-key /trusted/path/release-public.pem \
  --signature release-manifest.json.sig \
  --manifest release-manifest.json \
  --root ./release-artifacts
```

The signed JSON manifest schema is `sleelavirin-release-manifest-v1` with a non-empty `artifacts` array. Each entry has `path`, `size`, and `sha256`. Paths must be canonical relative paths; symlinks, duplicate paths, path traversal, missing files, and mismatched sizes or hashes are rejected. Key custody, rotation, and revocation remain release-operator responsibilities.

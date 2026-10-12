<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






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

**Implementation boundary:** the current C core compares caller-supplied fingerprints and evaluates caller-supplied heuristic indicators. It does not read or hash files itself, update signature databases, or invoke ClamAV, Windows AMSI, or another provider. The self-test is a unit test of the current core, not evidence of full malware-detection coverage. Provider adapters and safe file-hashing integration remain separate work.

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

**Architectural / initial package structure.** Individual scanners, providers, and rule sets become **Implemented** only when their corresponding source and tests exist.

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

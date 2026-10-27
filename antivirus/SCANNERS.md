# Sleelavirin™ Scanners

Scanners are external or internal engines that consume a Sleelavirin scan request and return a normalized verdict.

## Provider model

``
SLeeLa Package
      |
Sleelavirin Request
      |
+-----+----------------+
|                      |
Native SLeeLa       External AV
Scanner              Provider
|                      |
+----------+-----------+
           |
     Normalized Verdict
```

### Windows

AMSI is a primary integration reference. Microsoft documents APIs for initializing a scanning session, scanning buffers/strings, correlating requests, and interpreting malware results. https://learn.microsoft.com/en-us/windows/win32/amsi/antimalware-scan-interface-functions

### Other platforms

Linux and macOS adapters should use the locally installed security provider or an explicitly declared scanner. Sleelavirin must not assume that one vendor or engine exists on every system.


## Native C/C++ engine

The in-tree native engine (`antivirus/src/`, built with `make`) is a
self-contained, license-clean scanner: SHA-256 whole-file hashing **plus** a
from-scratch **Aho–Corasick** content (byte-pattern) matcher, a fuzzy similarity
check, the Trap-2 heuristic, a stable C bridge ABI, and a C++ wrapper. It is
exposed as `build/libsleelavirin.a`, `build/libsleelavirin_cpp.a`, and the
`build/sleelavirin-scan` CLI. See **`NATIVE-ENGINE.md`** for details. This is the
default, dependency-free backend; external providers below are optional adapters
for broader coverage.

## Native-first engine selection

- **Native (default):** SHA-256 whole-file hash matching and content byte-pattern matching (Aho–Corasick), with directory enumeration, exclusions, size/depth limits, and bounded worker count managed by the scan coordinator.
- **Both:** run the native pass first, then scan the same eligible files with the installed ClamAV engine. Recommended when ClamAV is available.
- **ClamAV:** use ClamAV alone as the detection backend.

Example: `python3 antivirus/tools/sleelavirin_scan.py --engine both --jobs 4 --max-depth 6 --action report /path/to/scan`

The native index importer accepts only extracted text `.hsb` records with a 64-hex whole-file SHA-256 digest. It does not decode signed `.cvd` or `.cld` archives and cannot represent most ClamAV signature families. ClamAV's own engine remains necessary for full ClamAV coverage. See the official [signature format guide](https://docs.clamav.net/manual/Signatures.html) and [terminology](https://docs.clamav.net/appendix/Terminology.html).

### Options

| Option | Purpose |
|---|---|
| `--engine native|clamav|both` | Native first, ClamAV only, or native then ClamAV |
| `--max-depth N` | Directory depth below supplied roots; `0` means unlimited |
| `--jobs N` | Concurrent native file scans; default 1 |
| `--max-file-size 100M` | Skip oversized files before scanning |
| `--exclude GLOB` | Exclude matching path/basename; repeatable |
| `--extension .exe` | Restrict to selected extensions; repeatable |
| `--follow-symlinks` | Opt in to following symbolic links |
| `--action report|quarantine|stop` | Report (default), move detections to quarantine, or stop at the first detection |
| `--quarantine-dir DIR` | Quarantine destination; files are moved, not deleted |

`--jobs` controls native workers, not ClamAV internal threads. ClamAV scan limits for nested archives are controlled by ClamAV itself. Skipped, unreadable, excluded, or over-limit files mean incomplete coverage, never a clean verdict.


## Resource controls, verdict integrity, and response policy

The coordinator exposes independent `--jobs` (native worker count) and `--clamav-jobs` (maximum concurrent `clamscan` processes). The ClamAV process count is the explicit CPU/resource control; each ClamAV process receives `--max-filesize`, `--max-scansize`, and `--max-recursion` limits. Defaults are one worker for each engine, 100 MiB maximum input file, 400 MiB ClamAV scan budget per file, and recursion depth 8.

The coordinator records file size, modification timestamp, change timestamp, inode, and file mode where available, and treats a file changing during a scan as an error. These metadata checks catch more accidental or concurrent modifications, but they are not a cryptographic snapshot and cannot eliminate all time-of-check/time-of-use races. Quarantine refuses destinations inside a scanned directory, avoids overwriting name collisions, and refuses to move files whose metadata changed after detection. These checks reduce risk but cannot eliminate all filesystem races; isolate or quiesce high-risk targets for high-assurance response.

Exit codes: `0` means no detection and no known scan errors; `1` means one or more detections; `2` means an error or incomplete scan (for example inaccessible paths, skipped oversized files, missing engine, or invalid configuration). A detection takes exit-code priority over simultaneous errors, so inspect the emitted `INCOMPLETE_SCAN` and `ERROR` lines too.

Examples:

```sh
# Four native workers, two ClamAV worker processes
python3 antivirus/tools/sleelavirin_scan.py --engine both --jobs 4 --clamav-jobs 2 /path/to/scan

# Tighter archive limits
python3 antivirus/tools/sleelavirin_scan.py --engine clamav --clamav-jobs 1 --max-file-size 80M --clamav-max-scansize 250M --clamav-max-recursion 4 /path/to/scan
```


Engine subprocesses have a 120-second per-file timeout by default. Configure `--scan-timeout N`; `0` disables the timeout. Timeouts return an error verdict and contribute to incomplete/error exit status, never a clean verdict. See [SECURITY.md](SECURITY.md) for integrity, adversarial test, quarantine, and release requirements.


## ClamAV database updates and provenance

The optional ClamAV backend uses the installed ClamAV engine and its own official signature database. Update and snapshot databases with:

```sh
python3 antivirus/tools/sleelavirin_update_clamav.py \
  --root antivirus/signatures/clamav-cache
```

The updater uses FreshClam, validates the staged database by loading it with `clamscan`, records per-file SHA-256 hashes and sizes, and atomically updates a pointer to a content-addressed snapshot. Do not commit downloaded databases into the source repository. See [README.md](README.md) and [SECURITY.md](SECURITY.md) for trust limitations and rollback guidance.


## Reports and release authenticity

Use `--report-json PATH` to produce an atomic JSON summary with per-engine results, counts, completion status, and limitations. The CLI exit codes remain authoritative: 0 = complete no-match, 1 = detection, 2 = error/incomplete (detection may coexist with errors). Never treat `CLEAN_NO_MATCH` as proof of safety.

For signed release artifacts, use `tools/sleelavirin_verify_signature.py` with a detached OpenSSL SHA-256 signature and a public key obtained independently. See [SECURITY.md](SECURITY.md) before treating signed artifacts as a release gate.

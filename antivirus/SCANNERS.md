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


## Native-first engine selection

- **Native (default):** SHA-256 whole-file hash matching, with directory enumeration, exclusions, size/depth limits, and bounded worker count managed by the scan coordinator.
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

<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# antivirus: Explanation

| | |
|---|---|
| Nature | daemon (`bin/sleelactl help antivirus`: verbs `start stop restart reload status enable disable`) |
| Registry summary | Antivirus coordinator (sleelavirin scan engine) |
| Lives in | `antivirus/` (engine in `src/`, coordinator in `tools/`, unit in `systemd/`) |
| Writeup type | Explanation, because the registered daemon (systemd unit `sleela-antivirus.service`) cannot be started here (no service manager, no host env file, no `/opt/sleela`), and `make test` in the module fails. The scanner itself does work when run by hand. |
| Verified | 2026-11-13 against `master` e048707fd: `make -C antivirus` builds; the C and C++ self-tests pass; the CLI and coordinator detect the sample; `make test` fails 1 Python test (`test_quarantine_collision_and_changed_file_refusal`). |

## What it is

Sleelavirin is a native antivirus scanning engine written in C11 with a C++17 wrapper, plus Python tools that coordinate scans. `antivirus/NATIVE-ENGINE.md` describes two detection methods (SHA-256 whole-file hash lookup and Aho-Corasick content signatures) plus a fuzzy fingerprint and a "Trap-2" heuristic scorer. A "no match" result is explicitly not proof of safety.

The registry (`bin/sleelactl.d/registry.conf:32`) calls the daemon "Antivirus coordinator". It has `unit=sleela-antivirus.service` and no `launchd=` and no `dir=`.

## How it works

- **Engine.** `antivirus/Makefile` builds `build/libsleelavirin.a`, `build/libsleelavirin_cpp.a`, the test binaries and `build/sleelavirin-scan` (usage: `sleelavirin-scan [--database FILE] [--content-db FILE] <file> ...`; exit 1 on detection, 0 on no match).
- **Coordinator.** `antivirus/tools/sleelavirin_scan.py` walks paths, calls the native scanner (and optionally ClamAV), writes a JSON report, and can quarantine (`--action report|quarantine|stop`).
- **Service.** `antivirus/systemd/sleela-antivirus.service` is `Type=simple`, `Restart=on-failure`. Its `ExecStart` is `python3 /opt/sleela/antivirus/tools/sleelavirin_scan.py --engine ${SLEELA_ANTIVIRUS_ENGINE} --database ${..._DB} --report-json ${..._REPORT} --quarantine-dir ${..._QUARANTINE} ${..._ROOT}`, so the "daemon" is one scan of a root directory, not a resident process. The values come from `/etc/default/sleela-antivirus` or `/etc/sleela-antivirus/sleela-antivirus.env`.
- **Controller.** `antivirus/tools/sleela-antivirus-ctl` (a shell wrapper around `sleela-antivirus-ctl.py`) offers `status`, `validate-host`, `start`, `stop`, `restart`, `enable`, `disable`, `policy`, `scan`, `install-unit`, `install-env`, `install-host`. It is a second controller for the same unit, separate from `bin/sleelactl`.
- **sleelactl.** `bin/sleelactl [--dry-run] <verb> antivirus` turns into `systemctl --system <verb> sleela-antivirus.service`. On macOS there is no mapping: `SLEELACTL_SERVICE_MANAGER=launchd bin/sleelactl --dry-run start antivirus` prints `no launchd service registered (launchd= is not set in the registry)`. `antivirus/PLATFORMS.md` covers building on macOS and Windows but I found no plist or Windows service definition in `antivirus/` (`antivirus/platform/README.md` is the only file in that directory).

## Where it lives

| File | Role |
|---|---|
| `bin/sleelactl.d/registry.conf:32` | registry entry (`unit=` only) |
| `antivirus/src/sleelavirin_*.{c,h,cpp,hpp}` | engine: scan/hash map, heuristics, native SHA-256, Aho-Corasick, signature loader, C bridge, C++ wrapper, CLI main |
| `antivirus/Makefile`, `CMakeLists.txt`, `Makefile.win`, `build-macos.sh`, `build-windows.ps1` | builds |
| `antivirus/tools/sleelavirin_scan.py` | scan coordinator (what the unit runs) |
| `antivirus/tools/sleela-antivirus-ctl(.py)` | service/policy controller |
| `antivirus/tools/sleelavirin_{clamav,import_clamav,update_clamav,verify_signature,manifest}.py` | ClamAV adapter, signature import/update/verify, integrity manifest |
| `antivirus/systemd/sleela-antivirus.service`, `sleela-antivirus.env.example`, `README.md` | unit and host configuration guidance |
| `antivirus/signatures/native-sha256.txt`, `tests/native-sha256.txt`, `tests/content-signatures.slv` | signature data (the `tests/` one is test-only) |
| `lib/antivirus/antivirus-policy.example.json` | default policy printed by `sleela-antivirus-ctl policy` |
| `antivirus/NATIVE-ENGINE.md`, `SCANNERS.md`, `SIGNATURES.md`, `POLICY.md`, `SECURITY.md`, `ROADMAP.md` | design docs |

## Status and limits

Works (run in `antivirus/`):

- `make` builds everything into `antivirus/build/` (git-ignored by `antivirus/.gitignore`), no errors.
- `build/test_scan`: `Sleelavirin scan/Trap-2 selftest: PASS`. `build/test_ac`: `Sleelavirin AC/signature self-test: PASS`. `build/test_bridge` prints `Bridge verdict=DETECTED detail=signature match: test-sample`. `build/test_cpp_scan` prints `Exact match: native_sample (confidence=100)`.
- `build/sleelavirin-scan --database tests/native-sha256.txt <copy of tests/native_sample.txt>` prints `DETECTED ... : test-sample` and exits 1. With `--database signatures/native-sha256.txt` it prints `NO_MATCH` and exits 0.
- `python3 tools/sleelavirin_scan.py --engine native --native-scanner build/sleelavirin-scan --database tests/native-sha256.txt --report-json <file> <dir>` on a scratch directory with one clean file and the sample printed `SCAN_SUMMARY status=DETECTED eligible=2 detections=1 errors=0 complete=true` (rc 1) and wrote the JSON report.
- `python3 tools/sleela-antivirus-ctl.py policy` prints the example policy (engine `clamav`, `quarantineEnabled: false`). `validate-host --json` lists the six missing `SLEELA_ANTIVIRUS_*` variables because `/etc/sleela-antivirus/sleela-antivirus.env` is absent.

Does not work or is wrong:

- **Daemon not operable here.** `bin/sleelactl status antivirus` exits 1; `--dry-run start antivirus` prints `systemctl --system start sleela-antivirus.service`; real `systemctl` says systemd is not PID 1. `sleela-antivirus-ctl.py status` still reports `systemd available: True` (and `state: unknown`) in this container, so that field only means a `systemctl` binary exists.
- **`make test` fails.** `python3 -m unittest` runs 36 tests: 1 error, 1 skipped. The error is `test_tools.ScanCoordinatorTests.test_quarantine_collision_and_changed_file_refusal`: `OSError: [Errno 9] Bad file descriptor` at `tools/sleelavirin_scan.py:265` (`os.close(target_fd)`). In `safe_quarantine`, `target_fd` is set to `-1` after `os.fdopen` takes ownership (line 228), but the `finally` block checks `target_fd is not None` (line 264) and so closes fd `-1`. The failure appears in the quarantine path.
- **`antivirus/README.md` is the wrong file.** It is byte-identical to `antivirus/Makefile` (`cmp` equal; both are git blob `6152d5bd2a0c66145ddfcbf6df9df2f7f0369950`, 180 lines). The module's real overview is `NATIVE-ENGINE.md` / `PACKAGE.md`.
- **Unit depends on a host install.** It needs `/opt/sleela/antivirus/tools/sleelavirin_scan.py`, the env file and `/var/lib/sleela-antivirus`, `/var/log/sleela-antivirus`. None exist here. I did not run `install-unit`, `install-env` or `install-host`.
- The default policy (`lib/antivirus/antivirus-policy.example.json`) selects `clamav` while the module docs call the native engine primary; ClamAV is not installed here and I did not test it.
- `Makefile`'s `test` target also runs `definitions-check` after the Python tests; it never ran because the earlier step failed.

## Related

- [server-edition](server-edition.md), [server-edition3](server-edition3.md): the other two registry daemons.
- [`antivirus/NATIVE-ENGINE.md`](../../../antivirus/NATIVE-ENGINE.md), [`antivirus/systemd/README.md`](../../../antivirus/systemd/README.md), [`bin/SLEELACTL.md`](../../../bin/SLEELACTL.md).

## Sources

Files read: `bin/sleelactl.d/registry.conf`, `bin/SLEELACTL.md`, `antivirus/README.md`, `Makefile`, `NATIVE-ENGINE.md`, `PACKAGE.md`, `systemd/sleela-antivirus.service`, `systemd/README.md`, `tools/sleelavirin_scan.py` (lines 205-275), `tools/sleela-antivirus-ctl`, `.gitignore`.

Commands run: `bin/sleelactl help|status|--dry-run start antivirus` (also with `SLEELACTL_SERVICE_MANAGER=launchd|none`), `make -C antivirus`, `make test`, the four `build/test_*` binaries, `build/sleelavirin-scan`, `sleelavirin_scan.py`, `sleela-antivirus-ctl.py status|validate-host --json|policy|--help`, `cmp README.md Makefile`.

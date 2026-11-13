<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# wifi-common: Explanation

| | |
|---|---|
| Nature | protocol (`bin/sleelactl help wifi-common`: verbs `negotiate grade simulate validate info`) |
| Registry summary | Shared Wi-Fi contracts |
| Lives in | `wifi-common/` (`include/sleela_wifi/`, `src/`, `tests/`, `Makefile`, `README.md`, `SECURITY.md`) |
| Writeup type | Explanation, because its real purpose (scan, connect, disconnect through the OS Wi-Fi stack) cannot be operated in this sandbox: there is no `nmcli`, so only the policy core and its contract tests run. |
| Verified | 2026-11-13 against `master` e048707fd: `make test` passes; all five source variants compile; a scratch program driving the policy core with a fake backend gave the results below; the real backend fails with `sh: nmcli: command not found`. |

## What it is

A client-side Wi-Fi control layer shared by the Wi-Fi 4, 5 and 6 modules. It offers a C ABI and a C++17 wrapper to scan, pick a result, apply a fail-closed security policy, request a connection and disconnect. The README says it is "not an 802.11 PHY/MAC implementation or monitor-mode packet sniffer" (`wifi-common/README.md`). It does not model any IEEE amendment. The design (policy struct, rank scale, "latest scan must match" rule, provider contract) is this project's own. The actual radio work is delegated to the OS: NetworkManager's `nmcli` on Linux, the Native Wi-Fi API on Windows, CoreWLAN on macOS.

## How it works

The core is `src/wifi_manager.c` (226 lines) around a `wifi_manager_t` that holds a backend (`wifi_backend_t`: name, `scan`, `connect`, `disconnect` function pointers), a policy and the last scan (up to `WIFI_MAX_SCAN_RESULTS` = 128 entries).

1. **Init** (`wifi_manager_init`): rejects NULL backend or missing callbacks; copies the given policy or the default; rejects a policy whose flags are not 0/1 or whose rank is outside 0..5.
2. **Scan** (`wifi_manager_scan`): clears the cache, calls the backend, drops entries that fail `valid_network` (empty or control-character SSID, malformed 17-character BSSID, signal over 100, unknown security), and stores the rest. Returns `BUFFER_TOO_SMALL` when the caller's array is smaller than the number kept.
3. **Select** (`wifi_manager_select`): copies entry `index` out of the cache.
4. **Connect** (`wifi_manager_connect`): the network passed in must match a cached scan entry on SSID, case-insensitive BSSID and security, else `NETWORK_NOT_FOUND` (this blocks forged or stale selections). Then policy checks, then per-auth-mode argument checks, then the backend call.
5. **Disconnect**: forwards to the backend.

Security ranks (`wifi_security_rank`): Open 0, WEP 1, WPA 3, WPA2 and OWE 4, WPA3 5, unknown -1. Default policy: minimum rank 4, no open networks, no WEP, enterprise TLS validation required, protected management frames (PMF) not required.

Observed with a fake backend (scratch program, not kept; the core compiled from `src/wifi_manager.c`):

| Case | Result |
|---|---|
| WPA2-Personal, OS-managed auth, default policy | ok |
| WPA-Personal / WEP / Open, default policy | blocked by Wi-Fi security policy |
| OWE, default policy | ok |
| WPA2-Personal with 5-byte passphrase | invalid argument |
| WPA2-Personal with 14-byte passphrase | ok |
| WPA2-Enterprise, user+password but no CA file / server name | blocked by Wi-Fi security policy |
| WPA2-Enterprise with CA file and server name | ok |
| PMF required, scan reports PMF unknown (-1) | blocked by Wi-Fi security policy |
| PMF required, scan reports PMF yes | ok |

Passphrase rules in code: 8..63 bytes, or exactly 64 hex digits as a raw PSK (rejected for WPA3-Personal).

### Providers

| Provider | How it works (from the source) |
|---|---|
| `wifi_backend_nmcli.c` (Linux) | Scan runs the constant command `nmcli --terse --escape yes --fields SSID,BSSID,CHAN,SIGNAL,SECURITY,IN-USE device wifi list --rescan yes` through `popen`. Connect uses `fork`/`execvp` with an argument vector (no shell). Passphrase and enterprise-password modes return `AUTH_REQUIRED`: secrets never go on a command line. It sets `protected_management_frames = -1` always, so a policy requiring PMF can never connect on Linux. It never sets `frequency_mhz`. |
| `wifi_backend_windows.c` (327 lines) | Native Wi-Fi API. Not compiled or run here (no Windows SDK). |
| `wifi_backend_macos.mm` (98 lines) | CoreWLAN. Not compiled or run here. It sets `frequency_mhz = 0`. |
| `wifi_backend_unavailable.c` | Every call returns `BACKEND_UNAVAILABLE`. Compiled here without error. |

Only one provider file is linked into a build; each defines `wifi_backend_default()`.

## Where it lives

| File | Role |
|---|---|
| `include/sleela_wifi/wifi_manager.h` | C ABI: types, status codes, policy defaults, function list |
| `include/sleela_wifi/wifi_manager_cpp.hpp`, `src/wifi_manager_cpp.cpp` | C++17 `sleela::wifi::Manager`, `Network`, `Credentials` (best-effort zeroing of strings on destruction) |
| `src/wifi_manager.c` | Policy, validation, selection |
| `src/wifi_backend_*.c / .mm` | OS providers listed above |
| `tests/test_wifi_manager.c` | Contract tests with a fake backend |
| `Makefile` | `all` (archive), `test`, `clean` |
| `SECURITY.md` | Defaults, authorized red/blue team guidance |

## Status and limits

Evidence from commands run:

- `make test` (also what `bin/sleelactl validate wifi-common` runs): compiles `src/wifi_manager.c` with `tests/test_wifi_manager.c` into `wifi-manager-test` and prints `wifi manager contract tests: PASS`. The tests cover defaults, a forged BSSID, an invalid policy flag and an open network given a passphrase.
- `src/wifi_backend_nmcli.c`, `src/wifi_backend_unavailable.c` and `src/wifi_manager_cpp.cpp` all compile warning-free with the documented flags (`-std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Wpedantic`, `-std=c++17`).
- The real Linux flow does not run here: the Wi-Fi 4/5/6 example programs (which link this code) print `sh: line 1: nmcli: command not found` then `native Wi-Fi backend unavailable`. No scan, connect or disconnect was possible. Windows and macOS providers were not built.
- `make` (target `all`) builds `libsleela_wifi.a` from `wifi_manager.o` only. The archive contains the core functions but no backend and no C++ wrapper (`nm libsleela_wifi.a` lists no `wifi_backend_default`), so linking against it alone leaves `wifi_backend_default` unresolved. The Makefile has a rule for `wifi_manager_cpp.o` but nothing depends on it. The README does not describe the archive; it tells you to compile the sources directly.
- `bin/sleelactl negotiate|grade wifi-common` print "no standalone ... CLI is built" and exit 3, pointing at a "negotiate/grade section in wifi-common/README.md" that does not exist. `simulate` prints "no simulator registered (sim= unset)" and exits 3. `info` works.
- `make test` leaves the untracked file `wifi-common/wifi-manager-test` (not covered by `.gitignore`; `*.o` and `*.a` are).
- Windows/macOS behavior described in the README is unverified here.

## Related

- [wi-fi-4](wi-fi-4.md), [wi-fi-5](wi-fi-5.md), [wi-fi-6](wi-fi-6.md): generation profiles and thin shims that include this API.
- [`wifi-common/SECURITY.md`](../../../wifi-common/SECURITY.md), [`wifi-common/README.md`](../../../wifi-common/README.md)

## Sources

Read: `wifi-common/README.md`, `SECURITY.md`, `Makefile`, `include/sleela_wifi/wifi_manager.h`, `wifi_manager_cpp.hpp`, `src/wifi_manager.c`, `src/wifi_backend_nmcli.c`, `src/wifi_backend_unavailable.c`, `src/wifi_manager_cpp.cpp`, `tests/test_wifi_manager.c`; line counts only for the Windows and macOS providers; `bin/sleelactl.d/registry.conf` (line 58).
Run: `make test`, `make`, `nm libsleela_wifi.a`, manual compiles of the nmcli/unavailable/C++ sources, a scratch policy program in `/tmp`, `bin/sleelactl info|validate|negotiate|grade|simulate wifi-common`, the example programs of `wi-fi-4/5/6`.

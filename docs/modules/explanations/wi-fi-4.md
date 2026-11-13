<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# wi-fi-4: Explanation

| | |
|---|---|
| Nature | protocol (`bin/sleelactl help wi-fi-4`: verbs `negotiate grade simulate validate info`) |
| Registry summary | Wi-Fi 4 (802.11n) model |
| Lives in | `wi-fi-4/` (`README.md`, `BUILDING.md`, `TECHNICAL-SPECIFICATIONS.md`, `src/c/`, `src/cpp/`, `src/java/`; no Makefile) |
| Writeup type | Explanation, because the module is static reference metadata about IEEE 802.11n (a profile printer), and its one operational part, the Wi-Fi manager example, cannot run here (no `nmcli`). |
| Verified | 2026-11-13 against `master` e048707fd: C, C++ and Java profile demos build with `-Werror` and run; C and C++ manager examples build but fail at scan with `nmcli: command not found`; `sleelactl validate/negotiate/grade/simulate` do nothing. |

## What it is

A "Wi-Fi 4 foundation": documentation plus a tiny profile library in C, C++ and Java that returns fixed facts about 802.11n, plus a one-line shim to the shared manager in [wifi-common](wifi-common.md). `README.md` says the source files "provide static reference metadata and simple capability-profile checks" and do not implement PHY/MAC, scan, or configure adapters. It also says implementation details "must distinguish verified standard requirements from proposed SLeeLa design choices".

## What models the standard and what is SLeeLa's own

| Part | Standard-level (as the module's files state it) | SLeeLa's own |
|---|---|---|
| IEEE 802.11n-2009 "HT", Wi-Fi 4 label, 2.4 + 5 GHz | `TECHNICAL-SPECIFICATIONS.md` | |
| 20/40 MHz, up to 4 spatial streams, 64-QAM at 5/6, A-MPDU/A-MSDU, short guard interval, 600 Mbit/s peak (4 streams, 40 MHz, short GI) | `TECHNICAL-SPECIFICATIONS.md`, repeated as constants in `wifi_profile.c`, `wifi_profile.cpp`, `WifiProfile.java` | |
| Profile struct (`wifi_profile_t`), band and width bit masks, `wifi_profile_supports_band/width_mhz` | | yes |
| Manager shim and the shared API | | yes (see wifi-common) |

The standard facts are quoted from the module's own documents. They were not checked here against the IEEE text (`TECHNICAL-SPECIFICATIONS.md` itself says to verify against the applicable IEEE text before implementing). The spec file states that SLeeLa "does **not** claim that SLeeLa implements an 802.11n radio or MAC stack".

## How it works

- `src/c/wifi_profile.c`: one constant `wifi_profile_t` ("Wi-Fi 4", "IEEE 802.11n", band mask 3 = 2.4+5 GHz, width mask 3 = 20+40 MHz, 4 streams, 64-QAM, 600 Mbit/s, four feature strings). `wifi_profile_supports_band(band)` tests the band mask; `wifi_profile_supports_width_mhz(w)` maps 20/40/80/160 to a bit; `wifi_profile_print()` prints it.
- `src/cpp/wifi_profile.cpp` and `src/java/WifiProfile.java` hold the same values as a `Profile` struct / class.
- `src/c/wifi_manager.c`: 3 lines. It includes the shared header (by the relative path `../../../wifi-common/include/sleela_wifi/wifi_manager.h`) and defines `sleela_wifi_4_generation()` returning the profile's generation string. The C++ equivalent is `sleela::wifi::v4::generation()`.
- `wifi_manager_example.c` / `.cpp`: scan, list networks, ask for an index on stdin, connect with OS-managed auth, using wifi-common.

Queried through the C functions (scratch program): 2.4 GHz yes, 5 GHz yes, 6 GHz no; widths 20 yes, 40 yes, 80 no, 160 no.

## Where it lives

| File | Role |
|---|---|
| `README.md` | Scope and status ("Research and reference-source foundation") |
| `TECHNICAL-SPECIFICATIONS.md` | 802.11n facts, peak-rate qualification, security notes, references |
| `BUILDING.md` | Build commands for profile demos and manager examples |
| `src/c/{wifi_profile,example}.c/.h` | C profile and demo |
| `src/cpp/{wifi_profile,example}.cpp/.hpp` | C++ profile and demo |
| `src/java/{WifiProfile,WifiProfileDemo}.java` | Java profile and demo |
| `src/{c,cpp}/wifi_manager*.{c,cpp,h,hpp}` | Shim and example for the shared manager |

## Status and limits

Commands (run from `wi-fi-4/`; outputs written to a scratch dir so nothing was left in the repo):

```sh
timeout 120 cc -std=c11 -Wall -Wextra -Wpedantic -Werror -Isrc/c src/c/wifi_profile.c src/c/example.c -o wifi-profile-c && ./wifi-profile-c
```
```
Wi-Fi 4 (IEEE 802.11n)
Bands: 2.4 GHz and 5 GHz, subject to hardware and regulatory support
Maximum theoretical PHY rate: 600 Mbit/s (not application throughput)
Maximum standard spatial streams: 4; highest listed modulation: 64-QAM
Reference features:
  - MIMO spatial streams
  - OFDM
  - A-MPDU/A-MSDU aggregation
  - Short guard interval

40 MHz profile support: yes
Query the operating system/driver for actual adapter capabilities.
```

- The documented C++ (`c++ -std=c++17 ... -Werror`) and Java (`javac -Xlint:all -Werror`, `java WifiProfileDemo`) builds also succeed, and the C++ and Java demos print the same text as the C one (checked by `diff` for Java vs C++).
- The manager examples build exactly as in `BUILDING.md` (C one-liner and the C-objects-then-C++ link) with no errors. Running them with stdin closed: the C one prints `sh: line 1: nmcli: command not found` / `native Wi-Fi backend unavailable` and exits 2; the C++ one prints the same and exits 1. No scan or connect ever happens in this sandbox.
- `bin/sleelactl validate wi-fi-4`: `wi-fi-4 has no validate/syntax/test target; nothing to run` (exit 3). There is no test code in the directory.
- `negotiate` / `grade`: "no standalone ... CLI is built" (exit 3), pointing to a section in `wi-fi-4/README.md` that does not exist. `simulate`: "no simulator registered (sim= unset)" (exit 3). `info` works (lists the three docs).
- `BUILDING.md` says the new management API "adds actual adapter discovery and connection requests"; true of the shared code, but unverifiable here.
- Nothing here models 802.11n signalling, MIMO, aggregation or rates. The 600 Mbit/s figure is a constant, not computed from streams, width or MCS.

## Related

[wifi-common](wifi-common.md) (the shared manager), [wi-fi-5](wi-fi-5.md), [wi-fi-6](wi-fi-6.md), [`wi-fi-4/BUILDING.md`](../../../wi-fi-4/BUILDING.md), [`wi-fi-4/TECHNICAL-SPECIFICATIONS.md`](../../../wi-fi-4/TECHNICAL-SPECIFICATIONS.md).

## Sources

Read: `wi-fi-4/README.md`, `BUILDING.md`, `TECHNICAL-SPECIFICATIONS.md`, all of `src/c`, `src/cpp` diffs against wi-fi-5/6, `src/java/WifiProfile.java` (via diff), `bin/sleelactl.d/registry.conf` (line 59).
Run: the three profile builds and demos, both manager builds and runs, a scratch band/width query, `bin/sleelactl info|validate|negotiate|grade|simulate wi-fi-4`.

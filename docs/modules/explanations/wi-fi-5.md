<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# wi-fi-5: Explanation

| | |
|---|---|
| Nature | protocol (`bin/sleelactl help wi-fi-5`: verbs `negotiate grade simulate validate info`) |
| Registry summary | Wi-Fi 5 (802.11ac) model |
| Lives in | `wi-fi-5/` (`README.md`, `BUILDING.md`, `TECHNICAL-SPECIFICATIONS.md`, `src/c/`, `src/cpp/`, `src/java/`; no Makefile) |
| Writeup type | Explanation, because the module is static reference metadata about IEEE 802.11ac (a profile printer), and its one operational part, the Wi-Fi manager example, cannot run here (no `nmcli`). |
| Verified | 2026-11-13 against `master` e048707fd: C, C++ and Java profile demos build with `-Werror` and run; C and C++ manager examples build but fail at scan with `nmcli: command not found`; `sleelactl validate/negotiate/grade/simulate` do nothing. |

## What it is

The Wi-Fi 5 sibling of [wi-fi-4](wi-fi-4.md): documentation plus a small constant-data profile library in C, C++ and Java and a shim onto [wifi-common](wifi-common.md). `README.md` says the sources provide "static reference metadata and simple capability-profile checks" and do not implement PHY/MAC, scan networks or configure adapters. The source tree differs from wi-fi-4 only in the profile values, the demo's width query (80 MHz instead of 40 MHz) and names (`diff -r wi-fi-4/src wi-fi-5/src`).

## What models the standard and what is SLeeLa's own

| Part | Standard-level (as the module's files state it) | SLeeLa's own |
|---|---|---|
| IEEE 802.11ac-2013 "VHT", Wi-Fi 5 label, primarily 5 GHz (a product using 2.4 GHz does so "through another 802.11 mode") | `TECHNICAL-SPECIFICATIONS.md` | |
| 20/40/80/160 MHz (incl. 80+80), up to 8 spatial streams, 256-QAM at 5/6, downlink MU-MIMO, beamforming, aggregation | `TECHNICAL-SPECIFICATIONS.md`; constants in `wifi_profile.c/.cpp`, `WifiProfile.java` | |
| Peak PHY rate: the spec says "approximately 6.9 Gbit/s" (8 streams, 160 MHz, short GI); the code constant is 6933 Mbit/s | both | |
| Profile struct, masks, support queries, manager shim | | yes |

Standard facts are as stated in the module's documents and were not checked here against IEEE text (the document itself tells readers to confirm against the IEEE text). The spec says SLeeLa "does **not** claim" to implement an 802.11ac PHY or MAC. Note that `TECHNICAL-SPECIFICATIONS.md` names the amendment "802.11ac-2013" while the code strings say just "IEEE 802.11ac".

## How it works

- `src/c/wifi_profile.c`: one constant profile: band mask 2 (5 GHz only), width mask 15 (20/40/80/160), 8 streams, 256-QAM, 6933 Mbit/s, features "Wide channels", "256-QAM", "Downlink MU-MIMO", "Beamforming", "A-MPDU/A-MSDU aggregation". `wifi_profile_supports_band` and `wifi_profile_supports_width_mhz` test those masks.
- C++ `Profile` and Java `WifiProfile` carry the same values (width list {20, 40, 80, 160}).
- `src/c/wifi_manager.c`: defines `sleela_wifi_5_generation()`; C++ has `sleela::wifi::v5::generation()`. The manager examples are identical to wi-fi-4's and use the shared API.

Queried through the C functions (scratch program): 2.4 GHz **no**, 5 GHz yes, 6 GHz no; widths 20, 40, 80, 160 all yes.

## Where it lives

| File | Role |
|---|---|
| `README.md`, `TECHNICAL-SPECIFICATIONS.md`, `BUILDING.md` | Scope, 802.11ac facts, build commands |
| `src/c/wifi_profile.{c,h}`, `src/c/example.c` | C profile and demo |
| `src/cpp/wifi_profile.{cpp,hpp}`, `src/cpp/example.cpp` | C++ profile and demo |
| `src/java/WifiProfile.java`, `WifiProfileDemo.java` | Java profile and demo |
| `src/c/wifi_manager*`, `src/cpp/wifi_manager*` | Shim and examples for wifi-common |

## Status and limits

```sh
cd wi-fi-5
timeout 120 cc -std=c11 -Wall -Wextra -Wpedantic -Werror -Isrc/c src/c/wifi_profile.c src/c/example.c -o wifi-profile-c && ./wifi-profile-c
```
```
Wi-Fi 5 (IEEE 802.11ac)
Bands: Primarily 5 GHz
Maximum theoretical PHY rate: 6933 Mbit/s (not application throughput)
Maximum standard spatial streams: 8; highest listed modulation: 256-QAM
Reference features:
  - Wide channels
  - 256-QAM
  - Downlink MU-MIMO
  - Beamforming
  - A-MPDU/A-MSDU aggregation

80 MHz profile support: yes
Query the operating system/driver for actual adapter capabilities.
```

(built into a scratch directory, so no binary was left in the repo)

- The C++ and Java profile builds from `BUILDING.md` also succeed with `-Werror` / `-Xlint:all -Werror`; both demos run and print output.
- Both manager examples compile as documented; run with no stdin they print `sh: line 1: nmcli: command not found` and `native Wi-Fi backend unavailable` (exit 2 for C, 1 for C++). No scan or connect was possible.
- `bin/sleelactl validate wi-fi-5`: "has no validate/syntax/test target; nothing to run" (exit 3). `negotiate` and `grade`: "no standalone ... CLI is built" (exit 3), referring to a README section that does not exist. `simulate`: "no simulator registered (sim= unset)" (exit 3). `info` works.
- The `BUILDING.md` note says Wi-Fi 5 "uses 5 GHz in normal 802.11ac operation", consistent with the code's 5-GHz-only mask.
- No test code exists. The rates, QAM order and stream counts are constants; nothing is computed from channel width or MCS.

## Related

[wifi-common](wifi-common.md), [wi-fi-4](wi-fi-4.md), [wi-fi-6](wi-fi-6.md), [`wi-fi-5/TECHNICAL-SPECIFICATIONS.md`](../../../wi-fi-5/TECHNICAL-SPECIFICATIONS.md), [`wi-fi-5/BUILDING.md`](../../../wi-fi-5/BUILDING.md).

## Sources

Read: `wi-fi-5/README.md`, `BUILDING.md`, `TECHNICAL-SPECIFICATIONS.md`; `diff -r wi-fi-4/src wi-fi-5/src`; `bin/sleelactl.d/registry.conf` (line 60).
Run: three profile builds and demos, two manager builds and runs, a scratch band/width query, `bin/sleelactl info|validate|negotiate|grade|simulate wi-fi-5`.

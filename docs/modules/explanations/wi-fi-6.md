<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# wi-fi-6: Explanation

| | |
|---|---|
| Nature | protocol (`bin/sleelactl help wi-fi-6`: verbs `negotiate grade simulate validate info`) |
| Registry summary | Wi-Fi 6 (802.11ax) model |
| Lives in | `wi-fi-6/` (`README.md`, `BUILDING.md`, `TECHNICAL-SPECIFICATIONS.md`, `src/c/`, `src/cpp/`, `src/java/`; no Makefile) |
| Writeup type | Explanation, because the module is static reference metadata about IEEE 802.11ax (a profile printer), and its one operational part, the Wi-Fi manager example, cannot run here (no `nmcli`). |
| Verified | 2026-11-13 against `master` e048707fd: C, C++ and Java profile demos build with `-Werror` and run; C and C++ manager examples build but fail at scan with `nmcli: command not found`; `sleelactl validate/negotiate/grade/simulate` do nothing. |

## What it is

The Wi-Fi 6 sibling of [wi-fi-4](wi-fi-4.md) and [wi-fi-5](wi-fi-5.md): documentation plus a constant-data profile library (C, C++, Java) and a shim onto [wifi-common](wifi-common.md). `README.md` says the files provide static reference metadata and "do not implement the radio PHY/MAC, scan for wireless networks, or configure adapters". `BUILDING.md` says it is "not a PHY/MAC implementation or monitor-mode packet sniffer".

## What models the standard and what is SLeeLa's own

| Part | Standard-level (as the module's files state it) | SLeeLa's own |
|---|---|---|
| IEEE 802.11ax-2021 "HE"; 2.4 + 5 GHz; Wi-Fi 6E adds 6 GHz where permitted | `TECHNICAL-SPECIFICATIONS.md` | |
| 20/40/80/160 MHz, uplink/downlink OFDMA, uplink/downlink MU-MIMO, 1024-QAM, BSS coloring/spatial reuse, Target Wake Time | `TECHNICAL-SPECIFICATIONS.md`; names in `wifi_profile.c/.cpp`, `WifiProfile.java` | |
| Peak PHY rate: spec says "approximately 9.6 Gbit/s" (8 streams, 160 MHz); code constant is 9608 Mbit/s | both | |
| Profile struct, masks, support queries, manager shim | | yes |

Standard facts are as stated in the module's documents; they were not checked against IEEE text here. The spec says SLeeLa "does **not** claim" to implement an 802.11ax PHY/MAC or certification. OFDMA appears only as a feature name. The scheduling idea behind it is not coded here. For a generic (non-802.11ax) OFDMA resource-grid and scheduler model, see the [ofdma](../tutorials/ofdma.md) module, which has no code link to this one.

## How it works

- `src/c/wifi_profile.c`: constant profile "Wi-Fi 6", "IEEE 802.11ax", bands string "2.4 GHz and 5 GHz; 6 GHz for Wi-Fi 6E where permitted", band mask 3, width mask 15, 8 streams, 1024-QAM, 9608 Mbit/s, five feature strings.
- C++ `Profile` and Java `WifiProfile` carry the same values. The C++ header `wifi_profile.hpp` is written compactly on one line, unlike its wi-fi-4/5 counterparts; behavior is the same.
- `wifi_manager.c` defines `sleela_wifi_6_generation()` (C++: `sleela::wifi::v6::generation()`); examples use the shared API.

Queried through the C functions (scratch program): 2.4 GHz yes, 5 GHz yes, **6 GHz no**; widths 20, 40, 80, 160 all yes. So the string and the spec mention 6E, but the band mask has no 6 GHz bit set (`WIFI_BAND_6_GHZ` exists in the enum). This is consistent with the text "6 GHz for Wi-Fi 6E where permitted" only if one reads the mask as "Wi-Fi 6 proper".

## Where it lives

| File | Role |
|---|---|
| `README.md`, `TECHNICAL-SPECIFICATIONS.md`, `BUILDING.md` | Scope, 802.11ax facts, build commands |
| `src/c/wifi_profile.{c,h}`, `src/c/example.c` | C profile and demo |
| `src/cpp/wifi_profile.{cpp,hpp}`, `src/cpp/example.cpp` | C++ profile and demo |
| `src/java/WifiProfile.java`, `WifiProfileDemo.java` | Java profile and demo |
| `src/c/wifi_manager*`, `src/cpp/wifi_manager*` | Shim and examples for wifi-common |

## Status and limits

```sh
cd wi-fi-6
timeout 120 cc -std=c11 -Wall -Wextra -Wpedantic -Werror -Isrc/c src/c/wifi_profile.c src/c/example.c -o wifi-profile-c && ./wifi-profile-c
```
```
Wi-Fi 6 (IEEE 802.11ax)
Bands: 2.4 GHz and 5 GHz; 6 GHz for Wi-Fi 6E where permitted
Maximum theoretical PHY rate: 9608 Mbit/s (not application throughput)
Maximum standard spatial streams: 8; highest listed modulation: 1024-QAM
Reference features:
  - OFDMA uplink/downlink
  - MU-MIMO uplink/downlink
  - 1024-QAM
  - BSS coloring/spatial reuse
  - Target Wake Time

80 MHz profile support: yes
Query the operating system/driver for actual adapter capabilities.
```

(built into a scratch directory, so no binary was left in the repo)

- The C++ and Java profile builds from `BUILDING.md` also succeed with `-Werror` / `-Xlint:all -Werror` and run.
- Both manager examples compile as documented; run with no stdin they print `sh: line 1: nmcli: command not found` and `native Wi-Fi backend unavailable` (exit 2 for C, 1 for C++). No scan or connect was possible.
- `bin/sleelactl validate wi-fi-6`: "has no validate/syntax/test target; nothing to run" (exit 3). `negotiate` and `grade`: "no standalone ... CLI is built" (exit 3), referring to a README section that does not exist. `simulate`: "no simulator registered (sim= unset)" (exit 3). `info` works.
- No tests. Nothing models OFDMA, MU-MIMO, BSS coloring or TWT behavior; the 9608 figure is a constant. `BUILDING.md` correctly warns these features need hardware and peer support.

## Related

[wifi-common](wifi-common.md), [wi-fi-4](wi-fi-4.md), [wi-fi-5](wi-fi-5.md), [ofdma](../tutorials/ofdma.md), [`wi-fi-6/TECHNICAL-SPECIFICATIONS.md`](../../../wi-fi-6/TECHNICAL-SPECIFICATIONS.md), [`wi-fi-6/BUILDING.md`](../../../wi-fi-6/BUILDING.md).

## Sources

Read: `wi-fi-6/README.md`, `BUILDING.md`, `TECHNICAL-SPECIFICATIONS.md`; `diff -r wi-fi-4/src wi-fi-6/src`; `bin/sleelactl.d/registry.conf` (line 61).
Run: three profile builds and demos, two manager builds and runs, a scratch band/width query, `bin/sleelactl info|validate|negotiate|grade|simulate wi-fi-6`.

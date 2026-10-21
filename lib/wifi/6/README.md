# SLeeLa Wi-Fi 6

SLeeLa-native reference package for Wi-Fi 6 / IEEE 802.11ax (HE).

## Files
- Wifi6Model.sleela — capability metadata and conservative profile checks.
- wifi.conf — human-editable static defaults.
- capabilities.json — machine-readable standard profile.
- WifiManager.contract.json — contract for the SLeeLa Wi-Fi OOD.
- src/c/Wifi6Manager.h and Wifi6Manager.c — C OOD adapter.
- src/cpp/Wifi6Manager.hpp and Wifi6Manager.cpp — C++17 adapter.
- examples/ValidateWifi6.sleela — model validation demonstration.

## Profile
Wi-Fi 6 covers 2.4 GHz and 5 GHz. Wi-Fi 6E extends Wi-Fi 6 operation into 6 GHz where permitted. Supported widths include 20/40/80/160 MHz; up to eight spatial streams; up to 1024-QAM in supported modes; commonly cited theoretical aggregate peak PHY rate is around 9.6 Gbit/s.

OFDMA, uplink/downlink MU-MIMO, BSS coloring/spatial reuse and Target Wake Time are important 802.11ax features. This package is metadata and validation only; it does not implement those radio/MAC functions.

The Wifi6Manager adapters use the shared manager API in [wifi-common](../../../wifi-common/README.md). They provide generation-specific naming and initialize the native provider; scan, select, authentication, connect and disconnect are implemented in the shared C/C++ manager. Wi-Fi 6E operation still depends on adapter/AP capability and local regulation. See the [technical specification](../../../wi-fi-6/TECHNICAL-SPECIFICATIONS.md) and [reference sources](../../../wi-fi-6/).

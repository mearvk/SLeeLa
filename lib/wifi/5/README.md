# SLeeLa Wi-Fi 5

SLeeLa-native reference package for Wi-Fi 5 / IEEE 802.11ac (VHT).

## Files
- Wifi5Model.sleela — capability metadata and conservative profile checks.
- wifi.conf — human-editable static defaults.
- capabilities.json — machine-readable standard profile.
- WifiManager.contract.json — contract for the SLeeLa Wi-Fi OOD.
- src/c/Wifi5Manager.h and Wifi5Manager.c — C OOD adapter.
- src/cpp/Wifi5Manager.hpp and Wifi5Manager.cpp — C++17 adapter.
- examples/ValidateWifi5.sleela — model validation demonstration.

## Profile
Wi-Fi 5 operates primarily in 5 GHz; channel widths include 20/40/80/160 MHz; up to eight spatial streams; up to 256-QAM in supported modes; the commonly quoted theoretical peak PHY rate is about 6.9 Gbit/s.

Theoretical maxima require specific channel widths, stream counts, modulation/coding and guard interval. Runtime discovery and regulatory checks remain mandatory.

The SLeeLa-facing classes are available in [SLWifi5Manager](../SLWifi5Manager.sleela), with the shared model and policy types in the parent /lib/wifi directory. The Wifi5Manager C/C++ adapters use the shared manager API in [wifi-common](../../../wifi-common/README.md). They provide generation-specific naming and initialize the native provider; scan, select, authentication, connect and disconnect are implemented in the shared C/C++ manager. See the [technical specification](../../../wi-fi-5/TECHNICAL-SPECIFICATIONS.md) and [reference sources](../../../wi-fi-5/).

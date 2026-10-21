# SLeeLa Wi-Fi 4

SLeeLa-native reference package for Wi-Fi 4 / IEEE 802.11n (HT).

## Files
- Wifi4Model.sleela — capability metadata and conservative profile checks.
- wifi.conf — human-editable static defaults.
- capabilities.json — machine-readable standard profile.
- WifiManager.contract.json — contract for the SLeeLa Wi-Fi OOD.
- src/c/Wifi4Manager.h and Wifi4Manager.c — C OOD adapter.
- src/cpp/Wifi4Manager.hpp and Wifi4Manager.cpp — C++17 adapter.
- examples/ValidateWifi4.sleela — model demonstration.

## Profile
2.4 GHz and 5 GHz where supported; 20/40 MHz channels; up to four spatial streams; OFDM and up to 64-QAM; commonly quoted theoretical peak PHY rate of 600 Mbit/s under a highest-rate four-stream, 40 MHz, short-guard-interval configuration.

These values describe the standard's capability envelope, not actual hardware. Do not infer supported channels, transmit power, DFS behavior or regulatory permissions from this profile alone.

The SLeeLa-facing classes are available in [SLWifi4Manager](../SLWifi4Manager.sleela), with the shared model and policy types in the parent /lib/wifi directory. The Wifi4Manager C/C++ adapters use the shared manager API in [wifi-common](../../../wifi-common/README.md). They provide generation-specific naming and initialize the native provider; scan, select, authentication, connect and disconnect are performed by the shared C/C++ manager. See the [technical specification](../../../wi-fi-4/TECHNICAL-SPECIFICATIONS.md) and [reference sources](../../../wi-fi-4/).

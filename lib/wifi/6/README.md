# SLeeLa Wi-Fi 6

SLeeLa-native reference package for Wi-Fi 6 / IEEE 802.11ax (HE).

## Files

- Wifi6Model.sleela — capability metadata and conservative profile checks.
- wifi.conf — human-editable static defaults.
- capabilities.json — machine-readable standard profile.
- WifiManager.contract.json — contract for the SLeeLa Wi-Fi OOD.
- examples/ValidateWifi6.sleela — model validation demonstration.

## Profile

Wi-Fi 6 covers 2.4 GHz and 5 GHz. Wi-Fi 6E extends Wi-Fi 6 operation into 6 GHz where permitted. Supported widths include 20/40/80/160 MHz; up to eight spatial streams; up to 1024-QAM in supported modes; commonly cited theoretical aggregate peak PHY rate is around 9.6 Gbit/s.

OFDMA, uplink/downlink MU-MIMO, BSS coloring/spatial reuse and Target Wake Time are important 802.11ax features. This package is metadata and validation only; it does not implement those radio/MAC functions.

The native management implementation is shared across generations: see the [C/C++ API and platform providers](../../../wifi-common/README.md), [technical specification](../../../wi-fi-6/TECHNICAL-SPECIFICATIONS.md), and [reference sources](../../../wi-fi-6/).

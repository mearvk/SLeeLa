# SLeeLa Wi-Fi 5

SLeeLa-native reference package for Wi-Fi 5 / IEEE 802.11ac (VHT).

## Files

- Wifi5Model.sleela — capability metadata and conservative profile checks.
- wifi.conf — human-editable static defaults.
- capabilities.json — machine-readable standard profile.
- WifiManager.contract.json — contract for the SLeeLa Wi-Fi OOD.
- examples/ValidateWifi5.sleela — model validation demonstration.

## Profile

Wi-Fi 5 operates primarily in 5 GHz; channel widths include 20/40/80/160 MHz; up to eight spatial streams; up to 256-QAM in supported modes; the commonly quoted theoretical peak PHY rate is about 6.9 Gbit/s.

Theoretical maxima require specific channel widths, stream counts, modulation/coding and guard interval. Runtime discovery and regulatory checks remain mandatory.

The native management implementation is shared across generations: see the [C/C++ API and platform providers](../../../wifi-common/README.md), [technical specification](../../../wi-fi-5/TECHNICAL-SPECIFICATIONS.md), and [reference sources](../../../wi-fi-5/).

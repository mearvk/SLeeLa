# SLeeLa Wi-Fi 4

SLeeLa-native reference package for Wi-Fi 4 / IEEE 802.11n (HT).

## Files

- Wifi4Model.sleela — capability metadata and conservative profile checks.
- wifi.conf — human-editable static defaults.
- capabilities.json — machine-readable standard profile.
- WifiManager.contract.json — contract for the SLeeLa Wi-Fi OOD.
- examples/ValidateWifi4.sleela — small model demonstration.

## Profile

2.4 GHz and 5 GHz where supported; 20/40 MHz channels; up to four spatial streams; OFDM and up to 64-QAM; commonly quoted theoretical peak PHY rate of 600 Mbit/s under a highest-rate 4-stream, 40 MHz, short-guard-interval configuration.

These values describe the standard's capability envelope, not actual hardware. Do not infer supported channels, transmit power, DFS behavior or regulatory permissions from this profile alone.

The native management implementation is shared across generations: see the [C/C++ API and platform providers](../../../wifi-common/README.md), [technical specification](../../../wi-fi-4/TECHNICAL-SPECIFICATIONS.md), and [reference sources](../../../wi-fi-4/).

# SLeeLa Wi-Fi 6

SLeeLa-native reference package for **Wi-Fi 6 / IEEE 802.11ax (HE)**.

## Files

- `Wifi6Model.sleela` — capability metadata and conservative profile checks.
- `wifi.conf` — human-editable static defaults.
- `capabilities.json` — machine-readable standard profile.
- `examples/ValidateWifi6.sleela` — small SLeeLa validation demonstration.

## Profile

Wi-Fi 6 covers 2.4 GHz and 5 GHz. Wi-Fi 6E extends Wi-Fi 6 operation into 6 GHz where permitted. Supported channel widths include 20/40/80/160 MHz; up to eight spatial streams; up to 1024-QAM in supported modes; theoretical aggregate peak PHY rate is commonly cited around 9.6 Gbit/s.

OFDMA, uplink/downlink MU-MIMO, BSS coloring/spatial reuse, and Target Wake Time are important 802.11ax features. This package is metadata and validation only; it does not implement those radio/MAC functions.

See the companion [technical specification](../../../wi-fi-6/TECHNICAL-SPECIFICATIONS.md) and [C/C++/Java reference sources](../../../wi-fi-6/).

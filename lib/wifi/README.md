<img align="right" src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">

# SLeeLa Wi-Fi Library

The SLeeLa Wi-Fi library combines static reference models for Wi-Fi 4, Wi-Fi 5 and Wi-Fi 6 with a shared C/C++ operating-system management API.

## Packages

- [Wi-Fi 4 / IEEE 802.11n](4/README.md) — [OOD contract](4/WifiManager.contract.json)
- [Wi-Fi 5 / IEEE 802.11ac](5/README.md) — [OOD contract](5/WifiManager.contract.json)
- [Wi-Fi 6 / IEEE 802.11ax](6/README.md) — [OOD contract](6/WifiManager.contract.json)

Each version retains its .sleela model, static wifi.conf, capabilities.json and validation example. Those files are descriptive metadata. The shared manager in [wifi-common](../../wifi-common/README.md) supplies scan, select, connect and disconnect methods over a native OS provider. It is designed as the control layer for a SLeeLa Wi-Fi OOD; it does not implement the 802.11 PHY/MAC.

## Safety and correctness

The C ABI and C++17 wrapper return SSID, BSSID, signal, channel and security classification. The manager validates a selection against the most recent scan and applies a fail-closed default policy: WPA2/OWE or stronger, with open networks and WEP rejected. Credentials are not written to logs or process arguments. Linux uses NetworkManager's secret agent or a saved profile; Windows and macOS use OS-managed profiles for protected/enterprise authentication.

A Wi-Fi generation label does not guarantee a device's adapter capabilities. Honor the adapter, driver, firmware and local regulatory domain. Wi-Fi 6 at 6 GHz is Wi-Fi 6E and must not be inferred from generic Wi-Fi 6 support.

See [security and authorized test guidance](../../wifi-common/SECURITY.md).

<img align="right" src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">

# SLeeLa Wi-Fi Library

The SLeeLa Wi-Fi library combines Wi-Fi 4/5/6 reference models with a shared native C/C++ management API and the matching SLeeLa object model for a Wi-Fi OOD.

## SLeeLa source classes

- SLWifiSecurity.sleela and SLWifiStatus.sleela — stable values aligned with the C enums.
- SLWifiNetwork.sleela — discovered access-point record.
- SLWifiCredentials.sleela — one-attempt credentials/profile request.
- SLWifiSecurityPolicy.sleela — default rank 4 policy; denies open and WEP.
- SLWifiManager.sleela — scan/selection/connection state facade with policy validation.
- SLWifiOOD.sleela — UI-independent connectivity panel/controller contract.
- SLWifi4Manager.sleela, SLWifi5Manager.sleela, SLWifi6Manager.sleela — generation-specific profile facades.
- examples/WifiOODPolicySmoke.sleela — policy and state contract example.

These classes model state, validation and UI/OOD behavior but cannot scan a radio on their own. A host SLVM Wi-Fi bridge must bind an wifi_manager_t instance and forward scan/connect/disconnect calls to the native API. Until that bridge is bound, scan requests return BRIDGE_REQUIRED, not simulated success. See WIFI.VM.BRIDGE.md for the VM handle and operation contract.

## Wi-Fi generation packages

- Wi-Fi 4 / IEEE 802.11n (4/README.md) — OOD contract (4/WifiManager.contract.json)
- Wi-Fi 5 / IEEE 802.11ac (5/README.md) — OOD contract (5/WifiManager.contract.json)
- Wi-Fi 6 / IEEE 802.11ax (6/README.md) — OOD contract (6/WifiManager.contract.json)

Each generation retains its .sleela profile, static wifi.conf, capabilities.json, and C/C++ adapters. They describe standard-level capabilities but do not claim that a physical adapter supports every feature.

## Native implementation

The shared manager in wifi-common/README.md provides scanning, selection, policy enforcement, credential-mode checking, connection and disconnection over platform OS providers. The Wi-Fi-native CI workflow compiles/tests Linux, macOS and Windows providers: see .github/workflows/wifi-native.yml.

## Safety and correctness

The C API and C++17 wrapper return SSID, BSSID, signal, channel and security classification. A connection selection must match the latest scan and defaults to WPA2/OWE or stronger; open networks and WEP are rejected. The Linux provider uses NetworkManager's trusted secret agent or saved profile. Windows and macOS use native Wi-Fi APIs with transient credential handling for personal networks; enterprise authentication remains profile-managed with server-certificate validation.

The generation label does not guarantee adapter capability. Honor the adapter, driver, firmware and local regulatory domain. Wi-Fi 6 at 6 GHz is Wi-Fi 6E and must not be inferred from generic Wi-Fi 6 support.

See wifi-common/SECURITY.md for security and authorized-testing guidance.

# SLeeLa Wi-Fi SLVM bridge contract

## Native ownership

The live adapter operations are implemented in wifi-common/ as the C wifi_manager_t API and the C++17 wrapper. The source objects under /lib/wifi are the SLeeLa-facing data model, policy layer, and OOD state facade. They do not fabricate successful radio results.

The current SLVM must register a platform bridge before an SLeeLa OOD can discover live access points. The bridge owns a bounded table of wifi_manager_t instances; SLWifiManager.nativeHandle is a VM-local index into that table, not a native pointer. Only the bridge may create/destroy the underlying manager or dereference its handle.

## Required operations

The VM/OS adapter maps these logical operations to the C ABI:

| SLeeLa OOD operation | Native C operation | Result |
|---|---|---|
| initialize with policy | wifi_security_policy_default, wifi_manager_init | VM-local manager handle or status |
| scan | wifi_manager_scan | current bounded result set and count |
| select result | wifi_manager_select | an exact network value copied into VM-owned memory |
| connect | wifi_manager_connect | native connection request status |
| disconnect | wifi_manager_disconnect | disconnection status |
| report security/status | wifi_security_name, wifi_status_name | UTF-8 diagnostic text |

Native scan results must be copied into VM-owned objects. Never expose pointers into the C manager's internal cache to SLeeLa code. Revalidate the selected SSID, BSSID and security using the native API at connection time. Treat all strings crossing the bridge as UTF-8 and length-bound them to the C ABI's 32-octet SSID and 17-character BSSID limits.

## Authentication boundary

- Prefer WIFI_AUTH_OS_MANAGED or a named OS-saved profile.
- Do not place a password in process arguments, logs, exception text or diagnostic metadata.
- The manager policy defaults to rank 4 (WPA2/OWE or stronger), denies open and WEP networks, and requires the enterprise server certificate and expected server name when direct enterprise fields are accepted.
- Native platform providers may require managed profiles even when the common C struct exposes additional fields. The bridge must report WIFI_STATUS_AUTH_REQUIRED, not silently downgrade security.
- Clear mutable native secret buffers after each attempt. SLeeLa's immutable String values can only have their references cleared; that is not a guarantee of physical memory erasure.

## State contract

SLWifiManager.beginScanRequest() enters SCANNING only after a native handle has been bound. The bridge reports completion through acceptScanResult(status, count). The UI must populate the list only from results returned by the current scan. A connection attempt first runs the SLeeLa policy checks, then passes the copied selection and credential mode to the native API. Only acceptConnectionResult(WIFI_STATUS_OK) may move the facade to CONNECTED.

The portable C API and native provider build/test workflow is .github/workflows/wifi-native.yml. Platform SDK, driver, permissions and regulatory state still govern what hardware can do.

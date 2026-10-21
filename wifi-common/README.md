# SLeeLa Wi-Fi Management Core (C11 / C++17)

This shared control layer sits above the Wi-Fi 4/5/6 reference profiles. It provides a stable C ABI and C++ wrapper for scanning, selecting an exact SSID/BSSID result, applying fail-closed security policy, requesting native authentication, connecting and disconnecting. It is a client-side control layer, not an 802.11 PHY/MAC implementation or monitor-mode packet sniffer.

## Layout

- include/sleela_wifi/wifi_manager.h — C ABI, data model, policy and provider contract.
- include/sleela_wifi/wifi_manager_cpp.hpp — C++17 Manager, Network and Credentials.
- src/wifi_manager.c — policy checks, selection and input validation.
- src/wifi_backend_nmcli.c — Linux/NetworkManager provider.
- src/wifi_backend_windows.c — Windows Native Wi-Fi API provider using configured OS profiles.
- src/wifi_backend_macos.mm — macOS CoreWLAN provider; enterprise auth requires a managed profile.
- tests/test_wifi_manager.c — platform-independent contract tests.

## Platform builds

Linux: compile src/wifi_manager.c and src/wifi_backend_nmcli.c with C11, -D_POSIX_C_SOURCE=200809L, -Iinclude.

Windows: compile src/wifi_manager.c and src/wifi_backend_windows.c with the Windows SDK; link wlanapi and ole32.

macOS: compile src/wifi_manager.c as C and src/wifi_backend_macos.mm as Objective-C++; link CoreWLAN, Foundation and Security frameworks, and link the final app with clang++.

Other targets: compile src/wifi_backend_unavailable.c to receive an explicit unsupported-provider status rather than a false success. A downstream OS adapter may implement the public wifi_backend_t contract.

Run the portable contract tests with make test. The Linux provider deliberately does not accept raw PSKs or EAP passwords: use a saved NetworkManager profile or its trusted secret agent / interactive prompt. The Windows provider also uses OS-managed profiles. This avoids exposing secrets in process arguments. Direct credential modes are available only to providers that can safely consume them.

## OOD flow

1. Initialize with wifi_manager_init(manager, wifi_backend_default(), NULL, policy).
2. Scan and render SSID, BSSID, signal and security.
3. Select a current result using wifi_manager_select.
4. Connect using OS-managed auth or an OS saved profile; direct credential modes are accepted only where the native provider safely supports them.
5. Disconnect using wifi_manager_disconnect.

Connect accepts a network only if its SSID, BSSID and security match the latest scan. The native provider must still revalidate current state. Regulatory rules and adapter capabilities remain with the operating system and firmware.

See SECURITY.md for defaults and authorized red/blue-team guidance.

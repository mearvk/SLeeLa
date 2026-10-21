# Wi-Fi Security and Authorized Testing

## Defensive defaults

- The manager defaults to WPA2/OWE or stronger, rejects open networks and WEP, and can require protected management frames when the platform reports support.
- WPA3 and 6 GHz capability is never inferred from a folder name. Actual adapter, access point, driver and regulatory-domain support controls availability.
- Enterprise connections must use an OS-managed profile or explicitly provide a CA certificate and expected RADIUS/server name. Never disable certificate validation to force a connection.
- Prefer WPA3-Personal (SAE) or WPA2-AES for small managed networks. Organizations should prefer WPA3/WPA2-Enterprise, EAP-TLS where practicable, per-user identities, managed certificates and validated RADIUS servers.
- Never log, print, commit, or put Wi-Fi passphrases in process arguments. Linux uses NetworkManager's secret agent or interactive prompt; Windows and macOS enterprise authentication should use OS-managed profiles.
- Use TLS with certificate verification for application traffic even when the Wi-Fi link is encrypted. Consider a VPN on untrusted networks.

## Authorized red-team work

Test only networks/devices you own or are explicitly authorized to assess, within a written scope and maintenance window. Prefer a staging SSID and test accounts. Document target APs, timing, and allowed techniques. Use benign checks for encryption settings, PMF, guest/client isolation, rogue-AP detection, firmware patch level, EAP certificate validation and credentials in logs. Stop and report unexpected disruption immediately.

This library deliberately excludes password guessing, credential spraying, WPA handshake cracking, packet injection, deauthentication, rogue-AP impersonation and interception of other people's traffic. Red-team simulations belong in an approved lab with documented authorization. Never test real users' credentials.

## Blue-team practices

Disable WPS where feasible; retire WEP, WPA/TKIP and shared passwords; use unique long passphrases or per-user enterprise credentials; protect RADIUS and management interfaces; rotate exposed secrets; segment guest/IoT networks; monitor unexpected BSSIDs/authentication failures; deploy rogue-AP/WIDS detection; patch AP firmware; and maintain an incident record. Hidden SSIDs and MAC filtering are not meaningful security controls by themselves.

## Credential lifetime

Use credentials for a single connection attempt and clear mutable C buffers with wifi_secure_zero. The C++ Credentials object clears owned string buffers at destruction; this is best-effort and cannot erase copies held by operating-system frameworks. Prefer native credential vaults and managed profiles.

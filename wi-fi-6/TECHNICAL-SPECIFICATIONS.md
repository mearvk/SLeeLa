# Wi-Fi 6 — Known Specifications And Technical Details

## Identity And Standards Basis

- **Wi-Fi generation:** Wi-Fi 6
- **IEEE amendment:** IEEE 802.11ax-2021 (High Efficiency WLAN, commonly HE)
- **Bands:** 2.4 GHz and 5 GHz for Wi-Fi 6; **Wi-Fi 6E** extends Wi-Fi 6 operation into 6 GHz where permitted.
- **Primary purpose:** Improve aggregate efficiency, latency behavior, and performance in dense networks, not merely peak speed.

Wi-Fi 6 and Wi-Fi 6E use 802.11ax capabilities. A 6 GHz-capable product is commonly described as Wi-Fi 6E; 6 GHz availability, power rules, and indoor/outdoor permissions vary by jurisdiction.

## Known Technical Capabilities

- **Channel widths:** 20, 40, 80, and 160 MHz, subject to band, hardware, and regulatory restrictions.
- **OFDMA:** Orthogonal Frequency-Division Multiple Access allows an access point to allocate resource units to multiple clients within a channel transmission; 802.11ax supports downlink and uplink OFDMA.
- **MU-MIMO:** Supports downlink and uplink multi-user MIMO in supported configurations.
- **Modulation:** Up to 1024-QAM in high-signal conditions and supported modes.
- **BSS coloring / spatial reuse:** Helps devices identify overlapping basic service sets and may allow more efficient reuse of spectrum when conditions and configuration permit.
- **Target Wake Time (TWT):** Provides negotiated wake/sleep scheduling intended to help reduce contention and conserve client power.
- **Longer OFDM symbol duration:** Improved robustness in some multipath conditions, with guard-interval choices that affect rate and robustness.
- **Legacy coexistence:** Supports coexistence with earlier 802.11 devices; mixed-generation operation can reduce the efficiency benefits available to newer clients.

## Peak Data Rate — Important Qualification

The often-quoted maximum theoretical PHY rate is approximately **9.6 Gbit/s**, based on an eight-spatial-stream, 160 MHz configuration and the highest supported modulation/coding assumptions. This is not typical single-client application throughput. Actual speed depends on client capability, channel width, signal quality, contention, interference, access-point scheduling, wired backhaul, and protocol overhead.

## Bands And Regulatory Constraints

Wi-Fi 6 on 2.4/5 GHz should be distinguished from Wi-Fi 6E, which enables 6 GHz operation. 6 GHz can provide additional spectrum and wider channels, but range, penetration, permitted power, and indoor/outdoor rules differ. Available channels and power levels are country-specific. Software should obtain supported capabilities from the OS/driver and must not hard-code a universal channel plan.

## Security And Interoperability

Wi-Fi 6 itself does not mean every network is configured securely. Wi-Fi 6 certification programs commonly require WPA3 support for Wi-Fi 6 devices, but actual network security still depends on device certification, configuration, and enabled mode. Prefer WPA3-Personal or WPA3-Enterprise where possible; use WPA2-AES only where compatibility requires it and the equipment supports it. Avoid WEP and WPA/TKIP. Secure application traffic independently with modern TLS and correct certificate validation.

## SLeeLa Implementation Notes

This document records known standard-level capabilities and does **not** claim that SLeeLa implements an 802.11ax PHY/MAC or Wi-Fi 6 certification. OFDMA scheduling, MU-MIMO, BSS coloring, and radio power-save behavior require compatible radios, firmware, drivers, and access-point/client cooperation; they are not features that a typical user-space application can independently enable through ordinary sockets.

For SLeeLa, prioritize platform network APIs, robust reconnection, DNS and route error handling, secure transport, connection diagnostics, and tests under congestion. Report negotiated link information only when the platform exposes it, and distinguish it from end-to-end throughput.

## Verification Checklist

- Distinguish Wi-Fi 6 from Wi-Fi 6E and verify supported bands on each endpoint.
- Query real device capabilities instead of assuming 160 MHz, eight streams, or 1024-QAM.
- Test dense-client performance, latency, packet loss, roaming, power-save behavior, and legacy coexistence.
- Test WPA3 and certificate validation where available.
- Measure application throughput separately from advertised PHY rates.
- Keep regulatory channel and transmit-power decisions in certified platform components.

## References

- IEEE SA, *IEEE 802.11-2020* and incorporated amendments: https://standards.ieee.org/standard/802_11-2020.html
- IEEE SA, *The Evolution of Wi-Fi Technology and Standards*: https://standards.ieee.org/beyond-standards/the-evolution-of-wi-fi-technology-and-standards/
- IEEE 802.11 Working Group, standards and amendment timelines: https://www.ieee802.org/11/Reports/802.11_Timelines.htm

---
**Status:** Research reference. Verify the applicable IEEE normative text, Wi-Fi Alliance certification requirements, local regulatory rules, and actual hardware support before implementation.

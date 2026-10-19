# Wi-Fi 5 — Known Specifications And Technical Details

## Identity And Standards Basis

- **Wi-Fi generation:** Wi-Fi 5
- **IEEE amendment:** IEEE 802.11ac-2013 (Very High Throughput, commonly VHT)
- **Published generation:** 2013
- **Primary band:** 5 GHz
- **Primary purpose:** Increase WLAN capacity and throughput through wider channels, higher-order modulation, and multi-user transmission features.

Wi-Fi 5 is the industry generation label for 802.11ac. Feature support varies by access point, client, chipset, firmware, channel, and regulatory domain. The original 802.11ac amendment is primarily a 5 GHz technology; a product that also uses 2.4 GHz commonly does so through another 802.11 mode.

## Known Technical Capabilities

- **Channel widths:** 20, 40, 80, and 160 MHz, including 80+80 MHz operation where supported.
- **MIMO:** Up to eight spatial streams in the standard's supported configurations.
- **Modulation:** Up to 256-QAM (with a 5/6 coding rate) in high-signal conditions and supported modes.
- **Downlink MU-MIMO:** An access point can transmit to multiple clients in parallel when compatible clients and scheduling conditions allow it.
- **Beamforming:** Standardized beamforming mechanisms can help focus transmission toward supported clients.
- **Frame aggregation:** A-MPDU/A-MSDU aggregation reduces overhead and improves efficiency.
- **Backward compatibility:** Coexists with earlier 802.11 networks using shared spectrum and protection mechanisms; actual performance depends on legacy-client activity and configuration.

## Peak Data Rate — Important Qualification

Common references quote approximately **6.9 Gbit/s** as the maximum theoretical PHY rate for an eight-spatial-stream, 160 MHz, short-guard-interval configuration. This is a theoretical maximum, not typical user throughput. Many client devices have one, two, or three spatial streams, and real-world performance is limited by signal quality, contention, interference, channel availability, and protocol overhead.

## Bands, Channels, And Regulatory Constraints

The 5 GHz band offers wider-channel options than many 2.4 GHz deployments, but 80/160 MHz channels consume more spectrum and may be more susceptible to congestion or unavailable in a given region. Some channels require Dynamic Frequency Selection (DFS) and radar avoidance. Channel availability, power limits, DFS behavior, and antenna rules are jurisdiction-dependent.

## Security And Interoperability

802.11ac is a radio/MAC amendment, not a guarantee that a product uses a particular security configuration. Prefer WPA3 when all required equipment supports it; WPA2 with AES-CCMP remains a compatibility option on supported systems. Avoid WEP and WPA/TKIP. For application traffic, use current TLS with certificate validation; Wi-Fi association security does not replace end-to-end security.

## SLeeLa Implementation Notes

This document describes known standard-level capabilities and does **not** claim that SLeeLa implements an 802.11ac PHY or MAC. Implementing radio behavior requires compatible hardware, firmware/driver support, regulatory approval, and conformance testing. Ordinary application-level network APIs expose connectivity and packet transport, not direct control of all 802.11ac radio features.

For SLeeLa applications, define platform-neutral connectivity status, network error handling, secure application protocols, diagnostics, and repeatable performance tests instead of assuming a particular adapter feature set.

## Verification Checklist

- Query actual client and access-point capabilities, including supported streams and channel widths.
- Test 5 GHz operation under the relevant regulatory domain.
- Test mixed-client airtime, DFS/channel changes, roaming, latency, and packet loss.
- Separate PHY/link-rate observations from measured application throughput.
- Validate TLS certificates and avoid storing Wi-Fi secrets in logs or source control.

## References

- IEEE SA, *IEEE 802.11-2020* and amendment history: https://standards.ieee.org/standard/802_11-2020.html
- IEEE SA, *The Evolution of Wi-Fi Technology and Standards*: https://standards.ieee.org/beyond-standards/the-evolution-of-wi-fi-technology-and-standards/
- IEEE 802.11 Working Group, standards and amendment timelines: https://www.ieee802.org/11/Reports/802.11_Timelines.htm

---
**Status:** Research reference. Confirm device-specific features and current normative requirements against the applicable IEEE text and certification documents before implementing protocol-level functionality.

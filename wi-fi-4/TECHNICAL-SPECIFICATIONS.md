# Wi-Fi 4 — Known Specifications And Technical Details

## Identity And Standards Basis

- **Wi-Fi generation:** Wi-Fi 4
- **IEEE amendment:** IEEE 802.11n-2009 (High Throughput, commonly HT)
- **Published generation:** 2009
- **Bands:** 2.4 GHz and 5 GHz, subject to device support and local regulatory rules
- **Primary purpose:** Improve WLAN throughput and reliability over earlier 802.11 generations.

Wi-Fi 4 is an industry generation label for 802.11n. Actual supported features depend on the access point, client, firmware, channel configuration, and regulatory domain.

## Known Technical Capabilities

- **Channel widths:** 20 MHz and 40 MHz.
- **MIMO:** Multiple-input/multiple-output spatial streams; 802.11n supports up to four spatial streams.
- **Modulation:** Includes OFDM and up to 64-QAM with a 5/6 coding rate in its highest-rate modes.
- **Aggregation:** A-MPDU and A-MSDU frame aggregation can reduce per-frame overhead.
- **Guard interval:** Normal and short guard intervals; short GI can increase the PHY rate in supported configurations.
- **Channel bonding:** A 40 MHz channel combines two 20 MHz channels where permitted and where coexistence rules allow.
- **Legacy interoperability:** Designed to coexist with earlier 802.11 equipment, although performance and protection overhead depend on the mix of clients and configuration.

## Peak Data Rate — Important Qualification

The commonly quoted maximum PHY rate is **600 Mbit/s** with four spatial streams, a 40 MHz channel, and short guard interval. This is a theoretical physical-layer rate, not a guaranteed application throughput. Many products implement fewer streams or use 20 MHz channels, and real throughput is lower due to protocol overhead, interference, signal conditions, and shared airtime.

## Bands, Channels, And Regulatory Constraints

The 2.4 GHz band often offers broader propagation but has fewer non-overlapping channels and more sources of interference. The 5 GHz band offers more channel options in many jurisdictions but propagation and availability vary. Permitted channels, transmit power, DFS requirements, and channel widths depend on the country and equipment certification; software must not assume one universal channel plan.

## Security And Interoperability

802.11n describes radio/MAC capabilities, not one mandatory security configuration for every product. Security depends on the network's authentication and encryption configuration and device support. Prefer WPA3-Personal or WPA3-Enterprise when supported; WPA2 with AES-CCMP may be needed for compatibility with older equipment. Avoid WEP and WPA/TKIP. Do not treat a network name (SSID) as proof of identity.

## SLeeLa Implementation Notes

This document records known standard-level information; it does **not** claim that SLeeLa implements an 802.11n radio or MAC stack. A normal application cannot implement the PHY through ordinary socket APIs alone; radio hardware, firmware, operating-system drivers, regulatory compliance, and suitable test equipment are required.

For SLeeLa software that uses Wi-Fi, focus on platform network APIs, connection-state reporting, secure transport, configuration validation, and tests that run without requiring raw radio control.

## Verification Checklist

- Record adapter and access-point capabilities rather than assuming four-stream/40 MHz operation.
- Test 2.4 GHz and 5 GHz separately where supported.
- Verify security mode and certificate validation for application traffic.
- Measure throughput, latency, packet loss, and roaming under repeatable conditions.
- Keep regulatory settings under the operating system/driver and certified hardware control.

## References

- IEEE Standards Association, *IEEE 802.11-2020* and amendment history: https://standards.ieee.org/standard/802_11-2020.html
- IEEE SA, *The Evolution of Wi-Fi Technology and Standards*: https://standards.ieee.org/beyond-standards/the-evolution-of-wi-fi-technology-and-standards/
- IEEE 802.11 Working Group, standards and amendment timelines: https://www.ieee802.org/11/Reports/802.11_Timelines.htm

---
**Status:** Research reference. Verify device-specific behavior and current normative requirements against the applicable IEEE text and certification documents before implementing protocol-level functionality.

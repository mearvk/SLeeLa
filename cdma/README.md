<img align="right" src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">

# CDMA

**SLeeLa Code-Division Multiple Access Reference Models**

This directory contains compact reference models for direct-sequence CDMA channelization, spreading, correlation, and multi-user shared-channel behavior. These models are for education and software tests; they are not production radio/baseband implementations and do not claim full conformance to any air-interface standard.

| Directory | Scope | Key parameters modeled |
|---|---|---|
| `common/` | Shared Walsh/OVSF, PN sequence, spread/correlate and channel primitives | Integer chip-domain model |
| `is-95-cdmaone/` | cdmaOne / IS-95 (2G) | 1.2288 Mcps, Walsh-64, PN concepts |
| `cdma2000-1xrtt/` | CDMA2000 1xRTT | 1.2288 Mcps, configurable Walsh spreading |
| `cdma2000-evdo/` | CDMA2000 1x EV-DO | Slot and DRC tier model |
| `wcdma-umts/` | W-CDMA / UMTS | 3.84 Mcps, OVSF spreading factors 4–512 |
| `../lib/cdma/` | SLeeLa-language reference front end | Pure-model counterparts and demo |

The named standards are design references, not a claim of interoperability or full conformance. Build/test entry point: `make -C cdma/build all test`.

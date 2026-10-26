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

## Link-level stack and tooling

Beyond the per-standard channelization models, `common/` provides a
**link-level simulation layer** (`cdma_link.h`): a deterministic PRNG, AWGN and
multipath channel models, a RAKE combiner, closed-loop power control, and
Monte-Carlo bit-error-rate evaluation. A laptop command-line driver,
`tools/cdma-sim`, ties these together:

```sh
make -C build all test                 # build everything + run all tests
./tools/cdma-sim --std wcdma --sf 64 --users 16 --ebn0 6 --paths 3 --trials 20000
```

See **[`STACK.md`](STACK.md)** for the full build/run guide, how the pieces fit
together, and an honest map of what a real device would need on top of this.

## License and scope

This directory is **MIT-licensed** software (repository-root
[`LICENSE`](../LICENSE); every source file carries an `SPDX-License-Identifier:
MIT` tag). The MIT grant covers the **source code only**. It does **not** grant
CDMA air-interface patent rights (SEP/FRAND), FCC equipment authorization, or
carrier certification, all of which a real device requires. These models are
for education, algorithm study, and golden-model validation — not deployable
baseband. The full, non-legal-advice notice is in
**[`LEGAL-AND-SCOPE.md`](LEGAL-AND-SCOPE.md)**.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

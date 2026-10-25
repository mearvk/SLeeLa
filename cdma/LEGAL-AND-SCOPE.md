# CDMA Reference Stack — License, Scope, and Regulatory Notice

> **This document is informational, not legal advice.** It explains what the
> software license does and does not grant. For any commercial, regulatory, or
> patent question, consult qualified intellectual-property and communications
> counsel.

## 1. Software license

All source code in `cdma/` is licensed under the **MIT License** (see the
repository-root [`LICENSE`](../LICENSE)). The MIT grant covers **copyright in
this source code**: you may use, copy, modify, and redistribute the software,
including in commercial products, provided the copyright and permission notice
are retained.

Each source file carries an `SPDX-License-Identifier: MIT` tag so the grant is
machine-readable and travels with the file.

## 2. What the MIT license does NOT grant

A copyright license on software is **not** a license to practice the CDMA air
interface. In particular, the MIT grant here does **not** convey:

- **Patent rights to the CDMA standards.** IS-95 (cdmaOne), CDMA2000 1xRTT and
  1x EV-DO, and W-CDMA/UMTS are standardized air interfaces covered by
  standard-essential patents (SEPs) held by various parties and typically
  licensed on fair, reasonable, and non-discriminatory (FRAND) terms. Building
  or selling equipment that implements these air interfaces generally requires
  separate patent licenses from the relevant rights holders. Nothing in this
  repository grants, implies, or exhausts any such patent right.
- **Regulatory authorization.** In the United States, radio equipment that
  transmits on licensed spectrum requires **FCC equipment authorization**
  (e.g. certification under 47 CFR) and operation consistent with the spectrum
  licensee's authorization. This software performs none of that and provides no
  such authorization.
- **Carrier / network certification.** Devices that attach to a commercial
  mobile network must pass the operator's and industry (e.g. PTCRB/GCF-style)
  certification. This software is not certified and makes no conformance claim.

## 3. Technical scope: what this code is (and is not)

This is a **link-level reference and simulation model**, intended for
education, algorithm study, and use as a *golden oracle* against which an
optimized implementation can be checked. It models, in portable C/C++:

- Walsh and OVSF channelization codes and PN sequences;
- symbol spreading, correlation, and hard-decision recovery;
- a shared/multi-user channel, AWGN noise, a multipath profile, a RAKE
  combiner, closed-loop power-control stepping, and Monte-Carlo BER evaluation.

It is deliberately **NOT**, and must not be represented as:

- a conforming or interoperable implementation of any air-interface standard;
- real-time baseband/DSP firmware, or anything that runs on modem silicon;
- a complete protocol stack — there is no RF front end, no fixed-point DSP, no
  channel coding (turbo/convolutional)/HARQ, and no MAC/RLC/RRC/NAS layer.

See [`README.md`](README.md) for the per-standard module layout and
[`STACK.md`](STACK.md) for how the simulation pieces fit together and what a
real device would need on top of them.

## 4. Export and jurisdiction

Telecommunications and cryptographic software can be subject to export control
(e.g. U.S. EAR). This reference model implements no cryptography, but
redistributors remain responsible for their own export-control and local-law
compliance. The MIT license text governs the software grant; it does not waive
any statutory or regulatory obligation.

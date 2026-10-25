# CDMA Software Stack — Guide

A practical guide to the SLeeLa CDMA link-level stack: what each piece does,
how to build and run it on a laptop, and an honest map of what a real device
would need on top of it.

> Scope and licensing are covered in [`LEGAL-AND-SCOPE.md`](LEGAL-AND-SCOPE.md).
> In short: MIT-licensed **software**; it conveys **no** CDMA air-interface
> patent rights, FCC authorization, or carrier certification, and is a
> reference/simulation model — not deployable modem firmware.

## 1. What is in the box

| Component | File(s) | Role |
|---|---|---|
| DS-CDMA primitives | `common/src/cdma_common.c`, `.h`, `.hpp` | Walsh / OVSF codes, PN sequence, spread, correlate, shared channel |
| Link-level model | `common/src/cdma_link.c`, `common/include/cdma_link.h` | Deterministic PRNG, AWGN, multipath, RAKE combiner, power control, Monte-Carlo BER |
| Standard front-ends | `is-95-cdmaone/`, `cdma2000-1xrtt/`, `cdma2000-evdo/`, `wcdma-umts/` | Per-standard spreading-factor caps and code family wrappers |
| Laptop CLI | `tools/cdma_sim.c` → `cdma-sim` | Pick a standard, users, Eb/N0, multipath; reports reference-user BER |
| Tests | `*/build/test*.c` | Shared-channel recovery + link-model (PRNG/BER/RAKE/power-control) |

## 2. Build and run (desktop / laptop)

Requires a C/C++ toolchain and GNU Make. From `cdma/`:

```sh
make -C build all          # build every module + the cdma-sim CLI
make -C build test         # run all unit tests and CLI smoke tests
make -C build clean        # remove build artifacts
```

Run the simulator directly:

```sh
make -C tools               # builds ./tools/cdma-sim
./tools/cdma-sim --help

# IS-95, single user, high SNR -> essentially error-free:
./tools/cdma-sim --std is95 --users 1 --ebn0 20 --trials 20000

# W-CDMA, SF 64, 16 users, 6 dB Eb/N0, 3-path multipath with RAKE:
./tools/cdma-sim --std wcdma --sf 64 --users 16 --ebn0 6 --paths 3 --trials 20000
```

`cdma-sim` is **reproducible**: the same `--seed` yields the same result on any
platform, because the model uses its own deterministic PRNG (xorshift128+ with
a SplitMix64-seeded state), not the C library's `rand()`.

## 3. How the simulation fits together

```
 per-user symbols (+/-1)
        |  spread by Walsh/OVSF code[u], scaled by amplitude[u] (power control)
        v
 superposition over all users  --->  multipath channel  --->  + AWGN(sigma)
                                                                   |
                                                       RAKE combine (per-path fingers,
                                                       maximal-ratio by path gain)
                                                                   |
                                                       hard decision -> recovered symbol
```

- **Eb/N0 → sigma.** `cdma_ebn0_to_sigma()` maps a target Eb/N0 (dB) to the
  per-chip noise standard deviation for a BPSK symbol over `SF` unit-energy
  chips, so results are comparable across spreading factors.
- **Orthogonal-code behavior.** On a synchronous flat channel, Walsh/OVSF codes
  are mutually orthogonal, so interferers cancel and BER is governed by noise
  alone — adding users barely moves BER. Multipath/asynchrony breaks that
  orthogonality; the RAKE receiver then trades interference against recovered
  path energy. Both effects are visible by varying `--users` and `--paths`.

## 4. Using it as a golden model

The intended professional use is as a **slow, obviously-correct reference**
("golden oracle"): an optimized fixed-point or hardware implementation can be
validated against this model's outputs for the same inputs. The deterministic
PRNG makes trials bit-reproducible, which is what a conformance/regression
harness needs.

## 5. What a real device would require (not in this repo)

| Layer | Needed for a shipping device | Provided here |
|---|---|---|
| RF front end | Antenna, LNA/PA, mixers, filters, ADC/DAC | ✗ hardware |
| Baseband DSP (PHY) | Fixed-point spreading/despreading, channel estimation, RAKE/equalizer, timing & frequency recovery, turbo/convolutional coding, interleaving, HARQ, closed-loop power control at chip rate | ⚠ idealized floating-point model only |
| MAC / RLC (L2) | Scheduling, ARQ/HARQ, segmentation, channel mapping | ✗ |
| RRC / mobility (L3) | Connection setup, handover, paging, cell selection | ✗ |
| NAS / core signaling | Mobility & session management to the network | ✗ |
| Build & deployment | Vendor cross-compiler for the target DSP/ISA, RTOS, drivers, flashing | ✗ |
| Compliance | SEP/FRAND patent licenses, FCC authorization, carrier certification | ✗ (see LEGAL-AND-SCOPE.md) |

Getting from this model to a product is not a "binary conversion"; it is
building (or licensing) the full baseband + protocol stack, expressing the hot
paths in fixed-point for dedicated DSP/accelerator silicon, cross-compiling
through a vendor toolchain, and clearing the patent/regulatory requirements.
In practice device makers license a modem (e.g. from a baseband vendor) that
already contains all of the above.

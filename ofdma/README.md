# OFDMA Reference Stack

OFDMA (Orthogonal Frequency-Division Multiple Access) reference workspace for SLeeLa. This directory starts a layered, educational C/C++ implementation that can be expanded into PHY, scheduler, resource-grid, channel, and test modules.

## Scope
- Numerology: subcarrier spacing, useful-symbol duration, cyclic prefix and FFT sizing.
- Resource-grid accounting: resource blocks, symbols, and subcarrier counts.
- Deterministic round-robin resource-block scheduling across active users.
- Portable C11 and C++17 builds with no external dependencies.

This is a **simulation/reference model**, not a modem, conformance-tested 3GPP implementation, or production radio. It does not transmit RF or implement channel coding, synchronization, MIMO, HARQ, or standards-complete scheduling. Validate assumptions against the target standard before use.

## Layout
- `include/ofdma.h`: public C API and data types.
- `src/ofdma.c`: numerology validation and resource-grid calculations.
- `src/scheduler.c`: scheduler extension point.
- `cpp/ofdma.hpp`, `cpp/ofdma_demo.cpp`: C++17 wrapper and demo.
- `examples/ofdma.conf`: static starter configuration.
- `tests/test_ofdma.c`: regression tests.
- `Makefile`: build, test, clean.

## Build
```sh
make
make test
./build/ofdma-demo
```
Override compiler commands with `CC=` or `CXX=`.

For subcarrier spacing Δf, useful symbol duration is Tu = 1/Δf. This generic model treats a resource block as a configurable number of adjacent subcarriers over OFDM symbols. RB definitions, guard bands, pilots, control channels, and scheduling rules vary by radio standard and belong in standards-specific profiles.

## Expansion Roadmap
1. LTE and 5G NR standards profiles and numerologies.
2. Resource-grid masks, guard bands, reference signals and control channels.
3. AWGN/fading channels, EVM and BER/BLER measurements.
4. Modulation, coding and link adaptation.
5. MIMO, HARQ and QoS-aware schedulers.
6. Standards-derived conformance vectors and interoperability tests.

Max Rupplin - MEARVK LLC - 2026

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

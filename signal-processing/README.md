<!-- Author: Max Rupplin — MEARVK LLC — 2026 -->

# SLeeLa Signal Processing

A portable signal-processing library for C11 and C++17, with no third-party
runtime dependencies. The timeline folders cover engineering history from 1951
through the present (2026); they provide context alongside a small, tested
implementation rather than claiming to reproduce every historical method.

## Library features

- **Time-domain statistics:** compensated mean, scaled RMS, causal moving
  average with defined partial-window behavior at the start of a signal.
- **Filtering:** full linear convolution for finite impulse-response kernels.
- **Windowing:** symmetric Hann window, including a defined one-sample case.
- **Frequency analysis:** in-place radix-2 Cooley–Tukey FFT, direct DFT reference,
  and unnormalized nonnegative-frequency magnitude spectrum.
- **Portable interfaces:** stable C API plus an ergonomic C++17 vector wrapper.
- **Defensive contracts:** explicit sizes and capacities, status codes, checked
  size arithmetic, non-finite input checks, and regression tests.

The current core deliberately uses double precision and transparent algorithms.
It is a foundation to extend, not a substitute for an application-specific
anti-alias filter, ADC front end, calibrated measurement pipeline, or real-time
deadline analysis.

## Build and test

From this directory, use either build system:

    make
    make test

or:

    cmake -S . -B build-cmake
    cmake --build build-cmake
    ctest --test-dir build-cmake --output-on-failure

CMake installs the static library and both public headers with the usual
cmake --install command. The Makefile builds the archive and examples under
build/. A C consumer links the archive and math library; a C++ consumer links
with a C++ linker and the math library on Unix-like systems.

## C example

    #include "sleela_signal_processing.h"
    double x[] = {1, 2, 3, 4, 5};
    double y[5];
    SleelaSPStatus status = slsp_moving_average(x, 5, 3, y, 5);

The C API returns SLEELA_SP_OK on success. Always check the return value;
slsp_status_string() maps errors to readable messages. Input/output arrays
for moving average and convolution must not overlap. The FFT works in place and
requires a positive power-of-two length. Its inverse is normalized by length.

## C++ example

    #include "sleela_signal_processing.hpp"
    auto smoothed = sleela::signal::moving_average({1, 2, 3, 4, 5}, 3);
    auto window = sleela::signal::hann_window(5);

C++ vector helpers throw std::invalid_argument for invalid parameters,
std::overflow_error for non-finite numeric results, and std::runtime_error for
other library failures. The direct fft() wrapper returns a Status.

## Engineering notes

The shorter engineering notes are collected under [docs/](docs/README.md):
[API contracts](docs/API-CONTRACTS.md), [algorithm behavior](docs/ALGORITHM-NOTES.md),
[numerical safety](docs/NUMERICAL-SAFETY.md), [build and testing](docs/TESTING-AND-BUILD.md),
and a clearly labeled [development roadmap](docs/ROADMAP.md). The public headers,
implementation, examples and regression tests each also have a short README.

## Historical organization: 1951–present

Each decades/ folder records representative research/application themes and
links them to the current library where appropriate. A decade is an organizing
lens, not an assertion that all listed techniques were invented then. The
foundational theory often predates practical digital implementations.

| Folder | Context and representative themes |
| --- | --- |
| 1951-1959/ | Statistical estimation, noise reduction, sampled data and early computer-aided analysis |
| 1960s/ | Digital signal-processing research, spectrum estimation, fast Fourier transform |
| 1970s/ | Digital audio, numerical filter design, linear prediction and convolution |
| 1980s/ | Dedicated DSP hardware, real-time speech/audio and adaptive filtering |
| 1990s/ | Wavelets, filter banks, subband coding and digital media |
| 2000s/ | Software-defined radio, sensor networks and sparse/compressive methods |
| 2010s/ | Mobile sensing, large-scale analytics and learning-assisted interpretation |
| 2020s-present/ | Edge inference, heterogeneous compute, energy-aware processing and reproducible evaluation |

The decade folders document context and suggested implementation exercises.
Supported functionality is defined by the public header and its tests; no
historical note implies that an algorithm not yet implemented is available.

## Numerical and engineering boundaries

- Samples are assumed equally spaced unless callers perform timebase correction.
- The library does not infer units, sample rate, calibration, or ADC properties.
- The moving average is causal with a shorter divisor before its window fills;
  it is not a zero-phase filter.
- Convolution returns the complete linear result (N + M - 1 samples).
- The FFT is radix-2 only; slsp_dft() is a slower arbitrary-length reference.
- APIs reject NaN/infinite values where they are consumed as numeric inputs.
- No thread-safety, hard-real-time, SIMD, GPU, or constant-time guarantees are
  implied by version 1.0.0.

## Selected historical reading

- IEEE Signal Processing Society, [Empowering the Growth of Signal Processing](https://read.nxtbook.com/ieee/signal_processing/signal_processing_june_2023/empowering_the_growth_of_sign.html), historical overview through 2023.
- IBM Research, [Cooley and Tukey's 1965 Fourier-series algorithm](https://research.ibm.com/publications/an-algorithm-for-the-machine-calculation-of-complex-fourier-series).
- MIT Digital Signals and Processing Group, [History](https://dsp-group.mit.edu/history/), context for mid-1960s research and applications.
- Cooley, Lewis, and Welch, [Historical Notes on the Fast Fourier Transform](https://research.ibm.com/publications/historical-notes-on-the-fast-fourier-transform--1), IEEE Transactions on Audio and Electroacoustics (1967).

---

Max Rupplin — MEARVK LLC — 2026

<!-- Author: Max Rupplin — MEARVK LLC — 2026 -->

# SLeeLa Signal Processing Package

The SLeeLa source-side signal-processing implementation lives here. It mirrors
the portable native C11/C++17 library in the repository's top-level
[signal-processing/](../../signal-processing/README.md) directory.

## Source classes

- SLSignalProcessingStatus — shared status codes, version and finite-value guard.
- SLSignalSeries / SLSignalSample — real samples held in a doubly-linked series.
- SLSignalComplex / SLSignalComplexSeries / SLSignalComplexSample — complex values.
- SLSignalStatistics — compensated mean and scaled sum-of-squares RMS.
- SLSignalFilters — causal moving average, full linear convolution and Hann window.
- SLSignalDFT — direct forward/inverse discrete Fourier transform.
- SLSignalFFT — recursive radix-2 Cooley–Tukey transform.
- SLSignalMagnitudeSpectrum — unnormalized bins zero through floor(N/2).
- SLSignalProcessing — compact facade for the operations above.

The SLeeLa language currently has no native arrays, so the source implementation
uses linked series rather than pretending to provide the C interface's contiguous
buffers. The algorithms and numerical conventions mirror the native library;
the linked representation has different memory overhead and performance. Use
the native C/C++ implementation for throughput-sensitive workloads.

## Semantics

- Samples use double precision; non-finite values are rejected where checked.
- Mean uses compensated accumulation; RMS uses scale-and-sum-of-squares.
- Moving average is causal, with a partial divisor until its window fills.
- Convolution returns the complete linear result with N + M - 1 samples.
- Hann window is symmetric; a one-sample window is exactly 1.
- Forward DFT/FFT use the negative exponent. Inverse transforms use the
  positive exponent and divide by N.
- FFT is radix-2 only; DFT supports any positive series length.
- The magnitude spectrum is not normalized by N.
- Series are linked objects: output arguments must be distinct from inputs for
  the operations that document this restriction. Methods return a status code.

The native capacity status remains in the common status vocabulary for parity
with the C API. Linked-series operations grow their output dynamically and do
not expose a caller-provided element capacity.

## Example and smoke test

- [Runnable demonstration](example/SLSignalProcessingDemo.sleela)
- [Deterministic smoke checks](tests/SLSignalProcessingSmoke.sleela)

Both are SLeeLa source programs, not native C/C++ tests. They depend on the
standard SLeeLa math module for square roots and trigonometric functions.

---

Max Rupplin — MEARVK LLC — 2026

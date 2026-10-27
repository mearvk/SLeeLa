<!-- Author: Max Rupplin — MEARVK LLC — 2026 -->

# Implementation files

- slep_core.c — status/version strings, mean, RMS, and causal moving average.
- slep_filter.c — Hann window and full direct linear convolution.
- slep_fft.c — direct DFT, radix-2 FFT, and magnitude spectrum.
- slep_cpp.cpp — C++17 wrappers around the C ABI.

The C files form the portable computational core. Keep algorithms deterministic
where practical, validate sizes before arithmetic, detect non-finite values,
and document whether an output is modified before an error is returned.
The C++ layer should delegate to the C implementation rather than maintain
a second algorithm with subtly different conventions.

See ../docs/ALGORITHM-NOTES.md and ../docs/NUMERICAL-SAFETY.md.

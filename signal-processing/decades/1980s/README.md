<!-- Author: Max Rupplin — MEARVK LLC — 2026 -->

# 1980s — Real-time and dedicated digital processing

Dedicated digital signal processors and more capable embedded systems helped
move algorithms from offline calculation toward low-latency speech, audio,
control, and communications. Algorithm choices increasingly had to consider
throughput, memory, fixed-point range, quantization and power.

**Library connection:** this version keeps a transparent double-precision
reference path. It is a baseline to compare with future fixed-point or
vectorized kernels, but is not yet a hard-real-time library.

Suggested exercise: benchmark FFT and convolution with representative sizes,
record compiler/CPU details, and measure numerical error before optimizing.

<!-- Author: Max Rupplin — MEARVK LLC — 2026 -->

# Numerical safety and known boundaries

## Input validation

The C routines check required pointers, positive lengths, transform modes,
capacities, and relevant size arithmetic. Numeric routines reject NaN and
infinite input samples where documented and check for non-finite results.
Callers must still supply valid allocated buffers of the claimed lengths; C
cannot verify the actual size or lifetime of arbitrary pointers.

## Precision

The public sample type is double. Some accumulations use long double, whose
range differs by platform. Floating-point results can vary slightly among
architectures, compiler flags, math libraries, and evaluation order. Tests
should compare floating-point results with reasoned tolerances rather than
require exact bitwise identity for transcendental operations.

## Aliasing

The moving average and convolution contracts require non-overlapping input and
output buffers. The DFT also requires separate, non-overlapping arrays. The FFT
is explicitly in place. The routines do not attempt general alias detection.

## Operational boundaries

Version 1.0.0 does not promise hard-real-time behavior, bounded execution time,
thread-safe shared state guarantees for future stateful processors, fixed-point
arithmetic, SIMD/GPU acceleration, denormal handling policy, or safety
certification. The current routines are stateless and use no shared mutable
library globals, but callers are responsible for synchronizing their own
buffers and processing pipelines.

Before using results in regulated, medical, flight, automotive, or other
safety-critical equipment, the system owner must create a domain-specific
validation plan and independently verify the full acquisition and processing
chain. These baseline utilities alone are not certified for such use.

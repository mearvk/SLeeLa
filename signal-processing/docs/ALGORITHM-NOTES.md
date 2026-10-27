<!-- Author: Max Rupplin — MEARVK LLC — 2026 -->

# Algorithm notes

These notes give the practical interpretation of the implemented routines.

## Mean and RMS

The mean uses compensated accumulation in long double before conversion to
double. RMS uses a scaled sum-of-squares method to reduce unnecessary overflow
from squaring large samples. The range and precision of long double vary by
compiler and target, so these methods improve robustness but do not promise
identical bit patterns across platforms.

Both require a non-empty finite input sequence.

## Causal moving average

For samples x[n] and window length W, output y[n] is the average of the available
samples from max(0, n-W+1) through n. At the leading edge, the divisor is the
number of samples available; once the window is full, the divisor is W.

This is a causal smoother. It adds delay and attenuates features depending on
their frequency. It is not a zero-phase filter and is not automatically an
appropriate anti-alias filter before downsampling.

## Full linear convolution

For input x of length N and kernel h of length M, output y has length N+M-1:

    y[n] = sum(x[k] * h[n-k])

Only terms with valid input and kernel indices contribute. This is a direct
implementation with O(N*M) work. It is a good reference for tests and short
kernels; large workloads may eventually justify overlap-add, overlap-save, or
FFT-based convolution.

## Hann window

For N greater than 1, coefficients are

    w[n] = 0.5 - 0.5*cos(2*pi*n/(N-1)), n = 0 ... N-1

N=1 is explicitly defined as a single coefficient of 1. The current routine
creates the symmetric Hann form. Do not confuse it with the periodic Hann form
often used when constructing DFT analysis frames.

## DFT, FFT, and magnitudes

The direct DFT is the simple reference transform. The radix-2 FFT uses
bit-reversal followed by butterfly stages, requiring N to be a positive power
of two. Its forward convention is the negative exponential; its inverse is
normalized by N.

The magnitude routine computes abs(FFT[k]) over the nonnegative-frequency half
spectrum, without amplitude or power normalization. For a sinusoid amplitude
estimate, callers must account for window coherent gain, transform length, and
special handling for DC and Nyquist bins. Those normalizations are deliberately
not guessed by the current API.

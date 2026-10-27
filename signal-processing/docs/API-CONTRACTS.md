<!-- Author: Max Rupplin — MEARVK LLC — 2026 -->

# API contracts

## General rules

The public C interface is declared in include/sleela_signal_processing.h.
Samples are represented as double precision values, and complex values use
SleelaSPComplex with separate real and imaginary members. Callers own all input
and output storage. APIs that accept a capacity require the capacity to be
expressed in elements, not bytes.

Check every SleelaSPStatus return value. slsp_status_string() maps the library's
status values to a short diagnostic. The API distinguishes invalid arguments,
insufficient capacity, allocation failure, unsupported/overflowing size, and
numeric failure.

## Buffers and overlap

- slsp_moving_average() writes input_len output elements. Its output buffer must
  hold at least that many elements. Input and output must not overlap.
- slsp_fir_convolve() writes input_len + kernel_len - 1 elements. output_len is
  required and is set to the required element count before capacity is checked.
  Input, kernel, and output must not overlap.
- slsp_fft() intentionally operates in place and needs no second array.
- slsp_dft() writes a separate output array; input and output must not overlap.
- slsp_magnitude_spectrum() writes floor(length / 2) + 1 magnitudes and uses an
  internal copy so the supplied complex input remains unchanged.

A capacity error is not permission to use a partial output. For some numeric
errors, calculations may have written earlier output elements before detecting
the failure; callers should discard that output unless an API specifically
documents transactional behavior.

## Status values

| Status | Meaning |
| --- | --- |
| SLEELA_SP_OK | Operation completed successfully |
| SLEELA_SP_ERR_INVALID | A required pointer, length, window, or mode is invalid |
| SLEELA_SP_ERR_CAPACITY | Output storage is too small |
| SLEELA_SP_ERR_ALLOC | The library could not allocate working storage |
| SLEELA_SP_ERR_SIZE | Size is unsupported or would overflow an allocation calculation |
| SLEELA_SP_ERR_NUMERIC | A non-finite input or result was detected |

C++ vector helpers convert invalid parameters to std::invalid_argument, numeric
failures to std::overflow_error, and other library errors to std::runtime_error.
std::bad_alloc from vector allocation may propagate. The direct C++ fft() helper
is noexcept and returns a status value instead of throwing.

## Transform conventions

- Forward DFT/FFT: negative exponent, exp(-j 2 pi k n / N).
- Inverse DFT/FFT: positive exponent, divided by N.
- slsp_fft(): positive power-of-two lengths only.
- slsp_dft(): any positive length, at O(N^2) computational cost.
- Magnitude spectrum: bins 0 through floor(N/2), unnormalized magnitude.

The library does not infer sample rate, physical units, ADC calibration, or
frequency-axis labels. The caller derives frequency bin k as k * sample_rate / N
for a uniformly sampled signal.

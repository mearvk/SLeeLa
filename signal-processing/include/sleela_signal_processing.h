/* Author: Max Rupplin — MEARVK LLC — 2026
 * SLeeLa Signal Processing: stable C ABI for portable numeric signal routines.
 * The API uses double-precision samples and explicit lengths/capacities.
 */
#ifndef SLEELA_SIGNAL_PROCESSING_H
#define SLEELA_SIGNAL_PROCESSING_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#define SLEELA_SP_VERSION_MAJOR 1
#define SLEELA_SP_VERSION_MINOR 0
#define SLEELA_SP_VERSION_PATCH 0
#define SLEELA_SP_VERSION_STRING "1.0.0"

typedef struct SleelaSPComplex { double re; double im; } SleelaSPComplex;

typedef enum SleelaSPStatus {
    SLEELA_SP_OK = 0,
    SLEELA_SP_ERR_INVALID = -1,
    SLEELA_SP_ERR_CAPACITY = -2,
    SLEELA_SP_ERR_ALLOC = -3,
    SLEELA_SP_ERR_SIZE = -4,
    SLEELA_SP_ERR_NUMERIC = -5
} SleelaSPStatus;

const char *slsp_version_string(void);
const char *slsp_status_string(SleelaSPStatus status);

/* Causal moving average; output length equals input_len. Initial windows are
 * partial (first output is input[0]). window must be [1,input_len]. Input and
 * output must not overlap. Non-finite inputs/results are rejected. */
SleelaSPStatus slsp_moving_average(const double *input, size_t input_len,
                                   size_t window, double *output,
                                   size_t output_capacity);
SleelaSPStatus slsp_mean(const double *input, size_t input_len, double *out_mean);
SleelaSPStatus slsp_rms(const double *input, size_t input_len, double *out_rms);

/* Full linear convolution output length is input_len+kernel_len-1. output_len
 * is required and receives the required length before capacity validation.
 * Input, kernel and output regions must not overlap. */
SleelaSPStatus slsp_fir_convolve(const double *input, size_t input_len,
                                 const double *kernel, size_t kernel_len,
                                 double *output, size_t output_capacity,
                                 size_t *output_len);

/* Symmetric Hann window. length=1 is defined as a single coefficient of 1. */
SleelaSPStatus slsp_hann_window(double *output, size_t length);

/* In-place radix-2 Cooley-Tukey FFT. length must be a power of two and > 0.
 * inverse must be 0 (forward) or 1 (inverse); inverse output is divided by N. */
SleelaSPStatus slsp_fft(SleelaSPComplex *data, size_t length, int inverse);

/* Direct DFT reference for any positive length. Forward sign is negative;
 * inverse sign is positive and normalized by N. Input/output must not overlap. */
SleelaSPStatus slsp_dft(const SleelaSPComplex *input, SleelaSPComplex *output,
                        size_t length, int inverse);

/* Unnormalized |FFT[k]| for bins 0..N/2 inclusive. Requires radix-2 length. */
SleelaSPStatus slsp_magnitude_spectrum(const SleelaSPComplex *input,
                                       size_t length, double *magnitudes,
                                       size_t magnitude_capacity);

#ifdef __cplusplus
}
#endif
#endif /* SLEELA_SIGNAL_PROCESSING_H */

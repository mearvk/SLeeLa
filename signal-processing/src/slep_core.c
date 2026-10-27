/* Author: Max Rupplin — MEARVK LLC — 2026 */
#include "sleela_signal_processing.h"
#include <math.h>

const char *slsp_version_string(void) { return SLEELA_SP_VERSION_STRING; }

const char *slsp_status_string(SleelaSPStatus status) {
    switch (status) {
    case SLEELA_SP_OK: return "success";
    case SLEELA_SP_ERR_INVALID: return "invalid argument";
    case SLEELA_SP_ERR_CAPACITY: return "output capacity too small";
    case SLEELA_SP_ERR_ALLOC: return "allocation failed";
    case SLEELA_SP_ERR_SIZE: return "unsupported or overflowing size";
    case SLEELA_SP_ERR_NUMERIC: return "non-finite input or numerical overflow";
    default: return "unknown signal-processing status";
    }
}

SleelaSPStatus slsp_mean(const double *input, size_t n, double *out_mean) {
    if (!input || !out_mean || n == 0) return SLEELA_SP_ERR_INVALID;
    long double sum = 0.0L, correction = 0.0L;
    size_t i;
    for (i = 0; i < n; ++i) {
        if (!isfinite(input[i])) return SLEELA_SP_ERR_NUMERIC;
        long double value = (long double)input[i] - correction;
        long double next = sum + value;
        correction = (next - sum) - value;
        sum = next;
    }
    *out_mean = (double)(sum / (long double)n);
    return isfinite(*out_mean) ? SLEELA_SP_OK : SLEELA_SP_ERR_NUMERIC;
}

SleelaSPStatus slsp_rms(const double *input, size_t n, double *out_rms) {
    if (!input || !out_rms || n == 0) return SLEELA_SP_ERR_INVALID;
    long double scale = 0.0L, ssq = 1.0L;
    size_t i;
    for (i = 0; i < n; ++i) {
        long double x;
        if (!isfinite(input[i])) return SLEELA_SP_ERR_NUMERIC;
        x = fabsl((long double)input[i]);
        if (x != 0.0L) {
            if (scale < x) {
                long double ratio = scale / x;
                ssq = 1.0L + ssq * ratio * ratio;
                scale = x;
            } else {
                long double ratio = x / scale;
                ssq += ratio * ratio;
            }
        }
    }
    *out_rms = scale == 0.0L ? 0.0 : (double)(scale * sqrtl(ssq / (long double)n));
    return isfinite(*out_rms) ? SLEELA_SP_OK : SLEELA_SP_ERR_NUMERIC;
}

SleelaSPStatus slsp_moving_average(const double *input, size_t n,
                                   size_t window, double *output, size_t cap) {
    if (!input || !output || n == 0 || window == 0 || window > n)
        return SLEELA_SP_ERR_INVALID;
    if (cap < n) return SLEELA_SP_ERR_CAPACITY;
    long double sum = 0.0L;
    size_t i;
    for (i = 0; i < n; ++i) {
        if (!isfinite(input[i])) return SLEELA_SP_ERR_NUMERIC;
        sum += (long double)input[i];
        if (i >= window) sum -= (long double)input[i - window];
        size_t count = i + 1 < window ? i + 1 : window;
        output[i] = (double)(sum / (long double)count);
        if (!isfinite(output[i])) return SLEELA_SP_ERR_NUMERIC;
    }
    return SLEELA_SP_OK;
}

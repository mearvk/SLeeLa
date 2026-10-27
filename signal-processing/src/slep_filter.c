/* Author: Max Rupplin — MEARVK LLC — 2026 */
#include "sleela_signal_processing.h"
#include <math.h>
#include <stdint.h>

SleelaSPStatus slsp_hann_window(double *out, size_t n) {
    if (!out || n == 0) return SLEELA_SP_ERR_INVALID;
    if (n == 1) { out[0] = 1.0; return SLEELA_SP_OK; }
    const double pi = acos(-1.0);
    size_t i;
    for (i = 0; i < n; ++i)
        out[i] = 0.5 - 0.5 * cos((2.0 * pi * (double)i) / (double)(n - 1));
    return SLEELA_SP_OK;
}

SleelaSPStatus slsp_fir_convolve(const double *x, size_t nx,
                                 const double *h, size_t nh, double *y,
                                 size_t cap, size_t *out_len) {
    size_t needed, n, k;
    if (!x || !h || !y || !out_len || nx == 0 || nh == 0)
        return SLEELA_SP_ERR_INVALID;
    if (nx > SIZE_MAX - (nh - 1)) return SLEELA_SP_ERR_SIZE;
    needed = nx + nh - 1;
    *out_len = needed;
    if (cap < needed) return SLEELA_SP_ERR_CAPACITY;
    for (n = 0; n < nx; ++n)
        if (!isfinite(x[n])) return SLEELA_SP_ERR_NUMERIC;
    for (k = 0; k < nh; ++k)
        if (!isfinite(h[k])) return SLEELA_SP_ERR_NUMERIC;
    for (n = 0; n < needed; ++n) {
        size_t first = n >= nh - 1 ? n - (nh - 1) : 0;
        size_t last = n < nx - 1 ? n : nx - 1;
        long double sum = 0.0L;
        size_t i;
        for (i = first; i <= last; ++i)
            sum += (long double)x[i] * (long double)h[n - i];
        y[n] = (double)sum;
        if (!isfinite(y[n])) return SLEELA_SP_ERR_NUMERIC;
    }
    return SLEELA_SP_OK;
}

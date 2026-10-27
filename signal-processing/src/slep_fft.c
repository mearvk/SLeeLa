/* Author: Max Rupplin — MEARVK LLC — 2026 */
#include "sleela_signal_processing.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

static int is_power_of_two(size_t n) { return n != 0 && (n & (n - 1)) == 0; }
static int finite_complex(SleelaSPComplex z) { return isfinite(z.re) && isfinite(z.im); }

SleelaSPStatus slsp_fft(SleelaSPComplex *a, size_t n, int inverse) {
    size_t i, j, len;
    if (!a || n == 0 || (inverse != 0 && inverse != 1)) return SLEELA_SP_ERR_INVALID;
    if (!is_power_of_two(n)) return SLEELA_SP_ERR_SIZE;
    for (i = 0; i < n; ++i) if (!finite_complex(a[i])) return SLEELA_SP_ERR_NUMERIC;

    for (i = 1, j = 0; i < n; ++i) {
        size_t bit = n >> 1;
        while (j & bit) { j ^= bit; bit >>= 1; }
        j ^= bit;
        if (i < j) { SleelaSPComplex tmp = a[i]; a[i] = a[j]; a[j] = tmp; }
    }
    for (len = 2; len <= n; ) {
        double angle = (inverse ? 2.0 : -2.0) * acos(-1.0) / (double)len;
        SleelaSPComplex root = {cos(angle), sin(angle)};
        size_t base;
        for (base = 0; base < n; base += len) {
            SleelaSPComplex w = {1.0, 0.0};
            size_t k;
            for (k = 0; k < len / 2; ++k) {
                SleelaSPComplex u = a[base + k];
                SleelaSPComplex v = {
                    a[base + k + len / 2].re * w.re - a[base + k + len / 2].im * w.im,
                    a[base + k + len / 2].re * w.im + a[base + k + len / 2].im * w.re
                };
                a[base + k].re = u.re + v.re;
                a[base + k].im = u.im + v.im;
                a[base + k + len / 2].re = u.re - v.re;
                a[base + k + len / 2].im = u.im - v.im;
                { double nr = w.re * root.re - w.im * root.im;
                  w.im = w.re * root.im + w.im * root.re; w.re = nr; }
            }
        }
        if (len == n) break;
        len <<= 1;
    }
    if (inverse) for (i = 0; i < n; ++i) {
        a[i].re /= (double)n; a[i].im /= (double)n;
    }
    for (i = 0; i < n; ++i) if (!finite_complex(a[i])) return SLEELA_SP_ERR_NUMERIC;
    return SLEELA_SP_OK;
}

SleelaSPStatus slsp_dft(const SleelaSPComplex *in, SleelaSPComplex *out,
                        size_t n, int inverse) {
    size_t k, t;
    if (!in || !out || n == 0 || (inverse != 0 && inverse != 1))
        return SLEELA_SP_ERR_INVALID;
    for (t = 0; t < n; ++t) if (!finite_complex(in[t])) return SLEELA_SP_ERR_NUMERIC;
    const double sign = inverse ? 1.0 : -1.0;
    const double pi = acos(-1.0);
    for (k = 0; k < n; ++k) {
        long double re = 0.0L, im = 0.0L;
        for (t = 0; t < n; ++t) {
            double angle = sign * 2.0 * pi * (double)k * (double)t / (double)n;
            double c = cos(angle), s = sin(angle);
            re += (long double)in[t].re * c - (long double)in[t].im * s;
            im += (long double)in[t].re * s + (long double)in[t].im * c;
        }
        if (inverse) { re /= (long double)n; im /= (long double)n; }
        out[k].re = (double)re; out[k].im = (double)im;
        if (!finite_complex(out[k])) return SLEELA_SP_ERR_NUMERIC;
    }
    return SLEELA_SP_OK;
}

SleelaSPStatus slsp_magnitude_spectrum(const SleelaSPComplex *in, size_t n,
                                       double *mag, size_t cap) {
    size_t bins, i;
    SleelaSPComplex *copy;
    SleelaSPStatus status;
    if (!in || !mag || n == 0) return SLEELA_SP_ERR_INVALID;
    if (!is_power_of_two(n)) return SLEELA_SP_ERR_SIZE;
    bins = n / 2 + 1;
    if (cap < bins) return SLEELA_SP_ERR_CAPACITY;
    if (n > SIZE_MAX / sizeof(*copy)) return SLEELA_SP_ERR_SIZE;
    copy = (SleelaSPComplex *)malloc(n * sizeof(*copy));
    if (!copy) return SLEELA_SP_ERR_ALLOC;
    for (i = 0; i < n; ++i) copy[i] = in[i];
    status = slsp_fft(copy, n, 0);
    if (status == SLEELA_SP_OK) for (i = 0; i < bins; ++i) {
        mag[i] = hypot(copy[i].re, copy[i].im);
        if (!isfinite(mag[i])) { status = SLEELA_SP_ERR_NUMERIC; break; }
    }
    free(copy);
    return status;
}

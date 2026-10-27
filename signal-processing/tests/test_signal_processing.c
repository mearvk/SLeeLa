/* Author: Max Rupplin — MEARVK LLC — 2026 */
#include "sleela_signal_processing.h"
#include <math.h>
#include <stdio.h>

static int failures;
#define CHECK(c, m) do { if (!(c)) { fprintf(stderr, "FAIL: %s\n", (m)); ++failures; } \
                         else { printf("ok: %s\n", (m)); } } while (0)
static int near(double a, double b, double eps) { return fabs(a - b) <= eps; }

int main(void) {
    const double input[] = {1, 2, 3, 4, 5};
    double output[8] = {0}, value = 0;
    size_t out_len = 0, i;
    CHECK(slsp_version_string() != NULL, "version string is available");
    CHECK(slsp_mean(input, 5, &value) == SLEELA_SP_OK && near(value, 3.0, 1e-12),
          "compensated mean");
    {
        const double pair[] = {3, 4};
        CHECK(slsp_rms(pair, 2, &value) == SLEELA_SP_OK &&
              near(value, sqrt(12.5), 1e-12), "root mean square");
    }
    CHECK(slsp_moving_average(input, 5, 3, output, 8) == SLEELA_SP_OK,
          "causal moving average executes");
    CHECK(near(output[0], 1, 1e-12) && near(output[1], 1.5, 1e-12) &&
          near(output[2], 2, 1e-12) && near(output[3], 3, 1e-12) &&
          near(output[4], 4, 1e-12), "moving average uses partial initial windows");
    CHECK(slsp_moving_average(input, 5, 3, output, 4) == SLEELA_SP_ERR_CAPACITY,
          "moving average checks output capacity");
    CHECK(slsp_moving_average(input, 5, 0, output, 8) == SLEELA_SP_ERR_INVALID,
          "zero-sized smoothing window is rejected");
    CHECK(slsp_hann_window(output, 5) == SLEELA_SP_OK &&
          near(output[0], 0, 1e-12) && near(output[1], 0.5, 1e-12) &&
          near(output[2], 1, 1e-12) && near(output[3], 0.5, 1e-12) &&
          near(output[4], 0, 1e-12), "symmetric Hann window");
    CHECK(slsp_hann_window(output, 1) == SLEELA_SP_OK && output[0] == 1.0,
          "one-sample Hann window is defined");
    {
        const double x[] = {1, 2}, h[] = {1, 1};
        CHECK(slsp_fir_convolve(x, 2, h, 2, output, 8, &out_len) == SLEELA_SP_OK &&
              out_len == 3 && near(output[0], 1, 1e-12) &&
              near(output[1], 3, 1e-12) && near(output[2], 2, 1e-12),
              "full linear FIR convolution");
        CHECK(slsp_fir_convolve(x, 2, h, 2, output, 2, &out_len) == SLEELA_SP_ERR_CAPACITY &&
              out_len == 3, "convolution reports required capacity");
    }
    {
        SleelaSPComplex impulse[8] = {{1,0}};
        SleelaSPComplex original[4] = {{1,0}, {2,-1}, {-1,0.5}, {0,2}};
        SleelaSPComplex fast[4], direct[4];
        for (i = 0; i < 4; ++i) fast[i] = original[i];
        CHECK(slsp_fft(impulse, 8, 0) == SLEELA_SP_OK,
              "radix-2 FFT accepts a power-of-two length");
        for (i = 0; i < 8; ++i)
            CHECK(near(impulse[i].re, 1, 1e-12) && near(impulse[i].im, 0, 1e-12),
                  "FFT of an impulse is flat");
        CHECK(slsp_dft(original, direct, 4, 0) == SLEELA_SP_OK &&
              slsp_fft(fast, 4, 0) == SLEELA_SP_OK, "FFT and direct DFT execute");
        for (i = 0; i < 4; ++i)
            CHECK(near(fast[i].re, direct[i].re, 1e-9) &&
                  near(fast[i].im, direct[i].im, 1e-9), "FFT matches DFT reference");
        CHECK(slsp_fft(fast, 4, 1) == SLEELA_SP_OK, "inverse FFT executes");
        for (i = 0; i < 4; ++i)
            CHECK(near(fast[i].re, original[i].re, 1e-9) &&
                  near(fast[i].im, original[i].im, 1e-9), "FFT round trip restores input");
        CHECK(slsp_fft(fast, 3, 0) == SLEELA_SP_ERR_SIZE,
              "non-power-of-two FFT length is rejected");
        CHECK(slsp_magnitude_spectrum(original, 4, output, 3) == SLEELA_SP_OK,
              "half-spectrum magnitude output fits exact capacity");
        CHECK(slsp_magnitude_spectrum(original, 4, output, 2) == SLEELA_SP_ERR_CAPACITY,
              "half-spectrum capacity is checked");
    }
    {
        const double bad[] = {1, NAN, 3};
        CHECK(slsp_mean(bad, 3, &value) == SLEELA_SP_ERR_NUMERIC,
              "non-finite numeric input is rejected");
    }
    CHECK(slsp_mean(NULL, 0, &value) == SLEELA_SP_ERR_INVALID,
          "invalid empty input is rejected");
    printf("\n%s (%d failures)\n", failures ? "FAILED" : "PASSED", failures);
    return failures ? 1 : 0;
}

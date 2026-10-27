/* Author: Max Rupplin — MEARVK LLC — 2026 */
#include "sleela_signal_processing.h"
#include <stdio.h>

int main(void) {
    const double samples[] = {0.0, 1.0, 0.0, -1.0, 0.0, 1.0, 0.0, -1.0};
    double smoothed[8], rms = 0.0;
    SleelaSPComplex spectrum[8] = {{0}};
    double magnitudes[5];
    size_t i;
    for (i = 0; i < 8; ++i) spectrum[i].re = samples[i];
    if (slsp_moving_average(samples, 8, 3, smoothed, 8) != SLEELA_SP_OK ||
        slsp_rms(samples, 8, &rms) != SLEELA_SP_OK ||
        slsp_magnitude_spectrum(spectrum, 8, magnitudes, 5) != SLEELA_SP_OK) {
        fprintf(stderr, "signal processing failed\n");
        return 1;
    }
    printf("SLeeLa Signal Processing %s; RMS=%.6f\n", slsp_version_string(), rms);
    for (i = 0; i < 8; ++i) printf("sample[%zu] %.4f -> %.4f\n", i, samples[i], smoothed[i]);
    printf("half-spectrum magnitudes:");
    for (i = 0; i < 5; ++i) printf(" %.4f", magnitudes[i]);
    putchar('\n');
    return 0;
}

/* Author: Max Rupplin — MEARVK LLC — 2026 */
#include "sleela_signal_processing.hpp"
#include <cstdio>
#include <vector>

int main() {
    const std::vector<double> signal = {0.0, 1.0, 0.0, -1.0, 0.0, 1.0, 0.0, -1.0};
    const auto smoothed = sleela::signal::moving_average(signal, 3);
    std::vector<sleela::signal::Complex> bins;
    for (double sample : signal) bins.push_back({sample, 0.0});
    const auto magnitudes = sleela::signal::magnitude_spectrum(bins);
    const auto status = sleela::signal::fft(bins);
    if (status != SLEELA_SP_OK) {
        std::fprintf(stderr, "FFT failed: %s\n", slsp_status_string(status));
        return 1;
    }
    std::printf("SLeeLa C++ API: %zu smoothed samples, %zu half-spectrum bins\n",
                smoothed.size(), magnitudes.size());
    return 0;
}

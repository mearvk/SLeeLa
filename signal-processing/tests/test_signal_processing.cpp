/* Author: Max Rupplin — MEARVK LLC — 2026 */
#include "sleela_signal_processing.hpp"
#include <cmath>
#include <cstdio>
#include <stdexcept>
#include <vector>

int main() {
    const auto avg = sleela::signal::moving_average({1, 2, 3, 4}, 2);
    if (avg.size() != 4 || std::fabs(avg[3] - 3.5) > 1e-12) return 1;
    const auto conv = sleela::signal::convolve({1, 2}, {1, 1});
    if (conv.size() != 3 || std::fabs(conv[1] - 3.0) > 1e-12) return 2;
    const auto win = sleela::signal::hann_window(3);
    if (win.size() != 3 || std::fabs(win[1] - 1.0) > 1e-12) return 3;
    std::vector<sleela::signal::Complex> x = {{1,0}, {0,0}, {0,0}, {0,0}};
    if (sleela::signal::fft(x) != SLEELA_SP_OK) return 4;
    if (std::fabs(x[0].re - 1.0) > 1e-12 || std::fabs(x[3].re - 1.0) > 1e-12) return 5;
    const auto mags = sleela::signal::magnitude_spectrum({{1,0}, {0,0}, {0,0}, {0,0}});
    if (mags.size() != 3 || std::fabs(mags[2] - 1.0) > 1e-12) return 6;
    bool rejected = false;
    try { (void)sleela::signal::moving_average({1, 2}, 0); }
    catch (const std::invalid_argument&) { rejected = true; }
    if (!rejected) return 7;
    std::puts("PASSED (C++ signal-processing API)");
    return 0;
}

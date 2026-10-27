/* Author: Max Rupplin — MEARVK LLC — 2026 */
#include "sleela_signal_processing.hpp"
#include <stdexcept>
#include <string>

namespace sleela {
namespace signal {
namespace {
void check(SleelaSPStatus s) {
    if (s == SLEELA_SP_OK) return;
    const std::string message = slsp_status_string(s);
    if (s == SLEELA_SP_ERR_INVALID || s == SLEELA_SP_ERR_SIZE ||
        s == SLEELA_SP_ERR_CAPACITY)
        throw std::invalid_argument(message);
    if (s == SLEELA_SP_ERR_NUMERIC) throw std::overflow_error(message);
    throw std::runtime_error(message);
}
}
std::vector<double> moving_average(const std::vector<double>& input, std::size_t window) {
    if (input.empty()) throw std::invalid_argument("moving_average: input must not be empty");
    std::vector<double> output(input.size());
    check(slsp_moving_average(input.data(), input.size(), window, output.data(), output.size()));
    return output;
}
std::vector<double> convolve(const std::vector<double>& input,
                             const std::vector<double>& kernel) {
    if (input.empty() || kernel.empty())
        throw std::invalid_argument("convolve: input and kernel must not be empty");
    if (input.size() > static_cast<std::size_t>(-1) - (kernel.size() - 1))
        throw std::invalid_argument("convolve: size overflow");
    std::vector<double> output(input.size() + kernel.size() - 1);
    std::size_t output_len = 0;
    check(slsp_fir_convolve(input.data(), input.size(), kernel.data(), kernel.size(),
                            output.data(), output.size(), &output_len));
    return output;
}
std::vector<double> hann_window(std::size_t length) {
    if (length == 0) throw std::invalid_argument("hann_window: length must be positive");
    std::vector<double> output(length);
    check(slsp_hann_window(output.data(), output.size()));
    return output;
}
Status fft(std::vector<Complex>& data, bool inverse) noexcept {
    return slsp_fft(data.empty() ? nullptr : data.data(), data.size(), inverse ? 1 : 0);
}
std::vector<double> magnitude_spectrum(const std::vector<Complex>& input) {
    if (input.empty()) throw std::invalid_argument("magnitude_spectrum: input must not be empty");
    const std::size_t bins = input.size() / 2 + 1;
    std::vector<double> output(bins);
    check(slsp_magnitude_spectrum(input.data(), input.size(), output.data(), output.size()));
    return output;
}
} // namespace signal
} // namespace sleela

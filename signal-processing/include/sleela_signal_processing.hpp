/* Author: Max Rupplin — MEARVK LLC — 2026 */
#ifndef SLEELA_SIGNAL_PROCESSING_HPP
#define SLEELA_SIGNAL_PROCESSING_HPP
#include "sleela_signal_processing.h"
#include <cstddef>
#include <vector>

namespace sleela {
namespace signal {
using Complex = SleelaSPComplex;
using Status = SleelaSPStatus;

/* Invalid inputs throw std::invalid_argument, numeric overflow throws
 * std::overflow_error, other library errors throw std::runtime_error.
 * std::bad_alloc from vector allocation propagates normally. */
std::vector<double> moving_average(const std::vector<double>& input,
                                   std::size_t window);
std::vector<double> convolve(const std::vector<double>& input,
                             const std::vector<double>& kernel);
std::vector<double> hann_window(std::size_t length);
Status fft(std::vector<Complex>& data, bool inverse = false) noexcept;
std::vector<double> magnitude_spectrum(const std::vector<Complex>& input);
} // namespace signal
} // namespace sleela
#endif

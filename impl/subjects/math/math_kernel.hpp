#ifndef SLEELA_NATIVE_MATH_KERNEL_HPP
#define SLEELA_NATIVE_MATH_KERNEL_HPP

// Typed native numerical kernel for the SLeeLa math subject.
//
// This is the C++/IEEE-754 implementation boundary. It is deliberately kept
// separate from math_native.cpp, whose synthesized Core methods remain the
// portable VM fallback until typed native-call dispatch is wired end-to-end.
// All functions are noexcept and inherit the platform C++ <cmath> domain,
// range, signed-zero, infinity, and NaN behavior.

#include <cmath>

namespace sleela {
namespace native {
namespace math_kernel {

inline double abs(double x) noexcept { return std::fabs(x); }
inline double sqrt(double x) noexcept { return std::sqrt(x); }
inline double cbrt(double x) noexcept { return std::cbrt(x); }
inline double exp(double x) noexcept { return std::exp(x); }
inline double exp2(double x) noexcept { return std::exp2(x); }
inline double log(double x) noexcept { return std::log(x); }
inline double log2(double x) noexcept { return std::log2(x); }
inline double log10(double x) noexcept { return std::log10(x); }
inline double pow(double x, double y) noexcept { return std::pow(x, y); }
inline double sin(double x) noexcept { return std::sin(x); }
inline double cos(double x) noexcept { return std::cos(x); }
inline double tan(double x) noexcept { return std::tan(x); }
inline double asin(double x) noexcept { return std::asin(x); }
inline double acos(double x) noexcept { return std::acos(x); }
inline double atan(double x) noexcept { return std::atan(x); }
inline double atan2(double y, double x) noexcept { return std::atan2(y, x); }
inline double sinh(double x) noexcept { return std::sinh(x); }
inline double cosh(double x) noexcept { return std::cosh(x); }
inline double tanh(double x) noexcept { return std::tanh(x); }
inline double asinh(double x) noexcept { return std::asinh(x); }
inline double acosh(double x) noexcept { return std::acosh(x); }
inline double atanh(double x) noexcept { return std::atanh(x); }
inline double hypot(double x, double y) noexcept { return std::hypot(x, y); }
inline double fmod(double x, double y) noexcept { return std::fmod(x, y); }
inline double remainder(double x, double y) noexcept { return std::remainder(x, y); }
inline double floor(double x) noexcept { return std::floor(x); }
inline double ceil(double x) noexcept { return std::ceil(x); }
inline double trunc(double x) noexcept { return std::trunc(x); }
inline double round(double x) noexcept { return std::round(x); }
inline double copysign(double x, double y) noexcept { return std::copysign(x, y); }
inline double nextafter(double x, double toward) noexcept { return std::nextafter(x, toward); }
inline bool isfinite(double x) noexcept { return std::isfinite(x); }
inline bool isinf(double x) noexcept { return std::isinf(x); }
inline bool isnan(double x) noexcept { return std::isnan(x); }

} // namespace math_kernel
} // namespace native
} // namespace sleela

#endif // SLEELA_NATIVE_MATH_KERNEL_HPP

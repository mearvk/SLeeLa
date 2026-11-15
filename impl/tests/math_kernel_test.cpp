#include "../subjects/math/math_kernel.hpp"
#include "../subjects/math/sleela_math_kernel_c.h"

#include <cassert>
#include <cmath>
#include <limits>

namespace mk = sleela::native::math_kernel;

static bool near(double a, double b, double tolerance = 1e-12) {
    return std::fabs(a - b) <= tolerance * (1.0 + std::fabs(b));
}

int main() {
    double abi_result = 0.0;
    const double abi_args[] = {9.0};
    assert(sl_math_kernel_arity(SL_MATH_SQRT) == 1);
    assert(sl_math_kernel_arity(-1) == -1);
    assert(sl_math_kernel_method_name(SL_MATH_SQRT) != nullptr);
    assert(sl_math_kernel_call(SL_MATH_SQRT, abi_args, 1, &abi_result) == 0);
    assert(near(abi_result, 3.0));
    assert(sl_math_kernel_call(SL_MATH_SQRT, abi_args, 0, &abi_result) == -1);

    assert(near(mk::sqrt(2.0), std::sqrt(2.0)));
    assert(near(mk::cbrt(-27.0), -3.0));
    assert(near(mk::pow(-2.0, 3.0), -8.0));
    assert(near(mk::pow(-2.0, 4.0), 16.0));
    assert(std::isnan(mk::pow(-2.0, 0.5)));
    assert(std::isnan(mk::sqrt(-1.0)));
    assert(std::isnan(mk::log(-1.0)));
    assert(std::isinf(mk::log(0.0)) && mk::log(0.0) < 0.0);
    assert(near(mk::hypot(3.0, 4.0), 5.0));
    assert(near(mk::hypot(1.0e308, 1.0e308) / 1.0e308, std::sqrt(2.0)));
    assert(near(mk::hypot(1.0e-300, 1.0e-300) / 1.0e-300, std::sqrt(2.0)));
    assert(std::isnan(mk::fmod(1.0, 0.0)));
    assert(near(mk::asin(1.0), std::acos(-1.0) / 2.0));
    assert(near(mk::atan2(1.0, 0.0), std::acos(-1.0) / 2.0));
    assert(near(mk::sinh(0.0), 0.0));
    assert(near(mk::tanh(1000.0), 1.0));
    assert(near(mk::floor(-3.2), -4.0));
    assert(near(mk::ceil(-3.2), -3.0));
    assert(near(mk::trunc(-3.7), -3.0));
    assert(mk::isfinite(1.0) && mk::isinf(std::numeric_limits<double>::infinity()));
    assert(mk::isnan(std::numeric_limits<double>::quiet_NaN()));
    return 0;
}

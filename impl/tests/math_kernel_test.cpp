#include "../subjects/math/math_kernel.hpp"

#include <cassert>
#include <cmath>
#include <limits>

using namespace sleela::native::math_kernel;

static bool near(double a, double b, double tolerance = 1e-12) {
    return std::fabs(a - b) <= tolerance * (1.0 + std::fabs(b));
}

int main() {
    assert(near(sqrt(2.0), std::sqrt(2.0)));
    assert(near(cbrt(-27.0), -3.0));
    assert(near(pow(-2.0, 3.0), -8.0));
    assert(near(pow(-2.0, 4.0), 16.0));
    assert(std::isnan(pow(-2.0, 0.5)));
    assert(std::isnan(sqrt(-1.0)));
    assert(std::isnan(log(-1.0)));
    assert(std::isinf(log(0.0)) && log(0.0) < 0.0);
    assert(near(hypot(3.0, 4.0), 5.0));
    assert(near(hypot(1.0e308, 1.0e308) / 1.0e308, std::sqrt(2.0)));
    assert(near(hypot(1.0e-300, 1.0e-300) / 1.0e-300, std::sqrt(2.0)));
    assert(std::isnan(fmod(1.0, 0.0)));
    assert(near(asin(1.0), std::acos(-1.0) / 2.0));
    assert(near(atan2(1.0, 0.0), std::acos(-1.0) / 2.0));
    assert(near(sinh(0.0), 0.0));
    assert(near(tanh(1000.0), 1.0));
    assert(near(floor(-3.2), -4.0));
    assert(near(ceil(-3.2), -3.0));
    assert(near(trunc(-3.7), -3.0));
    assert(isfinite(1.0) && isinf(std::numeric_limits<double>::infinity()));
    assert(isnan(std::numeric_limits<double>::quiet_NaN()));
    return 0;
}

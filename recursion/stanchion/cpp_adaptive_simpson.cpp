// Scientific recursion: adaptive Simpson integration with tolerance and depth bounds.
// Build: c++ -std=c++17 -Wall -Wextra -Wpedantic cpp_adaptive_simpson.cpp -o cpp_adaptive_simpson
#include <cmath>
#include <functional>
#include <iostream>
#include <stdexcept>

double simpson(const std::function<double(double)>& f, double a, double b) {
    const double m = a + (b - a) / 2.0;
    return (b - a) * (f(a) + 4.0 * f(m) + f(b)) / 6.0;
}

double adaptive(const std::function<double(double)>& f, double a, double b,
                double eps, double whole, unsigned depth) {
    const double m = a + (b - a) / 2.0;
    const double left = simpson(f, a, m);
    const double right = simpson(f, m, b);
    const double delta = left + right - whole;
    if (!std::isfinite(left) || !std::isfinite(right))
        throw std::runtime_error("non-finite integrand result");
    if (std::abs(delta) <= 15.0 * eps) return left + right + delta / 15.0;
    if (depth == 0U) throw std::runtime_error("adaptive Simpson depth limit reached");
    return adaptive(f, a, m, eps / 2.0, left, depth - 1U)
         + adaptive(f, m, b, eps / 2.0, right, depth - 1U);
}

int main() {
    const auto f = [](double x) { return std::sin(x); };
    const double a = 0.0, b = 3.14159265358979323846;
    const double whole = simpson(f, a, b);
    try {
        const double result = adaptive(f, a, b, 1e-10, whole, 30U);
        std::cout.precision(12);
        std::cout << "integral sin(x) from 0 to pi ~= " << result << '\n';
    } catch (const std::exception& error) {
        std::cerr << "integration failed: " << error.what() << '\n';
        return 1;
    }
}

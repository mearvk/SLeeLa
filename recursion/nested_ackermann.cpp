// Nested recursion: bounded Ackermann example. Inputs are intentionally small;
// this function grows extremely quickly and consumes stack depth.
 // Build: c++ -std=c++17 -Wall -Wextra -Wpedantic nested_ackermann.cpp -o nested_ackermann
#include <iostream>

unsigned long long ackermann(unsigned int m, unsigned long long n) {
    if (m == 0) return n + 1;
    if (n == 0) return ackermann(m - 1, 1);
    return ackermann(m - 1, ackermann(m, n - 1));
}

int main() {
    constexpr unsigned int m = 3;
    constexpr unsigned long long n = 4;
    std::cout << "A(" << m << ", " << n << ") = " << ackermann(m, n) << '\n';
}

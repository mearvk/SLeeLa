// Generic C++ recursion: fold an iterator range with a supplied operation.
// Build: c++ -std=c++17 -Wall -Wextra -Wpedantic cpp_recursive_fold.cpp -o cpp_recursive_fold
#include <functional>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

template<class It, class T, class Op>
T recursive_fold(It first, It last, T initial, Op op, std::size_t depth = 0U) {
    constexpr std::size_t max_depth = 4096U;
    if (first == last) return initial;
    if (depth >= max_depth)
        throw std::length_error("recursive_fold depth limit exceeded");
    T next = op(initial, *first);
    ++first;
    return recursive_fold<It, T, Op>(first, last, next, op, depth + 1U);
}

int main() {
    const std::vector<int> values{2, 4, 6, 8};
    const auto sum = recursive_fold(values.begin(), values.end(), 0LL,
        [](long long total, int value) { return total + value; });
    std::cout << "fold sum = " << sum << '\n';
}

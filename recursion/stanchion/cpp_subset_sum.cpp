// Backtracking recursion: find one subset of non-negative values that sums to target.
// Build: c++ -std=c++17 -Wall -Wextra -Wpedantic cpp_subset_sum.cpp -o cpp_subset_sum
#include <iostream>
#include <vector>

bool find_subset(const std::vector<int>& values, std::size_t index, long long remaining,
                 std::vector<int>& chosen, std::size_t depth = 0U) {
    constexpr std::size_t max_depth = 2048U;
    if (remaining == 0) return true;
    if (index >= values.size() || depth >= max_depth || remaining < 0) return false;
    if (values[index] < 0) return false; // This prototype's contract is non-negative input.

    chosen.push_back(values[index]);
    if (find_subset(values, index + 1U, remaining - values[index], chosen, depth + 1U))
        return true;
    chosen.pop_back();
    return find_subset(values, index + 1U, remaining, chosen, depth + 1U);
}

int main() {
    const std::vector<int> values{3, 7, 2, 9, 4, 6};
    const long long target = 15;
    std::vector<int> chosen;
    if (find_subset(values, 0U, target, chosen)) {
        std::cout << "subset for " << target << ": ";
        for (int value : chosen) std::cout << value << ' ';
        std::cout << '\n';
    } else {
        std::cout << "no subset found (or depth limit reached)\n";
    }
}

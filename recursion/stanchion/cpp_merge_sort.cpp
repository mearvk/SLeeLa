// Divide-and-conquer recursion: merge sort over a vector.
// Build: c++ -std=c++17 -Wall -Wextra -Wpedantic cpp_merge_sort.cpp -o cpp_merge_sort
#include <iostream>
#include <vector>

void merge_sort(std::vector<int>& values, std::size_t lo, std::size_t hi) {
    if (hi - lo < 2U) return;
    const std::size_t mid = lo + (hi - lo) / 2U;
    merge_sort(values, lo, mid);
    merge_sort(values, mid, hi);
    std::vector<int> merged;
    merged.reserve(hi - lo);
    std::size_t i = lo, j = mid;
    while (i < mid && j < hi)
        merged.push_back(values[i] <= values[j] ? values[i++] : values[j++]);
    while (i < mid) merged.push_back(values[i++]);
    while (j < hi) merged.push_back(values[j++]);
    for (std::size_t k = 0; k < merged.size(); ++k) values[lo + k] = merged[k];
}

int main() {
    std::vector<int> values{9, 1, 5, 3, 7, 2, 8, 4, 6};
    merge_sort(values, 0U, values.size());
    for (int value : values) std::cout << value << ' ';
    std::cout << '\n';
}

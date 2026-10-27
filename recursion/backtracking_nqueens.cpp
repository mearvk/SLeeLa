// Backtracking with local iteration and tree-shaped recursion.
// Counts solutions for a small N-Queens board (N=8).
 // Build: c++ -std=c++17 -Wall -Wextra -Wpedantic backtracking_nqueens.cpp -o backtracking_nqueens
#include <cstdlib>
#include <iostream>
#include <vector>

bool safe(const std::vector<int>& columns, int row, int col) {
    for (int r = 0; r < row; ++r) {
        const int c = columns[static_cast<std::size_t>(r)];
        if (c == col || std::abs(c - col) == row - r) return false;
    }
    return true;
}

unsigned long long search(std::vector<int>& columns, int row) {
    constexpr int n = 8;
    if (row == n) return 1;
    unsigned long long solutions = 0;
    for (int col = 0; col < n; ++col) {  // local iteration at each depth
        if (safe(columns, row, col)) {
            columns[static_cast<std::size_t>(row)] = col;
            solutions += search(columns, row + 1); // recurse, then backtrack
        }
    }
    return solutions;
}

int main() {
    std::vector<int> columns(8, -1);
    std::cout << "8-Queens solutions = " << search(columns, 0) << '\n';
}

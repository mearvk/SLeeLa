// Mutual (indirect) recursion: is_even calls is_odd and vice versa.
 // Build: c++ -std=c++17 -Wall -Wextra -Wpedantic mutual_even_odd.cpp -o mutual_even_odd
#include <iostream>

bool is_odd(unsigned int n);

bool is_even(unsigned int n) {
    return n == 0 ? true : is_odd(n - 1);
}
bool is_odd(unsigned int n) {
    return n == 0 ? false : is_even(n - 1);
}

int main() {
    for (unsigned int n : {0U, 1U, 2U, 7U, 12U})
        std::cout << n << (is_even(n) ? " is even\n" : " is odd\n");
}

/* Divide-and-conquer recursion over a sorted array.
 * Build: cc -std=c11 -Wall -Wextra -Wpedantic c_recursive_binary_search.c -o c_recursive_binary_search
 */
#include <stddef.h>
#include <stdio.h>

static size_t lower_bound_recursive(const int *a, size_t lo, size_t hi, int key) {
    if (lo >= hi) return lo;
    const size_t mid = lo + (hi - lo) / 2U;
    if (a[mid] < key) return lower_bound_recursive(a, mid + 1U, hi, key);
    return lower_bound_recursive(a, lo, mid, key);
}

int main(void) {
    const int sorted[] = {-8, -2, 0, 3, 3, 7, 12, 19};
    const size_t n = sizeof sorted / sizeof sorted[0];
    const int key = 7;
    const size_t pos = lower_bound_recursive(sorted, 0U, n, key);
    if (pos < n && sorted[pos] == key)
        printf("%d found at index %zu\n", key, pos);
    else
        printf("%d not found (insertion index %zu)\n", key, pos);
    return 0;
}

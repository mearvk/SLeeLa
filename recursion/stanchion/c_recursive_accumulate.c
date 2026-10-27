/* Generic pattern: recursive accumulation over a bounded integer array.
 * Build: cc -std=c11 -Wall -Wextra -Wpedantic c_recursive_accumulate.c -o c_recursive_accumulate
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define MAX_RECURSION_DEPTH 4096U

static bool accumulate_from(const int *items, size_t length, size_t index,
                           size_t depth, long long *sum) {
    if (sum == NULL || (length != 0U && items == NULL)) return false;
    if (depth > MAX_RECURSION_DEPTH) return false;
    if (index >= length) {
        *sum = 0;
        return true;
    }

    long long rest = 0;
    if (!accumulate_from(items, length, index + 1U, depth + 1U, &rest))
        return false;
    *sum = (long long)items[index] + rest;
    return true;
}

int main(void) {
    const int values[] = {3, 1, 4, 1, 5, 9};
    long long result = 0;
    if (!accumulate_from(values, sizeof values / sizeof values[0], 0U, 0U,
                         &result)) {
        fputs("input invalid or recursion limit exceeded\n", stderr);
        return 1;
    }
    printf("sum = %lld\n", result);
    return 0;
}

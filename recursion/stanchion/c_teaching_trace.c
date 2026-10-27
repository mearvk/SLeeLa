/* Teaching pattern: print recursive calls and returns for factorial.
 * Build: cc -std=c11 -Wall -Wextra -Wpedantic c_teaching_trace.c -o c_teaching_trace
 */
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_N 20U

static bool factorial_trace(unsigned n, unsigned depth, unsigned long long *out) {
    for (unsigned i = 0; i < depth; ++i) fputs("  ", stdout);
    printf("factorial(%u) called\n", n);
    if (n <= 1U) {
        *out = 1ULL;
    } else {
        unsigned long long previous;
        if (!factorial_trace(n - 1U, depth + 1U, &previous)) return false;
        if (previous > ULLONG_MAX / n) return false;
        *out = previous * n;
    }
    for (unsigned i = 0; i < depth; ++i) fputs("  ", stdout);
    printf("factorial(%u) returns %llu\n", n, *out);
    return true;
}

int main(void) {
    unsigned long long result = 0;
    const unsigned n = 6U;
    if (n > MAX_N || !factorial_trace(n, 0U, &result)) {
        fputs("value outside supported range\n", stderr);
        return 1;
    }
    return 0;
}

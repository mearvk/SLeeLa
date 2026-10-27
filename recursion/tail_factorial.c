/* Tail recursion with an accumulator. C does not guarantee tail-call optimization.
 * Build: cc -std=c11 -Wall -Wextra -Wpedantic tail_factorial.c -o tail_factorial
 */
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

static bool factorial_tail(unsigned int n, unsigned long long acc,
                           unsigned long long *result) {
    if (n <= 1) { *result = acc; return true; }
    if (acc > ULLONG_MAX / n) return false;
    return factorial_tail(n - 1, acc * n, result); /* final operation */
}

int main(void) {
    unsigned long long result;
    const unsigned int n = 12;
    if (!factorial_tail(n, 1, &result)) {
        fputs("factorial overflows unsigned long long\n", stderr);
        return 1;
    }
    printf("%u! = %llu\n", n, result);
    return 0;
}

/* Applied numeric recursion: Euclid's greatest common divisor.
 * Build: cc -std=c11 -Wall -Wextra -Wpedantic c_recursive_gcd.c -o c_recursive_gcd
 */
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static unsigned long long gcd_u(unsigned long long a, unsigned long long b) {
    return b == 0ULL ? a : gcd_u(b, a % b);
}

static int parse_ull(const char *text, unsigned long long *out) {
    char *end = NULL;
    unsigned long long value;
    if (text == NULL || *text == '\0' || *text == '-') return 0;
    errno = 0;
    value = strtoull(text, &end, 10);
    if (errno == ERANGE || end == text || *end != '\0') return 0;
    *out = value;
    return 1;
}

int main(int argc, char **argv) {
    unsigned long long a, b;
    if (argc != 3 || !parse_ull(argv[1], &a) || !parse_ull(argv[2], &b)) {
        fprintf(stderr, "usage: %s NONNEGATIVE_INTEGER NONNEGATIVE_INTEGER\n", argv[0]);
        return 2;
    }
    printf("gcd = %llu\n", gcd_u(a, b));
    return 0;
}

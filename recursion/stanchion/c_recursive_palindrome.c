/* ASCII text normalization followed by two-ended recursion.
 * Build: cc -std=c11 -Wall -Wextra -Wpedantic c_recursive_palindrome.c -o c_recursive_palindrome
 * Note: byte-wise tolower/isalnum is not Unicode text processing.
 */
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define INPUT_CAPACITY 512U

static bool palindrome_range(const char *s, size_t left, size_t right) {
    while (left < right && !isalnum((unsigned char)s[left])) ++left;
    while (left < right && !isalnum((unsigned char)s[right])) --right;
    if (left >= right) return true;
    if (tolower((unsigned char)s[left]) != tolower((unsigned char)s[right]))
        return false;
    return palindrome_range(s, left + 1U, right - 1U);
}

int main(void) {
    char input[INPUT_CAPACITY];
    size_t n;
    fputs("Enter one line (maximum 511 bytes): ", stdout);
    if (fgets(input, sizeof input, stdin) == NULL) return 1;
    n = strcspn(input, "\n");
    if (input[n] == '\0' && n == sizeof input - 1U) {
        fputs("input may be too long; please use a shorter line\n", stderr);
        return 2;
    }
    input[n] = '\0';
    printf("%s\n", palindrome_range(input, 0U, n == 0U ? 0U : n - 1U)
                            ? "palindrome" : "not a palindrome");
    return 0;
}

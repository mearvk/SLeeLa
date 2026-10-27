/* Linear recursion over a string (ASCII example).
 * Build: cc -std=c11 -Wall -Wextra -Wpedantic string_palindrome.c -o string_palindrome
 */
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static bool is_palindrome_range(const char *s, size_t left, size_t right) {
    while (left < right && !isalnum((unsigned char)s[left])) ++left;
    while (left < right && !isalnum((unsigned char)s[right])) --right;
    if (left >= right) return true;
    if (tolower((unsigned char)s[left]) != tolower((unsigned char)s[right]))
        return false;
    return is_palindrome_range(s, left + 1, right - 1);
}

int main(void) {
    const char *text = "Never odd, or even!";
    size_t n = strlen(text);
    printf("\"%s\" %s a palindrome\n", text,
           is_palindrome_range(text, 0, n ? n - 1 : 0) ? "is" : "is not");
    return 0;
}

#include "feedback.h"

#include <stdio.h>
#include <string.h>

static int check(const char *input, char expected) {
    char actual = '?';
    if (sommeliers_feedback_symbol(input, &actual) != 0 || actual != expected) {
        fprintf(stderr, "FAIL input='%s': expected '%c', got '%c'\n", input, expected, actual);
        return 1;
    }
    return 0;
}

int main(void) {
    int failures = 0;
    char bits[33];
    failures += check("0", '0');
    failures += check("1", '1');
    failures += check("s", 's');
    failures += check("08s1", '1');
    if (sommeliers_to_binary("A", bits, sizeof(bits)) != 8 ||
        strcmp(bits, "01000001") != 0) {
        fprintf(stderr, "FAIL binary conversion for A: '%s'\n", bits);
        ++failures;
    }
    if (sommeliers_feedback_symbol("", bits) == 0) {
        fprintf(stderr, "FAIL empty input should be rejected\n");
        ++failures;
    }
    if (failures) return 1;
    puts("sommeliers feedback tests: PASS");
    return 0;
}

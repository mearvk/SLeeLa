#include "sleela_path.h"

#include <stdio.h>
#include <string.h>

int main(void) {
    char joined[512];
    char normalized[512];
    char absolute[1024];
    char cwd[1024];
    const char* leaf = "sleela-path-smoke.tmp";

    if (slpath_current_directory(cwd, sizeof(cwd)) != 0) return 1;
    if (slpath_join(joined, sizeof(joined), cwd, leaf) != 0) return 1;
    if (slpath_normalize(normalized, sizeof(normalized), joined) != 0) return 1;
    if (slpath_absolute(absolute, sizeof(absolute), normalized) != 0) return 1;
    if (strcmp(normalized, absolute) != 0) return 1;
    if (slpath_is_directory(cwd) != 1) return 1;
    if (slpath_is_file(absolute) != 0) return 1;

    printf("paths/filesystem platform: %s\n", slpath_platform_name());
    printf("path separator: %c\n", slpath_separator());
    printf("paths/filesystem platform smoke: PASS\n");
    return 0;
}

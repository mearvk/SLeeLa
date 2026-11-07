#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int utf8_codepoints(const unsigned char *s, size_t n) {
    size_t i = 0;
    int count = 0;
    while (i < n) {
        unsigned char c = s[i];
        size_t width;
        if (c <= 0x7f) width = 1;
        else if (c >= 0xc2 && c <= 0xdf) width = 2;
        else if (c >= 0xe0 && c <= 0xef) width = 3;
        else if (c >= 0xf0 && c <= 0xf4) width = 4;
        else return -1;
        if (i + width > n) return -1;
        for (size_t j = 1; j < width; ++j) {
            if ((s[i + j] & 0xc0) != 0x80) return -1;
        }
        i += width;
        ++count;
    }
    return count;
}

int main(void) {
    const int32_t min = INT32_MIN;
    const int32_t max = INT32_MAX;
    uint32_t wrapped = UINT32_MAX;
    const int32_t dividend = -7;
    const int32_t divisor = 3;
    const unsigned char text[] = "A\xc3\xa9\xf0\x9f\x98\x80";
    const size_t bytes = sizeof(text) - 1;

    wrapped += UINT32_C(1); /* Unsigned arithmetic is defined modulo 2^32. */
    printf("INT32_MIN=%" PRId32 "\n", min);
    printf("INT32_MAX=%" PRId32 "\n", max);
    printf("U32_WRAP=%" PRIu32 "\n", wrapped);
    printf("SIGNED_DIV=%" PRId32 "\n", dividend / divisor);
    printf("SIGNED_REM=%" PRId32 "\n", dividend % divisor);
    printf("UTF8_BYTES=%zu\n", bytes);
    printf("UNICODE_CODEPOINTS=%d\n", utf8_codepoints(text, bytes));
    return 0;
}

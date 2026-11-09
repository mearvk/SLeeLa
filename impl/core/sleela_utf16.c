#include "sleela_utf16.h"
#include <limits.h>
#include <stdlib.h>

static int decode_utf8(const unsigned char *s, size_t n, size_t *offset,
                       uint32_t *out_cp) {
    if (!s || !offset || !out_cp || *offset >= n) return -2;
    size_t i = *offset;
    unsigned char a = s[i++];
    uint32_t cp;
    size_t extra;
    if (a <= 0x7f) { cp = a; extra = 0; }
    else if (a >= 0xc2 && a <= 0xdf) { cp = (uint32_t)(a & 0x1f); extra = 1; }
    else if (a >= 0xe0 && a <= 0xef) { cp = (uint32_t)(a & 0x0f); extra = 2; }
    else if (a >= 0xf0 && a <= 0xf4) { cp = (uint32_t)(a & 0x07); extra = 3; }
    else return -2;
    if (extra > n - i) return -2;
    for (size_t k = 0; k < extra; ++k) {
        unsigned char b = s[i++];
        if ((b & 0xc0) != 0x80) return -2;
        cp = (cp << 6) | (uint32_t)(b & 0x3f);
    }
    if ((extra == 1 && cp < 0x80) || (extra == 2 && cp < 0x800) ||
        (extra == 3 && cp < 0x10000) ||
        (cp >= 0xd800 && cp <= 0xdfff) || cp > 0x10ffff) return -2;
    *offset = i; *out_cp = cp; return 0;
}

int sleela_utf8_utf16_length(const char *utf8, size_t byte_length,
                            int64_t *out_code_units) {
    if ((!utf8 && byte_length != 0) || !out_code_units) return -1;
    const unsigned char *s = (const unsigned char *)utf8;
    size_t offset = 0; int64_t count = 0;
    while (offset < byte_length) {
        uint32_t cp;
        int rc = decode_utf8(s, byte_length, &offset, &cp);
        if (rc != 0) return rc;
        int64_t width = cp > 0xffff ? 2 : 1;
        if (count > INT64_MAX - width) return -1;
        count += width;
    }
    *out_code_units = count; return 0;
}


int sleela_utf8_to_utf16(const char *utf8, size_t byte_length,
                         uint16_t **out_units, size_t *out_code_units) {
    if ((!utf8 && byte_length != 0) || !out_units || !out_code_units) return -1;
    *out_units = NULL;
    *out_code_units = 0;
    int64_t count = 0;
    int status = sleela_utf8_utf16_length(utf8, byte_length, &count);
    if (status != 0) return status;
    if ((uint64_t)count > SIZE_MAX / sizeof(uint16_t)) return -1;
    uint16_t *units = count ? (uint16_t *)malloc((size_t)count * sizeof(uint16_t)) : NULL;
    if (count && !units) return -1;
    const unsigned char *s = (const unsigned char *)utf8;
    size_t offset = 0, used = 0;
    while (offset < byte_length) {
        uint32_t cp;
        status = decode_utf8(s, byte_length, &offset, &cp);
        if (status != 0) { free(units); return status; }
        if (cp <= 0xffff) units[used++] = (uint16_t)cp;
        else {
            cp -= 0x10000;
            units[used++] = (uint16_t)(0xd800u + (cp >> 10));
            units[used++] = (uint16_t)(0xdc00u + (cp & 0x3ffu));
        }
    }
    *out_units = units;
    *out_code_units = used;
    return 0;
}

int sleela_utf8_utf16_char_at(const char *utf8, size_t byte_length,
                              int64_t index, uint16_t *out_code_unit) {
    if ((!utf8 && byte_length != 0) || !out_code_unit || index < 0) return -1;
    const unsigned char *s = (const unsigned char *)utf8;
    size_t offset = 0; int64_t position = 0;
    while (offset < byte_length) {
        uint32_t cp;
        int rc = decode_utf8(s, byte_length, &offset, &cp);
        if (rc != 0) return rc;
        if (cp <= 0xffff) {
            if (position == index) { *out_code_unit = (uint16_t)cp; return 0; }
            ++position;
        } else {
            uint32_t value = cp - 0x10000;
            uint16_t high = (uint16_t)(0xd800 + (value >> 10));
            uint16_t low = (uint16_t)(0xdc00 + (value & 0x3ff));
            if (position == index) { *out_code_unit = high; return 0; }
            if (position + 1 == index) { *out_code_unit = low; return 0; }
            position += 2;
        }
    }
    return -3;
}

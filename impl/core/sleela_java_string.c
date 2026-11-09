#include "sleela_java_string.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define SL_JAVA_STRING_MAX_LENGTH ((size_t)INT32_MAX)

static int valid_value(const SLJavaString *value) {
    return value && (value->length == 0 || value->units != NULL) &&
           value->length <= SL_JAVA_STRING_MAX_LENGTH;
}

void sljava_string_init_empty(SLJavaString *value) {
    if (!value) return;
    value->units = NULL;
    value->length = 0;
}

void sljava_string_destroy(SLJavaString *value) {
    if (!value) return;
    free(value->units);
    value->units = NULL;
    value->length = 0;
}

int sljava_string_from_utf16(SLJavaString *out, const uint16_t *units, size_t length) {
    uint16_t *copy = NULL;
    if (!out || (!units && length != 0)) return SL_JAVA_STRING_INVALID_ARGUMENT;
    if (length > SL_JAVA_STRING_MAX_LENGTH) return SL_JAVA_STRING_TOO_LARGE;
    if (length != 0) {
        if (length > SIZE_MAX / sizeof(*copy)) return SL_JAVA_STRING_TOO_LARGE;
        copy = (uint16_t *)malloc(length * sizeof(*copy));
        if (!copy) return SL_JAVA_STRING_OUT_OF_MEMORY;
        memcpy(copy, units, length * sizeof(*copy));
    }
    sljava_string_destroy(out);
    out->units = copy;
    out->length = length;
    return SL_JAVA_STRING_OK;
}

int sljava_string_clone(SLJavaString *out, const SLJavaString *value) {
    if (!valid_value(value) || !out) return SL_JAVA_STRING_INVALID_ARGUMENT;
    return sljava_string_from_utf16(out, value->units, value->length);
}

int sljava_string_length(const SLJavaString *value, int32_t *out_length) {
    if (!valid_value(value) || !out_length) return SL_JAVA_STRING_INVALID_ARGUMENT;
    *out_length = (int32_t)value->length;
    return SL_JAVA_STRING_OK;
}

int sljava_string_char_at(const SLJavaString *value, int32_t index, uint16_t *out_unit) {
    if (!valid_value(value) || !out_unit) return SL_JAVA_STRING_INVALID_ARGUMENT;
    if (index < 0 || (size_t)index >= value->length)
        return SL_JAVA_STRING_INDEX_OUT_OF_BOUNDS;
    *out_unit = value->units[index];
    return SL_JAVA_STRING_OK;
}

static int is_high_surrogate(uint16_t unit) { return unit >= 0xd800u && unit <= 0xdbffu; }
static int is_low_surrogate(uint16_t unit) { return unit >= 0xdc00u && unit <= 0xdfffu; }

int sljava_string_code_point_at(const SLJavaString *value, int32_t index,
                                int32_t *out_code_point) {
    uint16_t first;
    if (!valid_value(value) || !out_code_point) return SL_JAVA_STRING_INVALID_ARGUMENT;
    if (index < 0 || (size_t)index >= value->length)
        return SL_JAVA_STRING_INDEX_OUT_OF_BOUNDS;
    first = value->units[index];
    if (is_high_surrogate(first) && (size_t)index + 1 < value->length &&
        is_low_surrogate(value->units[index + 1])) {
        *out_code_point = 0x10000 +
            (((int32_t)first - 0xd800) << 10) +
            ((int32_t)value->units[index + 1] - 0xdc00);
    } else {
        /* Java preserves and returns an isolated surrogate's numeric value. */
        *out_code_point = (int32_t)first;
    }
    return SL_JAVA_STRING_OK;
}

int sljava_string_code_point_count(const SLJavaString *value, int32_t begin_index,
                                   int32_t end_index, int32_t *out_count) {
    size_t i;
    int32_t count = 0;
    if (!valid_value(value) || !out_count) return SL_JAVA_STRING_INVALID_ARGUMENT;
    if (begin_index < 0 || end_index < begin_index ||
        (size_t)end_index > value->length) return SL_JAVA_STRING_RANGE_OUT_OF_BOUNDS;
    i = (size_t)begin_index;
    while (i < (size_t)end_index) {
        if (is_high_surrogate(value->units[i]) && i + 1 < (size_t)end_index &&
            is_low_surrogate(value->units[i + 1])) i += 2;
        else i++;
        count++;
    }
    *out_count = count;
    return SL_JAVA_STRING_OK;
}

int sljava_string_offset_by_code_points(const SLJavaString *value, int32_t index,
                                        int32_t offset, int32_t *out_index) {
    int64_t position;
    int64_t remaining;
    if (!valid_value(value) || !out_index) return SL_JAVA_STRING_INVALID_ARGUMENT;
    if (index < 0 || (size_t)index > value->length)
        return SL_JAVA_STRING_INDEX_OUT_OF_BOUNDS;
    position = index;
    remaining = offset;
    while (remaining > 0) {
        if ((size_t)position >= value->length) return SL_JAVA_STRING_INDEX_OUT_OF_BOUNDS;
        if (is_high_surrogate(value->units[position]) &&
            (size_t)position + 1 < value->length &&
            is_low_surrogate(value->units[position + 1])) position += 2;
        else position++;
        remaining--;
    }
    while (remaining < 0) {
        if (position <= 0) return SL_JAVA_STRING_INDEX_OUT_OF_BOUNDS;
        position--;
        if (is_low_surrogate(value->units[position]) && position > 0 &&
            is_high_surrogate(value->units[position - 1])) position--;
        remaining++;
    }
    *out_index = (int32_t)position;
    return SL_JAVA_STRING_OK;
}

int sljava_string_substring(SLJavaString *out, const SLJavaString *value,
                            int32_t begin_index, int32_t end_index) {
    size_t length;
    uint16_t *copy = NULL;
    if (!out || !valid_value(value)) return SL_JAVA_STRING_INVALID_ARGUMENT;
    if (begin_index < 0 || end_index < begin_index ||
        (size_t)end_index > value->length) return SL_JAVA_STRING_RANGE_OUT_OF_BOUNDS;
    length = (size_t)(end_index - begin_index);
    if (length > SIZE_MAX / sizeof(*copy)) return SL_JAVA_STRING_TOO_LARGE;
    if (length) {
        copy = (uint16_t *)malloc(length * sizeof(*copy));
        if (!copy) return SL_JAVA_STRING_OUT_OF_MEMORY;
        memcpy(copy, value->units + begin_index, length * sizeof(*copy));
    }
    sljava_string_destroy(out);
    out->units = copy;
    out->length = length;
    return SL_JAVA_STRING_OK;
}

int sljava_string_concat(SLJavaString *out, const SLJavaString *left,
                         const SLJavaString *right) {
    size_t length;
    uint16_t *joined = NULL;
    if (!out || !valid_value(left) || !valid_value(right))
        return SL_JAVA_STRING_INVALID_ARGUMENT;
    if (right->length > SL_JAVA_STRING_MAX_LENGTH - left->length)
        return SL_JAVA_STRING_TOO_LARGE;
    length = left->length + right->length;
    if (length > SIZE_MAX / sizeof(*joined)) return SL_JAVA_STRING_TOO_LARGE;
    if (length) {
        joined = (uint16_t *)malloc(length * sizeof(*joined));
        if (!joined) return SL_JAVA_STRING_OUT_OF_MEMORY;
        if (left->length) memcpy(joined, left->units, left->length * sizeof(*joined));
        if (right->length) memcpy(joined + left->length, right->units,
                                  right->length * sizeof(*joined));
    }
    sljava_string_destroy(out);
    out->units = joined;
    out->length = length;
    return SL_JAVA_STRING_OK;
}

int sljava_string_equals(const SLJavaString *left, const SLJavaString *right,
                         int *out_equal) {
    if (!valid_value(left) || !valid_value(right) || !out_equal)
        return SL_JAVA_STRING_INVALID_ARGUMENT;
    *out_equal = left->length == right->length &&
        (left->length == 0 || memcmp(left->units, right->units,
                                     left->length * sizeof(uint16_t)) == 0);
    return SL_JAVA_STRING_OK;
}

int sljava_string_hash_code(const SLJavaString *value, int32_t *out_hash) {
    uint32_t hash = 0;
    int32_t signed_hash;
    if (!valid_value(value) || !out_hash) return SL_JAVA_STRING_INVALID_ARGUMENT;
    for (size_t i = 0; i < value->length; ++i)
        hash = hash * 31u + (uint32_t)value->units[i];
    /* Preserve Java's signed 32-bit two's-complement result without relying on
       implementation-defined unsigned-to-signed conversion. */
    memcpy(&signed_hash, &hash, sizeof(signed_hash));
    *out_hash = signed_hash;
    return SL_JAVA_STRING_OK;
}

int sljava_string_compare_to(const SLJavaString *left, const SLJavaString *right,
                             int32_t *out_order) {
    size_t common;
    if (!valid_value(left) || !valid_value(right) || !out_order)
        return SL_JAVA_STRING_INVALID_ARGUMENT;
    common = left->length < right->length ? left->length : right->length;
    for (size_t i = 0; i < common; ++i) {
        if (left->units[i] != right->units[i]) {
            *out_order = (int32_t)left->units[i] - (int32_t)right->units[i];
            return SL_JAVA_STRING_OK;
        }
    }
    *out_order = (int32_t)left->length - (int32_t)right->length;
    return SL_JAVA_STRING_OK;
}

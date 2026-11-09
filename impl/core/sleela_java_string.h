#ifndef SLEELA_JAVA_STRING_H
#define SLEELA_JAVA_STRING_H

/*
 * Lossless java.lang.String semantic kernel.
 *
 * Storage is an immutable-by-convention, length-delimited sequence of UTF-16
 * code units. Embedded U+0000 and isolated surrogate code units are preserved.
 * The caller owns each initialized result and releases it with destroy().
 *
 * This module is deliberately independent of SLValue for this first stage;
 * VM value-tag, artifact, and bytecode integration is a separate compatibility
 * change and must not silently reinterpret existing SL_STR values.
 */
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint16_t *units;
    size_t length;
} SLJavaString;

typedef enum {
    SL_JAVA_STRING_OK = 0,
    SL_JAVA_STRING_INVALID_ARGUMENT = -1,
    SL_JAVA_STRING_OUT_OF_MEMORY = -2,
    SL_JAVA_STRING_INDEX_OUT_OF_BOUNDS = -3,
    SL_JAVA_STRING_RANGE_OUT_OF_BOUNDS = -4,
    SL_JAVA_STRING_TOO_LARGE = -5
} SLJavaStringStatus;

/* Initializes an empty value. Safe to call before any operation. */
void sljava_string_init_empty(SLJavaString *value);
/* Copies exactly length UTF-16 code units; NULL units is valid only at length 0. */
int sljava_string_from_utf16(SLJavaString *out, const uint16_t *units, size_t length);
/* Releases owned storage and resets the value to empty. */
void sljava_string_destroy(SLJavaString *value);
/* Deep-copy immutable value semantics. */
int sljava_string_clone(SLJavaString *out, const SLJavaString *value);

/* Java String.length(): UTF-16 code-unit count. */
int sljava_string_length(const SLJavaString *value, int32_t *out_length);
/* Java String.charAt(): returns one unsigned UTF-16 code unit as 0..65535. */
int sljava_string_char_at(const SLJavaString *value, int32_t index, uint16_t *out_unit);
/* Java String.codePointAt(): combines a valid surrogate pair, otherwise returns the code unit. */
int sljava_string_code_point_at(const SLJavaString *value, int32_t index, int32_t *out_code_point);
/* Java String.codePointCount(begin,end), with a half-open UTF-16 range. */
int sljava_string_code_point_count(const SLJavaString *value, int32_t begin_index,
                                   int32_t end_index, int32_t *out_count);
/* Java String.offsetByCodePoints(index,offset), allowing index == length. */
int sljava_string_offset_by_code_points(const SLJavaString *value, int32_t index,
                                        int32_t offset, int32_t *out_index);

/* Java String.substring(begin,end), with a half-open UTF-16 range. */
int sljava_string_substring(SLJavaString *out, const SLJavaString *value,
                            int32_t begin_index, int32_t end_index);
/* Java String.concat(): result owns a new independent buffer. */
int sljava_string_concat(SLJavaString *out, const SLJavaString *left,
                         const SLJavaString *right);
/* Java String.equals(), hashCode(), and compareTo() over UTF-16 code units. */
int sljava_string_equals(const SLJavaString *left, const SLJavaString *right, int *out_equal);
int sljava_string_hash_code(const SLJavaString *value, int32_t *out_hash);
int sljava_string_compare_to(const SLJavaString *left, const SLJavaString *right,
                             int32_t *out_order);

#ifdef __cplusplus
}
#endif
#endif

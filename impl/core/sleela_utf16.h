#ifndef SLEELA_UTF16_H
#define SLEELA_UTF16_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* UTF-8 to Java UTF-16 code-unit primitives.
 *
 * Return values:
 *   0  success
 *  -1  invalid arguments (including a negative index)
 *  -2  malformed UTF-8 input
 *  -3  UTF-16 index out of range
 *
 * The input is length-delimited and may contain embedded NUL bytes. These
 * functions preserve Java's UTF-16 code-unit model: supplementary Unicode
 * scalar values occupy two independently addressable surrogate code units.
 */
int sleela_utf8_utf16_length(const char *utf8, size_t byte_length,
                            int64_t *out_code_units);
int sleela_utf8_utf16_char_at(const char *utf8, size_t byte_length,
                              int64_t index, uint16_t *out_code_unit);

#ifdef __cplusplus
}
#endif

#endif

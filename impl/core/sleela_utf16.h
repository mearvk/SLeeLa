#ifndef SLEELA_UTF16_H
#define SLEELA_UTF16_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
/* Return: 0 success, -1 invalid argument, -2 malformed UTF-8,
 * -3 UTF-16 code-unit index out of range. Input is length-delimited. */
int sleela_utf8_utf16_length(const char *utf8, size_t byte_length,
                            int64_t *out_code_units);
int sleela_utf8_to_utf16(const char *utf8, size_t byte_length,
                         uint16_t **out_units, size_t *out_code_units);
int sleela_utf8_utf16_char_at(const char *utf8, size_t byte_length,
                              int64_t index, uint16_t *out_code_unit);
#ifdef __cplusplus
}
#endif
#endif

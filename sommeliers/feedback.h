#ifndef SLEELA_SOMMELIER_FEEDBACK_H
#define SLEELA_SOMMELIER_FEEDBACK_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Returns 0 on success; output is exactly one of '0', '1', or 's'. */
int sommeliers_feedback_symbol(const char *input, char *output);

/* Convert a bounded UTF-8/ASCII byte string into '0'/'1' bits.
   Returns required bit count; writes at most capacity characters plus NUL. */
size_t sommeliers_to_binary(const char *input, char *output, size_t capacity);

#ifdef __cplusplus
}
#endif
#endif

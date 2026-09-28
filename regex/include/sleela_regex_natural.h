#ifndef SLEELA_REGEX_NATURAL_H
#define SLEELA_REGEX_NATURAL_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum sleela_regex_natural_status { SLEELA_REGEX_NATURAL_OK=0, SLEELA_REGEX_NATURAL_NULL=1, SLEELA_REGEX_NATURAL_UNKNOWN_WORD=2, SLEELA_REGEX_NATURAL_UNBALANCED=3, SLEELA_REGEX_NATURAL_INVALID_QUANTITY=4 } sleela_regex_natural_status;
sleela_regex_natural_status sleela_regex_natural_validate(const char*,char*,size_t);
int sleela_regex_natural_is_word(const char*,size_t);
const char* sleela_regex_natural_symbol_name(const char*,size_t);
#ifdef __cplusplus
}
#endif
#endif

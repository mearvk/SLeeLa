#ifndef SLEELA_REGEX_H
#define SLEELA_REGEX_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    SLEELA_REGEX_OK = 0,
    SLEELA_REGEX_INVALID_ARGUMENT = 1,
    SLEELA_REGEX_INVALID_PATTERN = 2,
    SLEELA_REGEX_UNSUPPORTED_LEVEL = 3,
    SLEELA_REGEX_UNSUPPORTED_PATTERN = 4
} SleelaRegexStatus;
typedef enum {
    SLEELA_REGEX_LEVEL_FIND = 1,
    SLEELA_REGEX_LEVEL_SHAPE = 2,
    SLEELA_REGEX_LEVEL_STRUCTURE = 3,
    SLEELA_REGEX_LEVEL_EXPRESSION = 4
} SleelaRegexLevel;
typedef struct {
    int matched;
    size_t start;
    size_t end;
    size_t length;
    SleelaRegexLevel level;
} SleelaRegexResult;
SleelaRegexStatus sleela_regex_match(const char *text, const char *pattern,
                                     SleelaRegexLevel level,
                                     SleelaRegexResult *result);
const char *sleela_regex_status_name(SleelaRegexStatus status);
#ifdef __cplusplus
}
#endif
#endif

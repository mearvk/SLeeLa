#include "sleela_regex.h"
#include <ctype.h>
#include <stddef.h>
#include <string.h>

static void reset_result(SleelaRegexResult *r, SleelaRegexLevel level) {
    r->matched = 0; r->start = 0; r->end = 0; r->length = 0; r->level = level;
}

static int shape_name(const char *name, const char **base, size_t *base_len,
                      size_t *min_count, size_t *max_count) {
    size_t n = strlen(name), i = 0, min = 1, max = 1;
    while (name[i] && isalpha((unsigned char)name[i])) ++i;
    if (i == 0) return 0;
    *base = name; *base_len = i;
    if (name[i] == '+') { min = 1; max = (size_t)-1; ++i; }
    else if (name[i] == '*') { min = 0; max = (size_t)-1; ++i; }
    else if (name[i] == '{') {
        size_t a = 0, b = 0, j = i + 1; int have_a = 0, have_b = 0;
        while (j < n && isdigit((unsigned char)name[j])) {
            have_a = 1; a = a * 10 + (size_t)(name[j] - '0'); ++j;
        }
        if (!have_a) return 0;
        if (name[j] == '}') { min = a; max = a; ++j; }
        else if (name[j] == ',') {
            ++j;
            while (j < n && isdigit((unsigned char)name[j])) {
                have_b = 1; b = b * 10 + (size_t)(name[j] - '0'); ++j;
            }
            if (name[j] != '}') return 0;
            min = a; max = have_b ? b : (size_t)-1;
            if (have_b && max < min) return 0;
            ++j;
        } else return 0;
        i = j;
    }
    return name[i] == '\0' ? (*min_count = min, *max_count = max, 1) : 0;
}

static int match_shape_exact(const char *text, const char *pattern) {
    const char *base; size_t base_len, min_count, max_count, n, count = 0;
    if (!shape_name(pattern, &base, &base_len, &min_count, &max_count)) return 0;
    n = strlen(text);
    while (count < n && (max_count == (size_t)-1 || count < max_count)) {
        unsigned char c = (unsigned char)text[count]; int ok = 0;
        if (base_len == 5 && strncmp(base, "digit", 5) == 0) ok = isdigit(c) != 0;
        else if (base_len == 4 && strncmp(base, "word", 4) == 0) ok = isalnum(c) || c == '_';
        else if (base_len == 5 && strncmp(base, "space", 5) == 0) ok = isspace(c) != 0;
        else if (base_len == 3 && strncmp(base, "any", 3) == 0) ok = 1;
        else return 0;
        if (!ok) break;
        ++count;
    }
    return count >= min_count && count == n;
}

static SleelaRegexStatus find_literal(const char *text, const char *literal,
                                       int mode, SleelaRegexResult *r) {
    size_t text_len = strlen(text), lit_len = strlen(literal); const char *p = NULL;
    if (mode == 0) p = strstr(text, literal);
    else if (mode == 1) {
        if (lit_len <= text_len && strncmp(text, literal, lit_len) == 0) p = text;
    } else if (mode == 2) {
        if (lit_len <= text_len &&
            strncmp(text + text_len - lit_len, literal, lit_len) == 0)
            p = text + text_len - lit_len;
    } else if (mode == 3) {
        if (text_len == lit_len && strcmp(text, literal) == 0) p = text;
    }
    if (p) {
        r->matched = 1; r->start = (size_t)(p - text);
        r->end = r->start + lit_len; r->length = lit_len;
    }
    return SLEELA_REGEX_OK;
}

SleelaRegexStatus sleela_regex_match(const char *text, const char *pattern,
                                     SleelaRegexLevel level,
                                     SleelaRegexResult *result) {
    const char *payload;
    if (!text || !pattern || !result) return SLEELA_REGEX_INVALID_ARGUMENT;
    reset_result(result, level);
    if (level < 1 || level > 4) return SLEELA_REGEX_INVALID_ARGUMENT;
    if (level >= SLEELA_REGEX_LEVEL_STRUCTURE) return SLEELA_REGEX_UNSUPPORTED_LEVEL;

    if (strncmp(pattern, "contains ", 9) == 0) return find_literal(text, pattern + 9, 0, result);
    if (strncmp(pattern, "starts ", 7) == 0) return find_literal(text, pattern + 7, 1, result);
    if (strncmp(pattern, "ends ", 5) == 0) return find_literal(text, pattern + 5, 2, result);
    if (strncmp(pattern, "exact ", 6) != 0) return SLEELA_REGEX_INVALID_PATTERN;

    payload = pattern + 6;
    if (level == SLEELA_REGEX_LEVEL_FIND) return find_literal(text, payload, 3, result);
    if (match_shape_exact(text, payload)) {
        result->matched = 1; result->start = 0; result->end = strlen(text); result->length = result->end;
        return SLEELA_REGEX_OK;
    }
    if (strchr(payload, '+') || strchr(payload, '*') || strchr(payload, '{'))
        return SLEELA_REGEX_OK;
    return find_literal(text, payload, 3, result);
}

const char *sleela_regex_status_name(SleelaRegexStatus status) {
    switch (status) {
        case SLEELA_REGEX_OK: return "ok";
        case SLEELA_REGEX_INVALID_ARGUMENT: return "invalid-argument";
        case SLEELA_REGEX_INVALID_PATTERN: return "invalid-pattern";
        case SLEELA_REGEX_UNSUPPORTED_LEVEL: return "unsupported-level";
        case SLEELA_REGEX_UNSUPPORTED_PATTERN: return "unsupported-pattern";
        default: return "unknown";
    }
}

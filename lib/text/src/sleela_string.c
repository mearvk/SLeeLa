/*
 * lib/text/src/sleela_string.c
 * SLeeLa Standard Library - text family native boundary (C implementation).
 * Max Rupplin - MEARVK LLC - 2026
 *
 * Pure C11, no dependency beyond the C standard library. Every search is
 * length-bounded and NUL-tolerant; every producing function returns an owned
 * buffer or a non-OK status without touching caller memory.
 */
#include "../include/sleela_string.h"

#include <stdlib.h>
#include <string.h>

/* ASCII-only case folding so behaviour is locale-independent and stable. */
static char sl_lower(char c) {
    return (c >= 'A' && c <= 'Z') ? (char)(c - 'A' + 'a') : c;
}
static char sl_upper(char c) {
    return (c >= 'a' && c <= 'z') ? (char)(c - 'a' + 'A') : c;
}
static int sl_is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}

/* Allocate an owned (len+1) buffer, copy, NUL-terminate. Returns status. */
static int sl_make(const char *s, size_t len, sleela_string_t *out) {
    char *buf;
    if (!out) return SLEELA_STRING_NULL;
    buf = (char *)malloc(len + 1);
    if (!buf) {
        out->data = NULL;
        out->length = 0;
        return SLEELA_STRING_NOMEM;
    }
    if (len && s) memcpy(buf, s, len);
    buf[len] = '\0';
    out->data = buf;
    out->length = len;
    return SLEELA_STRING_OK;
}

void sleela_string_free(sleela_string_t *out) {
    if (!out) return;
    free(out->data);
    out->data = NULL;
    out->length = 0;
}

const char *sleela_string_status_name(int status_code) {
    switch (status_code) {
        case SLEELA_STRING_OK:    return "OK";
        case SLEELA_STRING_NULL:  return "NULL";
        case SLEELA_STRING_RANGE: return "RANGE";
        case SLEELA_STRING_NOMEM: return "NOMEM";
        default:                  return "UNKNOWN";
    }
}

size_t sleela_string_length(const char *s, size_t max) {
    size_t n = 0;
    if (!s) return 0;
    while (n < max && s[n] != '\0') ++n;
    return n;
}

int sleela_string_is_empty(const char *s, size_t len) {
    return (!s || len == 0) ? 1 : 0;
}

int sleela_string_compare(const char *a, size_t alen,
                          const char *b, size_t blen) {
    size_t n = (alen < blen) ? alen : blen;
    size_t i;
    if (!a) alen = 0;
    if (!b) blen = 0;
    n = (alen < blen) ? alen : blen;
    for (i = 0; i < n; ++i) {
        unsigned char ca = (unsigned char)a[i];
        unsigned char cb = (unsigned char)b[i];
        if (ca != cb) return (ca < cb) ? -1 : 1;
    }
    if (alen == blen) return 0;
    return (alen < blen) ? -1 : 1;
}

int sleela_string_compare_ci(const char *a, size_t alen,
                             const char *b, size_t blen) {
    size_t n, i;
    if (!a) alen = 0;
    if (!b) blen = 0;
    n = (alen < blen) ? alen : blen;
    for (i = 0; i < n; ++i) {
        unsigned char ca = (unsigned char)sl_lower(a[i]);
        unsigned char cb = (unsigned char)sl_lower(b[i]);
        if (ca != cb) return (ca < cb) ? -1 : 1;
    }
    if (alen == blen) return 0;
    return (alen < blen) ? -1 : 1;
}

int sleela_string_equals(const char *a, size_t alen,
                         const char *b, size_t blen) {
    if (alen != blen) return 0;
    return sleela_string_compare(a, alen, b, blen) == 0 ? 1 : 0;
}

size_t sleela_string_find(const char *hay, size_t haylen,
                          const char *needle, size_t needlelen,
                          size_t from) {
    size_t i;
    if (from > haylen) from = haylen;
    if (needlelen == 0) return from;
    if (!hay || !needle || needlelen > haylen) return SLEELA_STRING_NPOS;
    for (i = from; i + needlelen <= haylen; ++i) {
        if (memcmp(hay + i, needle, needlelen) == 0) return i;
    }
    return SLEELA_STRING_NPOS;
}

size_t sleela_string_rfind(const char *hay, size_t haylen,
                           const char *needle, size_t needlelen) {
    size_t i;
    if (needlelen == 0) return haylen;
    if (!hay || !needle || needlelen > haylen) return SLEELA_STRING_NPOS;
    for (i = haylen - needlelen + 1; i-- > 0; ) {
        if (memcmp(hay + i, needle, needlelen) == 0) return i;
    }
    return SLEELA_STRING_NPOS;
}

size_t sleela_string_count(const char *hay, size_t haylen,
                           const char *needle, size_t needlelen) {
    size_t count = 0, pos = 0;
    if (needlelen == 0 || !hay || !needle || needlelen > haylen) return 0;
    while (pos + needlelen <= haylen) {
        size_t at = sleela_string_find(hay, haylen, needle, needlelen, pos);
        if (at == SLEELA_STRING_NPOS) break;
        ++count;
        pos = at + needlelen; /* non-overlapping */
    }
    return count;
}

int sleela_string_contains(const char *hay, size_t haylen,
                           const char *needle, size_t needlelen) {
    return sleela_string_find(hay, haylen, needle, needlelen, 0) != SLEELA_STRING_NPOS ? 1 : 0;
}

int sleela_string_starts_with(const char *hay, size_t haylen,
                              const char *needle, size_t needlelen) {
    if (needlelen == 0) return 1;
    if (!hay || !needle || needlelen > haylen) return 0;
    return memcmp(hay, needle, needlelen) == 0 ? 1 : 0;
}

int sleela_string_ends_with(const char *hay, size_t haylen,
                            const char *needle, size_t needlelen) {
    if (needlelen == 0) return 1;
    if (!hay || !needle || needlelen > haylen) return 0;
    return memcmp(hay + (haylen - needlelen), needle, needlelen) == 0 ? 1 : 0;
}

int sleela_string_substring(const char *s, size_t len,
                            size_t start, size_t count,
                            sleela_string_t *out) {
    if (!out) return SLEELA_STRING_NULL;
    if (start > len) return SLEELA_STRING_RANGE;
    if (count > len - start) count = len - start; /* clamp */
    return sl_make(s ? s + start : NULL, count, out);
}

int sleela_string_slice_from(const char *s, size_t len, size_t start,
                             sleela_string_t *out) {
    if (!out) return SLEELA_STRING_NULL;
    if (start > len) return SLEELA_STRING_RANGE;
    return sl_make(s ? s + start : NULL, len - start, out);
}

int sleela_string_duplicate(const char *s, size_t len, sleela_string_t *out) {
    return sl_make(s, len, out);
}

int sleela_string_concat(const char *a, size_t alen,
                         const char *b, size_t blen,
                         sleela_string_t *out) {
    char *buf;
    if (!out) return SLEELA_STRING_NULL;
    buf = (char *)malloc(alen + blen + 1);
    if (!buf) { out->data = NULL; out->length = 0; return SLEELA_STRING_NOMEM; }
    if (alen && a) memcpy(buf, a, alen);
    if (blen && b) memcpy(buf + alen, b, blen);
    buf[alen + blen] = '\0';
    out->data = buf;
    out->length = alen + blen;
    return SLEELA_STRING_OK;
}

int sleela_string_trim_left(const char *s, size_t len, sleela_string_t *out) {
    size_t i = 0;
    if (!out) return SLEELA_STRING_NULL;
    if (s) while (i < len && sl_is_space(s[i])) ++i;
    return sl_make(s ? s + i : NULL, len - i, out);
}

int sleela_string_trim_right(const char *s, size_t len, sleela_string_t *out) {
    size_t end = len;
    if (!out) return SLEELA_STRING_NULL;
    if (s) while (end > 0 && sl_is_space(s[end - 1])) --end;
    return sl_make(s, end, out);
}

int sleela_string_trim(const char *s, size_t len, sleela_string_t *out) {
    size_t i = 0, end = len;
    if (!out) return SLEELA_STRING_NULL;
    if (s) {
        while (i < len && sl_is_space(s[i])) ++i;
        while (end > i && sl_is_space(s[end - 1])) --end;
    }
    return sl_make(s ? s + i : NULL, end - i, out);
}

int sleela_string_to_upper(const char *s, size_t len, sleela_string_t *out) {
    int rc = sl_make(s, len, out);
    size_t i;
    if (rc == SLEELA_STRING_OK)
        for (i = 0; i < len; ++i) out->data[i] = sl_upper(out->data[i]);
    return rc;
}

int sleela_string_to_lower(const char *s, size_t len, sleela_string_t *out) {
    int rc = sl_make(s, len, out);
    size_t i;
    if (rc == SLEELA_STRING_OK)
        for (i = 0; i < len; ++i) out->data[i] = sl_lower(out->data[i]);
    return rc;
}

int sleela_string_reverse(const char *s, size_t len, sleela_string_t *out) {
    int rc = sl_make(s, len, out);
    size_t i;
    if (rc == SLEELA_STRING_OK && s)
        for (i = 0; i < len; ++i) out->data[i] = s[len - 1 - i];
    return rc;
}

int sleela_string_replace(const char *s, size_t len,
                          const char *from, size_t fromlen,
                          const char *to, size_t tolen,
                          sleela_string_t *out) {
    size_t occurrences, result_len, pos, w;
    char *buf;

    if (!out) return SLEELA_STRING_NULL;
    if (fromlen == 0) return sl_make(s, len, out); /* nothing to match */

    occurrences = sleela_string_count(s, len, from, fromlen);
    if (occurrences == 0) return sl_make(s, len, out);

    /* result_len = len + occurrences*(tolen - fromlen), computed without
       underflow by splitting the signed delta. */
    result_len = len - occurrences * fromlen + occurrences * tolen;

    buf = (char *)malloc(result_len + 1);
    if (!buf) { out->data = NULL; out->length = 0; return SLEELA_STRING_NOMEM; }

    pos = 0;
    w = 0;
    while (pos < len) {
        size_t at = sleela_string_find(s, len, from, fromlen, pos);
        if (at == SLEELA_STRING_NPOS) {
            memcpy(buf + w, s + pos, len - pos);
            w += len - pos;
            break;
        }
        if (at > pos) { memcpy(buf + w, s + pos, at - pos); w += at - pos; }
        if (tolen && to) { memcpy(buf + w, to, tolen); w += tolen; }
        pos = at + fromlen;
    }
    buf[w] = '\0';
    out->data = buf;
    out->length = w;
    return SLEELA_STRING_OK;
}

int sleela_string_split(const char *s, size_t len,
                        const char *sep, size_t seplen,
                        sleela_string_t **out, size_t *count) {
    size_t n, i, pos, start;
    sleela_string_t *fields;

    if (!out || !count) return SLEELA_STRING_NULL;
    *out = NULL;
    *count = 0;

    /* Empty separator: a single field containing the whole input. */
    if (seplen == 0) {
        fields = (sleela_string_t *)calloc(1, sizeof(*fields));
        if (!fields) return SLEELA_STRING_NOMEM;
        if (sl_make(s, len, &fields[0]) != SLEELA_STRING_OK) {
            free(fields);
            return SLEELA_STRING_NOMEM;
        }
        *out = fields;
        *count = 1;
        return SLEELA_STRING_OK;
    }

    n = sleela_string_count(s, len, sep, seplen) + 1;
    fields = (sleela_string_t *)calloc(n, sizeof(*fields));
    if (!fields) return SLEELA_STRING_NOMEM;

    pos = 0;
    i = 0;
    while (i < n) {
        size_t at = sleela_string_find(s, len, sep, seplen, pos);
        start = pos;
        if (at == SLEELA_STRING_NPOS) {
            if (sl_make(s + start, len - start, &fields[i]) != SLEELA_STRING_OK)
                goto fail;
            ++i;
            break;
        }
        if (sl_make(s + start, at - start, &fields[i]) != SLEELA_STRING_OK)
            goto fail;
        ++i;
        pos = at + seplen;
    }

    *out = fields;
    *count = i;
    return SLEELA_STRING_OK;

fail:
    sleela_string_split_free(fields, i);
    return SLEELA_STRING_NOMEM;
}

void sleela_string_split_free(sleela_string_t *fields, size_t count) {
    size_t i;
    if (!fields) return;
    for (i = 0; i < count; ++i) sleela_string_free(&fields[i]);
    free(fields);
}

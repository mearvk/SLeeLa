/*
 * lib/text/include/sleela_string.h
 * SLeeLa Standard Library - text family native boundary (C ABI).
 * Max Rupplin - MEARVK LLC - 2026
 *
 * Stable C ABI for string processing, manipulation, and substring services
 * used by the SLeeLa text library family (SLString, SLStringBuilder,
 * SLStringBuffer). SLeeLa definitions describe the text model and remain the
 * source-level contract; this header is the native implementation boundary.
 *
 * Contract notes:
 *   - All functions are length-explicit and NUL-tolerant: a string is a
 *     (pointer, length) pair, so embedded NUL bytes are preserved.
 *   - Functions that produce new storage return a heap buffer the caller frees
 *     with sleela_string_free(); the ABI never silently reuses caller memory.
 *   - Index/length results use size_t; "not found" is SLEELA_STRING_NPOS.
 *   - No function reads past the declared length of any input span.
 */
#ifndef SLEELA_STRING_H
#define SLEELA_STRING_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Sentinel returned by search functions when a match is not found. */
#define SLEELA_STRING_NPOS ((size_t)-1)

/* Status codes shared by the text native boundary. */
enum {
    SLEELA_STRING_OK = 0,
    SLEELA_STRING_NULL = 1,   /* required pointer argument was NULL  */
    SLEELA_STRING_RANGE = 2,  /* offset/length fell outside the span */
    SLEELA_STRING_NOMEM = 3   /* allocation of a result buffer failed */
};

/*
 * An owned, length-counted byte string. `data` is NUL-terminated for C
 * interop but `length` is authoritative and may be shorter than strlen(data)
 * if embedded NUL bytes are present. Release with sleela_string_free().
 */
typedef struct sleela_string {
    char  *data;
    size_t length;
} sleela_string_t;

/* ---- lifetime -------------------------------------------------------- */

/* Release a result produced by this ABI. Safe on a zeroed/empty value. */
void sleela_string_free(sleela_string_t *out);

/* Human-readable name for a SLEELA_STRING_* status code. */
const char *sleela_string_status_name(int status_code);

/* ---- measurement ----------------------------------------------------- */

/* Length of a C string, bounded by max (NPOS-free strnlen). */
size_t sleela_string_length(const char *s, size_t max);

/* 1 if the span [s, s+len) is empty or NULL, else 0. */
int sleela_string_is_empty(const char *s, size_t len);

/* ---- comparison ------------------------------------------------------ */

/* Lexicographic compare of two spans: <0, 0, >0. NULL sorts as empty. */
int sleela_string_compare(const char *a, size_t alen,
                          const char *b, size_t blen);

/* Case-insensitive (ASCII) variant of sleela_string_compare. */
int sleela_string_compare_ci(const char *a, size_t alen,
                             const char *b, size_t blen);

/* 1 if the two spans are byte-equal. */
int sleela_string_equals(const char *a, size_t alen,
                         const char *b, size_t blen);

/* ---- search ---------------------------------------------------------- */

/*
 * First index at/after `from` where `needle` occurs in `hay`, or
 * SLEELA_STRING_NPOS. An empty needle matches at `from` (clamped to haylen).
 */
size_t sleela_string_find(const char *hay, size_t haylen,
                          const char *needle, size_t needlelen,
                          size_t from);

/* Last index where `needle` occurs in `hay`, or SLEELA_STRING_NPOS. */
size_t sleela_string_rfind(const char *hay, size_t haylen,
                           const char *needle, size_t needlelen);

/* Number of non-overlapping occurrences of `needle` in `hay`. */
size_t sleela_string_count(const char *hay, size_t haylen,
                           const char *needle, size_t needlelen);

/* 1 if `hay` contains `needle`. */
int sleela_string_contains(const char *hay, size_t haylen,
                           const char *needle, size_t needlelen);

/* 1 if `hay` starts with / ends with `needle`. */
int sleela_string_starts_with(const char *hay, size_t haylen,
                              const char *needle, size_t needlelen);
int sleela_string_ends_with(const char *hay, size_t haylen,
                            const char *needle, size_t needlelen);

/* ---- substring / manipulation (produce owned results) ---------------- */

/*
 * Copy the substring [start, start+count) of `s` into `out`.
 * `count` is clamped so start+count never exceeds len. Returns a status code.
 */
int sleela_string_substring(const char *s, size_t len,
                            size_t start, size_t count,
                            sleela_string_t *out);

/* Everything from `start` to the end (equivalent to substring(start, len-start)). */
int sleela_string_slice_from(const char *s, size_t len, size_t start,
                             sleela_string_t *out);

/* Owned copy of the whole span. */
int sleela_string_duplicate(const char *s, size_t len, sleela_string_t *out);

/* Concatenate two spans into a new owned result. */
int sleela_string_concat(const char *a, size_t alen,
                         const char *b, size_t blen,
                         sleela_string_t *out);

/* Trim ASCII whitespace from both ends / left / right into a new result. */
int sleela_string_trim(const char *s, size_t len, sleela_string_t *out);
int sleela_string_trim_left(const char *s, size_t len, sleela_string_t *out);
int sleela_string_trim_right(const char *s, size_t len, sleela_string_t *out);

/* ASCII case conversion into a new result. */
int sleela_string_to_upper(const char *s, size_t len, sleela_string_t *out);
int sleela_string_to_lower(const char *s, size_t len, sleela_string_t *out);

/* Reverse the byte order of the span into a new result. */
int sleela_string_reverse(const char *s, size_t len, sleela_string_t *out);

/*
 * Replace every non-overlapping occurrence of `from` with `to`.
 * An empty `from` is a no-op copy. Returns a status code.
 */
int sleela_string_replace(const char *s, size_t len,
                          const char *from, size_t fromlen,
                          const char *to, size_t tolen,
                          sleela_string_t *out);

/* ---- split ----------------------------------------------------------- */

/*
 * Split `s` on `sep` into owned fields. On success *out points to a
 * heap array of `*count` sleela_string_t values; release with
 * sleela_string_split_free(fields, count). An empty `sep` yields one field
 * (the whole input). Returns a status code.
 */
int sleela_string_split(const char *s, size_t len,
                        const char *sep, size_t seplen,
                        sleela_string_t **out, size_t *count);

/* Release the array returned by sleela_string_split. */
void sleela_string_split_free(sleela_string_t *fields, size_t count);

#ifdef __cplusplus
}
#endif
#endif /* SLEELA_STRING_H */

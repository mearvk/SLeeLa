/*
 * reg_java_string.c -- regression suite for impl/core/sleela_java_string.c
 *
 * Contract (impl/core/sleela_java_string.h): a lossless java.lang.String
 * kernel over UTF-16 code units.  Embedded U+0000 and isolated surrogates are
 * preserved; results own independent buffers; every operation reports the
 * documented SLJavaStringStatus and leaves the destination untouched when it
 * fails.  Expected values for hashCode()/compareTo() are the ones the JDK
 * produces (hashes cross-computed independently in Python).
 */
#include "../../../impl/core/sleela_java_string.h"
#include "../../../impl/core/sleela_utf16.h"
#include "regression.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define U16(...) ((const uint16_t[]){__VA_ARGS__})
#define N16(...) (sizeof((const uint16_t[]){__VA_ARGS__}) / sizeof(uint16_t))
#define MAKE(out, ...) REG_EQ_I(SL_JAVA_STRING_OK, sljava_string_from_utf16((out), U16(__VA_ARGS__), N16(__VA_ARGS__)))

static void from_ascii(SLJavaString *s, const char *a) {
    size_t n = strlen(a); uint16_t *u = (uint16_t *)malloc(n * sizeof(uint16_t) + 1);
    for (size_t i = 0; i < n; ++i) u[i] = (uint16_t)(unsigned char)a[i];
    (void)sljava_string_from_utf16(s, u, n);
    free(u);
}

static int same(const SLJavaString *s, const uint16_t *u, size_t n) {
    return s->length == n && (n == 0 || memcmp(s->units, u, n * sizeof(uint16_t)) == 0);
}

static int32_t hash_of(const SLJavaString *s) {
    int32_t h = 0; (void)sljava_string_hash_code(s, &h); return h;
}

/* ---- independent references ------------------------------------------- */
static int32_t ref_compare(const uint16_t *a, size_t an, const uint16_t *b, size_t bn) {
    size_t m = an < bn ? an : bn;
    for (size_t i = 0; i < m; ++i) if (a[i] != b[i]) return (int32_t)a[i] - (int32_t)b[i];
    return (int32_t)an - (int32_t)bn;
}
static int sgn(int32_t v) { return (v > 0) - (v < 0); }
static uint32_t ref_hash(const uint16_t *a, size_t n) {
    uint32_t h = 0; for (size_t i = 0; i < n; ++i) h = 31u * h + a[i]; return h;
}
static uint32_t pow31(size_t n) { uint32_t p = 1; while (n--) p *= 31u; return p; }
static int is_hi(uint16_t u) { return u >= 0xD800 && u <= 0xDBFF; }
static int is_lo(uint16_t u) { return u >= 0xDC00 && u <= 0xDFFF; }
static int32_t ref_cp_count(const uint16_t *u, size_t n) {
    int32_t c = 0;
    for (size_t i = 0; i < n; ++i, ++c) if (is_hi(u[i]) && i + 1 < n && is_lo(u[i + 1])) ++i;
    return c;
}

/* ---- cases -------------------------------------------------------------- */

static void t_lifecycle(void) {
    SLJavaString s; uint16_t src[3] = {1, 2, 3};
    sljava_string_init_empty(&s);
    REG_CHECK(s.units == NULL); REG_EQ_U(0, s.length);
    sljava_string_init_empty(NULL);   /* must not crash */
    sljava_string_destroy(NULL);      /* must not crash */
    sljava_string_destroy(&s);        /* empty destroy is a no-op */

    REG_EQ_I(SL_JAVA_STRING_OK, sljava_string_from_utf16(&s, src, 3));
    src[0] = 99;                      /* the copy must be independent */
    REG_CHECK(same(&s, U16(1, 2, 3), 3));
    REG_CHECK(s.units != src);
    /* replacing a live value releases the old buffer (leak-checked per case) */
    REG_EQ_I(SL_JAVA_STRING_OK, sljava_string_from_utf16(&s, src, 1));
    REG_CHECK(same(&s, U16(99), 1));
    REG_EQ_I(SL_JAVA_STRING_OK, sljava_string_from_utf16(&s, NULL, 0));
    REG_CHECK(s.units == NULL); REG_EQ_U(0, s.length);
    sljava_string_destroy(&s); sljava_string_destroy(&s); /* idempotent */

    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_from_utf16(NULL, src, 1));
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_from_utf16(&s, NULL, 1));
}

static void t_invalid_values_rejected(void) {
    SLJavaString bad, out; int32_t i = 0; uint16_t u = 0; int eq = 0;
    sljava_string_init_empty(&out);
    bad.units = NULL; bad.length = 1;                 /* non-empty without storage */
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_length(&bad, &i));
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_char_at(&bad, 0, &u));
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_clone(&out, &bad));
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_equals(&bad, &bad, &eq));
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_length(NULL, &i));
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_length(&out, NULL));
#if SIZE_MAX > INT32_MAX
    {   /* length beyond the Java limit is rejected before any dereference */
        SLJavaString huge; uint16_t stub = 0;
        huge.units = &stub; huge.length = (size_t)INT32_MAX + 1u;
        REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_length(&huge, &i));
        REG_EQ_I(SL_JAVA_STRING_TOO_LARGE,
                 sljava_string_from_utf16(&out, &stub, (size_t)INT32_MAX + 1u));
        REG_CHECK(out.units == NULL && out.length == 0);
    }
#endif
}

static void t_length_and_char_at(void) {
    SLJavaString s; int32_t n = -1; uint16_t c = 0;
    sljava_string_init_empty(&s);
    REG_EQ_I(0, sljava_string_length(&s, &n)); REG_EQ_I(0, n);
    REG_EQ_I(SL_JAVA_STRING_INDEX_OUT_OF_BOUNDS, sljava_string_char_at(&s, 0, &c));
    MAKE(&s, 0x61, 0x0000, 0xD83D, 0xFFFF);
    REG_EQ_I(0, sljava_string_length(&s, &n)); REG_EQ_I(4, n);
    REG_EQ_I(0, sljava_string_char_at(&s, 1, &c)); REG_EQ_U(0, c);       /* NUL kept   */
    REG_EQ_I(0, sljava_string_char_at(&s, 2, &c)); REG_EQ_U(0xD83D, c);  /* lone high  */
    REG_EQ_I(0, sljava_string_char_at(&s, 3, &c)); REG_EQ_U(0xFFFF, c);  /* unsigned   */
    REG_EQ_I(SL_JAVA_STRING_INDEX_OUT_OF_BOUNDS, sljava_string_char_at(&s, 4, &c));
    REG_EQ_I(SL_JAVA_STRING_INDEX_OUT_OF_BOUNDS, sljava_string_char_at(&s, -1, &c));
    REG_EQ_I(SL_JAVA_STRING_INDEX_OUT_OF_BOUNDS, sljava_string_char_at(&s, INT32_MIN, &c));
    REG_EQ_I(SL_JAVA_STRING_INDEX_OUT_OF_BOUNDS, sljava_string_char_at(&s, INT32_MAX, &c));
    sljava_string_destroy(&s);
}

static void t_code_points(void) {
    SLJavaString s; int32_t v = 0, idx = 0;
    sljava_string_init_empty(&s);
    /* "a" U+1F600 "b", then a lone low, a lone high, and a high+non-low */
    MAKE(&s, 0x61, 0xD83D, 0xDE00, 0x62, 0xDC00, 0xD83D, 0xD83D, 0x63);
    REG_EQ_I(0, sljava_string_code_point_at(&s, 0, &v)); REG_EQ_I(0x61, v);
    REG_EQ_I(0, sljava_string_code_point_at(&s, 1, &v)); REG_EQ_I(0x1F600, v);
    REG_EQ_I(0, sljava_string_code_point_at(&s, 2, &v)); REG_EQ_I(0xDE00, v); /* mid-pair */
    REG_EQ_I(0, sljava_string_code_point_at(&s, 4, &v)); REG_EQ_I(0xDC00, v); /* lone low  */
    REG_EQ_I(0, sljava_string_code_point_at(&s, 5, &v)); REG_EQ_I(0xD83D, v); /* hi, hi    */
    REG_EQ_I(0, sljava_string_code_point_at(&s, 6, &v)); REG_EQ_I(0xD83D, v); /* hi, non-lo*/
    REG_EQ_I(SL_JAVA_STRING_INDEX_OUT_OF_BOUNDS, sljava_string_code_point_at(&s, 8, &v));
    REG_EQ_I(SL_JAVA_STRING_INDEX_OUT_OF_BOUNDS, sljava_string_code_point_at(&s, -1, &v));
    sljava_string_destroy(&s);

    MAKE(&s, 0x61, 0xD83D, 0xDE00, 0x62);
    REG_EQ_I(0, sljava_string_code_point_count(&s, 0, 4, &v)); REG_EQ_I(3, v);
    REG_EQ_I(0, sljava_string_code_point_count(&s, 1, 3, &v)); REG_EQ_I(1, v);
    REG_EQ_I(0, sljava_string_code_point_count(&s, 1, 2, &v)); REG_EQ_I(1, v); /* cut pair */
    REG_EQ_I(0, sljava_string_code_point_count(&s, 2, 3, &v)); REG_EQ_I(1, v);
    REG_EQ_I(0, sljava_string_code_point_count(&s, 4, 4, &v)); REG_EQ_I(0, v);
    REG_EQ_I(SL_JAVA_STRING_RANGE_OUT_OF_BOUNDS, sljava_string_code_point_count(&s, 3, 2, &v));
    REG_EQ_I(SL_JAVA_STRING_RANGE_OUT_OF_BOUNDS, sljava_string_code_point_count(&s, -1, 2, &v));
    REG_EQ_I(SL_JAVA_STRING_RANGE_OUT_OF_BOUNDS, sljava_string_code_point_count(&s, 0, 5, &v));

    REG_EQ_I(0, sljava_string_offset_by_code_points(&s, 0, 2, &idx)); REG_EQ_I(3, idx);
    REG_EQ_I(0, sljava_string_offset_by_code_points(&s, 0, 3, &idx)); REG_EQ_I(4, idx);
    REG_EQ_I(0, sljava_string_offset_by_code_points(&s, 4, -1, &idx)); REG_EQ_I(3, idx);
    REG_EQ_I(0, sljava_string_offset_by_code_points(&s, 4, -2, &idx)); REG_EQ_I(1, idx);
    REG_EQ_I(0, sljava_string_offset_by_code_points(&s, 3, -1, &idx)); REG_EQ_I(1, idx);
    REG_EQ_I(0, sljava_string_offset_by_code_points(&s, 2, -1, &idx)); REG_EQ_I(1, idx);
    REG_EQ_I(0, sljava_string_offset_by_code_points(&s, 2, 0, &idx)); REG_EQ_I(2, idx);
    REG_EQ_I(SL_JAVA_STRING_INDEX_OUT_OF_BOUNDS, sljava_string_offset_by_code_points(&s, 0, 4, &idx));
    REG_EQ_I(SL_JAVA_STRING_INDEX_OUT_OF_BOUNDS, sljava_string_offset_by_code_points(&s, 0, -1, &idx));
    REG_EQ_I(SL_JAVA_STRING_INDEX_OUT_OF_BOUNDS, sljava_string_offset_by_code_points(&s, 5, 0, &idx));
    REG_EQ_I(SL_JAVA_STRING_INDEX_OUT_OF_BOUNDS, sljava_string_offset_by_code_points(&s, 0, INT32_MAX, &idx));
    REG_EQ_I(SL_JAVA_STRING_INDEX_OUT_OF_BOUNDS, sljava_string_offset_by_code_points(&s, 4, INT32_MIN, &idx));
    sljava_string_destroy(&s);
}

static void t_substring(void) {
    SLJavaString s, r;
    sljava_string_init_empty(&s); sljava_string_init_empty(&r);
    MAKE(&s, 0x68, 0x65, 0x6C, 0x6C, 0x6F);
    REG_EQ_I(0, sljava_string_substring(&r, &s, 1, 4)); REG_CHECK(same(&r, U16(0x65, 0x6C, 0x6C), 3));
    REG_CHECK(r.units != s.units + 1);                     /* independent buffer */
    REG_EQ_I(0, sljava_string_substring(&r, &s, 0, 5)); REG_CHECK(same(&r, s.units, 5));
    REG_EQ_I(0, sljava_string_substring(&r, &s, 2, 2)); REG_EQ_U(0, r.length); REG_CHECK(r.units == NULL);
    REG_EQ_I(0, sljava_string_substring(&r, &s, 5, 5)); REG_EQ_U(0, r.length);
    /* failures leave the destination untouched */
    REG_EQ_I(0, sljava_string_substring(&r, &s, 0, 2));
    REG_EQ_I(SL_JAVA_STRING_RANGE_OUT_OF_BOUNDS, sljava_string_substring(&r, &s, 3, 2));
    REG_EQ_I(SL_JAVA_STRING_RANGE_OUT_OF_BOUNDS, sljava_string_substring(&r, &s, -1, 2));
    REG_EQ_I(SL_JAVA_STRING_RANGE_OUT_OF_BOUNDS, sljava_string_substring(&r, &s, 0, 6));
    REG_EQ_I(SL_JAVA_STRING_RANGE_OUT_OF_BOUNDS, sljava_string_substring(&r, &s, INT32_MAX, INT32_MAX));
    REG_CHECK(same(&r, U16(0x68, 0x65), 2));
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_substring(NULL, &s, 0, 1));
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_substring(&r, NULL, 0, 1));
    /* aliasing: destination is the source */
    REG_EQ_I(0, sljava_string_substring(&s, &s, 1, 3)); REG_CHECK(same(&s, U16(0x65, 0x6C), 2));
    sljava_string_destroy(&s); sljava_string_destroy(&r);
}

static void t_concat(void) {
    SLJavaString a, b, r;
    sljava_string_init_empty(&a); sljava_string_init_empty(&b); sljava_string_init_empty(&r);
    MAKE(&a, 0x61, 0xD83D); MAKE(&b, 0xDE00, 0x62);
    /* a lone high joined to a lone low forms a pair (value semantics, no validation) */
    REG_EQ_I(0, sljava_string_concat(&r, &a, &b));
    REG_CHECK(same(&r, U16(0x61, 0xD83D, 0xDE00, 0x62), 4));
    { int32_t v = 0; REG_EQ_I(0, sljava_string_code_point_at(&r, 1, &v)); REG_EQ_I(0x1F600, v); }
    REG_CHECK(r.units != a.units && r.units != b.units);
    /* empties */
    sljava_string_destroy(&b);
    REG_EQ_I(0, sljava_string_concat(&r, &a, &b)); REG_CHECK(same(&r, a.units, 2));
    REG_EQ_I(0, sljava_string_concat(&r, &b, &b)); REG_EQ_U(0, r.length); REG_CHECK(r.units == NULL);
    /* aliasing: out==left, out==right, left==right, out==both */
    MAKE(&b, 0x7A);
    REG_EQ_I(0, sljava_string_concat(&a, &a, &b)); REG_CHECK(same(&a, U16(0x61, 0xD83D, 0x7A), 3));
    REG_EQ_I(0, sljava_string_concat(&b, &a, &b)); REG_CHECK(same(&b, U16(0x61, 0xD83D, 0x7A, 0x7A), 4));
    REG_EQ_I(0, sljava_string_concat(&a, &a, &a)); REG_EQ_U(6, a.length);
    REG_EQ_MEM(a.units, U16(0x61, 0xD83D, 0x7A, 0x61, 0xD83D, 0x7A), 6 * sizeof(uint16_t));
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_concat(NULL, &a, &b));
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_concat(&r, NULL, &b));
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_concat(&r, &a, NULL));
    sljava_string_destroy(&a); sljava_string_destroy(&b); sljava_string_destroy(&r);
}

static void t_clone(void) {
    SLJavaString a, c; sljava_string_init_empty(&a); sljava_string_init_empty(&c);
    MAKE(&a, 1, 0, 0xDC00);
    REG_EQ_I(0, sljava_string_clone(&c, &a)); REG_CHECK(same(&c, a.units, 3)); REG_CHECK(c.units != a.units);
    a.units[0] = 9; REG_EQ_U(1, c.units[0]);
    sljava_string_destroy(&a);
    REG_EQ_I(0, sljava_string_clone(&c, &a)); REG_EQ_U(0, c.length);
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_clone(NULL, &a));
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_clone(&c, NULL));
    sljava_string_destroy(&c);
}

/* Values cross-computed with the JDK definition s[0]*31^(n-1)+...+s[n-1]. */
static void t_hash_code_golden(void) {
    static const struct { const char *ascii; int32_t h; } g[] = {
        {"", 0}, {"a", 97}, {"hello", 99162322}, {"Aa", 2112}, {"BB", 2112},
        {"Hello, World!", 1498789909},
        {"The quick brown fox jumps over the lazy dog", -609428141},
        {"polygenelubricants", INT32_MIN},            /* the famous Integer.MIN_VALUE */
        {"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", 641926002},
    };
    SLJavaString s; sljava_string_init_empty(&s);
    for (size_t i = 0; i < sizeof g / sizeof g[0]; ++i) {
        from_ascii(&s, g[i].ascii); REG_EQ_I(g[i].h, hash_of(&s));
    }
    MAKE(&s, 0xD83D, 0xDE00); REG_EQ_I(1772899, hash_of(&s));     /* U+1F600 */
    MAKE(&s, 0x00E9);         REG_EQ_I(233, hash_of(&s));
    MAKE(&s, 0x61, 0x00, 0x62); REG_EQ_I(93315, hash_of(&s));     /* NUL participates */
    MAKE(&s, 0xFFFF);         REG_EQ_I(65535, hash_of(&s));
    { int32_t h = 5; REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_hash_code(NULL, &h)); }
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_hash_code(&s, NULL));
    sljava_string_destroy(&s);
}

static void t_compare_and_equals_golden(void) {
    SLJavaString a, b; int32_t o = 0; int eq = -1;
    sljava_string_init_empty(&a); sljava_string_init_empty(&b);
    from_ascii(&a, "apple"); from_ascii(&b, "banana");
    REG_EQ_I(0, sljava_string_compare_to(&a, &b, &o)); REG_EQ_I(-1, o);
    REG_EQ_I(0, sljava_string_compare_to(&b, &a, &o)); REG_EQ_I(1, o);
    from_ascii(&a, "abc"); from_ascii(&b, "abcd");
    REG_EQ_I(0, sljava_string_compare_to(&a, &b, &o)); REG_EQ_I(-1, o);      /* prefix: length diff */
    from_ascii(&b, "abc");
    REG_EQ_I(0, sljava_string_compare_to(&a, &b, &o)); REG_EQ_I(0, o);
    REG_EQ_I(0, sljava_string_equals(&a, &b, &eq)); REG_EQ_I(1, eq);
    from_ascii(&b, "abd");
    REG_EQ_I(0, sljava_string_equals(&a, &b, &eq)); REG_EQ_I(0, eq);
    from_ascii(&a, "Z"); from_ascii(&b, "a");
    REG_EQ_I(0, sljava_string_compare_to(&a, &b, &o)); REG_EQ_I(-7, o);      /* 'Z'-'a' */
    /* UTF-16 unit order, NOT code point order: U+1F600 (D83D..) < U+FFFF */
    MAKE(&a, 0xD83D, 0xDE00); MAKE(&b, 0xFFFF);
    REG_EQ_I(0, sljava_string_compare_to(&a, &b, &o)); REG_EQ_I(0xD83D - 0xFFFF, o);
    /* empty strings */
    sljava_string_destroy(&a); sljava_string_destroy(&b);
    REG_EQ_I(0, sljava_string_equals(&a, &b, &eq)); REG_EQ_I(1, eq);
    REG_EQ_I(0, sljava_string_compare_to(&a, &b, &o)); REG_EQ_I(0, o);
    MAKE(&b, 0); /* "\0" is not the empty string */
    REG_EQ_I(0, sljava_string_equals(&a, &b, &eq)); REG_EQ_I(0, eq);
    REG_EQ_I(0, sljava_string_compare_to(&a, &b, &o)); REG_EQ_I(-1, o);
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_equals(&a, &b, NULL));
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_compare_to(&a, NULL, &o));
    sljava_string_destroy(&b);
}

/* Failure atomicity under out-of-memory: status is OUT_OF_MEMORY, the
 * destination keeps its previous value, nothing leaks. */
static void t_out_of_memory_is_atomic(void) {
    SLJavaString out, a, b; uint16_t keep[2] = {7, 8};
    sljava_string_init_empty(&out); sljava_string_init_empty(&a); sljava_string_init_empty(&b);
    MAKE(&a, 1, 2, 3); MAKE(&b, 4, 5);
    REG_EQ_I(0, sljava_string_from_utf16(&out, keep, 2));

    reg_alloc_fail_nth(1);
    REG_EQ_I(SL_JAVA_STRING_OUT_OF_MEMORY, sljava_string_from_utf16(&out, a.units, 3));
    REG_CHECK(reg_alloc_injected()); REG_CHECK(same(&out, keep, 2));
    reg_alloc_fail_nth(1);
    REG_EQ_I(SL_JAVA_STRING_OUT_OF_MEMORY, sljava_string_clone(&out, &a)); REG_CHECK(same(&out, keep, 2));
    reg_alloc_fail_nth(1);
    REG_EQ_I(SL_JAVA_STRING_OUT_OF_MEMORY, sljava_string_substring(&out, &a, 0, 2)); REG_CHECK(same(&out, keep, 2));
    reg_alloc_fail_nth(1);
    REG_EQ_I(SL_JAVA_STRING_OUT_OF_MEMORY, sljava_string_concat(&out, &a, &b)); REG_CHECK(same(&out, keep, 2));
    /* aliased concat failing must not destroy its inputs */
    reg_alloc_fail_nth(1);
    REG_EQ_I(SL_JAVA_STRING_OUT_OF_MEMORY, sljava_string_concat(&a, &a, &b));
    REG_CHECK(same(&a, U16(1, 2, 3), 3));
    reg_alloc_fail_nth(0);
    sljava_string_destroy(&out); sljava_string_destroy(&a); sljava_string_destroy(&b);
}

/* Property: algebraic laws over deterministic random strings. */
static void t_algebraic_properties(void) {
    static const uint16_t alpha[] = {0x00, 0x01, 0x61, 0x7F, 0x80, 0xD7FF, 0xD800, 0xD83D,
                                     0xDBFF, 0xDC00, 0xDE00, 0xDFFF, 0xE000, 0xFFFF};
    uint16_t abuf[24], bbuf[24];
    reg_seed(0xABCDEF0123ull);
    for (int it = 0; it < 3000; ++it) {
        size_t an = reg_below(24), bn = reg_below(24);
        SLJavaString a, b, c, d; int32_t o1 = 0, o2 = 0; int eq = 0;
        for (size_t i = 0; i < an; ++i) abuf[i] = alpha[reg_below(sizeof alpha / sizeof alpha[0])];
        for (size_t i = 0; i < bn; ++i) bbuf[i] = alpha[reg_below(sizeof alpha / sizeof alpha[0])];
        sljava_string_init_empty(&a); sljava_string_init_empty(&b);
        sljava_string_init_empty(&c); sljava_string_init_empty(&d);
        REG_REQUIRE(sljava_string_from_utf16(&a, abuf, an) == 0);
        REG_REQUIRE(sljava_string_from_utf16(&b, bbuf, bn) == 0);

        /* length additivity and content of concat */
        REG_REQUIRE(sljava_string_concat(&c, &a, &b) == 0);
        REG_EQ_U(an + bn, c.length);
        /* substring undoes concat */
        REG_REQUIRE(sljava_string_substring(&d, &c, 0, (int32_t)an) == 0);
        REG_CHECK(same(&d, abuf, an));
        REG_REQUIRE(sljava_string_substring(&d, &c, (int32_t)an, (int32_t)(an + bn)) == 0);
        REG_CHECK(same(&d, bbuf, bn));
        /* hash homomorphism: h(ab) = h(a)*31^|b| + h(b)  (mod 2^32) */
        REG_EQ_U(ref_hash(abuf, an) * pow31(bn) + ref_hash(bbuf, bn), (uint32_t)hash_of(&c));
        REG_EQ_U(ref_hash(abuf, an), (uint32_t)hash_of(&a));
        /* compare: matches reference, antisymmetric, zero iff equal; equal => equal hash */
        REG_REQUIRE(sljava_string_compare_to(&a, &b, &o1) == 0);
        REG_REQUIRE(sljava_string_compare_to(&b, &a, &o2) == 0);
        REG_EQ_I(ref_compare(abuf, an, bbuf, bn), o1);
        REG_EQ_I(-sgn(o1), sgn(o2));
        REG_REQUIRE(sljava_string_equals(&a, &b, &eq) == 0);
        REG_EQ_I(o1 == 0, eq);
        if (eq) REG_EQ_I(hash_of(&a), hash_of(&b));
        /* concat with empty is identity; clone equals original */
        sljava_string_destroy(&d);
        REG_REQUIRE(sljava_string_concat(&d, &a, &d) == 0);
        REG_CHECK(same(&d, abuf, an));
        /* code point accounting */
        { int32_t cnt = 0, back = 0;
          REG_REQUIRE(sljava_string_code_point_count(&a, 0, (int32_t)an, &cnt) == 0);
          REG_EQ_I(ref_cp_count(abuf, an), cnt);
          REG_REQUIRE(sljava_string_offset_by_code_points(&a, 0, cnt, &back) == 0);
          REG_EQ_I((long long)an, back);
          REG_REQUIRE(sljava_string_offset_by_code_points(&a, (int32_t)an, -cnt, &back) == 0);
          REG_EQ_I(0, back);
        }
        sljava_string_destroy(&a); sljava_string_destroy(&b);
        sljava_string_destroy(&c); sljava_string_destroy(&d);
    }
}

/* Property: compareTo is a total preorder consistent with equals (sorting). */
static void t_compare_is_a_total_order(void) {
    static const uint16_t alpha[] = {0x00, 0x61, 0x80, 0xD7FF, 0xD800, 0xDC00, 0xE000, 0xFFFF};
    enum { N = 120 };
    SLJavaString s[N]; uint16_t buf[8];
    reg_seed(0x13572468ull);
    for (int i = 0; i < N; ++i) {
        size_t n = reg_below(6);
        for (size_t k = 0; k < n; ++k) buf[k] = alpha[reg_below(8)];
        sljava_string_init_empty(&s[i]);
        REG_REQUIRE(sljava_string_from_utf16(&s[i], buf, n) == 0);
    }
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            int32_t ij = 0, ji = 0, jk = 0, ik = 0;
            (void)sljava_string_compare_to(&s[i], &s[j], &ij);
            (void)sljava_string_compare_to(&s[j], &s[i], &ji);
            if (sgn(ij) != -sgn(ji)) { REG_CHECK(sgn(ij) == -sgn(ji)); return; }
            for (int k = (i * 7 + j) % 11; k < N; k += 11) {
                (void)sljava_string_compare_to(&s[j], &s[k], &jk);
                (void)sljava_string_compare_to(&s[i], &s[k], &ik);
                if (ij <= 0 && jk <= 0 && ik > 0) {
                    REG_CHECK(!"transitivity: a<=b && b<=c implies a<=c"); return;
                }
            }
        }
    for (int i = 0; i < N; ++i) sljava_string_destroy(&s[i]);
}

/* Integration: UTF-8 -> UTF-16 (sleela_utf16) -> SLJavaString agree on counts. */
static void t_utf8_pipeline(void) {
    const char *text = "a\xF0\x9F\x98\x80" "b\xC3\xA9\xE2\x82\xAC"; /* a U+1F600 b é € */
    uint16_t *units = NULL; size_t n = 0; SLJavaString s; int32_t len = 0, cps = 0, cp = 0;
    sljava_string_init_empty(&s);
    REG_EQ_I(0, sleela_utf8_to_utf16(text, strlen(text), &units, &n));
    REG_REQUIRE(units != NULL);
    REG_EQ_I(0, sljava_string_from_utf16(&s, units, n));
    free(units);
    REG_EQ_I(0, sljava_string_length(&s, &len)); REG_EQ_I(6, len);
    REG_EQ_I(0, sljava_string_code_point_count(&s, 0, len, &cps)); REG_EQ_I(5, cps);
    REG_EQ_I(0, sljava_string_code_point_at(&s, 1, &cp)); REG_EQ_I(0x1F600, cp);
    REG_EQ_I(0, sljava_string_code_point_at(&s, 5, &cp)); REG_EQ_I(0x20AC, cp);
    sljava_string_destroy(&s);
}

int main(void) {
    REG_RUN(t_lifecycle);
    REG_RUN(t_invalid_values_rejected);
    REG_RUN(t_length_and_char_at);
    REG_RUN(t_code_points);
    REG_RUN(t_substring);
    REG_RUN(t_concat);
    REG_RUN(t_clone);
    REG_RUN(t_hash_code_golden);
    REG_RUN(t_compare_and_equals_golden);
    REG_RUN(t_out_of_memory_is_atomic);
    REG_RUN(t_algebraic_properties);
    REG_RUN(t_compare_is_a_total_order);
    REG_RUN(t_utf8_pipeline);
    return reg_finish("java_string");
}

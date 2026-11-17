/*
 * reg_utf16.c -- regression suite for impl/core/sleela_utf16.c
 *
 * Contract (impl/core/sleela_utf16.h): input is length-delimited UTF-8;
 * return 0 success, -1 invalid argument, -2 malformed UTF-8, -3 UTF-16
 * index out of range.  "Malformed" is the Unicode well-formedness set
 * (Table 3-7): no overlongs, no surrogates, nothing above U+10FFFF.
 *
 * The oracle below is written independently of the implementation (a
 * byte-range table instead of arithmetic range checks) and the test drives it
 * exhaustively over every 1-, 2- and 3-byte input, plus every code point and
 * a deterministic random sample of longer inputs.
 */
#include "../../../impl/core/sleela_utf16.h"
#include "regression.h"

#include <stdlib.h>
#include <string.h>

/* ---- independent oracle ------------------------------------------------ */

/* Returns the byte length (1..4) of the well-formed sequence at s, or 0. */
static size_t oracle_seq(const unsigned char *s, size_t n, uint32_t *cp) {
    unsigned char a;
    if (n == 0) return 0;
    a = s[0];
    if (a <= 0x7F) { *cp = a; return 1; }
    if (a >= 0xC2 && a <= 0xDF) {
        if (n < 2 || s[1] < 0x80 || s[1] > 0xBF) return 0;
        *cp = ((uint32_t)(a & 0x1Fu) << 6) | (uint32_t)(s[1] & 0x3Fu);
        return 2;
    }
    if (a >= 0xE0 && a <= 0xEF) {
        unsigned char lo = 0x80, hi = 0xBF;
        if (a == 0xE0) lo = 0xA0;
        if (a == 0xED) hi = 0x9F;
        if (n < 3 || s[1] < lo || s[1] > hi || s[2] < 0x80 || s[2] > 0xBF) return 0;
        *cp = ((uint32_t)(a & 0x0Fu) << 12) | ((uint32_t)(s[1] & 0x3Fu) << 6) |
              (uint32_t)(s[2] & 0x3Fu);
        return 3;
    }
    if (a >= 0xF0 && a <= 0xF4) {
        unsigned char lo = 0x80, hi = 0xBF;
        if (a == 0xF0) lo = 0x90;
        if (a == 0xF4) hi = 0x8F;
        if (n < 4 || s[1] < lo || s[1] > hi || s[2] < 0x80 || s[2] > 0xBF ||
            s[3] < 0x80 || s[3] > 0xBF) return 0;
        *cp = ((uint32_t)(a & 0x07u) << 18) | ((uint32_t)(s[1] & 0x3Fu) << 12) |
              ((uint32_t)(s[2] & 0x3Fu) << 6) | (uint32_t)(s[3] & 0x3Fu);
        return 4;
    }
    return 0;
}

/* Whole-buffer oracle. Returns 1 and fills units[] (capacity cap) if valid. */
static int oracle_decode(const unsigned char *s, size_t n, uint16_t *units,
                         size_t cap, size_t *count) {
    size_t off = 0, used = 0;
    while (off < n) {
        uint32_t cp; size_t k = oracle_seq(s + off, n - off, &cp);
        if (!k) return 0;
        if (cp < 0x10000u) { if (used >= cap) return 0; units[used++] = (uint16_t)cp; }
        else {
            if (used + 2 > cap) return 0;
            cp -= 0x10000u;
            units[used++] = (uint16_t)(0xD800u + (cp >> 10));
            units[used++] = (uint16_t)(0xDC00u + (cp & 0x3FFu));
        }
        off += k;
    }
    *count = used;
    return 1;
}

static size_t encode_utf8(uint32_t cp, unsigned char *o) {
    if (cp < 0x80u) { o[0] = (unsigned char)cp; return 1; }
    if (cp < 0x800u) {
        o[0] = (unsigned char)(0xC0u | (cp >> 6));
        o[1] = (unsigned char)(0x80u | (cp & 0x3Fu)); return 2;
    }
    if (cp < 0x10000u) {
        o[0] = (unsigned char)(0xE0u | (cp >> 12));
        o[1] = (unsigned char)(0x80u | ((cp >> 6) & 0x3Fu));
        o[2] = (unsigned char)(0x80u | (cp & 0x3Fu)); return 3;
    }
    o[0] = (unsigned char)(0xF0u | (cp >> 18));
    o[1] = (unsigned char)(0x80u | ((cp >> 12) & 0x3Fu));
    o[2] = (unsigned char)(0x80u | ((cp >> 6) & 0x3Fu));
    o[3] = (unsigned char)(0x80u | (cp & 0x3Fu)); return 4;
}

/* Compare the implementation against the oracle for one buffer. */
static void compare_one(const unsigned char *buf, size_t n) {
    uint16_t want[256]; size_t want_n = 0;
    int valid = oracle_decode(buf, n, want, 256, &want_n);
    int64_t len = -77;
    int rc = sleela_utf8_utf16_length((const char *)buf, n, &len);
    if (valid) {
        uint16_t *got = NULL; size_t got_n = 0;
        reg_note_check();
        if (rc != 0 || len != (int64_t)want_n) {
            reg_fail(__FILE__, __LINE__, "valid input rejected/miscounted (rc=%d len=%lld want=%zu, first byte 0x%02X)",
                     rc, (long long)len, want_n, buf[0]);
            return;
        }
        rc = sleela_utf8_to_utf16((const char *)buf, n, &got, &got_n);
        reg_note_check();
        if (rc != 0 || got_n != want_n || memcmp(got, want, want_n * sizeof(uint16_t)) != 0)
            reg_fail(__FILE__, __LINE__, "to_utf16 disagrees with oracle (first byte 0x%02X)", buf[0]);
        free(got);
        for (size_t i = 0; i < want_n; ++i) {
            uint16_t u = 0;
            int r2 = sleela_utf8_utf16_char_at((const char *)buf, n, (int64_t)i, &u);
            reg_note_check();
            if (r2 != 0 || u != want[i])
                reg_fail(__FILE__, __LINE__, "char_at(%zu) disagrees with oracle", i);
        }
    } else {
        reg_note_check();
        if (rc != -2)
            reg_fail(__FILE__, __LINE__, "malformed input accepted/misclassified (rc=%d, bytes %02X %02X %02X)",
                     rc, buf[0], n > 1 ? buf[1] : 0, n > 2 ? buf[2] : 0);
    }
}

/* ---- cases -------------------------------------------------------------- */

/* Contract: documented argument validation (sleela_utf16.h). */
static void t_argument_contract(void) {
    int64_t n = 123; uint16_t *u = (uint16_t *)0x1; size_t k = 9; uint16_t c = 0;
    REG_EQ_I(0, sleela_utf8_utf16_length(NULL, 0, &n)); REG_EQ_I(0, n);
    REG_EQ_I(0, sleela_utf8_utf16_length("", 0, &n)); REG_EQ_I(0, n);
    REG_EQ_I(-1, sleela_utf8_utf16_length(NULL, 1, &n));
    REG_EQ_I(-1, sleela_utf8_utf16_length("a", 1, NULL));

    REG_EQ_I(0, sleela_utf8_to_utf16(NULL, 0, &u, &k));
    REG_CHECK(u == NULL); REG_EQ_U(0, k);
    REG_EQ_I(-1, sleela_utf8_to_utf16(NULL, 1, &u, &k));
    REG_EQ_I(-1, sleela_utf8_to_utf16("a", 1, NULL, &k));
    REG_EQ_I(-1, sleela_utf8_to_utf16("a", 1, &u, NULL));

    REG_EQ_I(-1, sleela_utf8_utf16_char_at("a", 1, -1, &c));
    REG_EQ_I(-1, sleela_utf8_utf16_char_at("a", 1, 0, NULL));
    REG_EQ_I(-1, sleela_utf8_utf16_char_at(NULL, 1, 0, &c));
    REG_EQ_I(-3, sleela_utf8_utf16_char_at("", 0, 0, &c));
    REG_EQ_I(-3, sleela_utf8_utf16_char_at("a", 1, 1, &c));
    REG_EQ_I(-3, sleela_utf8_utf16_char_at("a", 1, INT64_MAX, &c));
}

/* Contract: output parameters are reset before any failure (no stale pointer). */
static void t_outputs_reset_on_failure(void) {
    uint16_t *u = (uint16_t *)0x1; size_t k = 9;
    REG_EQ_I(-2, sleela_utf8_to_utf16("\xFF", 1, &u, &k));
    REG_CHECK(u == NULL); REG_EQ_U(0, k);
    u = (uint16_t *)0x1; k = 9;
    REG_EQ_I(-2, sleela_utf8_to_utf16("ok\xC3", 3, &u, &k));
    REG_CHECK(u == NULL); REG_EQ_U(0, k);
}

/* Contract: input is length-delimited; embedded NUL is data, not a terminator. */
static void t_embedded_nul(void) {
    static const char s[] = {'a', '\0', 'b'};
    int64_t n = 0; uint16_t *u = NULL; size_t k = 0;
    REG_EQ_I(0, sleela_utf8_utf16_length(s, 3, &n)); REG_EQ_I(3, n);
    REG_EQ_I(0, sleela_utf8_to_utf16(s, 3, &u, &k)); REG_EQ_U(3, k);
    REG_REQUIRE(u != NULL);
    REG_EQ_U(0x61, u[0]); REG_EQ_U(0, u[1]); REG_EQ_U(0x62, u[2]);
    free(u);
    /* a NUL-terminated view of the same bytes must not be consulted */
    REG_EQ_I(0, sleela_utf8_utf16_length("ab\0\xFF", 2, &n)); REG_EQ_I(2, n);
}

/* Contract: well-known encodings, including the BMP/plane boundaries. */
static void t_known_vectors(void) {
    struct { const char *bytes; size_t nb; uint16_t u[2]; size_t nu; } v[] = {
        {"\xC3\xA9", 2, {0x00E9, 0}, 1},
        {"\xE2\x82\xAC", 3, {0x20AC, 0}, 1},
        {"\xF0\x9F\x98\x80", 4, {0xD83D, 0xDE00}, 2},
        {"\xF4\x8F\xBF\xBF", 4, {0xDBFF, 0xDFFF}, 2},
        {"\xF0\x90\x80\x80", 4, {0xD800, 0xDC00}, 2},
        {"\xEF\xBF\xBF", 3, {0xFFFF, 0}, 1},
        {"\xED\x9F\xBF", 3, {0xD7FF, 0}, 1},
        {"\xEE\x80\x80", 3, {0xE000, 0}, 1},
        {"\xEF\xBB\xBF", 3, {0xFEFF, 0}, 1}, /* BOM is preserved, never stripped */
        {"\xC2\x80", 2, {0x0080, 0}, 1},
        {"\xDF\xBF", 2, {0x07FF, 0}, 1},
        {"\xE0\xA0\x80", 3, {0x0800, 0}, 1},
    };
    for (size_t i = 0; i < sizeof v / sizeof v[0]; ++i) {
        uint16_t *u = NULL; size_t k = 0;
        REG_EQ_I(0, sleela_utf8_to_utf16(v[i].bytes, v[i].nb, &u, &k));
        REG_EQ_U(v[i].nu, k);
        if (u) REG_EQ_MEM(v[i].u, u, v[i].nu * sizeof(uint16_t));
        free(u);
    }
}

/* Contract: every ill-formed class is rejected with -2 by every entry point. */
static void t_malformed_vectors(void) {
    static const struct { const char *b; size_t n; const char *why; } bad[] = {
        {"\x80", 1, "lone continuation"}, {"\xBF", 1, "lone continuation"},
        {"\xC0\x80", 2, "overlong NUL"}, {"\xC1\xBF", 2, "overlong ASCII"},
        {"\xE0\x80\x80", 3, "overlong 3-byte"}, {"\xE0\x9F\xBF", 3, "overlong 3-byte max"},
        {"\xF0\x80\x80\x80", 4, "overlong 4-byte"}, {"\xF0\x8F\xBF\xBF", 4, "overlong 4-byte max"},
        {"\xED\xA0\x80", 3, "U+D800 surrogate"}, {"\xED\xBF\xBF", 3, "U+DFFF surrogate"},
        {"\xED\xA0\xBD\xED\xB8\x80", 6, "CESU-8 surrogate pair"},
        {"\xF4\x90\x80\x80", 4, "above U+10FFFF"}, {"\xF5\x80\x80\x80", 4, "F5 lead"},
        {"\xF8\x88\x80\x80\x80", 5, "5-byte form"}, {"\xFC\x84\x80\x80\x80\x80", 6, "6-byte form"},
        {"\xFE", 1, "FE"}, {"\xFF", 1, "FF"},
        {"\xC3", 1, "truncated 2-byte"}, {"\xE2\x82", 2, "truncated 3-byte"},
        {"\xF0\x9F\x98", 3, "truncated 4-byte"},
        {"\xC3(", 2, "bad continuation"}, {"\xE2\x28\xA1", 3, "bad continuation 2"},
        {"ok\xFF" "ok", 5, "bad byte mid-string"},
    };
    for (size_t i = 0; i < sizeof bad / sizeof bad[0]; ++i) {
        int64_t n = 0; uint16_t *u = NULL; size_t k = 0;
        reg_note_check();
        if (sleela_utf8_utf16_length(bad[i].b, bad[i].n, &n) != -2)
            reg_fail(__FILE__, __LINE__, "length accepted: %s", bad[i].why);
        reg_note_check();
        if (sleela_utf8_to_utf16(bad[i].b, bad[i].n, &u, &k) != -2 || u != NULL)
            reg_fail(__FILE__, __LINE__, "to_utf16 accepted: %s", bad[i].why);
    }
}

/* Exhaustive: every 1-byte and 2-byte input (65,792 buffers). */
static void t_exhaustive_1_and_2_bytes(void) {
    unsigned char b[2];
    for (unsigned x = 0; x < 256; ++x) { b[0] = (unsigned char)x; compare_one(b, 1); }
    for (unsigned x = 0; x < 256; ++x)
        for (unsigned y = 0; y < 256; ++y) {
            b[0] = (unsigned char)x; b[1] = (unsigned char)y;
            compare_one(b, 2);
        }
}

/* Exhaustive: every 3-byte input (16,777,216 buffers). */
static void t_exhaustive_3_bytes(void) {
    unsigned char b[3];
    for (unsigned x = 0; x < 256; ++x)
        for (unsigned y = 0; y < 256; ++y)
            for (unsigned z = 0; z < 256; ++z) {
                int64_t len = 0; uint16_t want[4]; size_t wn = 0;
                int valid, rc;
                b[0] = (unsigned char)x; b[1] = (unsigned char)y; b[2] = (unsigned char)z;
                valid = oracle_decode(b, 3, want, 4, &wn);
                rc = sleela_utf8_utf16_length((const char *)b, 3, &len);
                if (valid ? (rc != 0 || len != (int64_t)wn) : (rc != -2)) {
                    reg_note_check();
                    reg_fail(__FILE__, __LINE__, "3-byte disagreement at %02X %02X %02X (rc=%d valid=%d)",
                             x, y, z, rc, valid);
                    return;
                }
                /* heavier cross-check on a stride so the whole space stays fast */
                if (valid && ((x + y + z) & 15u) == 0) compare_one(b, 3);
            }
    REG_CHECK(1);
}

/* 4-byte inputs: every combination drawn from the boundary bytes of each
 * position (lead F0..F4 and every edge value of the continuation ranges). */
static void t_four_byte_boundaries(void) {
    static const unsigned char edge[] = {0x00, 0x7F, 0x80, 0x8F, 0x90, 0x9F, 0xA0, 0xBF, 0xC0, 0xFF};
    unsigned char b[4];
    for (unsigned lead = 0xEF; lead <= 0xF8; ++lead)
        for (size_t i = 0; i < sizeof edge; ++i)
            for (size_t j = 0; j < sizeof edge; ++j)
                for (size_t k = 0; k < sizeof edge; ++k) {
                    b[0] = (unsigned char)lead; b[1] = edge[i]; b[2] = edge[j]; b[3] = edge[k];
                    compare_one(b, 4);
                }
}

/* Every scalar value round-trips through an independent encoder. */
static void t_every_code_point(void) {
    for (uint32_t cp = 0; cp <= 0x10FFFFu; ++cp) {
        unsigned char b[4]; size_t nb; int64_t len = 0; uint16_t u = 0, hi = 0, lo = 0;
        int rc;
        if (cp >= 0xD800u && cp <= 0xDFFFu) continue;
        nb = encode_utf8(cp, b);
        rc = sleela_utf8_utf16_length((const char *)b, nb, &len);
        if (rc != 0 || len != (cp > 0xFFFFu ? 2 : 1)) {
            reg_note_check();
            reg_fail(__FILE__, __LINE__, "U+%04X length rc=%d len=%lld", cp, rc, (long long)len);
            return;
        }
        if (cp <= 0xFFFFu) {
            rc = sleela_utf8_utf16_char_at((const char *)b, nb, 0, &u);
            if (rc != 0 || u != (uint16_t)cp) {
                reg_note_check();
                reg_fail(__FILE__, __LINE__, "U+%04X char_at rc=%d u=%04X", cp, rc, u);
                return;
            }
        } else {
            uint32_t v = cp - 0x10000u;
            rc = sleela_utf8_utf16_char_at((const char *)b, nb, 0, &hi);
            rc |= sleela_utf8_utf16_char_at((const char *)b, nb, 1, &lo);
            if (rc != 0 || hi != (uint16_t)(0xD800u + (v >> 10)) ||
                lo != (uint16_t)(0xDC00u + (v & 0x3FFu))) {
                reg_note_check();
                reg_fail(__FILE__, __LINE__, "U+%04X surrogate pair wrong", cp);
                return;
            }
        }
    }
    REG_CHECK(1);
}

/* Deterministic random strings: valid ones must match; one-byte mutations
 * must be accepted/rejected exactly as the oracle says. */
static void t_random_strings_and_mutations(void) {
    reg_seed(0x5EE1EA0001ull);
    for (int iter = 0; iter < 4000; ++iter) {
        unsigned char buf[4 * 40]; size_t n = 0, cps = reg_below(41);
        for (size_t i = 0; i < cps; ++i) {
            uint32_t cp;
            switch (reg_below(5)) {
                case 0: cp = reg_below(0x80); break;
                case 1: cp = 0x80u + reg_below(0x780); break;
                case 2: cp = 0x800u + reg_below(0xD800 - 0x800); break;
                case 3: cp = 0xE000u + reg_below(0x10000 - 0xE000); break;
                default: cp = 0x10000u + reg_below(0x100000); break;
            }
            n += encode_utf8(cp, buf + n);
        }
        if (n == 0) continue;
        compare_one(buf, n);
        for (int m = 0; m < 6; ++m) {
            unsigned char mut[sizeof buf];
            memcpy(mut, buf, n);
            mut[reg_below((uint32_t)n)] = (unsigned char)reg_below(256);
            compare_one(mut, n);
        }
    }
}

/* CHARACTERIZATION of current behaviour: char_at decodes lazily from the front.
 * Malformed bytes AFTER the addressed unit do not affect the answer; malformed
 * bytes BEFORE (or at) it yield -2; an index past the end of valid text is -3. */
static void t_char_at_is_lazy(void) {
    uint16_t c = 0;
    REG_EQ_I(0, sleela_utf8_utf16_char_at("A\xFF", 2, 0, &c)); REG_EQ_U(0x41, c);
    REG_EQ_I(-2, sleela_utf8_utf16_char_at("A\xFF", 2, 1, &c));
    REG_EQ_I(-2, sleela_utf8_utf16_char_at("A\xFF", 2, 5, &c));
    REG_EQ_I(-3, sleela_utf8_utf16_char_at("A", 1, 1, &c));
    /* but the strict whole-string entry points reject it */
    { int64_t n; REG_EQ_I(-2, sleela_utf8_utf16_length("A\xFF", 2, &n)); }
    /* surrogate pair addressing: index 0 = high, 1 = low, 2 = out of range */
    REG_EQ_I(0, sleela_utf8_utf16_char_at("\xF0\x9F\x98\x80", 4, 0, &c)); REG_EQ_U(0xD83D, c);
    REG_EQ_I(0, sleela_utf8_utf16_char_at("\xF0\x9F\x98\x80", 4, 1, &c)); REG_EQ_U(0xDE00, c);
    REG_EQ_I(-3, sleela_utf8_utf16_char_at("\xF0\x9F\x98\x80", 4, 2, &c));
}

/* Out-of-memory: fail each allocation in turn; no leak, no stale output. */
static void t_out_of_memory(void) {
    for (unsigned long nth = 1; nth <= 3; ++nth) {
        uint16_t *u = (uint16_t *)0x1; size_t k = 9; int rc;
        reg_alloc_fail_nth(nth);
        rc = sleela_utf8_to_utf16("h\xC3\xA9llo", 6, &u, &k);
        if (reg_alloc_injected()) {
            REG_EQ_I(-1, rc); REG_CHECK(u == NULL); REG_EQ_U(0, k);
        } else {
            REG_EQ_I(0, rc); REG_EQ_U(5, k); free(u);
        }
        reg_alloc_fail_nth(0);
    }
    /* the very first allocation is the only one the call makes */
    {
        uint16_t *u = (uint16_t *)0x1; size_t k = 9;
        reg_alloc_fail_nth(1);
        REG_EQ_I(-1, sleela_utf8_to_utf16("abc", 3, &u, &k));
        REG_CHECK(reg_alloc_injected()); REG_CHECK(u == NULL); REG_EQ_U(0, k);
    }
}

/* Scale: a 4 MiB all-supplementary string (1 Mi code points, 2 Mi units). */
static void t_large_input(void) {
    const size_t cps = 1u << 20; const size_t nb = cps * 4u;
    unsigned char *b = (unsigned char *)malloc(nb);
    int64_t len = 0; uint16_t *u = NULL; size_t k = 0;
    REG_REQUIRE(b != NULL);
    for (size_t i = 0; i < cps; ++i) memcpy(b + i * 4u, "\xF0\x9F\x98\x80", 4);
    REG_EQ_I(0, sleela_utf8_utf16_length((const char *)b, nb, &len));
    REG_EQ_I((long long)(cps * 2u), len);
    REG_EQ_I(0, sleela_utf8_to_utf16((const char *)b, nb, &u, &k));
    REG_EQ_U(cps * 2u, k);
    if (u) { REG_EQ_U(0xD83D, u[0]); REG_EQ_U(0xDE00, u[k - 1]); }
    free(u);
    b[nb - 1] = 0x20; /* corrupt the final continuation byte */
    REG_EQ_I(-2, sleela_utf8_utf16_length((const char *)b, nb, &len));
    free(b);
}

int main(void) {
    REG_RUN(t_argument_contract);
    REG_RUN(t_outputs_reset_on_failure);
    REG_RUN(t_embedded_nul);
    REG_RUN(t_known_vectors);
    REG_RUN(t_malformed_vectors);
    REG_RUN(t_exhaustive_1_and_2_bytes);
    REG_RUN(t_exhaustive_3_bytes);
    REG_RUN(t_four_byte_boundaries);
    REG_RUN(t_every_code_point);
    REG_RUN(t_random_strings_and_mutations);
    REG_RUN(t_char_at_is_lazy);
    REG_RUN(t_out_of_memory);
    REG_RUN(t_large_input);
    return reg_finish("utf16");
}

/*
 * reg_java_string_codec.c -- regression suite for
 * impl/core/sleela_java_string_codec.c
 *
 * Wire contract (sleela_java_string_codec.h): "SLJS", version byte 1, three
 * reserved zero bytes, uint32 little-endian UTF-16 unit count, then that many
 * little-endian uint16 units.  Decoding must accept exactly that and nothing
 * else: any other header byte, any size that is not 12 + 2*count, and any
 * count above INT32_MAX is rejected without allocating or touching `out`.
 */
#include "../../../impl/core/sleela_java_string_codec.h"
#include "regression.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>

static const uint8_t HEADER_HI[12] = {'S', 'L', 'J', 'S', 1, 0, 0, 0, 0, 0, 0, 0};

static int same(const SLJavaString *s, const uint16_t *u, size_t n) {
    return s->length == n && (n == 0 || memcmp(s->units, u, n * sizeof(uint16_t)) == 0);
}

/* Contract: byte-exact layout, checked against a hand-assembled record. */
static void t_golden_bytes(void) {
    /* "Hi" + U+1F600 + NUL + lone low surrogate */
    const uint16_t units[] = {0x0048, 0x0069, 0xD83D, 0xDE00, 0x0000, 0xDC00};
    const uint8_t want[] = {'S', 'L', 'J', 'S', 1, 0, 0, 0, 6, 0, 0, 0,
                            0x48, 0x00, 0x69, 0x00, 0x3D, 0xD8, 0x00, 0xDE,
                            0x00, 0x00, 0x00, 0xDC};
    SLJavaString s; uint8_t *bytes = NULL; size_t size = 0;
    sljava_string_init_empty(&s);
    REG_EQ_I(0, sljava_string_from_utf16(&s, units, 6));
    REG_EQ_I(0, sljava_string_encode_record(&s, &bytes, &size));
    REG_EQ_U(sizeof want, size);
    REG_REQUIRE(bytes != NULL);
    REG_EQ_MEM(want, bytes, sizeof want);
    REG_EQ_U(12, SL_JAVA_STRING_RECORD_HEADER_SIZE);
    free(bytes); sljava_string_destroy(&s);
}

static void t_empty_record(void) {
    SLJavaString s, d; uint8_t *bytes = NULL; size_t size = 0;
    sljava_string_init_empty(&s); sljava_string_init_empty(&d);
    REG_EQ_I(0, sljava_string_encode_record(&s, &bytes, &size));
    REG_EQ_U(12, size); REG_REQUIRE(bytes != NULL);
    REG_EQ_MEM(HEADER_HI, bytes, 12);
    REG_EQ_I(0, sljava_string_decode_record(&d, bytes, size));
    REG_EQ_U(0, d.length); REG_CHECK(d.units == NULL);
    free(bytes);
}

/* The count field is four little-endian bytes. Counts whose bytes are all
 * different (and non-zero) catch any shift, mask or ordering mistake. */
static void check_count_field(size_t n) {
    SLJavaString s, d; uint16_t *u = (uint16_t *)malloc(n * sizeof(uint16_t));
    uint8_t *bytes = NULL; size_t size = 0;
    REG_REQUIRE(u != NULL);
    for (size_t i = 0; i < n; ++i) u[i] = (uint16_t)((i * 2654435761u) >> 7);
    sljava_string_init_empty(&s); sljava_string_init_empty(&d);
    REG_EQ_I(0, sljava_string_from_utf16(&s, u, n));
    REG_EQ_I(0, sljava_string_encode_record(&s, &bytes, &size));
    REG_REQUIRE(bytes != NULL);
    REG_EQ_U(12u + 2u * n, size);
    REG_EQ_U(n & 0xFFu, bytes[8]); REG_EQ_U((n >> 8) & 0xFFu, bytes[9]);
    REG_EQ_U((n >> 16) & 0xFFu, bytes[10]); REG_EQ_U((n >> 24) & 0xFFu, bytes[11]);
    REG_EQ_I(0, sljava_string_decode_record(&d, bytes, size));
    REG_CHECK(same(&d, u, n));
    free(bytes); free(u); sljava_string_destroy(&s); sljava_string_destroy(&d);
}

static void t_length_field_endianness(void) {
    check_count_field(0x0A0B0Cu);      /* bytes 0C 0B 0A 00 */
    check_count_field(0x01FF02u);      /* bytes 02 FF 01 00 */
}

/* A 4 GiB-class count is not reachable here, but the top byte of the count
 * field is: 0x01010101 units is 32 MiB of text. */
static void t_length_field_top_byte(void) {
    check_count_field(0x01010101u);    /* bytes 01 01 01 01 */
}

/* Round trip over deterministic random content, incl. NUL and lone surrogates. */
static void t_round_trip_random(void) {
    reg_seed(0xC0DEC0DEull);
    for (int it = 0; it < 2000; ++it) {
        uint16_t buf[64]; size_t n = reg_below(65);
        SLJavaString s, d; uint8_t *bytes = NULL; size_t size = 0;
        for (size_t i = 0; i < n; ++i) buf[i] = (uint16_t)reg_next();
        sljava_string_init_empty(&s); sljava_string_init_empty(&d);
        REG_REQUIRE(sljava_string_from_utf16(&s, buf, n) == 0);
        REG_REQUIRE(sljava_string_encode_record(&s, &bytes, &size) == 0);
        REG_EQ_U(12 + 2 * n, size);
        REG_REQUIRE(sljava_string_decode_record(&d, bytes, size) == 0);
        REG_CHECK(same(&d, buf, n));
        free(bytes); sljava_string_destroy(&s); sljava_string_destroy(&d);
    }
}

/* Every single-byte change in the 8 fixed header bytes is rejected. */
static void t_header_mutation_exhaustive(void) {
    uint8_t rec[12 + 4] = {'S', 'L', 'J', 'S', 1, 0, 0, 0, 2, 0, 0, 0, 0x41, 0, 0x42, 0};
    SLJavaString d; sljava_string_init_empty(&d);
    REG_EQ_I(0, sljava_string_decode_record(&d, rec, sizeof rec));
    sljava_string_destroy(&d);
    for (size_t pos = 0; pos < 8; ++pos)
        for (unsigned v = 0; v < 256; ++v) {
            uint8_t m[sizeof rec]; int rc;
            if (v == rec[pos]) continue;
            memcpy(m, rec, sizeof rec); m[pos] = (uint8_t)v;
            sljava_string_init_empty(&d);
            rc = sljava_string_decode_record(&d, m, sizeof m);
            reg_note_check();
            if (rc != SL_JAVA_STRING_INVALID_ARGUMENT || d.length != 0 || d.units != NULL)
                reg_fail(__FILE__, __LINE__, "header byte %zu = 0x%02X accepted (rc=%d)", pos, v, rc);
            sljava_string_destroy(&d);
        }
}

/* Any change to the count that breaks size == 12 + 2*count is rejected. */
static void t_count_size_consistency(void) {
    uint8_t rec[12 + 6] = {'S', 'L', 'J', 'S', 1, 0, 0, 0, 3, 0, 0, 0, 1, 0, 2, 0, 3, 0};
    SLJavaString d; sljava_string_init_empty(&d);
    for (unsigned count = 0; count < 300; ++count) {
        int rc;
        rec[8] = (uint8_t)count; rec[9] = (uint8_t)(count >> 8);
        rc = sljava_string_decode_record(&d, rec, sizeof rec);
        reg_note_check();
        if ((count == 3) != (rc == 0))
            reg_fail(__FILE__, __LINE__, "count %u vs 3 units: rc=%d", count, rc);
        sljava_string_destroy(&d);
    }
    /* huge counts never reach the allocator */
    {
        static const uint32_t huge[] = {0x7FFFFFFFu, 0x80000000u, 0xFFFFFFFFu, 0x40000000u};
        for (size_t i = 0; i < sizeof huge / sizeof huge[0]; ++i) {
            uint8_t h[12]; unsigned long before = reg_alloc_calls();
            memcpy(h, HEADER_HI, 12);
            h[8] = (uint8_t)huge[i]; h[9] = (uint8_t)(huge[i] >> 8);
            h[10] = (uint8_t)(huge[i] >> 16); h[11] = (uint8_t)(huge[i] >> 24);
            REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_decode_record(&d, h, 12));
            REG_EQ_U(before, reg_alloc_calls());
        }
    }
}

/* Every truncation and every over-long buffer of a valid record is rejected. */
static void t_truncation_and_trailing_bytes(void) {
    uint8_t rec[12 + 6 + 8] = {'S', 'L', 'J', 'S', 1, 0, 0, 0, 3, 0, 0, 0, 1, 0, 2, 0, 3, 0};
    SLJavaString d; sljava_string_init_empty(&d);
    for (size_t size = 0; size <= sizeof rec; ++size) {
        int rc = sljava_string_decode_record(&d, rec, size);
        reg_note_check();
        if ((size == 18) != (rc == 0))
            reg_fail(__FILE__, __LINE__, "size %zu: rc=%d", size, rc);
        sljava_string_destroy(&d);
    }
}

/* A failed decode never disturbs the caller's existing value. */
static void t_decode_failure_preserves_out(void) {
    uint8_t good[14] = {'S', 'L', 'J', 'S', 1, 0, 0, 0, 1, 0, 0, 0, 0x7A, 0};
    uint8_t bad[14]; SLJavaString d; const uint16_t keep[2] = {5, 6};
    sljava_string_init_empty(&d);
    REG_EQ_I(0, sljava_string_from_utf16(&d, keep, 2));
    memcpy(bad, good, sizeof bad); bad[0] = 'X';
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_decode_record(&d, bad, sizeof bad));
    REG_CHECK(same(&d, keep, 2));
    REG_EQ_I(0, sljava_string_decode_record(&d, good, sizeof good)); /* success replaces */
    REG_CHECK(same(&d, (const uint16_t[]){0x7A}, 1));
    sljava_string_destroy(&d);
}

static void t_argument_validation(void) {
    SLJavaString s, d; uint8_t *bytes = NULL; size_t size = 0; uint8_t rec[12];
    sljava_string_init_empty(&s); sljava_string_init_empty(&d);
    memcpy(rec, HEADER_HI, 12);
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_encode_record(NULL, &bytes, &size));
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_encode_record(&s, NULL, &size));
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_encode_record(&s, &bytes, NULL));
    s.length = 2; s.units = NULL;                      /* non-empty without storage */
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_encode_record(&s, &bytes, &size));
    s.length = 0;
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_decode_record(NULL, rec, 12));
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_decode_record(&d, NULL, 12));
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_decode_record(&d, rec, 0));
    REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_decode_record(&d, rec, 11));
    REG_CHECK(bytes == NULL); REG_EQ_U(0, size);
}

static void t_out_of_memory(void) {
    SLJavaString s, d; uint8_t *bytes = NULL; size_t size = 0;
    uint8_t rec[14] = {'S', 'L', 'J', 'S', 1, 0, 0, 0, 1, 0, 0, 0, 0x7A, 0};
    const uint16_t keep[1] = {9};
    sljava_string_init_empty(&s); sljava_string_init_empty(&d);
    REG_EQ_I(0, sljava_string_from_utf16(&s, keep, 1));
    reg_alloc_fail_nth(1);
    REG_EQ_I(SL_JAVA_STRING_OUT_OF_MEMORY, sljava_string_encode_record(&s, &bytes, &size));
    REG_CHECK(bytes == NULL);
    REG_EQ_I(0, sljava_string_from_utf16(&d, keep, 1));
    reg_alloc_fail_nth(1);
    REG_EQ_I(SL_JAVA_STRING_OUT_OF_MEMORY, sljava_string_decode_record(&d, rec, sizeof rec));
    REG_CHECK(same(&d, keep, 1));
    /* second allocation (inside from_utf16) failing must free the temp buffer */
    reg_alloc_fail_nth(2);
    REG_EQ_I(SL_JAVA_STRING_OUT_OF_MEMORY, sljava_string_decode_record(&d, rec, sizeof rec));
    reg_alloc_fail_nth(0);
    REG_CHECK(same(&d, keep, 1));
    sljava_string_destroy(&s); sljava_string_destroy(&d);
}

int main(void) {
    REG_RUN(t_golden_bytes);
    REG_RUN(t_empty_record);
    REG_RUN(t_length_field_endianness);
    REG_RUN(t_length_field_top_byte);
    REG_RUN(t_round_trip_random);
    REG_RUN(t_header_mutation_exhaustive);
    REG_RUN(t_count_size_consistency);
    REG_RUN(t_truncation_and_trailing_bytes);
    REG_RUN(t_decode_failure_preserves_out);
    REG_RUN(t_argument_validation);
    REG_RUN(t_out_of_memory);
    return reg_finish("java_string_codec");
}

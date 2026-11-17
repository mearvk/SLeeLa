/*
 * reg_http6.c -- regression suite for the HTTP 6.0 frame layer
 * (http-6.0/http6_protocol.c)
 *
 * Contract (http-6.0/http6_protocol.h):
 *   - a frame is a 32-byte big-endian header followed by payload_length bytes:
 *       0 version(6)  1 type(1..20)  2..3 flags  4..11 stream  12..19 request
 *       20..27 sequence  28..31 payload length (at most 16 MiB);
 *   - validate: -1 NULL header, -2 wrong version, -3 unknown type, -4 length over
 *     the cap, -5 length beyond the bytes available, 0 acceptable;
 *   - decode never reads past the buffer it is given: a short header is -2, a
 *     truncated payload, an oversized length, a bad version or type is -3;
 *   - encode never writes past out_cap and encode/decode round-trip exactly.
 *
 * Every buffer handed to the code under test is a separate exact-size heap
 * block, so the harness's guard bytes catch a one-byte over-read or over-write.
 */
#include "../../../http-6.0/http6_protocol.h"
#include "regression.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HDR HTTP6_FRAME_HEADER_SIZE

static http6_frame_header_t header(uint8_t type, uint32_t len) {
    http6_frame_header_t h;
    memset(&h, 0, sizeof h);
    h.version = HTTP6_VERSION;
    h.type = type;
    h.payload_length = len;
    return h;
}

/* Encode into an exact-size block; returns NULL on failure. */
static uint8_t *encode_exact(const http6_frame_header_t *h, const uint8_t *payload, size_t *len) {
    size_t cap = HDR + h->payload_length;
    uint8_t *w = (uint8_t *)malloc(cap ? cap : 1);
    if (!w) return NULL;
    if (http6_frame_encode(h, payload, w, cap, len) != 0) { free(w); return NULL; }
    return w;
}

/* ---- validate -------------------------------------------------------------- */
static void t_validate_codes(void) {
    http6_frame_header_t h = header(HTTP6_FRAME_DATA, 10);
    REG_EQ_I(-1, http6_frame_validate(NULL, 100));
    REG_EQ_I(0, http6_frame_validate(&h, 10));
    REG_EQ_I(0, http6_frame_validate(&h, 11));
    REG_EQ_I(-5, http6_frame_validate(&h, 9));         /* payload longer than what is there */
    REG_EQ_I(-5, http6_frame_validate(&h, 0));

    h.version = 5;  REG_EQ_I(-2, http6_frame_validate(&h, 10));
    h.version = 7;  REG_EQ_I(-2, http6_frame_validate(&h, 10));
    h.version = 0;  REG_EQ_I(-2, http6_frame_validate(&h, 10));
    h.version = 255; REG_EQ_I(-2, http6_frame_validate(&h, 10));
    h.version = HTTP6_VERSION;

    h.type = 0;   REG_EQ_I(-3, http6_frame_validate(&h, 10));
    h.type = 21;  REG_EQ_I(-3, http6_frame_validate(&h, 10));
    h.type = 255; REG_EQ_I(-3, http6_frame_validate(&h, 10));
    h.type = HTTP6_FRAME_DATA;

    h.payload_length = HTTP6_MAX_PAYLOAD + 1u;
    REG_EQ_I(-4, http6_frame_validate(&h, (size_t)-1));  /* over the cap even if the bytes exist */
    h.payload_length = 0xFFFFFFFFu;
    REG_EQ_I(-4, http6_frame_validate(&h, (size_t)-1));
    h.payload_length = HTTP6_MAX_PAYLOAD;
    REG_EQ_I(0, http6_frame_validate(&h, HTTP6_MAX_PAYLOAD));       /* the cap itself is allowed */
    REG_EQ_I(-5, http6_frame_validate(&h, HTTP6_MAX_PAYLOAD - 1u));
}

static void t_every_defined_type_is_valid_and_named(void) {
    static const char *const names[] = {
        "OPEN", "DATA", "END", "RESET", "WINDOW", "PING", "PONG", "RESUME", "CAPSULE",
        "FRIENDS_PACK", "BONUS_OFFER", "FP_UPDATE", "AUDIT", "CONSOLIDATED_FRIENDS_BET",
        "TEAMSTER_DEBATE", "CONSOLIDATE_IQ", "TEAM_AREA", "DEBATE_TOPIC", "DEBATE_POSITION",
        "RECIPIENT_LABEL" };
    unsigned t;
    for (t = 1; t <= 20; ++t) {
        http6_frame_header_t h = header((uint8_t)t, 0);
        REG_EQ_I(0, http6_frame_validate(&h, 0));
        REG_EQ_S(names[t - 1], http6_frame_type_name((http6_frame_type_t)t));
    }
    REG_EQ_S("UNKNOWN", http6_frame_type_name((http6_frame_type_t)0));
    REG_EQ_S("UNKNOWN", http6_frame_type_name((http6_frame_type_t)21));
    REG_EQ_S("UNKNOWN", http6_frame_type_name((http6_frame_type_t)255));
}

/* ---- wire layout ------------------------------------------------------------ */
static void t_wire_layout_is_big_endian(void) {
    http6_frame_header_t h = header(HTTP6_FRAME_PING, 3);
    static const uint8_t payload[3] = { 0xAA, 0xBB, 0xCC };
    static const uint8_t expect[HTTP6_FRAME_HEADER_SIZE + 3] = {
        6, 6,                                           /* version, type PING */
        0x12, 0x34,                                     /* flags */
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, /* stream id */
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, /* request id */
        0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, /* sequence */
        0, 0, 0, 3,                                     /* payload length */
        0xAA, 0xBB, 0xCC };
    size_t n = 0;
    uint8_t *w;
    h.flags = 0x1234;
    h.stream_id = 0x0102030405060708ull;
    h.request_id = 0x1112131415161718ull;
    h.sequence = 0x2122232425262728ull;
    w = encode_exact(&h, payload, &n);
    REG_REQUIRE(w != NULL);
    REG_EQ_U(sizeof expect, n);
    REG_EQ_MEM(expect, w, sizeof expect);
    free(w);
}

/* ---- encode ----------------------------------------------------------------- */
static void t_encode_refusals(void) {
    http6_frame_header_t h = header(HTTP6_FRAME_DATA, 4);
    uint8_t payload[4] = { 1, 2, 3, 4 };
    uint8_t *out = (uint8_t *)malloc(HDR + 4);
    size_t n = 0;
    REG_REQUIRE(out != NULL);
    REG_EQ_I(-1, http6_frame_encode(NULL, payload, out, HDR + 4, &n));
    REG_EQ_I(-1, http6_frame_encode(&h, payload, NULL, HDR + 4, &n));
    REG_EQ_I(-1, http6_frame_encode(&h, payload, out, HDR + 4, NULL));
    REG_EQ_I(-3, http6_frame_encode(&h, NULL, out, HDR + 4, &n));          /* length > 0 but no bytes */
    REG_EQ_I(-4, http6_frame_encode(&h, payload, out, HDR + 3, &n));       /* one byte short */
    REG_EQ_I(-4, http6_frame_encode(&h, payload, out, 0, &n));
    h.version = 9;
    REG_EQ_I(-2, http6_frame_encode(&h, payload, out, HDR + 4, &n));       /* invalid header */
    h = header(0, 4);
    REG_EQ_I(-2, http6_frame_encode(&h, payload, out, HDR + 4, &n));
    h = header(HTTP6_FRAME_DATA, HTTP6_MAX_PAYLOAD + 1u);
    REG_EQ_I(-2, http6_frame_encode(&h, payload, out, HDR + 4, &n));
    free(out);
}

static void t_encode_exact_capacity_and_empty_payload(void) {
    http6_frame_header_t h = header(HTTP6_FRAME_END, 0);
    size_t n = 99;
    uint8_t *out = (uint8_t *)malloc(HDR);
    REG_REQUIRE(out != NULL);
    REG_EQ_I(0, http6_frame_encode(&h, NULL, out, HDR, &n));               /* no payload: NULL is fine */
    REG_EQ_U(HDR, n);
    free(out);
}

/* ---- decode ----------------------------------------------------------------- */
static void t_decode_refusals(void) {
    uint8_t *w;
    size_t n = 0, used = 0;
    http6_frame_view_t v;
    http6_frame_header_t h = header(HTTP6_FRAME_DATA, 5);
    static const uint8_t payload[5] = { 9, 8, 7, 6, 5 };
    w = encode_exact(&h, payload, &n);
    REG_REQUIRE(w != NULL);
    REG_EQ_I(-1, http6_frame_decode(NULL, n, &v, &used));
    REG_EQ_I(-1, http6_frame_decode(w, n, NULL, &used));
    REG_EQ_I(-1, http6_frame_decode(w, n, &v, NULL));
    free(w);
}

/* Truncate a valid frame at every possible length: each shorter prefix fails
 * with the right code and, being an exact-size block, cannot be over-read. */
static void t_every_truncation_is_refused(void) {
    http6_frame_header_t h = header(HTTP6_FRAME_CAPSULE, 40);
    uint8_t payload[40];
    size_t full = 0, cut, bad_short = 0, bad_trunc = 0;
    uint8_t *w;
    unsigned i;
    for (i = 0; i < sizeof payload; ++i) payload[i] = (uint8_t)(i * 7u + 1u);
    w = encode_exact(&h, payload, &full);
    REG_REQUIRE(w != NULL);
    REG_EQ_U(HDR + 40, full);
    for (cut = 0; cut < full; ++cut) {
        uint8_t *copy = (uint8_t *)malloc(cut ? cut : 1);
        http6_frame_view_t v;
        size_t used = 12345;
        int rc;
        REG_REQUIRE(copy != NULL);
        memcpy(copy, w, cut);
        rc = http6_frame_decode(copy, cut, &v, &used);
        if (cut < HDR) { if (rc != -2) ++bad_short; }
        else if (rc != -3) ++bad_trunc;
        if (used != 12345) ++bad_trunc;                /* a refused decode leaves *consumed alone */
        free(copy);
    }
    REG_EQ_U(0, bad_short);
    REG_EQ_U(0, bad_trunc);
    free(w);
}

static void t_oversized_and_hostile_lengths(void) {
    static const uint32_t lengths[] = { HTTP6_MAX_PAYLOAD + 1u, 0x7FFFFFFFu, 0x80000000u, 0xFFFFFFFFu, 1u << 24 | 1u };
    size_t i;
    for (i = 0; i < sizeof lengths / sizeof lengths[0]; ++i) {
        uint8_t *w = (uint8_t *)calloc(HDR + 8, 1);
        http6_frame_view_t v;
        size_t used = 0;
        REG_REQUIRE(w != NULL);
        w[0] = 6; w[1] = 2;
        w[28] = (uint8_t)(lengths[i] >> 24); w[29] = (uint8_t)(lengths[i] >> 16);
        w[30] = (uint8_t)(lengths[i] >> 8);  w[31] = (uint8_t)lengths[i];
        REG_EQ_I(-3, http6_frame_decode(w, HDR + 8, &v, &used));
        free(w);
    }
}

static void t_bad_version_and_type_refused_on_the_wire(void) {
    uint8_t *w = (uint8_t *)calloc(HDR, 1);
    http6_frame_view_t v;
    size_t used = 0;
    REG_REQUIRE(w != NULL);
    w[0] = 6; w[1] = 1;
    REG_EQ_I(0, http6_frame_decode(w, HDR, &v, &used));
    w[0] = 5; REG_EQ_I(-3, http6_frame_decode(w, HDR, &v, &used));
    w[0] = 6; w[1] = 0;  REG_EQ_I(-3, http6_frame_decode(w, HDR, &v, &used));
    w[1] = 21; REG_EQ_I(-3, http6_frame_decode(w, HDR, &v, &used));
    free(w);
}

static void t_trailing_bytes_are_not_consumed(void) {
    http6_frame_header_t h = header(HTTP6_FRAME_DATA, 2);
    static const uint8_t payload[2] = { 0x10, 0x20 };
    size_t n = 0, used = 0;
    uint8_t *w = encode_exact(&h, payload, &n);
    uint8_t *two;
    http6_frame_view_t v;
    REG_REQUIRE(w != NULL);
    two = (uint8_t *)malloc(n * 2);
    REG_REQUIRE(two != NULL);
    memcpy(two, w, n);
    memcpy(two + n, w, n);                             /* two frames back to back */
    REG_EQ_I(0, http6_frame_decode(two, n * 2, &v, &used));
    REG_EQ_U(n, used);                                 /* only the first frame */
    REG_EQ_I(0, http6_frame_decode(two + used, n * 2 - used, &v, &used));
    REG_EQ_U(n, used);
    free(two);
    free(w);
}

/* ---- round trips ------------------------------------------------------------ */
static void t_round_trip_every_type_and_size(void) {
    static const uint32_t sizes[] = { 0, 1, 2, 31, 32, 33, 255, 256, 4096, 65535, 65536 };
    unsigned t;
    size_t si, bad = 0, checked = 0;
    for (t = 1; t <= 20; ++t) {
        for (si = 0; si < sizeof sizes / sizeof sizes[0]; ++si) {
            http6_frame_header_t h = header((uint8_t)t, sizes[si]), back;
            uint8_t *payload = (uint8_t *)malloc(sizes[si] ? sizes[si] : 1), *wire;
            size_t n = 0, used = 0;
            http6_frame_view_t v;
            uint32_t i;
            REG_REQUIRE(payload != NULL);
            for (i = 0; i < sizes[si]; ++i) payload[i] = (uint8_t)reg_below(256);
            h.flags = (uint16_t)reg_next();
            h.stream_id = reg_next();
            h.request_id = reg_next();
            h.sequence = reg_next();
            wire = encode_exact(&h, payload, &n);
            REG_REQUIRE(wire != NULL);
            ++checked;
            if (http6_frame_decode(wire, n, &v, &used) != 0) { ++bad; free(wire); free(payload); continue; }
            back = v.header;
            if (used != n || back.version != h.version || back.type != h.type || back.flags != h.flags ||
                back.stream_id != h.stream_id || back.request_id != h.request_id ||
                back.sequence != h.sequence || back.payload_length != h.payload_length ||
                v.payload != wire + HDR || (sizes[si] && memcmp(v.payload, payload, sizes[si]) != 0))
                ++bad;
            free(wire);
            free(payload);
        }
    }
    REG_EQ_U(20 * 11, checked);
    REG_EQ_U(0, bad);
}

static void t_maximum_payload_round_trips(void) {
    http6_frame_header_t h = header(HTTP6_FRAME_DATA, HTTP6_MAX_PAYLOAD);
    uint8_t *payload = (uint8_t *)calloc(HTTP6_MAX_PAYLOAD, 1), *wire;
    size_t n = 0, used = 0;
    http6_frame_view_t v;
    REG_REQUIRE(payload != NULL);
    payload[0] = 0x5A; payload[HTTP6_MAX_PAYLOAD - 1] = 0xA5;
    wire = encode_exact(&h, payload, &n);
    REG_REQUIRE(wire != NULL);
    REG_EQ_U(HDR + HTTP6_MAX_PAYLOAD, n);
    REG_EQ_I(0, http6_frame_decode(wire, n, &v, &used));
    REG_EQ_U(n, used);
    REG_EQ_I(0x5A, v.payload[0]);
    REG_EQ_I(0xA5, v.payload[HTTP6_MAX_PAYLOAD - 1]);
    REG_EQ_I(-3, http6_frame_decode(wire, n - 1, &v, &used));       /* one byte short of the maximum */
    free(wire);
    free(payload);
}

/* ---- hostile input ----------------------------------------------------------- */
/* Random buffers and single-byte corruptions of valid frames.  Whatever the
 * outcome, decode must stay inside the exact-size block (the guard bytes check
 * this), and a success must be self-consistent. */
static void t_random_input_never_escapes_the_buffer(void) {
    size_t i, accepted = 0, inconsistent = 0;
    reg_seed(0x6B6B6B6Bull);
    for (i = 0; i < 20000; ++i) {
        size_t len = reg_below(120), k;
        uint8_t *buf = (uint8_t *)malloc(len ? len : 1);
        http6_frame_view_t v;
        size_t used = 0;
        REG_REQUIRE(buf != NULL);
        for (k = 0; k < len; ++k) buf[k] = (uint8_t)reg_below(256);
        if (len >= 2 && (i & 1u)) { buf[0] = 6; buf[1] = (uint8_t)(1u + reg_below(20)); }   /* often header-plausible */
        if (len >= HDR && (i & 2u)) { buf[28] = 0; buf[29] = 0; buf[30] = 0; buf[31] = (uint8_t)reg_below(90); }
        if (http6_frame_decode(buf, len, &v, &used) == 0) {
            ++accepted;
            if (used > len || used != HDR + v.header.payload_length || v.payload != buf + HDR) ++inconsistent;
        }
        free(buf);
    }
    REG_CHECK(accepted > 100);                         /* the generator really reaches the accept path */
    REG_EQ_U(0, inconsistent);
}

static void t_single_byte_corruption_is_contained(void) {
    http6_frame_header_t h = header(HTTP6_FRAME_DATA, 16);
    uint8_t payload[16] = { 0 };
    size_t n = 0, pos, bad = 0;
    unsigned bit;
    uint8_t *w = encode_exact(&h, payload, &n);
    REG_REQUIRE(w != NULL);
    for (pos = 0; pos < n; ++pos) {
        for (bit = 0; bit < 8; ++bit) {
            uint8_t *copy = (uint8_t *)malloc(n);
            http6_frame_view_t v;
            size_t used = 0;
            REG_REQUIRE(copy != NULL);
            memcpy(copy, w, n);
            copy[pos] ^= (uint8_t)(1u << bit);
            if (http6_frame_decode(copy, n, &v, &used) == 0 && used > n) ++bad;
            free(copy);
        }
    }
    REG_EQ_U(0, bad);
    free(w);
}

int main(void) {
    printf("http6 frame regression suite\n");
    REG_RUN(t_validate_codes);
    REG_RUN(t_every_defined_type_is_valid_and_named);
    REG_RUN(t_wire_layout_is_big_endian);
    REG_RUN(t_encode_refusals);
    REG_RUN(t_encode_exact_capacity_and_empty_payload);
    REG_RUN(t_decode_refusals);
    REG_RUN(t_every_truncation_is_refused);
    REG_RUN(t_oversized_and_hostile_lengths);
    REG_RUN(t_bad_version_and_type_refused_on_the_wire);
    REG_RUN(t_trailing_bytes_are_not_consumed);
    REG_RUN(t_round_trip_every_type_and_size);
    REG_RUN(t_maximum_payload_round_trips);
    REG_RUN(t_random_input_never_escapes_the_buffer);
    REG_RUN(t_single_byte_corruption_is_contained);
    return reg_finish("http6");
}

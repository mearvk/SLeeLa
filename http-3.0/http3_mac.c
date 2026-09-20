/* ==========================================================================
 * http3_mac.c -- SipHash-2-4 keyed MAC (reference implementation).
 *
 * SipHash-2-4 as specified by Aumasson & Bernstein: c=2 compression rounds per
 * message block, d=4 finalization rounds. The key and message bytes are read
 * little-endian, which makes the tag identical across machine endianness and
 * across the C and Python references.
 * ========================================================================== */
#include "http3_mac.h"

#include <string.h>

static uint64_t rotl64(uint64_t x, int b)
{
    return (x << b) | (x >> (64 - b));
}

static uint64_t load_u64_le(const uint8_t *p)
{
    return ((uint64_t)p[0])       | ((uint64_t)p[1] << 8)  |
           ((uint64_t)p[2] << 16) | ((uint64_t)p[3] << 24) |
           ((uint64_t)p[4] << 32) | ((uint64_t)p[5] << 40) |
           ((uint64_t)p[6] << 48) | ((uint64_t)p[7] << 56);
}

#define SIPROUND()                              \
    do {                                        \
        v0 += v1; v1 = rotl64(v1, 13); v1 ^= v0; v0 = rotl64(v0, 32); \
        v2 += v3; v3 = rotl64(v3, 16); v3 ^= v2;                      \
        v0 += v3; v3 = rotl64(v3, 21); v3 ^= v0;                      \
        v2 += v1; v1 = rotl64(v1, 17); v1 ^= v2; v2 = rotl64(v2, 32); \
    } while (0)

uint64_t http3_mac_siphash24(const uint8_t key[HTTP3_MAC_KEY_BYTES],
                             const void *data, size_t len)
{
    const uint8_t *in = (const uint8_t *)data;
    uint64_t k0 = load_u64_le(key);
    uint64_t k1 = load_u64_le(key + 8);
    uint64_t v0 = 0x736f6d6570736575ULL ^ k0;
    uint64_t v1 = 0x646f72616e646f6dULL ^ k1;
    uint64_t v2 = 0x6c7967656e657261ULL ^ k0;
    uint64_t v3 = 0x7465646279746573ULL ^ k1;
    uint64_t b;
    uint64_t m;
    const uint8_t *end;
    size_t left = len & 7u;
    int i;

    end = in + (len - left);
    for (; in != end; in += 8) {
        m = load_u64_le(in);
        v3 ^= m;
        SIPROUND();
        SIPROUND();
        v0 ^= m;
    }

    b = (uint64_t)len << 56;
    switch (left) {
        case 7: b |= (uint64_t)in[6] << 48; /* fall through */
        case 6: b |= (uint64_t)in[5] << 40; /* fall through */
        case 5: b |= (uint64_t)in[4] << 32; /* fall through */
        case 4: b |= (uint64_t)in[3] << 24; /* fall through */
        case 3: b |= (uint64_t)in[2] << 16; /* fall through */
        case 2: b |= (uint64_t)in[1] << 8;  /* fall through */
        case 1: b |= (uint64_t)in[0];       /* fall through */
        case 0: break;
        default: break;
    }

    v3 ^= b;
    SIPROUND();
    SIPROUND();
    v0 ^= b;

    v2 ^= 0xff;
    for (i = 0; i < 4; ++i) {
        SIPROUND();
    }
    return v0 ^ v1 ^ v2 ^ v3;
}

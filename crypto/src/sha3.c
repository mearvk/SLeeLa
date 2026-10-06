/*
 * crypto/src/sha3.c
 * SHA3-256 / SHA3-512 (Keccak-f[1600]) — FIPS 202 reference implementation.
 *
 * SECURITY NOTE: correct reference code, not hardened/constant-time. See
 * crypto/include/slcrypto.h.
 */
#include "slcrypto.h"
#include <string.h>

static uint64_t rol64(uint64_t x, int n) { return (x << n) | (x >> (64 - n)); }

static const int RHO[24] = {
    1,3,6,10,15,21,28,36,45,55,2,14,27,41,56,8,25,43,62,18,39,61,20,44
};
static const int PI[24] = {
    10,7,11,17,18,3,5,16,8,21,24,4,15,23,19,13,12,2,20,14,22,9,6,1
};
static const uint64_t RC[24] = {
    0x0000000000000001ULL,0x0000000000008082ULL,0x800000000000808aULL,0x8000000080008000ULL,
    0x000000000000808bULL,0x0000000080000001ULL,0x8000000080008081ULL,0x8000000000008009ULL,
    0x000000000000008aULL,0x0000000000000088ULL,0x0000000080008009ULL,0x000000008000000aULL,
    0x000000008000808bULL,0x800000000000008bULL,0x8000000000008089ULL,0x8000000000008003ULL,
    0x8000000000008002ULL,0x8000000000000080ULL,0x000000000000800aULL,0x800000008000000aULL,
    0x8000000080008081ULL,0x8000000000008080ULL,0x0000000080000001ULL,0x8000000080008008ULL
};

static void keccak_f1600(uint64_t s[25]) {
    for (int round = 0; round < 24; round++) {
        uint64_t bc[5], t;
        /* theta */
        for (int i = 0; i < 5; i++) bc[i] = s[i] ^ s[i+5] ^ s[i+10] ^ s[i+15] ^ s[i+20];
        for (int i = 0; i < 5; i++) {
            t = bc[(i+4)%5] ^ rol64(bc[(i+1)%5], 1);
            for (int j = 0; j < 25; j += 5) s[j+i] ^= t;
        }
        /* rho + pi */
        t = s[1];
        for (int i = 0; i < 24; i++) {
            int j = PI[i];
            uint64_t tmp = s[j];
            s[j] = rol64(t, RHO[i]);
            t = tmp;
        }
        /* chi */
        for (int j = 0; j < 25; j += 5) {
            for (int i = 0; i < 5; i++) bc[i] = s[j+i];
            for (int i = 0; i < 5; i++) s[j+i] ^= (~bc[(i+1)%5]) & bc[(i+2)%5];
        }
        /* iota */
        s[0] ^= RC[round];
    }
}

static void sha3_init(slc_sha3_ctx *c, size_t rate) {
    memset(c->state, 0, sizeof c->state);
    memset(c->buf, 0, sizeof c->buf);
    c->rate = rate; c->absorbed = 0;
}
void slc_sha3_256_init(slc_sha3_ctx *c) { sha3_init(c, 136); } /* 1600/8 - 2*256/8 */
void slc_sha3_512_init(slc_sha3_ctx *c) { sha3_init(c, 72);  } /* 1600/8 - 2*512/8 */

static void sha3_absorb_block(slc_sha3_ctx *c) {
    for (size_t i = 0; i < c->rate / 8; i++) {
        uint64_t lane = 0;
        for (int b = 0; b < 8; b++) lane |= (uint64_t)c->buf[i*8 + b] << (8 * b);
        c->state[i] ^= lane;
    }
    keccak_f1600(c->state);
}

void slc_sha3_update(slc_sha3_ctx *c, const void *data, size_t len) {
    const uint8_t *p = (const uint8_t *)data;
    while (len) {
        size_t take = c->rate - c->absorbed;
        if (take > len) take = len;
        memcpy(c->buf + c->absorbed, p, take);
        c->absorbed += take; p += take; len -= take;
        if (c->absorbed == c->rate) { sha3_absorb_block(c); c->absorbed = 0; }
    }
}

void slc_sha3_final(slc_sha3_ctx *c, uint8_t *out) {
    /* pad10*1 with the SHA-3 domain suffix 0x06 */
    memset(c->buf + c->absorbed, 0, c->rate - c->absorbed);
    c->buf[c->absorbed] = 0x06;
    c->buf[c->rate - 1] |= 0x80;
    sha3_absorb_block(c);
    /* squeeze: output length is 1600/8 - rate/2, i.e. 32 for rate 136, 64 for 72 */
    size_t out_len = (200 - c->rate) / 2;
    for (size_t i = 0; i < out_len; i++)
        out[i] = (uint8_t)(c->state[i/8] >> (8 * (i % 8)));
}

void slc_sha3_256(const void *data, size_t len, uint8_t out[SLC_SHA3_256_DIGEST_LEN]) {
    slc_sha3_ctx c; slc_sha3_256_init(&c); slc_sha3_update(&c, data, len); slc_sha3_final(&c, out);
}
void slc_sha3_512(const void *data, size_t len, uint8_t out[SLC_SHA3_512_DIGEST_LEN]) {
    slc_sha3_ctx c; slc_sha3_512_init(&c); slc_sha3_update(&c, data, len); slc_sha3_final(&c, out);
}

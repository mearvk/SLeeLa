/*
 * crypto/src/sha2.c
 * SHA-256 / SHA-384 / SHA-512 — FIPS 180-4 reference implementation.
 *
 * SECURITY NOTE: correct reference code, not hardened/constant-time. See
 * crypto/include/slcrypto.h. Use an audited library to protect real data.
 */
#include "slcrypto.h"
#include <string.h>

/* ---- shared helpers ------------------------------------------------------ */
static uint32_t ror32(uint32_t x, int n) { return (x >> n) | (x << (32 - n)); }
static uint64_t ror64(uint64_t x, int n) { return (x >> n) | (x << (64 - n)); }

static void store_be32(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)(v >> 24); p[1] = (uint8_t)(v >> 16);
    p[2] = (uint8_t)(v >> 8);  p[3] = (uint8_t)v;
}
static void store_be64(uint8_t *p, uint64_t v) {
    for (int i = 0; i < 8; i++) p[i] = (uint8_t)(v >> (56 - 8 * i));
}
static uint32_t load_be32(const uint8_t *p) {
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
           ((uint32_t)p[2] << 8)  |  (uint32_t)p[3];
}
static uint64_t load_be64(const uint8_t *p) {
    uint64_t v = 0; for (int i = 0; i < 8; i++) v = (v << 8) | p[i]; return v;
}

/* ======================= SHA-256 ========================================= */
static const uint32_t K256[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

void slc_sha256_init(slc_sha256_ctx *c) {
    static const uint32_t iv[8] = {
        0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19
    };
    memcpy(c->h, iv, sizeof iv); c->len = 0; c->n = 0;
}

static void sha256_block(slc_sha256_ctx *c, const uint8_t *p) {
    uint32_t w[64];
    for (int i = 0; i < 16; i++) w[i] = load_be32(p + 4 * i);
    for (int i = 16; i < 64; i++) {
        uint32_t s0 = ror32(w[i-15],7) ^ ror32(w[i-15],18) ^ (w[i-15] >> 3);
        uint32_t s1 = ror32(w[i-2],17) ^ ror32(w[i-2],19) ^ (w[i-2] >> 10);
        w[i] = w[i-16] + s0 + w[i-7] + s1;
    }
    uint32_t a=c->h[0],b=c->h[1],d2=c->h[2],d=c->h[3],e=c->h[4],f=c->h[5],g=c->h[6],h=c->h[7];
    for (int i = 0; i < 64; i++) {
        uint32_t S1 = ror32(e,6) ^ ror32(e,11) ^ ror32(e,25);
        uint32_t ch = (e & f) ^ (~e & g);
        uint32_t t1 = h + S1 + ch + K256[i] + w[i];
        uint32_t S0 = ror32(a,2) ^ ror32(a,13) ^ ror32(a,22);
        uint32_t maj = (a & b) ^ (a & d2) ^ (b & d2);
        uint32_t t2 = S0 + maj;
        h=g; g=f; f=e; e=d+t1; d=d2; d2=b; b=a; a=t1+t2;
    }
    c->h[0]+=a; c->h[1]+=b; c->h[2]+=d2; c->h[3]+=d;
    c->h[4]+=e; c->h[5]+=f; c->h[6]+=g;  c->h[7]+=h;
}

void slc_sha256_update(slc_sha256_ctx *c, const void *data, size_t len) {
    const uint8_t *p = (const uint8_t *)data;
    c->len += len;
    while (len) {
        size_t take = SLC_SHA256_BLOCK - c->n;
        if (take > len) take = len;
        memcpy(c->buf + c->n, p, take);
        c->n += take; p += take; len -= take;
        if (c->n == SLC_SHA256_BLOCK) { sha256_block(c, c->buf); c->n = 0; }
    }
}

void slc_sha256_final(slc_sha256_ctx *c, uint8_t out[SLC_SHA256_DIGEST_LEN]) {
    uint64_t bits = c->len * 8;
    uint8_t pad = 0x80;
    slc_sha256_update(c, &pad, 1);
    uint8_t zero = 0;
    while (c->n != 56) slc_sha256_update(c, &zero, 1);
    uint8_t lenb[8]; store_be64(lenb, bits);
    slc_sha256_update(c, lenb, 8);
    for (int i = 0; i < 8; i++) store_be32(out + 4 * i, c->h[i]);
}

void slc_sha256(const void *data, size_t len, uint8_t out[SLC_SHA256_DIGEST_LEN]) {
    slc_sha256_ctx c; slc_sha256_init(&c); slc_sha256_update(&c, data, len); slc_sha256_final(&c, out);
}

/* ======================= SHA-512 / SHA-384 =============================== */
static const uint64_t K512[80] = {
    0x428a2f98d728ae22ULL,0x7137449123ef65cdULL,0xb5c0fbcfec4d3b2fULL,0xe9b5dba58189dbbcULL,
    0x3956c25bf348b538ULL,0x59f111f1b605d019ULL,0x923f82a4af194f9bULL,0xab1c5ed5da6d8118ULL,
    0xd807aa98a3030242ULL,0x12835b0145706fbeULL,0x243185be4ee4b28cULL,0x550c7dc3d5ffb4e2ULL,
    0x72be5d74f27b896fULL,0x80deb1fe3b1696b1ULL,0x9bdc06a725c71235ULL,0xc19bf174cf692694ULL,
    0xe49b69c19ef14ad2ULL,0xefbe4786384f25e3ULL,0x0fc19dc68b8cd5b5ULL,0x240ca1cc77ac9c65ULL,
    0x2de92c6f592b0275ULL,0x4a7484aa6ea6e483ULL,0x5cb0a9dcbd41fbd4ULL,0x76f988da831153b5ULL,
    0x983e5152ee66dfabULL,0xa831c66d2db43210ULL,0xb00327c898fb213fULL,0xbf597fc7beef0ee4ULL,
    0xc6e00bf33da88fc2ULL,0xd5a79147930aa725ULL,0x06ca6351e003826fULL,0x142929670a0e6e70ULL,
    0x27b70a8546d22ffcULL,0x2e1b21385c26c926ULL,0x4d2c6dfc5ac42aedULL,0x53380d139d95b3dfULL,
    0x650a73548baf63deULL,0x766a0abb3c77b2a8ULL,0x81c2c92e47edaee6ULL,0x92722c851482353bULL,
    0xa2bfe8a14cf10364ULL,0xa81a664bbc423001ULL,0xc24b8b70d0f89791ULL,0xc76c51a30654be30ULL,
    0xd192e819d6ef5218ULL,0xd69906245565a910ULL,0xf40e35855771202aULL,0x106aa07032bbd1b8ULL,
    0x19a4c116b8d2d0c8ULL,0x1e376c085141ab53ULL,0x2748774cdf8eeb99ULL,0x34b0bcb5e19b48a8ULL,
    0x391c0cb3c5c95a63ULL,0x4ed8aa4ae3418acbULL,0x5b9cca4f7763e373ULL,0x682e6ff3d6b2b8a3ULL,
    0x748f82ee5defb2fcULL,0x78a5636f43172f60ULL,0x84c87814a1f0ab72ULL,0x8cc702081a6439ecULL,
    0x90befffa23631e28ULL,0xa4506cebde82bde9ULL,0xbef9a3f7b2c67915ULL,0xc67178f2e372532bULL,
    0xca273eceea26619cULL,0xd186b8c721c0c207ULL,0xeada7dd6cde0eb1eULL,0xf57d4f7fee6ed178ULL,
    0x06f067aa72176fbaULL,0x0a637dc5a2c898a6ULL,0x113f9804bef90daeULL,0x1b710b35131c471bULL,
    0x28db77f523047d84ULL,0x32caab7b40c72493ULL,0x3c9ebe0a15c9bebcULL,0x431d67c49c100d4cULL,
    0x4cc5d4becb3e42b6ULL,0x597f299cfc657e2aULL,0x5fcb6fab3ad6faecULL,0x6c44198c4a475817ULL
};

void slc_sha512_init(slc_sha512_ctx *c) {
    static const uint64_t iv[8] = {
        0x6a09e667f3bcc908ULL,0xbb67ae8584caa73bULL,0x3c6ef372fe94f82bULL,0xa54ff53a5f1d36f1ULL,
        0x510e527fade682d1ULL,0x9b05688c2b3e6c1fULL,0x1f83d9abfb41bd6bULL,0x5be0cd19137e2179ULL
    };
    memcpy(c->h, iv, sizeof iv); c->len_hi = c->len_lo = 0; c->n = 0;
}
void slc_sha384_init(slc_sha512_ctx *c) {
    static const uint64_t iv[8] = {
        0xcbbb9d5dc1059ed8ULL,0x629a292a367cd507ULL,0x9159015a3070dd17ULL,0x152fecd8f70e5939ULL,
        0x67332667ffc00b31ULL,0x8eb44a8768581511ULL,0xdb0c2e0d64f98fa7ULL,0x47b5481dbefa4fa4ULL
    };
    memcpy(c->h, iv, sizeof iv); c->len_hi = c->len_lo = 0; c->n = 0;
}

static void sha512_block(slc_sha512_ctx *c, const uint8_t *p) {
    uint64_t w[80];
    for (int i = 0; i < 16; i++) w[i] = load_be64(p + 8 * i);
    for (int i = 16; i < 80; i++) {
        uint64_t s0 = ror64(w[i-15],1) ^ ror64(w[i-15],8) ^ (w[i-15] >> 7);
        uint64_t s1 = ror64(w[i-2],19) ^ ror64(w[i-2],61) ^ (w[i-2] >> 6);
        w[i] = w[i-16] + s0 + w[i-7] + s1;
    }
    uint64_t a=c->h[0],b=c->h[1],d2=c->h[2],d=c->h[3],e=c->h[4],f=c->h[5],g=c->h[6],h=c->h[7];
    for (int i = 0; i < 80; i++) {
        uint64_t S1 = ror64(e,14) ^ ror64(e,18) ^ ror64(e,41);
        uint64_t ch = (e & f) ^ (~e & g);
        uint64_t t1 = h + S1 + ch + K512[i] + w[i];
        uint64_t S0 = ror64(a,28) ^ ror64(a,34) ^ ror64(a,39);
        uint64_t maj = (a & b) ^ (a & d2) ^ (b & d2);
        uint64_t t2 = S0 + maj;
        h=g; g=f; f=e; e=d+t1; d=d2; d2=b; b=a; a=t1+t2;
    }
    c->h[0]+=a; c->h[1]+=b; c->h[2]+=d2; c->h[3]+=d;
    c->h[4]+=e; c->h[5]+=f; c->h[6]+=g;  c->h[7]+=h;
}

void slc_sha512_update(slc_sha512_ctx *c, const void *data, size_t len) {
    const uint8_t *p = (const uint8_t *)data;
    uint64_t old = c->len_lo;
    c->len_lo += len;
    if (c->len_lo < old) c->len_hi++;
    while (len) {
        size_t take = SLC_SHA512_BLOCK - c->n;
        if (take > len) take = len;
        memcpy(c->buf + c->n, p, take);
        c->n += take; p += take; len -= take;
        if (c->n == SLC_SHA512_BLOCK) { sha512_block(c, c->buf); c->n = 0; }
    }
}

void slc_sha512_final(slc_sha512_ctx *c, uint8_t *out) {
    uint64_t hi = (c->len_hi << 3) | (c->len_lo >> 61);
    uint64_t lo = c->len_lo << 3;
    uint8_t pad = 0x80;
    slc_sha512_update(c, &pad, 1);
    uint8_t zero = 0;
    while (c->n != 112) slc_sha512_update(c, &zero, 1);
    uint8_t lenb[16]; store_be64(lenb, hi); store_be64(lenb + 8, lo);
    slc_sha512_update(c, lenb, 16);
    for (int i = 0; i < 8; i++) store_be64(out + 8 * i, c->h[i]);
    /* SHA-384 caller passes a 48-byte buffer; we wrote 64 into its own 64-byte
       buffer via the public wrappers below, so truncation happens there. */
}

void slc_sha512(const void *data, size_t len, uint8_t out[SLC_SHA512_DIGEST_LEN]) {
    slc_sha512_ctx c; slc_sha512_init(&c); slc_sha512_update(&c, data, len); slc_sha512_final(&c, out);
}
void slc_sha384(const void *data, size_t len, uint8_t out[SLC_SHA384_DIGEST_LEN]) {
    slc_sha512_ctx c; uint8_t full[64];
    slc_sha384_init(&c); slc_sha512_update(&c, data, len); slc_sha512_final(&c, full);
    memcpy(out, full, SLC_SHA384_DIGEST_LEN);
}

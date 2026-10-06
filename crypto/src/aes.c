/*
 * crypto/src/aes.c
 * AES-128/192/256 — FIPS 197 reference implementation (table-free S-box).
 *
 * SECURITY NOTE: this is a correct REFERENCE implementation. It is NOT
 * constant-time (S-box/field ops leak via caches/timing) and must not be used
 * to protect real data. See crypto/include/slcrypto.h.
 */
#include "slcrypto.h"
#include <string.h>

static const uint8_t SBOX[256] = {
0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};

static uint8_t INV_SBOX[256];
static int inv_ready = 0;
static void build_inv(void) {
    if (inv_ready) return;
    for (int i = 0; i < 256; i++) INV_SBOX[SBOX[i]] = (uint8_t)i;
    inv_ready = 1;
}

static uint8_t xtime(uint8_t x) { return (uint8_t)((x << 1) ^ ((x >> 7) * 0x1b)); }
static uint8_t gmul(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1) p ^= a;
        uint8_t hi = a & 0x80;
        a <<= 1;
        if (hi) a ^= 0x1b;
        b >>= 1;
    }
    return p;
}

static const uint8_t RCON[11] = {0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36};

slcrypto_status slc_aes_set_encrypt_key(slc_aes_key *k, const uint8_t *key, int key_bits) {
    int nk;
    if (key_bits == 128) { nk = 4; k->rounds = 10; }
    else if (key_bits == 192) { nk = 6; k->rounds = 12; }
    else if (key_bits == 256) { nk = 8; k->rounds = 14; }
    else return SLCRYPTO_ERR_PARAM;
    int total = 4 * (k->rounds + 1);
    uint32_t *rk = k->rk;
    for (int i = 0; i < nk; i++)
        rk[i] = ((uint32_t)key[4*i]<<24)|((uint32_t)key[4*i+1]<<16)|((uint32_t)key[4*i+2]<<8)|key[4*i+3];
    for (int i = nk; i < total; i++) {
        uint32_t t = rk[i-1];
        if (i % nk == 0) {
            t = (t << 8) | (t >> 24); /* RotWord */
            t = ((uint32_t)SBOX[(t>>24)&0xff]<<24)|((uint32_t)SBOX[(t>>16)&0xff]<<16)|
                ((uint32_t)SBOX[(t>>8)&0xff]<<8)|SBOX[t&0xff];
            t ^= (uint32_t)RCON[i/nk] << 24;
        } else if (nk > 6 && i % nk == 4) {
            t = ((uint32_t)SBOX[(t>>24)&0xff]<<24)|((uint32_t)SBOX[(t>>16)&0xff]<<16)|
                ((uint32_t)SBOX[(t>>8)&0xff]<<8)|SBOX[t&0xff];
        }
        rk[i] = rk[i-nk] ^ t;
    }
    return SLCRYPTO_OK;
}

slcrypto_status slc_aes_set_decrypt_key(slc_aes_key *k, const uint8_t *key, int key_bits) {
    build_inv();
    return slc_aes_set_encrypt_key(k, key, key_bits); /* same schedule; inverse applied in decrypt */
}

static void add_round_key(uint8_t s[16], const uint32_t *rk) {
    for (int c = 0; c < 4; c++) {
        s[4*c+0] ^= (uint8_t)(rk[c] >> 24);
        s[4*c+1] ^= (uint8_t)(rk[c] >> 16);
        s[4*c+2] ^= (uint8_t)(rk[c] >> 8);
        s[4*c+3] ^= (uint8_t)(rk[c]);
    }
}

void slc_aes_encrypt_block(const slc_aes_key *k, const uint8_t in[16], uint8_t out[16]) {
    uint8_t s[16]; memcpy(s, in, 16);
    add_round_key(s, k->rk);
    for (int r = 1; r < k->rounds; r++) {
        for (int i = 0; i < 16; i++) s[i] = SBOX[s[i]];                 /* SubBytes */
        uint8_t t[16];                                                  /* ShiftRows */
        for (int c = 0; c < 4; c++) for (int row = 0; row < 4; row++)
            t[4*c+row] = s[4*((c+row)%4)+row];
        for (int c = 0; c < 4; c++) {                                   /* MixColumns */
            uint8_t a0=t[4*c],a1=t[4*c+1],a2=t[4*c+2],a3=t[4*c+3];
            s[4*c+0]=(uint8_t)(xtime(a0)^(xtime(a1)^a1)^a2^a3);
            s[4*c+1]=(uint8_t)(a0^xtime(a1)^(xtime(a2)^a2)^a3);
            s[4*c+2]=(uint8_t)(a0^a1^xtime(a2)^(xtime(a3)^a3));
            s[4*c+3]=(uint8_t)((xtime(a0)^a0)^a1^a2^xtime(a3));
        }
        add_round_key(s, k->rk + 4*r);
    }
    for (int i = 0; i < 16; i++) s[i] = SBOX[s[i]];
    uint8_t t[16];
    for (int c = 0; c < 4; c++) for (int row = 0; row < 4; row++)
        t[4*c+row] = s[4*((c+row)%4)+row];
    memcpy(s, t, 16);
    add_round_key(s, k->rk + 4*k->rounds);
    memcpy(out, s, 16);
}

void slc_aes_decrypt_block(const slc_aes_key *k, const uint8_t in[16], uint8_t out[16]) {
    build_inv();
    uint8_t s[16]; memcpy(s, in, 16);
    add_round_key(s, k->rk + 4*k->rounds);
    for (int r = k->rounds - 1; r >= 1; r--) {
        uint8_t t[16];                                                  /* InvShiftRows */
        for (int c = 0; c < 4; c++) for (int row = 0; row < 4; row++)
            t[4*c+row] = s[4*((c-row+4)%4)+row];
        for (int i = 0; i < 16; i++) s[i] = INV_SBOX[t[i]];             /* InvSubBytes */
        add_round_key(s, k->rk + 4*r);
        for (int c = 0; c < 4; c++) {                                   /* InvMixColumns */
            uint8_t a0=s[4*c],a1=s[4*c+1],a2=s[4*c+2],a3=s[4*c+3];
            s[4*c+0]=(uint8_t)(gmul(a0,14)^gmul(a1,11)^gmul(a2,13)^gmul(a3,9));
            s[4*c+1]=(uint8_t)(gmul(a0,9)^gmul(a1,14)^gmul(a2,11)^gmul(a3,13));
            s[4*c+2]=(uint8_t)(gmul(a0,13)^gmul(a1,9)^gmul(a2,14)^gmul(a3,11));
            s[4*c+3]=(uint8_t)(gmul(a0,11)^gmul(a1,13)^gmul(a2,9)^gmul(a3,14));
        }
    }
    uint8_t t[16];
    for (int c = 0; c < 4; c++) for (int row = 0; row < 4; row++)
        t[4*c+row] = s[4*((c-row+4)%4)+row];
    for (int i = 0; i < 16; i++) s[i] = INV_SBOX[t[i]];
    add_round_key(s, k->rk);
    memcpy(out, s, 16);
}

/* ============================ GCM (SP 800-38D) =========================== */

static void ghash_mul(uint8_t x[16], const uint8_t h[16]) {
    uint8_t z[16] = {0}, v[16];
    memcpy(v, h, 16);
    for (int i = 0; i < 128; i++) {
        if ((x[i/8] >> (7 - (i%8))) & 1)
            for (int j = 0; j < 16; j++) z[j] ^= v[j];
        int lsb = v[15] & 1;
        for (int j = 15; j > 0; j--) v[j] = (uint8_t)((v[j] >> 1) | (v[j-1] << 7));
        v[0] >>= 1;
        if (lsb) v[0] ^= 0xe1;
    }
    memcpy(x, z, 16);
}

static void gcm_inc32(uint8_t ctr[16]) {
    for (int i = 15; i >= 12; i--) { if (++ctr[i]) break; }
}

static void gcm_core(const slc_aes_key *k, const uint8_t h[16],
                     const uint8_t *iv, size_t iv_len,
                     const uint8_t *aad, size_t aad_len,
                     const uint8_t *in, size_t in_len, uint8_t *out,
                     int encrypt, uint8_t j0[16], uint8_t s[16]) {
    /* Compute J0 */
    if (iv_len == 12) {
        memcpy(j0, iv, 12); j0[12]=0; j0[13]=0; j0[14]=0; j0[15]=1;
    } else {
        uint8_t y[16] = {0};
        size_t p = 0;
        while (p < iv_len) {
            size_t take = iv_len - p; if (take > 16) take = 16;
            for (size_t i = 0; i < take; i++) y[i] ^= iv[p+i];
            if (take == 16) { ghash_mul(y, h); }
            else { ghash_mul(y, h); }
            p += take;
        }
        uint8_t lenblk[16] = {0};
        uint64_t bits = (uint64_t)iv_len * 8;
        for (int i = 0; i < 8; i++) lenblk[15-i] = (uint8_t)(bits >> (8*i));
        for (int i = 0; i < 16; i++) y[i] ^= lenblk[i];
        ghash_mul(y, h);
        memcpy(j0, y, 16);
    }

    /* GHASH over AAD */
    memset(s, 0, 16);
    size_t p = 0;
    while (p < aad_len) {
        size_t take = aad_len - p; if (take > 16) take = 16;
        for (size_t i = 0; i < take; i++) s[i] ^= aad[p+i];
        ghash_mul(s, h);
        p += take;
    }

    /* CTR encrypt/decrypt starting at inc32(J0), GHASH over ciphertext */
    uint8_t ctr[16]; memcpy(ctr, j0, 16); gcm_inc32(ctr);
    p = 0;
    while (p < in_len) {
        uint8_t ks[16];
        slc_aes_encrypt_block(k, ctr, ks);
        gcm_inc32(ctr);
        size_t take = in_len - p; if (take > 16) take = 16;
        for (size_t i = 0; i < take; i++) out[p+i] = in[p+i] ^ ks[i];
        /* GHASH uses ciphertext in both directions */
        const uint8_t *ctblk = encrypt ? &out[p] : &in[p];
        for (size_t i = 0; i < take; i++) s[i] ^= ctblk[i];
        ghash_mul(s, h);
        p += take;
    }

    /* length block */
    uint8_t lenblk[16] = {0};
    uint64_t abits = (uint64_t)aad_len * 8, cbits = (uint64_t)in_len * 8;
    for (int i = 0; i < 8; i++) lenblk[7-i]  = (uint8_t)(abits >> (8*i));
    for (int i = 0; i < 8; i++) lenblk[15-i] = (uint8_t)(cbits >> (8*i));
    for (int i = 0; i < 16; i++) s[i] ^= lenblk[i];
    ghash_mul(s, h);
}

slcrypto_status slc_aes_gcm_encrypt(
    const uint8_t *key, int key_bits,
    const uint8_t *iv, size_t iv_len,
    const uint8_t *aad, size_t aad_len,
    const uint8_t *pt, size_t pt_len,
    uint8_t *ct_out, uint8_t *tag_out, size_t tag_len)
{
    if (!key || tag_len == 0 || tag_len > 16) return SLCRYPTO_ERR_PARAM;
    slc_aes_key k;
    if (slc_aes_set_encrypt_key(&k, key, key_bits) != SLCRYPTO_OK) return SLCRYPTO_ERR_PARAM;
    uint8_t h[16] = {0}, zero[16] = {0};
    slc_aes_encrypt_block(&k, zero, h);
    uint8_t j0[16], s[16];
    gcm_core(&k, h, iv, iv_len, aad, aad_len, pt, pt_len, ct_out, 1, j0, s);
    uint8_t ej0[16]; slc_aes_encrypt_block(&k, j0, ej0);
    for (size_t i = 0; i < tag_len; i++) tag_out[i] = s[i] ^ ej0[i];
    return SLCRYPTO_OK;
}

slcrypto_status slc_aes_gcm_decrypt(
    const uint8_t *key, int key_bits,
    const uint8_t *iv, size_t iv_len,
    const uint8_t *aad, size_t aad_len,
    const uint8_t *ct, size_t ct_len,
    const uint8_t *tag, size_t tag_len,
    uint8_t *pt_out)
{
    if (!key || tag_len == 0 || tag_len > 16) return SLCRYPTO_ERR_PARAM;
    slc_aes_key k;
    if (slc_aes_set_encrypt_key(&k, key, key_bits) != SLCRYPTO_OK) return SLCRYPTO_ERR_PARAM;
    uint8_t h[16] = {0}, zero[16] = {0};
    slc_aes_encrypt_block(&k, zero, h);
    uint8_t j0[16], s[16];
    gcm_core(&k, h, iv, iv_len, aad, aad_len, ct, ct_len, pt_out, 0, j0, s);
    uint8_t ej0[16]; slc_aes_encrypt_block(&k, j0, ej0);
    uint8_t expect[16];
    for (size_t i = 0; i < tag_len; i++) expect[i] = s[i] ^ ej0[i];
    if (!slc_ct_equal(expect, tag, tag_len)) {
        /* scrub released plaintext on auth failure */
        for (size_t i = 0; i < ct_len; i++) pt_out[i] = 0;
        return SLCRYPTO_ERR_AUTH;
    }
    return SLCRYPTO_OK;
}

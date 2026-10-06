/*
 * crypto/src/mac_kdf.c
 * HMAC (FIPS 198-1 / RFC 2104) over SHA-256 and SHA-512, and
 * HKDF (RFC 5869) over HMAC-SHA-256.
 *
 * SECURITY NOTE: correct reference code; see crypto/include/slcrypto.h.
 */
#include "slcrypto.h"
#include <string.h>

int slc_ct_equal(const void *a, const void *b, size_t len) {
    const uint8_t *x = (const uint8_t *)a, *y = (const uint8_t *)b;
    uint8_t d = 0;
    for (size_t i = 0; i < len; i++) d |= (uint8_t)(x[i] ^ y[i]);
    return d == 0;
}

void slc_hmac_sha256(const uint8_t *key, size_t key_len,
                     const uint8_t *msg, size_t msg_len,
                     uint8_t out[SLC_SHA256_DIGEST_LEN]) {
    uint8_t k0[SLC_SHA256_BLOCK], ipad[SLC_SHA256_BLOCK], opad[SLC_SHA256_BLOCK];
    memset(k0, 0, sizeof k0);
    if (key_len > SLC_SHA256_BLOCK) slc_sha256(key, key_len, k0);
    else memcpy(k0, key, key_len);
    for (int i = 0; i < SLC_SHA256_BLOCK; i++) { ipad[i] = k0[i] ^ 0x36; opad[i] = k0[i] ^ 0x5c; }

    slc_sha256_ctx c; uint8_t inner[SLC_SHA256_DIGEST_LEN];
    slc_sha256_init(&c);
    slc_sha256_update(&c, ipad, SLC_SHA256_BLOCK);
    slc_sha256_update(&c, msg, msg_len);
    slc_sha256_final(&c, inner);

    slc_sha256_init(&c);
    slc_sha256_update(&c, opad, SLC_SHA256_BLOCK);
    slc_sha256_update(&c, inner, SLC_SHA256_DIGEST_LEN);
    slc_sha256_final(&c, out);
}

void slc_hmac_sha512(const uint8_t *key, size_t key_len,
                     const uint8_t *msg, size_t msg_len,
                     uint8_t out[SLC_SHA512_DIGEST_LEN]) {
    uint8_t k0[SLC_SHA512_BLOCK], ipad[SLC_SHA512_BLOCK], opad[SLC_SHA512_BLOCK];
    memset(k0, 0, sizeof k0);
    if (key_len > SLC_SHA512_BLOCK) slc_sha512(key, key_len, k0);
    else memcpy(k0, key, key_len);
    for (int i = 0; i < SLC_SHA512_BLOCK; i++) { ipad[i] = k0[i] ^ 0x36; opad[i] = k0[i] ^ 0x5c; }

    slc_sha512_ctx c; uint8_t inner[SLC_SHA512_DIGEST_LEN];
    slc_sha512_init(&c);
    slc_sha512_update(&c, ipad, SLC_SHA512_BLOCK);
    slc_sha512_update(&c, msg, msg_len);
    slc_sha512_final(&c, inner);

    slc_sha512_init(&c);
    slc_sha512_update(&c, opad, SLC_SHA512_BLOCK);
    slc_sha512_update(&c, inner, SLC_SHA512_DIGEST_LEN);
    slc_sha512_final(&c, out);
}

void slc_hkdf_sha256(const uint8_t *salt, size_t salt_len,
                     const uint8_t *ikm, size_t ikm_len,
                     const uint8_t *info, size_t info_len,
                     uint8_t *okm, size_t okm_len) {
    /* Extract: PRK = HMAC(salt, IKM); empty salt => block of zeros */
    uint8_t prk[SLC_SHA256_DIGEST_LEN];
    uint8_t zero_salt[SLC_SHA256_DIGEST_LEN];
    if (salt == 0 || salt_len == 0) {
        memset(zero_salt, 0, sizeof zero_salt);
        slc_hmac_sha256(zero_salt, sizeof zero_salt, ikm, ikm_len, prk);
    } else {
        slc_hmac_sha256(salt, salt_len, ikm, ikm_len, prk);
    }

    /* Expand */
    uint8_t t[SLC_SHA256_DIGEST_LEN];
    size_t t_len = 0, done = 0;
    uint8_t counter = 1;
    while (done < okm_len) {
        slc_sha256_ctx hc; (void)hc;
        /* T(i) = HMAC(PRK, T(i-1) | info | i) — build the message then HMAC it */
        uint8_t msg[SLC_SHA256_DIGEST_LEN + 1024 + 1];
        size_t off = 0;
        if (t_len) { memcpy(msg + off, t, t_len); off += t_len; }
        if (info_len) {
            size_t cap = sizeof(msg) - off - 1;
            size_t use = info_len < cap ? info_len : cap;
            memcpy(msg + off, info, use); off += use;
        }
        msg[off++] = counter;
        slc_hmac_sha256(prk, sizeof prk, msg, off, t);
        t_len = SLC_SHA256_DIGEST_LEN;
        size_t take = okm_len - done; if (take > t_len) take = t_len;
        memcpy(okm + done, t, take);
        done += take; counter++;
    }
}

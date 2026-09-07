// ===========================================================================
// stp_crypto.c -- STP-0001 crypto via OpenSSL 3 libcrypto.
// Built only when STP_HAVE_OPENSSL is defined.
// ===========================================================================
#include "stp_crypto.h"

#include <stdlib.h>
#include <string.h>

#include <openssl/evp.h>
#include <openssl/kdf.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>
#include <openssl/core_names.h>
#include <openssl/params.h>

struct stp_key { EVP_PKEY* pkey; };

static stp_key* wrap(EVP_PKEY* p) {
    if (!p) return NULL;
    stp_key* k = (stp_key*) calloc(1, sizeof(stp_key));
    if (!k) { EVP_PKEY_free(p); return NULL; }
    k->pkey = p;
    return k;
}

stp_key* stp_ed25519_generate(void) { return wrap(EVP_PKEY_Q_keygen(NULL, NULL, "ED25519")); }
stp_key* stp_x25519_generate(void)  { return wrap(EVP_PKEY_Q_keygen(NULL, NULL, "X25519")); }

void stp_key_free(stp_key* k) { if (k) { EVP_PKEY_free(k->pkey); free(k); } }

int stp_key_raw_public(const stp_key* k, uint8_t out32[32]) {
    size_t len = 32;
    return EVP_PKEY_get_raw_public_key(k->pkey, out32, &len) == 1 && len == 32 ? 0 : -1;
}

stp_key* stp_ed25519_from_raw_public(const uint8_t raw32[32]) {
    return wrap(EVP_PKEY_new_raw_public_key(EVP_PKEY_ED25519, NULL, raw32, 32));
}
stp_key* stp_x25519_from_raw_public(const uint8_t raw32[32]) {
    return wrap(EVP_PKEY_new_raw_public_key(EVP_PKEY_X25519, NULL, raw32, 32));
}

int stp_ed25519_sign(const stp_key* priv, const uint8_t* msg, size_t mlen, uint8_t out_sig[64]) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) return -1;
    int rc = -1;
    size_t siglen = 64;
    if (EVP_DigestSignInit(ctx, NULL, NULL, NULL, priv->pkey) == 1 &&
        EVP_DigestSign(ctx, out_sig, &siglen, msg, mlen) == 1 && siglen == 64) {
        rc = 0;
    }
    EVP_MD_CTX_free(ctx);
    return rc;
}

int stp_ed25519_verify(const stp_key* pub, const uint8_t* msg, size_t mlen, const uint8_t sig[64]) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) return 0;
    int ok = 0;
    if (EVP_DigestVerifyInit(ctx, NULL, NULL, NULL, pub->pkey) == 1) {
        ok = (EVP_DigestVerify(ctx, sig, 64, msg, mlen) == 1);
    }
    EVP_MD_CTX_free(ctx);
    return ok;
}

int stp_x25519_agree(const stp_key* my_priv, const stp_key* their_pub, uint8_t out_ss[32]) {
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(my_priv->pkey, NULL);
    if (!ctx) return -1;
    int rc = -1;
    size_t sl = 32;
    if (EVP_PKEY_derive_init(ctx) == 1 &&
        EVP_PKEY_derive_set_peer(ctx, their_pub->pkey) == 1 &&
        EVP_PKEY_derive(ctx, out_ss, &sl) == 1 && sl == 32) {
        rc = 0;
    }
    EVP_PKEY_CTX_free(ctx);
    return rc;
}

int stp_hkdf(const uint8_t* ikm, size_t ikm_len,
             const uint8_t* salt, size_t salt_len,
             const uint8_t* info, size_t info_len,
             uint8_t* out, size_t out_len) {
    EVP_KDF* kdf = EVP_KDF_fetch(NULL, "HKDF", NULL);
    if (!kdf) return -1;
    EVP_KDF_CTX* ctx = EVP_KDF_CTX_new(kdf);
    EVP_KDF_free(kdf);
    if (!ctx) return -1;
    OSSL_PARAM params[5]; int n = 0;
    params[n++] = OSSL_PARAM_construct_utf8_string(OSSL_KDF_PARAM_DIGEST, (char*) "SHA256", 0);
    params[n++] = OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_KEY, (void*) ikm, ikm_len);
    if (salt && salt_len)
        params[n++] = OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_SALT, (void*) salt, salt_len);
    if (info && info_len)
        params[n++] = OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_INFO, (void*) info, info_len);
    params[n] = OSSL_PARAM_construct_end();
    int rc = EVP_KDF_derive(ctx, out, out_len, params) == 1 ? 0 : -1;
    EVP_KDF_CTX_free(ctx);
    return rc;
}

int stp_hmac_sha256(const uint8_t* key, size_t key_len,
                    const uint8_t* msg, size_t msg_len, uint8_t out32[32]) {
    unsigned int ol = 32;
    return HMAC(EVP_sha256(), key, (int) key_len, msg, msg_len, out32, &ol) && ol == 32 ? 0 : -1;
}

int stp_aead_seal(const uint8_t key32[32], const uint8_t nonce12[12],
                  const uint8_t* aad, size_t aad_len,
                  const uint8_t* pt, size_t pt_len, uint8_t* out, size_t* out_len) {
    EVP_CIPHER_CTX* c = EVP_CIPHER_CTX_new();
    if (!c) return -1;
    int rc = -1, l = 0;
    uint8_t tag[16];
    if (EVP_EncryptInit_ex(c, EVP_chacha20_poly1305(), NULL, key32, nonce12) == 1) {
        int outl = 0;
        if (aad && aad_len) EVP_EncryptUpdate(c, NULL, &l, aad, (int) aad_len);
        if (EVP_EncryptUpdate(c, out, &outl, pt, (int) pt_len) == 1) {
            int fl = 0;
            if (EVP_EncryptFinal_ex(c, out + outl, &fl) == 1 &&
                EVP_CIPHER_CTX_ctrl(c, EVP_CTRL_AEAD_GET_TAG, 16, tag) == 1) {
                memcpy(out + outl + fl, tag, 16);
                *out_len = (size_t)(outl + fl) + 16;
                rc = 0;
            }
        }
    }
    EVP_CIPHER_CTX_free(c);
    return rc;
}

int stp_aead_open(const uint8_t key32[32], const uint8_t nonce12[12],
                  const uint8_t* aad, size_t aad_len,
                  const uint8_t* ct, size_t ct_len, uint8_t* out, size_t* out_len) {
    if (ct_len < 16) return -1;
    size_t body = ct_len - 16;
    EVP_CIPHER_CTX* c = EVP_CIPHER_CTX_new();
    if (!c) return -1;
    int rc = -1, l = 0;
    uint8_t tag[16];
    memcpy(tag, ct + body, 16);
    if (EVP_DecryptInit_ex(c, EVP_chacha20_poly1305(), NULL, key32, nonce12) == 1) {
        int outl = 0;
        if (aad && aad_len) EVP_DecryptUpdate(c, NULL, &l, aad, (int) aad_len);
        if (EVP_DecryptUpdate(c, out, &outl, ct, (int) body) == 1 &&
            EVP_CIPHER_CTX_ctrl(c, EVP_CTRL_AEAD_SET_TAG, 16, tag) == 1) {
            int fl = 0;
            if (EVP_DecryptFinal_ex(c, out + outl, &fl) == 1) {  // verifies tag
                *out_len = (size_t)(outl + fl);
                rc = 0;
            }
        }
    }
    EVP_CIPHER_CTX_free(c);
    return rc;
}

int stp_sha256(const uint8_t* data, size_t len, uint8_t out32[32]) {
    unsigned int ol = 32;
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) return -1;
    int rc = -1;
    if (EVP_DigestInit_ex(ctx, EVP_sha256(), NULL) == 1 &&
        EVP_DigestUpdate(ctx, data, len) == 1 &&
        EVP_DigestFinal_ex(ctx, out32, &ol) == 1) rc = 0;
    EVP_MD_CTX_free(ctx);
    return rc;
}

int stp_random(uint8_t* out, size_t n) { return RAND_bytes(out, (int) n) == 1 ? 0 : -1; }

void stp_hex(const uint8_t* in, size_t n, char* out) {
    static const char* h = "0123456789abcdef";
    for (size_t i = 0; i < n; i++) { out[2*i] = h[in[i] >> 4]; out[2*i+1] = h[in[i] & 0xf]; }
    out[2*n] = '\0';
}

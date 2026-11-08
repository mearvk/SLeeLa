/*
 * kex.c - DH / ECDH key agreement + HKDF session key (see kex.h).
 *
 * Uses OpenSSL 3.x EVP_PKEY APIs. For X25519 we use raw public keys; for
 * P-256 and FFDHE we use SubjectPublicKeyInfo DER so the group parameters
 * travel with the key.
 */
#include "kex.h"

#include <stdlib.h>
#include <string.h>

#include <openssl/evp.h>
#include <openssl/kdf.h>
#include <openssl/core_names.h>
#include <openssl/param_build.h>
#include <openssl/x509.h>

struct kex_keypair {
    kex_group group;
    EVP_PKEY *pkey;
};

static int is_raw(kex_group g) { return g == KEX_X25519; }

static const char *group_sn(kex_group g) {
    switch (g) {
        case KEX_P256:      return "prime256v1";
        case KEX_FFDHE2048: return "ffdhe2048";
        case KEX_FFDHE3072: return "ffdhe3072";
        default:            return NULL;
    }
}

const char *kex_group_name(kex_group g) {
    switch (g) {
        case KEX_X25519:    return "X25519";
        case KEX_P256:      return "P-256 (prime256v1)";
        case KEX_FFDHE2048: return "FFDHE2048";
        case KEX_FFDHE3072: return "FFDHE3072";
    }
    return "unknown";
}

kex_status kex_generate(kex_group group, kex_keypair **out) {
    if (!out) return KEX_ERR_ARG;
    *out = NULL;

    EVP_PKEY_CTX *ctx = NULL;
    EVP_PKEY *pkey = NULL;
    kex_status rc = KEX_ERR_CRYPTO;

    if (group == KEX_X25519) {
        ctx = EVP_PKEY_CTX_new_from_name(NULL, "X25519", NULL);
        if (!ctx || EVP_PKEY_keygen_init(ctx) != 1) goto done;
        if (EVP_PKEY_keygen(ctx, &pkey) != 1) goto done;
    } else if (group == KEX_P256) {
        ctx = EVP_PKEY_CTX_new_from_name(NULL, "EC", NULL);
        if (!ctx || EVP_PKEY_keygen_init(ctx) != 1) goto done;
        if (EVP_PKEY_CTX_set_group_name(ctx, "prime256v1") != 1) goto done;
        if (EVP_PKEY_keygen(ctx, &pkey) != 1) goto done;
    } else if (group == KEX_FFDHE2048 || group == KEX_FFDHE3072) {
        ctx = EVP_PKEY_CTX_new_from_name(NULL, "DH", NULL);
        if (!ctx || EVP_PKEY_keygen_init(ctx) != 1) goto done;
        OSSL_PARAM params[2];
        params[0] = OSSL_PARAM_construct_utf8_string(
            OSSL_PKEY_PARAM_GROUP_NAME, (char *)group_sn(group), 0);
        params[1] = OSSL_PARAM_construct_end();
        if (EVP_PKEY_CTX_set_params(ctx, params) != 1) goto done;
        if (EVP_PKEY_keygen(ctx, &pkey) != 1) goto done;
    } else {
        rc = KEX_ERR_ARG;
        goto done;
    }

    kex_keypair *kp = calloc(1, sizeof(*kp));
    if (!kp) { rc = KEX_ERR_CRYPTO; goto done; }
    kp->group = group;
    kp->pkey = pkey;
    pkey = NULL;
    *out = kp;
    rc = KEX_OK;

done:
    if (pkey) EVP_PKEY_free(pkey);
    if (ctx) EVP_PKEY_CTX_free(ctx);
    return rc;
}

kex_status kex_export_public(const kex_keypair *kp, uint8_t *pub, size_t *len) {
    if (!kp || !len) return KEX_ERR_ARG;

    if (is_raw(kp->group)) {
        size_t need = 0;
        if (EVP_PKEY_get_raw_public_key(kp->pkey, NULL, &need) != 1)
            return KEX_ERR_CRYPTO;
        if (!pub || *len < need) { *len = need; return KEX_ERR_BUFFER; }
        if (EVP_PKEY_get_raw_public_key(kp->pkey, pub, len) != 1)
            return KEX_ERR_CRYPTO;
        return KEX_OK;
    }

    /* SubjectPublicKeyInfo DER. */
    unsigned char *der = NULL;
    int n = i2d_PUBKEY(kp->pkey, &der);
    if (n <= 0) return KEX_ERR_CRYPTO;
    if (!pub || *len < (size_t)n) { *len = (size_t)n; OPENSSL_free(der); return KEX_ERR_BUFFER; }
    memcpy(pub, der, (size_t)n);
    *len = (size_t)n;
    OPENSSL_free(der);
    return KEX_OK;
}

static EVP_PKEY *import_peer(kex_group group, const uint8_t *pub, size_t len) {
    if (is_raw(group)) {
        return EVP_PKEY_new_raw_public_key_ex(NULL, "X25519", NULL, pub, len);
    }
    const unsigned char *p = pub;
    return d2i_PUBKEY(NULL, &p, (long)len);
}

static kex_status hkdf_sha256(const uint8_t *ikm, size_t ikm_len,
                              const char *info,
                              uint8_t *out, size_t out_len) {
    EVP_KDF *kdf = EVP_KDF_fetch(NULL, "HKDF", NULL);
    if (!kdf) return KEX_ERR_CRYPTO;
    EVP_KDF_CTX *kctx = EVP_KDF_CTX_new(kdf);
    EVP_KDF_free(kdf);
    if (!kctx) return KEX_ERR_CRYPTO;

    OSSL_PARAM params[4];
    size_t i = 0;
    params[i++] = OSSL_PARAM_construct_utf8_string(OSSL_KDF_PARAM_DIGEST, (char *)"SHA256", 0);
    params[i++] = OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_KEY, (void *)ikm, ikm_len);
    if (info)
        params[i++] = OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_INFO, (void *)info, strlen(info));
    params[i] = OSSL_PARAM_construct_end();

    kex_status rc = (EVP_KDF_derive(kctx, out, out_len, params) == 1) ? KEX_OK : KEX_ERR_CRYPTO;
    EVP_KDF_CTX_free(kctx);
    return rc;
}

kex_status kex_derive(const kex_keypair *kp,
                      const uint8_t *peer_pub, size_t peer_pub_len,
                      const char *info,
                      uint8_t *out_key, size_t key_len) {
    if (!kp || !peer_pub || !out_key || key_len == 0) return KEX_ERR_ARG;

    EVP_PKEY *peer = import_peer(kp->group, peer_pub, peer_pub_len);
    if (!peer) return KEX_ERR_CRYPTO;

    EVP_PKEY_CTX *dctx = EVP_PKEY_CTX_new(kp->pkey, NULL);
    kex_status rc = KEX_ERR_CRYPTO;
    uint8_t *secret = NULL;
    size_t secret_len = 0;

    if (!dctx) goto done;
    if (EVP_PKEY_derive_init(dctx) != 1) goto done;
    if (EVP_PKEY_derive_set_peer(dctx, peer) != 1) goto done;
    if (EVP_PKEY_derive(dctx, NULL, &secret_len) != 1) goto done;
    secret = OPENSSL_malloc(secret_len);
    if (!secret) goto done;
    if (EVP_PKEY_derive(dctx, secret, &secret_len) != 1) goto done;

    rc = hkdf_sha256(secret, secret_len, info, out_key, key_len);

done:
    if (secret) OPENSSL_clear_free(secret, secret_len);
    if (dctx) EVP_PKEY_CTX_free(dctx);
    if (peer) EVP_PKEY_free(peer);
    return rc;
}

void kex_free(kex_keypair *kp) {
    if (!kp) return;
    if (kp->pkey) EVP_PKEY_free(kp->pkey);
    free(kp);
}

const char *kex_strerror(kex_status st) {
    switch (st) {
        case KEX_OK:         return "ok";
        case KEX_ERR_ARG:    return "invalid argument";
        case KEX_ERR_CRYPTO: return "cryptographic operation failed";
        case KEX_ERR_BUFFER: return "output buffer too small";
    }
    return "unknown error";
}

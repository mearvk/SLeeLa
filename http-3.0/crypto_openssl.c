#include "crypto_openssl.h"

#include <openssl/evp.h>
#include <openssl/kdf.h>
#include <openssl/rand.h>
#include <openssl/core_names.h>
#include <openssl/params.h>
#include <limits.h>
#include <string.h>

static int get_raw_public(EVP_PKEY *key, uint8_t out[32])
{
    size_t len = 32U;
    return EVP_PKEY_get_raw_public_key(key, out, &len) == 1 && len == 32U ? 0 : -1;
}

static int get_raw_private(EVP_PKEY *key, uint8_t out[32])
{
    size_t len = 32U;
    return EVP_PKEY_get_raw_private_key(key, out, &len) == 1 && len == 32U ? 0 : -1;
}

int http3_openssl_x25519_generate(uint8_t private_key[32], uint8_t public_key[32])
{
    EVP_PKEY *key = NULL;
    int rc = -1;

    if (private_key == NULL || public_key == NULL) {
        return -1;
    }

    key = EVP_PKEY_Q_keygen(NULL, NULL, "X25519");
    if (key != NULL && get_raw_private(key, private_key) == 0 &&
        get_raw_public(key, public_key) == 0) {
        rc = 0;
    }
    EVP_PKEY_free(key);
    return rc;
}

int http3_openssl_x25519_derive(const uint8_t private_key[32],
                                const uint8_t peer_public_key[32],
                                uint8_t shared_secret[32])
{
    EVP_PKEY *local = NULL;
    EVP_PKEY *peer = NULL;
    EVP_PKEY_CTX *ctx = NULL;
    size_t secret_len = 32U;
    int rc = -1;

    if (private_key == NULL || peer_public_key == NULL || shared_secret == NULL) {
        return -1;
    }

    local = EVP_PKEY_new_raw_private_key(EVP_PKEY_X25519, NULL, private_key, 32U);
    peer = EVP_PKEY_new_raw_public_key(EVP_PKEY_X25519, NULL, peer_public_key, 32U);
    if (local == NULL || peer == NULL) {
        goto done;
    }

    ctx = EVP_PKEY_CTX_new(local, NULL);
    if (ctx == NULL || EVP_PKEY_derive_init(ctx) != 1 ||
        EVP_PKEY_derive_set_peer(ctx, peer) != 1 ||
        EVP_PKEY_derive(ctx, shared_secret, &secret_len) != 1 ||
        secret_len != 32U) {
        goto done;
    }
    rc = 0;

done:
    if (rc != 0) {
        memset(shared_secret, 0, 32U);
    }
    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(peer);
    EVP_PKEY_free(local);
    return rc;
}

int http3_openssl_hkdf_sha256(const uint8_t *secret, size_t secret_len,
                              const uint8_t *salt, size_t salt_len,
                              const uint8_t *info, size_t info_len,
                              uint8_t output[32])
{
    EVP_KDF *kdf = NULL;
    EVP_KDF_CTX *ctx = NULL;
    OSSL_PARAM params[5];
    size_t n = 0U;
    int rc = -1;

    if (secret == NULL || output == NULL ||
        (salt == NULL && salt_len != 0U) ||
        (info == NULL && info_len != 0U) ||
        secret_len > INT_MAX || salt_len > INT_MAX || info_len > INT_MAX) {
        return -1;
    }

    kdf = EVP_KDF_fetch(NULL, "HKDF", NULL);
    ctx = kdf == NULL ? NULL : EVP_KDF_CTX_new(kdf);
    if (ctx == NULL) {
        goto done;
    }

    params[n++] = OSSL_PARAM_construct_utf8_string(OSSL_KDF_PARAM_DIGEST,
                                                    "SHA256", 0);
    params[n++] = OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_KEY,
                                                     (void *)secret, secret_len);
    if (salt_len != 0U) {
        params[n++] = OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_SALT,
                                                         (void *)salt, salt_len);
    }
    if (info_len != 0U) {
        params[n++] = OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_INFO,
                                                         (void *)info, info_len);
    }
    params[n] = OSSL_PARAM_construct_end();

    if (EVP_KDF_derive(ctx, output, 32U, params) == 1) {
        rc = 0;
    }

done:
    if (rc != 0) {
        memset(output, 0, 32U);
    }
    EVP_KDF_CTX_free(ctx);
    EVP_KDF_free(kdf);
    return rc;
}

int http3_openssl_random(uint8_t *output, size_t output_len)
{
    if (output == NULL || output_len == 0U || output_len > INT_MAX) {
        return -1;
    }
    return RAND_bytes(output, (int)output_len) == 1 ? 0 : -1;
}

int http3_openssl_aes256gcm_encrypt(const uint8_t key[32], const uint8_t nonce[12],
                                    const uint8_t *aad, size_t aad_len,
                                    const uint8_t *plaintext, size_t plaintext_len,
                                    uint8_t *ciphertext, uint8_t tag[16])
{
    EVP_CIPHER_CTX *ctx = NULL;
    int out_len = 0;
    int final_len = 0;
    int rc = -1;

    if (key == NULL || nonce == NULL || plaintext == NULL || ciphertext == NULL ||
        tag == NULL || aad_len > INT_MAX || plaintext_len > INT_MAX) {
        return -1;
    }

    ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL || EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL) != 1 ||
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 12, NULL) != 1 ||
        EVP_EncryptInit_ex(ctx, NULL, NULL, key, nonce) != 1) {
        goto done;
    }
    if (aad_len != 0U &&
        EVP_EncryptUpdate(ctx, NULL, &out_len, aad, (int)aad_len) != 1) {
        goto done;
    }
    if (EVP_EncryptUpdate(ctx, ciphertext, &out_len, plaintext, (int)plaintext_len) != 1) {
        goto done;
    }
    if (EVP_EncryptFinal_ex(ctx, ciphertext + out_len, &final_len) != 1 ||
        (size_t)(out_len + final_len) != plaintext_len ||
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag) != 1) {
        goto done;
    }
    rc = 0;

done:
    EVP_CIPHER_CTX_free(ctx);
    return rc;
}

int http3_openssl_aes256gcm_decrypt(const uint8_t key[32], const uint8_t nonce[12],
                                    const uint8_t *aad, size_t aad_len,
                                    const uint8_t *ciphertext, size_t ciphertext_len,
                                    const uint8_t tag[16], uint8_t *plaintext)
{
    EVP_CIPHER_CTX *ctx = NULL;
    int out_len = 0;
    int final_len = 0;
    int rc = -1;

    if (key == NULL || nonce == NULL || ciphertext == NULL || tag == NULL ||
        plaintext == NULL || aad_len > INT_MAX || ciphertext_len > INT_MAX) {
        return -1;
    }

    ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL || EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL) != 1 ||
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 12, NULL) != 1 ||
        EVP_DecryptInit_ex(ctx, NULL, NULL, key, nonce) != 1) {
        goto done;
    }
    if (aad_len != 0U &&
        EVP_DecryptUpdate(ctx, NULL, &out_len, aad, (int)aad_len) != 1) {
        goto done;
    }
    if (EVP_DecryptUpdate(ctx, plaintext, &out_len, ciphertext, (int)ciphertext_len) != 1 ||
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, (void *)tag) != 1 ||
        EVP_DecryptFinal_ex(ctx, plaintext + out_len, &final_len) != 1 ||
        (size_t)(out_len + final_len) != ciphertext_len) {
        memset(plaintext, 0, ciphertext_len);
        goto done;
    }
    rc = 0;

done:
    EVP_CIPHER_CTX_free(ctx);
    return rc;
}

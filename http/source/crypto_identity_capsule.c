#include "crypto_identity_capsule.h"
#include "crypto_openssl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int is_forbidden_sensitive_name(const char *name)
{
    static const char *const forbidden[] = {
        "national_id", "national_id_number", "phone_number",
        "telephone_number", "dna_id", "biometric_id", "raw_dna", NULL
    };
    size_t i;

    if (name == NULL) {
        return 1;
    }
    for (i = 0U; forbidden[i] != NULL; ++i) {
        if (strcmp(name, forbidden[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int http3_cic_validate_metadata(const http3_cic_metadata_t *metadata,
                                uint64_t current_time)
{
    if (metadata == NULL || metadata->expiration <= current_time ||
        metadata->jurisdiction_id[0] == '\0' ||
        metadata->sender_reference[0] == '\0' ||
        metadata->emergency_endpoint_reference[0] == '\0') {
        return -1;
    }
    return 0;
}

int http3_cic_create_id(uint8_t capsule_id[HTTP3_CIC_ID_SIZE])
{
    return http3_openssl_random(capsule_id, HTTP3_CIC_ID_SIZE);
}

int http3_cic_reject_sensitive_attribute_name(const char *name)
{
    return is_forbidden_sensitive_name(name) ? -1 : 0;
}

int http3_cic_bind_context(const uint8_t capsule_id[HTTP3_CIC_ID_SIZE],
                          const char *jurisdiction_id,
                          const uint8_t *context,
                          size_t context_len,
                          uint8_t *output,
                          size_t output_capacity,
                          size_t *output_len)
{
    size_t jurisdiction_len;

    if (capsule_id == NULL || jurisdiction_id == NULL ||
        (context == NULL && context_len != 0U) || output == NULL ||
        output_len == NULL) {
        return -1;
    }

    jurisdiction_len = strlen(jurisdiction_id);
    if (jurisdiction_len >= HTTP3_CIC_MAX_JURISDICTION ||
        HTTP3_CIC_ID_SIZE + jurisdiction_len + context_len > output_capacity) {
        return -1;
    }

    memcpy(output, capsule_id, HTTP3_CIC_ID_SIZE);
    memcpy(output + HTTP3_CIC_ID_SIZE, jurisdiction_id, jurisdiction_len);
    if (context_len != 0U) {
        memcpy(output + HTTP3_CIC_ID_SIZE + jurisdiction_len, context, context_len);
    }
    *output_len = HTTP3_CIC_ID_SIZE + jurisdiction_len + context_len;
    return 0;
}

static int make_aad(const http3_cic_capsule_t *capsule,
                    uint8_t aad[HTTP3_CIC_ID_SIZE + HTTP3_CIC_MAX_JURISDICTION],
                    size_t *aad_len)
{
    size_t jurisdiction_len;

    if (capsule == NULL || aad == NULL || aad_len == NULL) {
        return -1;
    }
    jurisdiction_len = strlen(capsule->jurisdiction_id);
    if (jurisdiction_len >= HTTP3_CIC_MAX_JURISDICTION) {
        return -1;
    }
    memcpy(aad, capsule->capsule_id, HTTP3_CIC_ID_SIZE);
    memcpy(aad + HTTP3_CIC_ID_SIZE, capsule->jurisdiction_id, jurisdiction_len);
    *aad_len = HTTP3_CIC_ID_SIZE + jurisdiction_len;
    return 0;
}

int http3_cic_encrypt(const http3_cic_metadata_t *metadata,
                      const uint8_t recipient_public_key[32],
                      const uint8_t *plaintext,
                      size_t plaintext_len,
                      http3_cic_capsule_t *capsule)
{
    uint8_t ephemeral_private[32];
    uint8_t shared_secret[32];
    uint8_t key[32];
    uint8_t aad[HTTP3_CIC_ID_SIZE + HTTP3_CIC_MAX_JURISDICTION];
    size_t aad_len = 0U;
    size_t total_len;
    int rc = -1;

    memset(ephemeral_private, 0, sizeof(ephemeral_private));
    memset(shared_secret, 0, sizeof(shared_secret));
    memset(key, 0, sizeof(key));

    if (metadata == NULL || recipient_public_key == NULL || plaintext == NULL ||
        capsule == NULL || plaintext_len > SIZE_MAX - HTTP3_CIC_TAG_SIZE ||
        http3_cic_validate_metadata(metadata, 0U) != 0) {
        return -1;
    }

    memset(capsule, 0, sizeof(*capsule));
    capsule->version = HTTP3_CIC_VERSION;
    memcpy(capsule->capsule_id, metadata->capsule_id, HTTP3_CIC_ID_SIZE);
    memcpy(capsule->jurisdiction_id, metadata->jurisdiction_id,
           sizeof(capsule->jurisdiction_id));

    if (http3_openssl_x25519_generate(ephemeral_private,
                                      capsule->ephemeral_public_key) != 0 ||
        http3_openssl_x25519_derive(ephemeral_private, recipient_public_key,
                                    shared_secret) != 0 ||
        http3_openssl_hkdf_sha256(shared_secret, sizeof(shared_secret),
                                  metadata->capsule_id, HTTP3_CIC_ID_SIZE,
                                  (const uint8_t *)"SLeeLa/HTTP3/CIC/v1",
                                  sizeof("SLeeLa/HTTP3/CIC/v1") - 1U, key) != 0 ||
        http3_openssl_random(capsule->nonce, sizeof(capsule->nonce)) != 0 ||
        make_aad(capsule, aad, &aad_len) != 0) {
        goto done;
    }

    total_len = plaintext_len + HTTP3_CIC_TAG_SIZE;
    capsule->ciphertext = (uint8_t *)malloc(total_len);
    if (capsule->ciphertext == NULL) {
        goto done;
    }

    if (http3_openssl_aes256gcm_encrypt(
            key, capsule->nonce, aad, aad_len, plaintext, plaintext_len,
            capsule->ciphertext, capsule->ciphertext + plaintext_len) != 0) {
        http3_cic_free_capsule(capsule);
        goto done;
    }

    capsule->ciphertext_len = total_len;
    rc = 0;

done:
    memset(ephemeral_private, 0, sizeof(ephemeral_private));
    memset(shared_secret, 0, sizeof(shared_secret));
    memset(key, 0, sizeof(key));
    return rc;
}

int http3_cic_decrypt(const http3_cic_capsule_t *capsule,
                      const uint8_t recipient_private_key[32],
                      uint8_t *plaintext,
                      size_t plaintext_capacity,
                      size_t *plaintext_len)
{
    uint8_t shared_secret[32];
    uint8_t key[32];
    uint8_t aad[HTTP3_CIC_ID_SIZE + HTTP3_CIC_MAX_JURISDICTION];
    size_t aad_len = 0U;
    size_t data_len;
    int rc = -1;

    memset(shared_secret, 0, sizeof(shared_secret));
    memset(key, 0, sizeof(key));

    if (capsule == NULL || recipient_private_key == NULL || plaintext == NULL ||
        plaintext_len == NULL || capsule->version != HTTP3_CIC_VERSION ||
        capsule->ciphertext == NULL || capsule->ciphertext_len < HTTP3_CIC_TAG_SIZE ||
        capsule->ciphertext_len - HTTP3_CIC_TAG_SIZE > plaintext_capacity ||
        make_aad(capsule, aad, &aad_len) != 0) {
        return -1;
    }

    data_len = capsule->ciphertext_len - HTTP3_CIC_TAG_SIZE;
    if (http3_openssl_x25519_derive(recipient_private_key,
                                    capsule->ephemeral_public_key,
                                    shared_secret) != 0 ||
        http3_openssl_hkdf_sha256(shared_secret, sizeof(shared_secret),
                                  capsule->capsule_id, HTTP3_CIC_ID_SIZE,
                                  (const uint8_t *)"SLeeLa/HTTP3/CIC/v1",
                                  sizeof("SLeeLa/HTTP3/CIC/v1") - 1U, key) != 0 ||
        http3_openssl_aes256gcm_decrypt(
            key, capsule->nonce, aad, aad_len, capsule->ciphertext, data_len,
            capsule->ciphertext + data_len, plaintext) != 0) {
        memset(plaintext, 0, plaintext_capacity);
        goto done;
    }

    *plaintext_len = data_len;
    rc = 0;

done:
    memset(shared_secret, 0, sizeof(shared_secret));
    memset(key, 0, sizeof(key));
    return rc;
}

void http3_cic_free_capsule(http3_cic_capsule_t *capsule)
{
    if (capsule == NULL) {
        return;
    }
    if (capsule->ciphertext != NULL) {
        memset(capsule->ciphertext, 0, capsule->ciphertext_len);
        free(capsule->ciphertext);
    }
    memset(capsule, 0, sizeof(*capsule));
}

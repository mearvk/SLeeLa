#ifndef SLEELA_HTTP3_CRYPTO_IDENTITY_CAPSULE_H
#define SLEELA_HTTP3_CRYPTO_IDENTITY_CAPSULE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP3_CIC_VERSION 1U
#define HTTP3_CIC_ID_SIZE 16U
#define HTTP3_CIC_KEY_SIZE 32U
#define HTTP3_CIC_NONCE_SIZE 12U
#define HTTP3_CIC_TAG_SIZE 16U
#define HTTP3_CIC_MAX_REFERENCE 256U
#define HTTP3_CIC_MAX_ENDPOINT 256U
#define HTTP3_CIC_MAX_JURISDICTION 16U

typedef struct {
    uint8_t capsule_id[HTTP3_CIC_ID_SIZE];
    char jurisdiction_id[HTTP3_CIC_MAX_JURISDICTION];
    uint64_t expiration;
    char emergency_endpoint_reference[HTTP3_CIC_MAX_ENDPOINT];
    char sender_reference[HTTP3_CIC_MAX_REFERENCE];
    char authorized_contact_reference[HTTP3_CIC_MAX_REFERENCE];
    char identity_registry_reference[HTTP3_CIC_MAX_REFERENCE];
    char flow_context[HTTP3_CIC_MAX_REFERENCE];
} http3_cic_metadata_t;

typedef struct {
    uint8_t version;
    uint8_t capsule_id[HTTP3_CIC_ID_SIZE];
    char jurisdiction_id[HTTP3_CIC_MAX_JURISDICTION];
    uint8_t ephemeral_public_key[32];
    uint8_t nonce[HTTP3_CIC_NONCE_SIZE];
    uint8_t *ciphertext;
    size_t ciphertext_len;
} http3_cic_capsule_t;

int http3_cic_validate_metadata(const http3_cic_metadata_t *metadata,
                                uint64_t current_time);
int http3_cic_create_id(uint8_t capsule_id[HTTP3_CIC_ID_SIZE]);
int http3_cic_reject_sensitive_attribute_name(const char *name);
int http3_cic_bind_context(const uint8_t capsule_id[HTTP3_CIC_ID_SIZE],
                          const char *jurisdiction_id,
                          const uint8_t *context,
                          size_t context_len,
                          uint8_t *output,
                          size_t output_capacity,
                          size_t *output_len);

/* Actual provider-backed CIC construction and opening. The ciphertext buffer
 * contains ciphertext followed by a 16-byte AES-GCM authentication tag. */
int http3_cic_encrypt(const http3_cic_metadata_t *metadata,
                      const uint8_t recipient_public_key[32],
                      const uint8_t *plaintext,
                      size_t plaintext_len,
                      http3_cic_capsule_t *capsule);

int http3_cic_decrypt(const http3_cic_capsule_t *capsule,
                      const uint8_t recipient_private_key[32],
                      uint8_t *plaintext,
                      size_t plaintext_capacity,
                      size_t *plaintext_len);

void http3_cic_free_capsule(http3_cic_capsule_t *capsule);

#ifdef __cplusplus
}
#endif

#endif

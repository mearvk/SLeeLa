#include "crypto_identity_capsule.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int is_forbidden_sensitive_name(const char *name)
{
    static const char *const forbidden[] = {
        "national_id",
        "national_id_number",
        "phone_number",
        "telephone_number",
        "dna_id",
        "biometric_id",
        "raw_dna",
        NULL
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
    if (metadata == NULL || metadata->expiration <= current_time) {
        return -1;
    }
    if (metadata->jurisdiction_id[0] == '\0' ||
        metadata->sender_reference[0] == '\0' ||
        metadata->emergency_endpoint_reference[0] == '\0') {
        return -1;
    }
    return 0;
}

int http3_cic_create_id(uint8_t capsule_id[HTTP3_CIC_ID_SIZE])
{
    FILE *random_file;
    size_t read_count;

    if (capsule_id == NULL) {
        return -1;
    }

    random_file = fopen("/dev/urandom", "rb");
    if (random_file == NULL) {
        return -1;
    }
    read_count = fread(capsule_id, 1U, HTTP3_CIC_ID_SIZE, random_file);
    fclose(random_file);
    return (read_count == HTTP3_CIC_ID_SIZE) ? 0 : -1;
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
    if (jurisdiction_len > HTTP3_CIC_MAX_JURISDICTION - 1U ||
        HTTP3_CIC_ID_SIZE + jurisdiction_len + context_len > output_capacity) {
        return -1;
    }

    /*
     * This is a serialization/context helper, not a hash or MAC. The caller
     * must feed the returned context into an audited AEAD implementation.
     */
    memcpy(output, capsule_id, HTTP3_CIC_ID_SIZE);
    memcpy(output + HTTP3_CIC_ID_SIZE, jurisdiction_id, jurisdiction_len);
    if (context_len != 0U) {
        memcpy(output + HTTP3_CIC_ID_SIZE + jurisdiction_len, context, context_len);
    }
    *output_len = HTTP3_CIC_ID_SIZE + jurisdiction_len + context_len;
    return 0;
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

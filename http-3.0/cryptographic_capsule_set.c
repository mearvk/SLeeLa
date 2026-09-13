#include "cryptographic_capsule_set.h"

#include <string.h>

int http3_capsule_set_add(http3_capsule_set_t *set,
                          const http3_jurisdiction_recipient_t *recipient)
{
    size_t i;

    if (set == NULL || recipient == NULL || recipient->jurisdiction_id[0] == '\0') {
        return -1;
    }
    if (set->recipient_count >= HTTP3_CAPSULE_MAX_JURISDICTIONS) {
        return -1;
    }

    for (i = 0U; i < set->recipient_count; ++i) {
        if (strcmp(set->recipients[i].jurisdiction_id,
                   recipient->jurisdiction_id) == 0) {
            return -1;
        }
    }

    set->recipients[set->recipient_count++] = *recipient;
    return 0;
}

int http3_capsule_set_has(const http3_capsule_set_t *set,
                          const char *jurisdiction_id)
{
    size_t index;
    return http3_capsule_set_authorized_index(set, jurisdiction_id, &index);
}

int http3_capsule_set_authorized_index(const http3_capsule_set_t *set,
                                       const char *jurisdiction_id,
                                       size_t *index)
{
    size_t i;

    if (set == NULL || jurisdiction_id == NULL || index == NULL) {
        return -1;
    }

    for (i = 0U; i < set->recipient_count; ++i) {
        if (strcmp(set->recipients[i].jurisdiction_id, jurisdiction_id) == 0) {
            *index = i;
            return 0;
        }
    }
    return -1;
}

int http3_capsule_set_encrypt(const http3_capsule_set_t *set,
                              const http3_cic_metadata_t *metadata_template,
                              const uint8_t *plaintext,
                              size_t plaintext_len,
                              http3_cic_capsule_t *capsules,
                              size_t capsule_capacity,
                              size_t *capsule_count)
{
    size_t i;

    if (set == NULL || metadata_template == NULL || plaintext == NULL ||
        capsules == NULL || capsule_count == NULL ||
        capsule_capacity < set->recipient_count) {
        return -1;
    }

    *capsule_count = 0U;
    memset(capsules, 0, capsule_capacity * sizeof(*capsules));

    for (i = 0U; i < set->recipient_count; ++i) {
        http3_cic_metadata_t metadata = *metadata_template;
        memcpy(metadata.jurisdiction_id, set->recipients[i].jurisdiction_id,
               sizeof(metadata.jurisdiction_id));
        memcpy(metadata.emergency_endpoint_reference,
               set->recipients[i].emergency_endpoint_reference,
               sizeof(metadata.emergency_endpoint_reference));

        if (http3_cic_encrypt(&metadata, set->recipients[i].public_key,
                              plaintext, plaintext_len, &capsules[i]) != 0) {
            http3_capsule_set_free_capsules(capsules, i);
            return -1;
        }
        ++(*capsule_count);
    }

    return 0;
}

void http3_capsule_set_free_capsules(http3_cic_capsule_t *capsules,
                                     size_t capsule_count)
{
    size_t i;

    if (capsules == NULL) {
        return;
    }
    for (i = 0U; i < capsule_count; ++i) {
        http3_cic_free_capsule(&capsules[i]);
    }
}

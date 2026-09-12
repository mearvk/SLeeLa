#ifndef SLEELA_HTTP3_CRYPTOGRAPHIC_CAPSULE_SET_H
#define SLEELA_HTTP3_CRYPTOGRAPHIC_CAPSULE_SET_H

#include "crypto_identity_capsule.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP3_CAPSULE_MAX_JURISDICTIONS 64U

typedef struct {
    char jurisdiction_id[HTTP3_CIC_MAX_JURISDICTION];
    uint8_t public_key[32];
    char emergency_endpoint_reference[HTTP3_CIC_MAX_ENDPOINT];
} http3_jurisdiction_recipient_t;

typedef struct {
    http3_jurisdiction_recipient_t recipients[HTTP3_CAPSULE_MAX_JURISDICTIONS];
    size_t recipient_count;
} http3_capsule_set_t;

int http3_capsule_set_add(http3_capsule_set_t *set,
                          const http3_jurisdiction_recipient_t *recipient);

int http3_capsule_set_has(const http3_capsule_set_t *set,
                          const char *jurisdiction_id);

int http3_capsule_set_authorized_index(const http3_capsule_set_t *set,
                                       const char *jurisdiction_id,
                                       size_t *index);

#ifdef __cplusplus
}
#endif

#endif

#ifndef SLEELA_HTTP3_CRYPTO_ROUTE_SESSION_H
#define SLEELA_HTTP3_CRYPTO_ROUTE_SESSION_H

#include <stddef.h>
#include <stdint.h>

#include "crypto_openssl.h"
#include "route_navigation.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint64_t route_generation;
    uint64_t key_epoch;
    uint8_t ephemeral_private_key[HTTP3_OPENSSL_X25519_KEY_SIZE];
    uint8_t ephemeral_public_key[HTTP3_OPENSSL_X25519_KEY_SIZE];
    uint8_t route_key[HTTP3_OPENSSL_HKDF_SIZE];
    int established;
} http3_route_session_t;

int http3_route_session_establish(http3_route_session_t *session,
                                  const http3_route_plan_t *plan,
                                  const uint8_t peer_public_key[32],
                                  uint64_t key_epoch);

int http3_route_session_rekey(http3_route_session_t *session,
                              const http3_route_plan_t *old_plan,
                              const http3_route_plan_t *new_plan,
                              const uint8_t peer_public_key[32],
                              uint64_t key_epoch);

int http3_route_session_derive_country_key(const http3_route_session_t *session,
                                           uint32_t country_id,
                                           uint8_t country_key[32]);

void http3_route_session_clear(http3_route_session_t *session);

#ifdef __cplusplus
}
#endif

#endif

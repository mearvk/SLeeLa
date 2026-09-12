#include "crypto_route_session.h"

#include <stdio.h>
#include <string.h>

static int derive_route_key(http3_route_session_t *session,
                            const http3_route_plan_t *plan,
                            const uint8_t peer_public_key[32])
{
    uint8_t shared[32];
    uint8_t salt[16];
    uint8_t info[32];
    size_t info_len;
    int rc = -1;

    memset(shared, 0, sizeof(shared));
    memset(salt, 0, sizeof(salt));
    memset(info, 0, sizeof(info));

    if (session == NULL || plan == NULL || peer_public_key == NULL) {
        return -1;
    }

    memcpy(salt, &plan->route_id, sizeof(plan->route_id));
    memcpy(salt + 8U, &plan->generation, sizeof(plan->generation));

    info_len = (size_t)snprintf((char *)info, sizeof(info),
                                "SLeeLa/HTTP3/ROUTE/%llu",
                                (unsigned long long)plan->generation);
    if (info_len >= sizeof(info) ||
        http3_openssl_x25519_derive(session->ephemeral_private_key,
                                    peer_public_key, shared) != 0 ||
        http3_openssl_hkdf_sha256(shared, sizeof(shared), salt, sizeof(salt),
                                  info, info_len, session->route_key) != 0) {
        goto done;
    }

    session->route_generation = plan->generation;
    rc = 0;

done:
    memset(shared, 0, sizeof(shared));
    memset(salt, 0, sizeof(salt));
    memset(info, 0, sizeof(info));
    return rc;
}

int http3_route_session_establish(http3_route_session_t *session,
                                  const http3_route_plan_t *plan,
                                  const uint8_t peer_public_key[32],
                                  uint64_t key_epoch)
{
    if (session == NULL || plan == NULL || peer_public_key == NULL ||
        key_epoch == 0U || http3_route_plan_validate(plan) != 0) {
        return -1;
    }

    memset(session, 0, sizeof(*session));
    if (http3_openssl_x25519_generate(session->ephemeral_private_key,
                                      session->ephemeral_public_key) != 0) {
        return -1;
    }

    session->key_epoch = key_epoch;
    if (derive_route_key(session, plan, peer_public_key) != 0) {
        http3_route_session_clear(session);
        return -1;
    }
    session->established = 1;
    return 0;
}

int http3_route_session_rekey(http3_route_session_t *session,
                              const http3_route_plan_t *old_plan,
                              const http3_route_plan_t *new_plan,
                              const uint8_t peer_public_key[32],
                              uint64_t key_epoch)
{
    http3_route_nav_decision_t decision;

    if (session == NULL || old_plan == NULL || new_plan == NULL ||
        peer_public_key == NULL || key_epoch == 0U ||
        http3_route_plan_compare(old_plan, new_plan, &decision) != 0) {
        return -1;
    }

    if (decision.action == HTTP3_ROUTE_NAV_KEEP) {
        return 0;
    }

    if (!session->established || key_epoch != session->key_epoch ||
        decision.crypto_rebuild_required) {
        return http3_route_session_establish(session, new_plan,
                                             peer_public_key, key_epoch);
    }

    return derive_route_key(session, new_plan, peer_public_key);
}

int http3_route_session_derive_country_key(const http3_route_session_t *session,
                                           uint32_t country_id,
                                           uint8_t country_key[32])
{
    uint8_t salt[4];
    uint8_t info[] = "SLeeLa/HTTP3/COUNTRY";

    if (session == NULL || country_key == NULL || country_id == 0U ||
        !session->established) {
        return -1;
    }

    memcpy(salt, &country_id, sizeof(salt));
    return http3_openssl_hkdf_sha256(session->route_key, sizeof(session->route_key),
                                     salt, sizeof(salt), info, sizeof(info) - 1U,
                                     country_key);
}

void http3_route_session_clear(http3_route_session_t *session)
{
    if (session != NULL) {
        memset(session, 0, sizeof(*session));
    }
}

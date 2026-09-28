#include "crypto_key_agreement.h"

#include <string.h>

int http3_key_agreement_add_peer(http3_key_agreement_context_t *ctx,
                                 const http3_key_peer_t *peer)
{
    size_t i;

    if (ctx == NULL || peer == NULL || peer->jurisdiction_id == 0 ||
        peer->scope < HTTP3_KEY_SCOPE_FLOW ||
        peer->scope > HTTP3_KEY_SCOPE_ROUTE ||
        peer->key_epoch == 0) {
        return -1;
    }

    for (i = 0; i < ctx->peer_count; ++i) {
        if (ctx->peers[i].jurisdiction_id == peer->jurisdiction_id &&
            ctx->peers[i].network_id == peer->network_id) {
            ctx->peers[i] = *peer;
            return 0;
        }
    }

    if (ctx->peer_count >= HTTP3_KEY_AGREEMENT_MAX_PEERS) {
        return -2;
    }

    ctx->peers[ctx->peer_count++] = *peer;
    return 0;
}

int http3_key_agreement_find_peer(const http3_key_agreement_context_t *ctx,
                                  uint32_t jurisdiction_id,
                                  uint32_t network_id,
                                  http3_key_peer_t *peer_out)
{
    size_t i;

    if (ctx == NULL || peer_out == NULL || jurisdiction_id == 0) {
        return -1;
    }

    for (i = 0; i < ctx->peer_count; ++i) {
        if (ctx->peers[i].jurisdiction_id == jurisdiction_id &&
            ctx->peers[i].network_id == network_id) {
            *peer_out = ctx->peers[i];
            return 0;
        }
    }

    return -2;
}

int http3_key_agreement_should_rotate(const http3_key_agreement_context_t *ctx,
                                      uint64_t current_epoch)
{
    size_t i;

    if (ctx == NULL || current_epoch == 0) {
        return -1;
    }

    for (i = 0; i < ctx->peer_count; ++i) {
        if (ctx->peers[i].key_epoch != current_epoch) {
            return 1;
        }
    }

    return 0;
}

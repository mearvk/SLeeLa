#ifndef HTTP3_CRYPTO_KEY_AGREEMENT_H
#define HTTP3_CRYPTO_KEY_AGREEMENT_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP3_KEY_AGREEMENT_VERSION 1u
#define HTTP3_KEY_AGREEMENT_PUBLIC_KEY_SIZE 32u
#define HTTP3_KEY_AGREEMENT_SHARED_SECRET_SIZE 32u
#define HTTP3_KEY_AGREEMENT_MAX_PEERS 64u

typedef enum {
    HTTP3_KEY_SCOPE_FLOW = 1,
    HTTP3_KEY_SCOPE_COUNTRY = 2,
    HTTP3_KEY_SCOPE_ROUTE = 3
} http3_key_scope_t;

typedef struct {
    uint32_t jurisdiction_id;
    uint32_t network_id;
    http3_key_scope_t scope;
    uint64_t key_epoch;
    unsigned char public_key[HTTP3_KEY_AGREEMENT_PUBLIC_KEY_SIZE];
} http3_key_peer_t;

typedef struct {
    uint64_t session_id;
    uint64_t key_epoch;
    size_t peer_count;
    http3_key_peer_t peers[HTTP3_KEY_AGREEMENT_MAX_PEERS];
} http3_key_agreement_context_t;

/*
 * The protocol permits a modern server-assisted key distributor, but does not
 * require one.  An implementation may instead use authenticated ephemeral
 * Diffie-Hellman between peers and signed/pre-provisioned trust anchors.
 *
 * These functions define the provider boundary.  The actual DH/KEM primitive
 * must come from an audited cryptographic library; this project does not
 * implement elliptic-curve arithmetic itself.
 */
int http3_key_agreement_add_peer(http3_key_agreement_context_t *ctx,
                                 const http3_key_peer_t *peer);
int http3_key_agreement_find_peer(const http3_key_agreement_context_t *ctx,
                                  uint32_t jurisdiction_id,
                                  uint32_t network_id,
                                  http3_key_peer_t *peer_out);
int http3_key_agreement_should_rotate(const http3_key_agreement_context_t *ctx,
                                      uint64_t current_epoch);

#ifdef __cplusplus
}
#endif

#endif

#ifndef SLEELA_HTTP3_KEY_DISTRIBUTION_SERVER_H
#define SLEELA_HTTP3_KEY_DISTRIBUTION_SERVER_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP3_KDS_MAX_KEYS_PER_MINUTE 10000U
#define HTTP3_KDS_KEY_SIZE 32U
#define HTTP3_KDS_CONTRACT_VERSION 1U
#define HTTP3_KDS_MAX_JURISDICTION 64U
#define HTTP3_KDS_MAX_CLIENT_ID 128U
#define HTTP3_KDS_MAX_CONTRACT 4096U

/* One-minute issuance window. The server never issues more than this count. */
typedef struct http3_kds_rate_limiter {
    uint64_t window_start;
    uint32_t issued;
} http3_kds_rate_limiter_t;

typedef struct http3_kds_server {
    uint8_t server_private[HTTP3_KDS_KEY_SIZE];
    uint8_t server_public[HTTP3_KDS_KEY_SIZE];
    char key_file[256];
    http3_kds_rate_limiter_t rate;
    void *lock; /* pthread_mutex_t, kept opaque to C callers. */
} http3_kds_server_t;

typedef struct http3_kds_contract {
    uint32_t version;
    uint64_t issued_at;
    uint64_t expires_at;
    uint8_t server_public[HTTP3_KDS_KEY_SIZE];
    uint8_t client_public[HTTP3_KDS_KEY_SIZE];
    uint8_t key_id[16];
    char jurisdiction[HTTP3_KDS_MAX_JURISDICTION];
    char client_id[HTTP3_KDS_MAX_CLIENT_ID];
    uint8_t ciphertext[HTTP3_KDS_MAX_CONTRACT];
    size_t ciphertext_len;
} http3_kds_contract_t;

/* Initializes or loads the server's long-lived X25519 bootstrap key. */
int http3_kds_server_init(http3_kds_server_t *server,
                          const char *key_file);

/* Securely clears server state. */
void http3_kds_server_clear(http3_kds_server_t *server);

/* Returns 1 if a new key may be issued, 0 when the 10,000/minute cap is hit. */
int http3_kds_allow_issue(http3_kds_server_t *server, uint64_t now);

/*
 * Creates an exact bootstrap contract using X25519/Diffie-Hellman.
 * client_public is the client's ephemeral X25519 public key.
 * The plaintext contract is encrypted to the resulting DH-derived key.
 */
int http3_kds_issue_contract(http3_kds_server_t *server,
                             const uint8_t client_public[HTTP3_KDS_KEY_SIZE],
                             const char *client_id,
                             const char *jurisdiction,
                             uint64_t now,
                             uint64_t ttl_seconds,
                             http3_kds_contract_t *contract);

/* Serializes the wire-safe contract as a compact JSON document. */
int http3_kds_contract_json(const http3_kds_contract_t *contract,
                            char *output,
                            size_t output_size);

#ifdef __cplusplus
}
#endif

#endif

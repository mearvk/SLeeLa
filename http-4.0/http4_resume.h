#ifndef SLEELA_HTTP4_RESUME_H
#define SLEELA_HTTP4_RESUME_H
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP4_RESUME_TRANSFER_ID_MAX 64u
#define HTTP4_RESUME_PRINCIPAL_MAX 128u
#define HTTP4_RESUME_VERSION_MAX 128u
#define HTTP4_RESUME_DIGEST_MAX 64u

typedef struct {
    char transfer_id[HTTP4_RESUME_TRANSFER_ID_MAX];
    char principal_id[HTTP4_RESUME_PRINCIPAL_MAX];
    char file_version[HTTP4_RESUME_VERSION_MAX];
    uint64_t total_size;
    uint64_t committed_offset;
    uint64_t expires_at_unix;
    uint8_t expected_digest[HTTP4_RESUME_DIGEST_MAX];
    size_t digest_size;
    int active;
    int committed;
} http4_resume_state_t;

/* Callbacks must perform real cryptographic verification in production. */
typedef int (*http4_resume_token_verifier)(const uint8_t *token, size_t token_len,
    const http4_resume_state_t *state, uint64_t now_unix, void *user);
typedef int (*http4_resume_chunk_verifier)(uint64_t offset, const uint8_t *data,
    size_t data_len, const uint8_t *digest, size_t digest_len, void *user);
typedef int (*http4_resume_final_verifier)(const http4_resume_state_t *state,
    const uint8_t *final_digest, size_t digest_len, void *user);

int http4_resume_state_validate(const http4_resume_state_t *state);
int http4_resume_authorize(const http4_resume_state_t *state,
    const char *principal_id, const char *file_version, uint64_t total_size,
    uint64_t offset, const uint8_t *token, size_t token_len, uint64_t now_unix,
    http4_resume_token_verifier verify_token, void *user);
int http4_resume_accept_chunk(http4_resume_state_t *state, uint64_t offset,
    const uint8_t *data, size_t data_len, const uint8_t *chunk_digest,
    size_t digest_len, http4_resume_chunk_verifier verify_chunk, void *user);
int http4_resume_commit(http4_resume_state_t *state, const uint8_t *final_digest,
    size_t digest_len, http4_resume_final_verifier verify_final, void *user);

#ifdef __cplusplus
}
#endif
#endif

#include "http4_resume.h"
#include <string.h>

static int bounded_string(const char *s, size_t cap) {
    return s && memchr(s, '\0', cap) != NULL && s[0] != '\0';
}

int http4_resume_state_validate(const http4_resume_state_t *s) {
    if (!s || !bounded_string(s->transfer_id, sizeof(s->transfer_id)) ||
        !bounded_string(s->principal_id, sizeof(s->principal_id)) ||
        !bounded_string(s->file_version, sizeof(s->file_version)) ||
        s->committed_offset > s->total_size ||
        s->digest_size == 0 || s->digest_size > HTTP4_RESUME_DIGEST_MAX ||
        s->committed > 1 || s->active > 1 || (s->active && s->committed))
        return -1;
    return 0;
}

int http4_resume_authorize(const http4_resume_state_t *s,
    const char *principal, const char *version, uint64_t total, uint64_t offset,
    const uint8_t *token, size_t token_len, uint64_t now,
    http4_resume_token_verifier verify, void *user) {
    if (http4_resume_state_validate(s) != 0 || !s->active || s->committed ||
        !principal || !version || !token || token_len == 0 || !verify)
        return -1;
    if (now >= s->expires_at_unix) return -2;
    if (strcmp(principal, s->principal_id) != 0 ||
        strcmp(version, s->file_version) != 0 || total != s->total_size ||
        offset != s->committed_offset) return -3;
    return verify(token, token_len, s, now, user) == 1 ? 0 : -4;
}

int http4_resume_accept_chunk(http4_resume_state_t *s, uint64_t offset,
    const uint8_t *data, size_t len, const uint8_t *digest, size_t digest_len,
    http4_resume_chunk_verifier verify, void *user) {
    if (http4_resume_state_validate(s) != 0 || !s->active || s->committed ||
        !verify || !digest || digest_len == 0 || (len && !data) || len == 0)
        return -1;
    if (offset != s->committed_offset || offset > s->total_size ||
        (uint64_t)len > s->total_size - offset) return -2;
    if (verify(offset, data, len, digest, digest_len, user) != 1) return -3;
    s->committed_offset += (uint64_t)len;
    return 0;
}

int http4_resume_commit(http4_resume_state_t *s, const uint8_t *digest,
    size_t digest_len, http4_resume_final_verifier verify, void *user) {
    if (http4_resume_state_validate(s) != 0 || !s->active || s->committed ||
        !digest || digest_len == 0 || !verify) return -1;
    if (s->committed_offset != s->total_size) return -2;
    if (verify(s, digest, digest_len, user) != 1) return -3;
    s->active = 0;
    s->committed = 1;
    return 0;
}

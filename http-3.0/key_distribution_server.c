#define _POSIX_C_SOURCE 200809L
#include "key_distribution_server.h"
#include "crypto_openssl.h"

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

static pthread_mutex_t *kds_mutex(http3_kds_server_t *server) {
    return (pthread_mutex_t *)server->lock;
}

static void secure_zero(void *p, size_t n) {
    volatile unsigned char *v = (volatile unsigned char *)p;
    while (n--) *v++ = 0;
}

static int write_key_file(const char *path, const uint8_t priv[32],
                          const uint8_t pub[32]) {
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    uint8_t record[64];
    ssize_t n;
    if (fd < 0) return 0;
    memcpy(record, priv, 32);
    memcpy(record + 32, pub, 32);
    n = write(fd, record, sizeof(record));
    secure_zero(record, sizeof(record));
    if (close(fd) != 0 || n != (ssize_t)sizeof(record)) return 0;
    return 1;
}

static int read_key_file(const char *path, uint8_t priv[32], uint8_t pub[32]) {
    int fd = open(path, O_RDONLY);
    uint8_t record[64];
    ssize_t n;
    if (fd < 0) return 0;
    n = read(fd, record, sizeof(record));
    close(fd);
    if (n != (ssize_t)sizeof(record)) {
        secure_zero(record, sizeof(record));
        return 0;
    }
    memcpy(priv, record, 32);
    memcpy(pub, record + 32, 32);
    secure_zero(record, sizeof(record));
    return 1;
}

static int valid_text(const char *s, size_t max) {
    size_t i, n;
    if (!s) return 0;
    n = strnlen(s, max + 1U);
    if (n == 0 || n > max) return 0;
    for (i = 0; i < n; ++i) {
        unsigned char c = (unsigned char)s[i];
        if (c < 0x20 || c == '"' || c == '\\') return 0;
    }
    return 1;
}

int http3_kds_server_init(http3_kds_server_t *server, const char *key_file) {
    pthread_mutex_t *mutex;
    if (!server || !key_file || strlen(key_file) >= sizeof(server->key_file)) return 0;
    memset(server, 0, sizeof(*server));
    mutex = (pthread_mutex_t *)calloc(1, sizeof(*mutex));
    if (!mutex) return 0;
    if (pthread_mutex_init(mutex, NULL) != 0) {
        free(mutex);
        return 0;
    }
    server->lock = mutex;
    strncpy(server->key_file, key_file, sizeof(server->key_file) - 1U);
    if (!read_key_file(key_file, server->server_private, server->server_public)) {
        if (!http3_openssl_x25519_generate(server->server_private,
                                           server->server_public) ||
            !write_key_file(key_file, server->server_private, server->server_public)) {
            http3_kds_server_clear(server);
            return 0;
        }
    }
    server->rate.window_start = 0;
    server->rate.issued = 0;
    return 1;
}

void http3_kds_server_clear(http3_kds_server_t *server) {
    pthread_mutex_t *mutex;
    if (!server) return;
    mutex = kds_mutex(server);
    if (mutex) {
        pthread_mutex_destroy(mutex);
        free(mutex);
    }
    secure_zero(server->server_private, sizeof(server->server_private));
    secure_zero(server->server_public, sizeof(server->server_public));
    secure_zero(server, sizeof(*server));
}

int http3_kds_allow_issue(http3_kds_server_t *server, uint64_t now) {
    int allowed = 0;
    pthread_mutex_t *mutex;
    if (!server || !server->lock) return 0;
    mutex = kds_mutex(server);
    pthread_mutex_lock(mutex);
    if (server->rate.window_start == 0 || now - server->rate.window_start >= 60U) {
        server->rate.window_start = now;
        server->rate.issued = 0;
    }
    if (server->rate.issued < HTTP3_KDS_MAX_KEYS_PER_MINUTE) {
        ++server->rate.issued;
        allowed = 1;
    }
    pthread_mutex_unlock(mutex);
    return allowed;
}

static int make_plaintext(const http3_kds_contract_t *contract,
                          char *out, size_t out_size) {
    int n;
    n = snprintf(out, out_size,
                 "SLeeLa-HTTP3-BOOTSTRAP-v1\\n"
                 "key_id=%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\\n"
                 "client_id=%s\\n"
                 "jurisdiction=%s\\n"
                 "issued_at=%llu\\n"
                 "expires_at=%llu\\n",
                 contract->key_id[0], contract->key_id[1], contract->key_id[2], contract->key_id[3],
                 contract->key_id[4], contract->key_id[5], contract->key_id[6], contract->key_id[7],
                 contract->key_id[8], contract->key_id[9], contract->key_id[10], contract->key_id[11],
                 contract->key_id[12], contract->key_id[13], contract->key_id[14], contract->key_id[15],
                 contract->client_id, contract->jurisdiction,
                 (unsigned long long)contract->issued_at,
                 (unsigned long long)contract->expires_at);
    return n > 0 && (size_t)n < out_size ? n : 0;
}

int http3_kds_issue_contract(http3_kds_server_t *server,
                             const uint8_t client_public[32],
                             const char *client_id,
                             const char *jurisdiction,
                             uint64_t now,
                             uint64_t ttl_seconds,
                             http3_kds_contract_t *contract) {
    uint8_t shared[32], key[32], nonce[12], tag[16];
    char plaintext[1024];
    int plain_len;
    if (!server || !client_public || !contract || !server->lock) return 0;
    if (!valid_text(client_id, HTTP3_KDS_MAX_CLIENT_ID - 1U) ||
        !valid_text(jurisdiction, HTTP3_KDS_MAX_JURISDICTION - 1U)) return 0;
    if (!http3_kds_allow_issue(server, now)) return 0;
    memset(contract, 0, sizeof(*contract));
    contract->version = HTTP3_KDS_CONTRACT_VERSION;
    contract->issued_at = now;
    contract->expires_at = now + (ttl_seconds ? ttl_seconds : 3600U);
    memcpy(contract->server_public, server->server_public, 32);
    memcpy(contract->client_public, client_public, 32);
    if (!http3_openssl_random(contract->key_id, sizeof(contract->key_id)) ||
        !http3_openssl_x25519_derive(server->server_private, client_public, shared) ||
        !http3_openssl_hkdf_sha256(shared, sizeof(shared), contract->key_id,
                                   sizeof(contract->key_id),
                                   (const uint8_t *)"SLeeLa/HTTP3/KDS/bootstrap/v1", 29,
                                   key) ||
        !http3_openssl_random(nonce, sizeof(nonce))) {
        secure_zero(shared, sizeof(shared));
        secure_zero(key, sizeof(key));
        return 0;
    }
    strncpy(contract->client_id, client_id, sizeof(contract->client_id) - 1U);
    strncpy(contract->jurisdiction, jurisdiction, sizeof(contract->jurisdiction) - 1U);
    plain_len = make_plaintext(contract, plaintext, sizeof(plaintext));
    if (!plain_len || !http3_openssl_aes256gcm_encrypt(
            key, nonce, contract->key_id, sizeof(contract->key_id),
            (const uint8_t *)plaintext, (size_t)plain_len,
            contract->ciphertext + sizeof(nonce), tag)) {
        secure_zero(shared, sizeof(shared));
        secure_zero(key, sizeof(key));
        secure_zero(plaintext, sizeof(plaintext));
        return 0;
    }
    memcpy(contract->ciphertext, nonce, sizeof(nonce));
    memcpy(contract->ciphertext + sizeof(nonce) + (size_t)plain_len, tag, sizeof(tag));
    contract->ciphertext_len = sizeof(nonce) + (size_t)plain_len + sizeof(tag);
    secure_zero(shared, sizeof(shared));
    secure_zero(key, sizeof(key));
    secure_zero(nonce, sizeof(nonce));
    secure_zero(tag, sizeof(tag));
    secure_zero(plaintext, sizeof(plaintext));
    return 1;
}

int http3_kds_contract_json(const http3_kds_contract_t *contract,
                            char *output, size_t output_size) {
    size_t i;
    size_t pos = 0;
    int n;
    if (!contract || !output || output_size == 0 || contract->ciphertext_len > HTTP3_KDS_MAX_CONTRACT) return 0;
    n = snprintf(output + pos, output_size - pos,
                 "{\"version\":%u,\"issued_at\":%llu,\"expires_at\":%llu,\"server_public\":\"",
                 contract->version,
                 (unsigned long long)contract->issued_at,
                 (unsigned long long)contract->expires_at);
    if (n < 0 || (size_t)n >= output_size - pos) return 0;
    pos += (size_t)n;
    for (i = 0; i < 32; ++i) {
        n = snprintf(output + pos, output_size - pos, "%02x", contract->server_public[i]);
        if (n < 0 || (size_t)n >= output_size - pos) return 0;
        pos += (size_t)n;
    }
    n = snprintf(output + pos, output_size - pos, "\",\"client_public\":\"");
    if (n < 0 || (size_t)n >= output_size - pos) return 0;
    pos += (size_t)n;
    for (i = 0; i < 32; ++i) {
        n = snprintf(output + pos, output_size - pos, "%02x", contract->client_public[i]);
        if (n < 0 || (size_t)n >= output_size - pos) return 0;
        pos += (size_t)n;
    }
    n = snprintf(output + pos, output_size - pos, "\",\"jurisdiction\":\"%s\",\"client_id\":\"%s\",\"key_id\":\"",
                 contract->jurisdiction, contract->client_id);
    if (n < 0 || (size_t)n >= output_size - pos) return 0;
    pos += (size_t)n;
    for (i = 0; i < sizeof(contract->key_id); ++i) {
        n = snprintf(output + pos, output_size - pos, "%02x", contract->key_id[i]);
        if (n < 0 || (size_t)n >= output_size - pos) return 0;
        pos += (size_t)n;
    }
    n = snprintf(output + pos, output_size - pos, "\",\"ciphertext\":\"");
    if (n < 0 || (size_t)n >= output_size - pos) return 0;
    pos += (size_t)n;
    for (i = 0; i < contract->ciphertext_len; ++i) {
        n = snprintf(output + pos, output_size - pos, "%02x", contract->ciphertext[i]);
        if (n < 0 || (size_t)n >= output_size - pos) return 0;
        pos += (size_t)n;
    }
    n = snprintf(output + pos, output_size - pos, "\"}\n");
    return n >= 0 && (size_t)n < output_size - pos ? (int)(pos + (size_t)n) : 0;
}

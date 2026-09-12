#define _POSIX_C_SOURCE 200809L

#include "key_distribution_server.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define KDS_DEFAULT_PORT 9443
#define KDS_MAX_REQUEST 8192U
#define KDS_MAX_RESPONSE 12288U

static volatile sig_atomic_t running = 1;
static http3_kds_server_t server_state;
static SSL_CTX *tls_ctx = NULL;

static void stop_server(int signo) {
    (void)signo;
    running = 0;
}

static int hex_decode_32(const char *hex, uint8_t out[32]) {
    size_t i;
    if (!hex || strlen(hex) != 64U) return 0;
    for (i = 0; i < 32U; ++i) {
        unsigned int v;
        if (sscanf(hex + i * 2U, "%2x", &v) != 1 || v > 255U) return 0;
        out[i] = (uint8_t)v;
    }
    return 1;
}

static int json_string(const char *body, const char *name, char *out, size_t out_size) {
    char needle[96];
    const char *p;
    const char *q;
    size_t n;
    int written;
    if (!body || !name || !out || out_size == 0) return 0;
    written = snprintf(needle, sizeof(needle), "\"%s\":\"", name);
    if (written < 0 || (size_t)written >= sizeof(needle)) return 0;
    p = strstr(body, needle);
    if (!p) return 0;
    p += strlen(needle);
    q = strchr(p, '"');
    if (!q) return 0;
    n = (size_t)(q - p);
    if (n == 0 || n >= out_size) return 0;
    memcpy(out, p, n);
    out[n] = '\0';
    return 1;
}

static int read_http_request(SSL *ssl, char *request, size_t request_size) {
    size_t used = 0;
    int n;
    while (used + 1U < request_size) {
        n = SSL_read(ssl, request + used, (int)(request_size - used - 1U));
        if (n <= 0) return 0;
        used += (size_t)n;
        request[used] = '\0';
        if (strstr(request, "\r\n\r\n")) return 1;
    }
    return 0;
}

static int write_http(SSL *ssl, int status, const char *body) {
    char response[KDS_MAX_RESPONSE];
    int n;
    const char *reason = status == 200 ? "OK" : (status == 429 ? "Too Many Requests" : "Bad Request");
    n = snprintf(response, sizeof(response),
                 "HTTP/1.1 %d %s\r\nContent-Type: application/json\r\n"
                 "Content-Length: %zu\r\nConnection: close\r\nCache-Control: no-store\r\n\r\n%s",
                 status, reason, strlen(body), body);
    if (n < 0 || (size_t)n >= sizeof(response)) return 0;
    return SSL_write(ssl, response, n) == n;
}

static void *handle_connection(void *arg) {
    int fd = *(int *)arg;
    SSL *ssl = SSL_new(tls_ctx);
    char request[KDS_MAX_REQUEST];
    char client_public_hex[65];
    char client_id[HTTP3_KDS_MAX_CLIENT_ID];
    char jurisdiction[HTTP3_KDS_MAX_JURISDICTION];
    uint8_t client_public[32];
    http3_kds_contract_t contract;
    char body[KDS_MAX_RESPONSE];
    time_t now;
    const char *path;
    const char *request_body;

    free(arg);
    if (!ssl) {
        close(fd);
        return NULL;
    }
    SSL_set_fd(ssl, fd);
    if (SSL_accept(ssl) != 1 || !read_http_request(ssl, request, sizeof(request))) {
        SSL_free(ssl);
        close(fd);
        return NULL;
    }

    path = strchr(request, ' ');
    if (!path) {
        write_http(ssl, 400, "{\"error\":\"bad_request\"}\n");
        goto done;
    }
    ++path;
    if (strncmp(path, "/v1/public-key", 14) == 0) {
        size_t i;
        char pubhex[65];
        for (i = 0; i < 32U; ++i) snprintf(pubhex + i * 2U, 3, "%02x", server_state.server_public[i]);
        pubhex[64] = '\0';
        snprintf(body, sizeof(body),
                 "{\"version\":1,\"algorithm\":\"X25519\",\"public_key\":\"%s\","
                 "\"purpose\":\"HTTP3-KDS-bootstrap\"}\n", pubhex);
        write_http(ssl, 200, body);
        goto done;
    }
    if (strncmp(path, "/v1/bootstrap", 13) != 0 || strncmp(request, "POST ", 5) != 0) {
        write_http(ssl, 400, "{\"error\":\"unsupported_endpoint\"}\n");
        goto done;
    }

    request_body = strstr(request, "\r\n\r\n");
    if (!request_body) {
        write_http(ssl, 400, "{\"error\":\"missing_body\"}\n");
        goto done;
    }
    request_body += 4;
    if (!json_string(request_body, "client_public", client_public_hex, sizeof(client_public_hex)) ||
        !json_string(request_body, "client_id", client_id, sizeof(client_id)) ||
        !json_string(request_body, "jurisdiction", jurisdiction, sizeof(jurisdiction)) ||
        !hex_decode_32(client_public_hex, client_public)) {
        write_http(ssl, 400, "{\"error\":\"invalid_bootstrap_contract\"}\n");
        goto done;
    }
    now = time(NULL);
    if (now < 0 || !http3_kds_issue_contract(&server_state, client_public, client_id,
                                              jurisdiction, (uint64_t)now, 3600U, &contract)) {
        write_http(ssl, 429, "{\"error\":\"issuance_limit_or_crypto_failure\",\"limit_per_minute\":10000}\n");
        goto done;
    }
    if (!http3_kds_contract_json(&contract, body, sizeof(body))) {
        write_http(ssl, 400, "{\"error\":\"contract_serialization_failure\"}\n");
        goto done;
    }
    write_http(ssl, 200, body);

done:
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(fd);
    return NULL;
}

static int create_listener(unsigned short port) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    int yes = 1;
    struct sockaddr_in addr;
    if (fd < 0) return -1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) != 0 || listen(fd, 128) != 0) {
        close(fd);
        return -1;
    }
    return fd;
}

static SSL_CTX *create_tls_context(const char *cert, const char *key) {
    SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());
    if (!ctx) return NULL;
    if (SSL_CTX_set_min_proto_version(ctx, TLS1_3_VERSION) != 1 ||
        SSL_CTX_use_certificate_file(ctx, cert, SSL_FILETYPE_PEM) != 1 ||
        SSL_CTX_use_PrivateKey_file(ctx, key, SSL_FILETYPE_PEM) != 1 ||
        SSL_CTX_check_private_key(ctx) != 1) {
        SSL_CTX_free(ctx);
        return NULL;
    }
    return ctx;
}

int main(int argc, char **argv) {
    const char *cert = argc > 1 ? argv[1] : "server.crt";
    const char *key = argc > 2 ? argv[2] : "server.key";
    const char *bootstrap_key = argc > 3 ? argv[3] : "kds-bootstrap.key";
    unsigned short port = argc > 4 ? (unsigned short)strtoul(argv[4], NULL, 10) : KDS_DEFAULT_PORT;
    int listener;

    signal(SIGINT, stop_server);
    signal(SIGTERM, stop_server);
    signal(SIGPIPE, SIG_IGN);

    OPENSSL_init_ssl(0, NULL);
    tls_ctx = create_tls_context(cert, key);
    if (!tls_ctx) {
        fprintf(stderr, "KDS: TLS initialization failed\n");
        return 1;
    }
    if (!http3_kds_server_init(&server_state, bootstrap_key)) {
        fprintf(stderr, "KDS: bootstrap key initialization failed\n");
        SSL_CTX_free(tls_ctx);
        return 1;
    }
    listener = create_listener(port);
    if (listener < 0) {
        fprintf(stderr, "KDS: unable to listen on port %u: %s\n", port, strerror(errno));
        http3_kds_server_clear(&server_state);
        SSL_CTX_free(tls_ctx);
        return 1;
    }

    fprintf(stdout, "SLeeLa HTTP 3.0 Key Distribution Server listening on TLS port %u\n", port);
    fprintf(stdout, "Issuance limit: %u bootstrap contracts per rolling minute window\n",
            HTTP3_KDS_MAX_KEYS_PER_MINUTE);
    fprintf(stdout, "Bootstrap key file: %s\n", bootstrap_key);
    while (running) {
        int *client_fd = (int *)malloc(sizeof(*client_fd));
        pthread_t thread;
        if (!client_fd) break;
        *client_fd = accept(listener, NULL, NULL);
        if (*client_fd < 0) {
            free(client_fd);
            if (errno == EINTR) continue;
            break;
        }
        if (pthread_create(&thread, NULL, handle_connection, client_fd) != 0) {
            close(*client_fd);
            free(client_fd);
            continue;
        }
        pthread_detach(thread);
    }
    close(listener);
    http3_kds_server_clear(&server_state);
    SSL_CTX_free(tls_ctx);
    return 0;
}

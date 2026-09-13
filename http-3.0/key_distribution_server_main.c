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
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define KDS_DEFAULT_PORT 9443
#define KDS_MAX_REQUEST 8192U
#define KDS_MAX_RESPONSE 12288U
#define KDS_MAX_BODY 4096U
#define KDS_MAX_CONNECTIONS 256U

static volatile sig_atomic_t running = 1;
static http3_kds_server_t server_state;
static SSL_CTX *tls_ctx = NULL;
static pthread_mutex_t connection_lock = PTHREAD_MUTEX_INITIALIZER;
static unsigned int active_connections = 0;

typedef struct {
    int fd;
    struct sockaddr_storage address;
    socklen_t address_len;
} connection_arg_t;

static void stop_server(int signo) {
    (void)signo;
    running = 0;
}

static int connection_acquire(void) {
    int ok = 0;
    pthread_mutex_lock(&connection_lock);
    if (active_connections < KDS_MAX_CONNECTIONS) {
        ++active_connections;
        ok = 1;
    }
    pthread_mutex_unlock(&connection_lock);
    return ok;
}

static void connection_release(void) {
    pthread_mutex_lock(&connection_lock);
    if (active_connections > 0) --active_connections;
    pthread_mutex_unlock(&connection_lock);
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

static int header_value(const char *headers, const char *name, char *out, size_t out_size) {
    const char *p = headers;
    size_t name_len = strlen(name);
    if (!headers || !name || !out || out_size == 0) return 0;
    while ((p = strstr(p, name)) != NULL) {
        const char *line_start = p == headers ? p : p - 1;
        const char *colon;
        const char *value;
        const char *end;
        size_t n;
        if (line_start != headers && *line_start != '\n') {
            p += name_len;
            continue;
        }
        colon = p + name_len;
        if (strncasecmp(p, name, name_len) != 0 || *colon != ':') {
            p += name_len;
            continue;
        }
        value = colon + 1;
        while (*value == ' ' || *value == '\t') ++value;
        end = strstr(value, "\r\n");
        if (!end) end = value + strlen(value);
        n = (size_t)(end - value);
        if (n == 0 || n >= out_size) return 0;
        memcpy(out, value, n);
        out[n] = '\0';
        return 1;
    }
    return 0;
}

static int read_http_request(SSL *ssl, char *request, size_t request_size,
                             size_t *header_len, size_t *body_len) {
    size_t used = 0;
    size_t expected_body = 0;
    int have_headers = 0;
    int n;
    char length_text[32];

    while (used + 1U < request_size) {
        n = SSL_read(ssl, request + used, (int)(request_size - used - 1U));
        if (n <= 0) return 0;
        used += (size_t)n;
        request[used] = '\0';

        if (!have_headers) {
            char *headers_end = strstr(request, "\r\n\r\n");
            if (!headers_end) continue;
            *header_len = (size_t)(headers_end - request) + 4U;
            if (*header_len >= request_size) return 0;
            have_headers = 1;
            if (header_value(request, "Content-Length", length_text, sizeof(length_text))) {
                char *end = NULL;
                unsigned long parsed = strtoul(length_text, &end, 10);
                if (!end || *end != '\0' || parsed > KDS_MAX_BODY) return 0;
                expected_body = (size_t)parsed;
            }
        }

        if (have_headers && used >= *header_len + expected_body) {
            if (*header_len + expected_body >= request_size) return 0;
            *body_len = expected_body;
            request[*header_len + expected_body] = '\0';
            return 1;
        }
    }
    return 0;
}

static int write_all(SSL *ssl, const char *data, size_t len) {
    size_t written = 0;
    while (written < len) {
        int n = SSL_write(ssl, data + written, (int)(len - written));
        if (n <= 0) return 0;
        written += (size_t)n;
    }
    return 1;
}

static int write_http(SSL *ssl, int status, const char *body) {
    char response[KDS_MAX_RESPONSE];
    int n;
    const char *reason = status == 200 ? "OK" :
                         (status == 400 ? "Bad Request" :
                          (status == 404 ? "Not Found" :
                           (status == 405 ? "Method Not Allowed" :
                            (status == 413 ? "Payload Too Large" :
                             (status == 429 ? "Too Many Requests" : "Service Unavailable")))));
    n = snprintf(response, sizeof(response),
                 "HTTP/1.1 %d %s\r\nContent-Type: application/json\r\n"
                 "Content-Length: %zu\r\nConnection: close\r\nCache-Control: no-store\r\n\r\n%s",
                 status, reason, strlen(body), body);
    if (n < 0 || (size_t)n >= sizeof(response)) return 0;
    return write_all(ssl, response, (size_t)n);
}

static int request_line(const char *request, char *method, size_t method_size,
                        char *path, size_t path_size) {
    const char *sp1 = strchr(request, ' ');
    const char *sp2;
    size_t method_len, path_len;
    if (!sp1) return 0;
    sp2 = strchr(sp1 + 1, ' ');
    if (!sp2) return 0;
    method_len = (size_t)(sp1 - request);
    path_len = (size_t)(sp2 - sp1 - 1);
    if (method_len == 0 || method_len >= method_size || path_len == 0 || path_len >= path_size) return 0;
    memcpy(method, request, method_len);
    method[method_len] = '\0';
    memcpy(path, sp1 + 1, path_len);
    path[path_len] = '\0';
    return 1;
}

static void *handle_connection(void *arg) {
    connection_arg_t *connection = (connection_arg_t *)arg;
    int fd = connection ? connection->fd : -1;
    SSL *ssl = NULL;
    char request[KDS_MAX_REQUEST];
    char method[16];
    char path[128];
    char client_public_hex[65];
    char client_id[HTTP3_KDS_MAX_CLIENT_ID];
    char jurisdiction[HTTP3_KDS_MAX_JURISDICTION];
    uint8_t client_public[32];
    http3_kds_contract_t contract;
    char body[KDS_MAX_RESPONSE];
    size_t header_len = 0, body_len = 0;
    time_t now;

    free(connection);
    if (fd < 0) {
        connection_release();
        return NULL;
    }
    ssl = SSL_new(tls_ctx);
    if (!ssl) goto done;
    SSL_set_fd(ssl, fd);
    if (SSL_accept(ssl) != 1 ||
        !read_http_request(ssl, request, sizeof(request), &header_len, &body_len)) {
        goto done;
    }
    if (!request_line(request, method, sizeof(method), path, sizeof(path))) {
        write_http(ssl, 400, "{\"error\":\"bad_request\"}\n");
        goto done;
    }

    if (strcmp(method, "GET") == 0 && strcmp(path, "/v1/public-key") == 0) {
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

    if (strcmp(method, "POST") != 0) {
        write_http(ssl, 405, "{\"error\":\"method_not_allowed\"}\n");
        goto done;
    }
    if (strcmp(path, "/v1/bootstrap") != 0) {
        write_http(ssl, 404, "{\"error\":\"unsupported_endpoint\"}\n");
        goto done;
    }
    if (body_len > KDS_MAX_BODY || header_len + body_len >= sizeof(request)) {
        write_http(ssl, 413, "{\"error\":\"payload_too_large\"}\n");
        goto done;
    }
    if (!json_string(request + header_len, "client_public", client_public_hex, sizeof(client_public_hex)) ||
        !json_string(request + header_len, "client_id", client_id, sizeof(client_id)) ||
        !json_string(request + header_len, "jurisdiction", jurisdiction, sizeof(jurisdiction)) ||
        !hex_decode_32(client_public_hex, client_public)) {
        write_http(ssl, 400, "{\"error\":\"invalid_bootstrap_contract\"}\n");
        goto done;
    }
    now = time(NULL);
    if (now < 0) {
        write_http(ssl, 503, "{\"error\":\"clock_failure\"}\n");
        goto done;
    }
    if (!http3_kds_issue_contract(&server_state, client_public, client_id,
                                  jurisdiction, (uint64_t)now, 3600U, &contract)) {
        write_http(ssl, 429, "{\"error\":\"issuance_limit_or_crypto_failure\",\"limit_per_minute\":10000}\n");
        goto done;
    }
    if (!http3_kds_contract_json(&contract, body, sizeof(body))) {
        write_http(ssl, 503, "{\"error\":\"contract_serialization_failure\"}\n");
        goto done;
    }
    write_http(ssl, 200, body);

done:
    if (ssl) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
    }
    if (fd >= 0) close(fd);
    connection_release();
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
        SSL_CTX_set_options(ctx, SSL_OP_NO_COMPRESSION) == 0 ||
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
    fprintf(stdout, "Concurrent connection limit: %u\n", KDS_MAX_CONNECTIONS);
    fprintf(stdout, "Bootstrap key file: %s\n", bootstrap_key);
    while (running) {
        connection_arg_t *connection = (connection_arg_t *)calloc(1, sizeof(*connection));
        pthread_t thread;
        if (!connection) break;
        connection->address_len = sizeof(connection->address);
        connection->fd = accept(listener, (struct sockaddr *)&connection->address,
                                 &connection->address_len);
        if (connection->fd < 0) {
            free(connection);
            if (errno == EINTR) continue;
            break;
        }
        if (!connection_acquire()) {
            close(connection->fd);
            free(connection);
            continue;
        }
        if (pthread_create(&thread, NULL, handle_connection, connection) != 0) {
            close(connection->fd);
            free(connection);
            connection_release();
            continue;
        }
        pthread_detach(thread);
    }
    close(listener);
    http3_kds_server_clear(&server_state);
    SSL_CTX_free(tls_ctx);
    return 0;
}

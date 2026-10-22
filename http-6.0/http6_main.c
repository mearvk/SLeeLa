/* http6_main.c — SLeeLa HTTP 6.0 server entry point */
#include "http6_server.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

static int echo_frame(const http6_frame_view_t *f, uint8_t *rp, size_t rc,
                      size_t *rl, void *ud) {
    (void)ud;
    size_t n = f->header.payload_length < rc ? f->header.payload_length : rc;
    if (n > 0) memcpy(rp, f->payload, n);
    *rl = n; return 0;
}

static int demo_http(const h6s_request *req, h6s_response *resp, void *ud) {
    (void)req;
    (void)ud;
    static const char body[] = "{\"status\":\"ok\",\"http_generation\":\"6.0\"}\\n";
    resp->status_code = 200;
    resp->content_type = "application/json";
    resp->body = body;
    resp->body_length = sizeof(body) - 1;
    return 0;
}

static int parse_port(const char *s, uint16_t *port) {
    char *end = NULL;
    unsigned long value;
    if (!s || !*s || !port) return -1;
    for (const unsigned char *p = (const unsigned char *)s; *p; ++p)
        if (*p < '0' || *p > '9') return -1;
    errno = 0;
    value = strtoul(s, &end, 10);
    if (errno == ERANGE || end == s || !end || *end != '\0' ||
        value < 1 || value > 65535 || value > UINT16_MAX) return -1;
    *port = (uint16_t)value;
    return 0;
}

int main(int argc, char **argv) {
    h6s_server_config cfg;
    h6s_server_config_init(&cfg);
    cfg.handler = demo_http;
    cfg.frame_handler = echo_frame;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--self-test") == 0) return h6s_self_test();
        else if (strcmp(argv[i], "--port") == 0 && i+1 < argc) {
            if (parse_port(argv[++i], &cfg.port) != 0) {
                fprintf(stderr, "invalid port (expected 1..65535)\n");
                return 2;
            }
        } else if (strcmp(argv[i], "--bind") == 0 && i+1 < argc) {
            const char *host = argv[++i];
            if (!host[0]) { fprintf(stderr, "invalid bind host\n"); return 2; }
            cfg.bind_address = host;
        } else if (strcmp(argv[i], "--allow-fallback") == 0)
            cfg.allow_negotiation_fallback = 1;
        else if (strcmp(argv[i], "--help") == 0) {
            printf("Usage: %s [--port N] [--bind HOST] [--allow-fallback] [--self-test] [--help]\n", argv[0]); return 0;
        } else { fprintf(stderr, "unknown option: %s\n", argv[i]); return 2; }
    }
    return h6s_server_run(&cfg);
}

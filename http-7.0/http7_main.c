/* http7_main.c — SLeeLa HTTP 7.0 server entry point */
#include "http7_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int echo_frame(const http7_frame_view_t *f, uint8_t *rp, size_t rc,
                      size_t *rl, void *ud) {
    (void)ud;
    size_t n = f->header.payload_length < rc ? f->header.payload_length : rc;
    if (n > 0) memcpy(rp, f->payload, n);
    *rl = n; return 0;
}

static int demo_http(const h7s_request *req, h7s_response *resp, void *ud) {
    (void)ud;
    static char body[1024];
    int n = snprintf(body, sizeof(body),
        "{\"method\":\"%s\",\"uri\":\"%s\",\"sleela_version\":\"%s\"}\n",
        req->method, req->uri, req->sleela_version);
    resp->status_code = 200; resp->content_type = "application/json";
    resp->body = body; resp->body_length = (size_t)n; return 0;
}

int main(int argc, char **argv) {
    h7s_server_config cfg;
    h7s_server_config_init(&cfg);
    cfg.handler = demo_http;
    cfg.frame_handler = echo_frame;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--self-test") == 0) return h7s_self_test();
        else if (strcmp(argv[i], "--port") == 0 && i+1 < argc)
            cfg.port = (uint16_t)atoi(argv[++i]);
        else if (strcmp(argv[i], "--help") == 0) {
            printf("Usage: %s [--port N] [--self-test] [--help]\n", argv[0]); return 0;
        } else { fprintf(stderr, "unknown option: %s\n", argv[i]); return 2; }
    }
    return h7s_server_run(&cfg);
}

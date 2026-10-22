/* http4_main.c — SLeeLa HTTP 4.0 server entry point
 *
 * Usage:  http4_server [--bind-address IPv4] [--port N] [--self-test] [--help]
 * Platforms: Linux, macOS, Windows 10+
 */
#include "http4_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>

/* Echo frame handler: returns the payload back in a response frame. */
static int echo_frame(const http4_frame_view_t *frame,
                      uint8_t *resp_payload, size_t resp_cap,
                      size_t *resp_len, void *ud)
{
    (void)ud;
    size_t n = frame->header.payload_length < resp_cap
             ? frame->header.payload_length : resp_cap;
    if (n > 0) memcpy(resp_payload, frame->payload, n);
    *resp_len = n;
    return 0;
}

/* HTTP handler for non-frame routes. */
static int demo_http(const h4s_request *req, h4s_response *resp, void *ud)
{
    (void)ud;
    static char body[1024];
    int n = snprintf(body, sizeof(body),
        "{\"method\":\"%s\","
         "\"uri\":\"%s\","
         "\"sleela_port\":\"%s\","
         "\"service_id\":\"%s\","
         "\"op_id\":\"%s\","
         "\"sleela_version\":\"%s\"}\n",
        req->method, req->uri,
        req->sleela_port, req->service_id,
        req->op_id, req->sleela_version);
    resp->status_code  = 200;
    resp->content_type = "application/json";
    resp->body         = body;
    resp->body_length  = (size_t)n;
    return 0;
}

static int parse_port(const char *text, uint16_t *port)
{
    if (!text || !*text || !port) return -1;
    for (const unsigned char *p = (const unsigned char *)text; *p; ++p)
        if (*p < '0' || *p > '9') return -1;
    errno = 0;
    char *end = NULL;
    unsigned long value = strtoul(text, &end, 10);
    if (errno == ERANGE || !end || *end != '\0' || value == 0 || value > 65535)
        return -1;
    *port = (uint16_t)value;
    return 0;
}

int main(int argc, char **argv)
{
    h4s_server_config cfg;
    h4s_server_config_init(&cfg);
    cfg.handler       = demo_http;
    cfg.frame_handler = echo_frame;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--self-test") == 0)
            return h4s_self_test();
        else if (strcmp(argv[i], "--port") == 0 && i + 1 < argc) {
            if (parse_port(argv[++i], &cfg.port) != 0) {
                fprintf(stderr, "invalid port: %s\n", argv[i]);
                return 2;
            }
        }
        else if (strcmp(argv[i], "--bind-address") == 0 && i + 1 < argc)
            cfg.bind_address = argv[++i];
        else if (strcmp(argv[i], "--help") == 0) {
            printf("Usage: %s [--bind-address IPv4] [--port N] [--self-test] [--help]\n", argv[0]);
            printf("Default bind is 127.0.0.1; public binding requires a trusted security boundary.\n");
            return 0;
        } else {
            fprintf(stderr, "unknown option: %s\n", argv[i]);
            return 2;
        }
    }

    return h4s_server_run(&cfg);
}

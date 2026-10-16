/* h21_main.c — SLeeLa HTTP 2.0/2.1 server entry point
 *
 * Usage:
 *   h21_server [--port N] [--self-test]
 *
 * Platforms: Linux, macOS, Windows 10+
 */
#include "h21_server.h"
#include "h21_protocol.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── demo service: echo ─────────────────────────────────────────────── */

static h21_status_t demo_echo(const uint8_t *payload, size_t payload_len,
                              uint8_t *result, size_t result_cap,
                              size_t *result_len, void *ctx)
{
    (void)ctx;
    size_t n = payload_len < result_cap ? payload_len : result_cap;
    memcpy(result, payload, n);
    *result_len = n;
    return H21_STATUS_OK;
}

/* ── demo HTTP handler for non-envelope routes ─────────────────────── */

static int demo_http_handler(const h21_http_request *req,
                             h21_http_response      *resp,
                             void                   *user_data)
{
    (void)user_data;

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

/* ── main ───────────────────────────────────────────────────────────── */

int main(int argc, char **argv)
{
    /* Set up the 2.1 pipeline with a demo echo service. */
    h21_pipeline_t pipeline;
    h21_pipeline_init(&pipeline);
    h21_pipeline_register_service(&pipeline, "echo", NULL);
    h21_pipeline_register_op(&pipeline, "echo", "echo",
                             H21_RETRY_READ, demo_echo);

    h21_server_config cfg;
    h21_server_config_init(&cfg);
    cfg.handler  = demo_http_handler;
    cfg.pipeline = &pipeline;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--self-test") == 0)
            return h21_self_test();
        else if (strcmp(argv[i], "--port") == 0 && i + 1 < argc)
            cfg.port = (uint16_t)atoi(argv[++i]);
        else if (strcmp(argv[i], "--help") == 0) {
            printf("Usage: %s [--port N] [--self-test] [--help]\n", argv[0]);
            return 0;
        } else {
            fprintf(stderr, "unknown option: %s\n", argv[i]);
            return 2;
        }
    }

    return h21_server_run(&cfg);
}

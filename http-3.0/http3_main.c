/* http3_main.c — SLeeLa HTTP 3.0 server entry point
 *
 * Usage:
 *   http3_server [--port N] [--self-test]
 *
 * Platforms: Linux, macOS, Windows 10+
 */
#include "http3_server.h"
#include "http3_protocol.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── demo service: echo ─────────────────────────────────────────────── */

static http3_status_t demo_echo(const uint8_t *payload, size_t payload_len,
                                uint8_t *result, size_t result_cap,
                                size_t *result_len, void *ctx)
{
    (void)ctx;
    size_t n = payload_len < result_cap ? payload_len : result_cap;
    memcpy(result, payload, n);
    *result_len = n;
    return HTTP3_STATUS_OK;
}

/* ── demo HTTP handler ──────────────────────────────────────────────── */

static int demo_http_handler(const h3s_request *req,
                             h3s_response      *resp,
                             void              *user_data)
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
    /* Demo MAC key — in production this comes from key agreement. */
    static const uint8_t demo_key[HTTP3_MAC_KEY_BYTES] = {
        1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

    /* Set up the 3.0 pipeline with integrity gate + demo echo. */
    http3_pipeline_t pipeline;
    http3_pipeline_init(&pipeline);
    http3_pipeline_set_mac_key(&pipeline, demo_key);
    http3_pipeline_reset_replay_window(&pipeline, 0);
    http3_pipeline_register_service(&pipeline, "echo", NULL);
    http3_pipeline_register_op(&pipeline, "echo", "echo",
                               HTTP3_RETRY_READ, demo_echo);

    h3s_server_config cfg;
    h3s_server_config_init(&cfg);
    cfg.handler  = demo_http_handler;
    cfg.pipeline = &pipeline;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--self-test") == 0)
            return h3s_self_test();
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

    return h3s_server_run(&cfg);
}

/* http1_main.c — SLeeLa HTTP/1.0 server entry point
 *
 * Usage:
 *   http1_server [--port N] [--self-test]
 *
 * Platforms: Linux, macOS, Windows 10+
 */
#include "http1_server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int demo_handler(const http1_request *req,
                        http1_response      *resp,
                        void                *user_data)
{
    (void)user_data;

    /* Echo the SLeeLa routing info back as JSON. */
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

int main(int argc, char **argv)
{
    http1_server_config cfg;
    http1_server_config_init(&cfg);
    cfg.handler = demo_handler;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--self-test") == 0)
            return http1_self_test();
        else if (strcmp(argv[i], "--port") == 0 && i + 1 < argc)
            cfg.port = (uint16_t)atoi(argv[++i]);
        else if (strcmp(argv[i], "--help") == 0) {
            printf("Usage: %s [--port N] [--self-test] [--help]\n",
                   argv[0]);
            return 0;
        } else {
            fprintf(stderr, "unknown option: %s\n", argv[i]);
            return 2;
        }
    }

    return http1_server_run(&cfg);
}

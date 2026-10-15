/* h21_server.h — SLeeLa HTTP 2.0/2.1 portable server (Linux · macOS · Windows 10+)
 *
 * Provides a real, internet-facing server for the HTTP 2.1 application
 * protocol.  The transport is HTTP/1.x over the portable sleela_net socket
 * layer; the SLeeLa 2.1 envelope/naming/pipeline core rides inside the
 * HTTP request body (textual or binary, auto-detected).
 *
 * Clients also reach the server via standard HTTP for health/status/negotiate
 * endpoints.  The SLeeLa 2.1 envelope path is at /__sleela/h21 (POST).
 */
#ifndef SLEELA_H21_SERVER_H
#define SLEELA_H21_SERVER_H

#include <stddef.h>
#include <stdint.h>

#include "h21_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── limits ─────────────────────────────────────────────────────────── */

#define H21_SERVER_MAX_HEADER   (64 * 1024)
#define H21_SERVER_MAX_BODY     (8 * 1024 * 1024)
#define H21_SERVER_MAX_HEADERS  100
#define H21_SERVER_MAX_METHOD   16
#define H21_SERVER_MAX_URI      8192
#define H21_SERVER_MAX_VERSION  16
#define H21_SERVER_MAX_HNAME    256
#define H21_SERVER_MAX_HVALUE   8192

/* ── download-mode threshold (shared with HTTP 1.0) ────────────────── */

#define H21_DOWNLOAD_THRESHOLD  (50ULL * 1024 * 1024)

/* ── HTTP-layer request / response ─────────────────────────────────── */

typedef struct {
    char name[H21_SERVER_MAX_HNAME];
    char value[H21_SERVER_MAX_HVALUE];
} h21_http_header;

typedef struct {
    char             method[H21_SERVER_MAX_METHOD];
    char             uri[H21_SERVER_MAX_URI];
    char             version[H21_SERVER_MAX_VERSION];
    h21_http_header  headers[H21_SERVER_MAX_HEADERS];
    int              header_count;
    char            *body;
    size_t           body_length;

    /* SLeeLa routing (from X-SLeeLa-* headers). */
    char sleela_port[64];
    char service_id[128];
    char op_id[128];
    char sleela_version[32];
    char sleela_accept[256];
} h21_http_request;

typedef struct {
    int              status_code;
    const char      *reason;
    h21_http_header  headers[H21_SERVER_MAX_HEADERS];
    int              header_count;
    const char      *body;
    size_t           body_length;
    const char      *content_type;
} h21_http_response;

/* ── resumable download state ──────────────────────────────────────── */

typedef struct {
    char     session_id[64];
    char     datetime[32];
    char     file_id[128];
    char     file_name[256];
    uint64_t index;
    uint64_t offset;
    uint64_t total_size;
} h21_download_state;

/* ── application-level route handler (HTTP layer) ──────────────────── */

typedef int (*h21_http_handler)(const h21_http_request *req,
                                h21_http_response      *resp,
                                void                   *user_data);

/* ── server configuration ──────────────────────────────────────────── */

typedef struct {
    uint16_t         port;
    const char      *bind_address;
    int              backlog;
    h21_http_handler handler;          /* optional HTTP-layer handler */
    void            *user_data;
    const char      *server_name;
    int              allow_negotiation_fallback;
    h21_pipeline_t  *pipeline;         /* application pipeline (2.1 core) */
} h21_server_config;

/* ── public API ────────────────────────────────────────────────────── */

void h21_server_config_init(h21_server_config *cfg);
int  h21_server_run(const h21_server_config *cfg);
void h21_server_stop(void);

int  h21_http_response_add_header(h21_http_response *resp,
                                  const char *name, const char *value);

int  h21_download_parse(const char *line, h21_download_state *out);
int  h21_download_serialise(const h21_download_state *st,
                            char *buf, size_t buf_size);
int  h21_download_should_resume(uint64_t content_length);

int  h21_self_test(void);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_H21_SERVER_H */

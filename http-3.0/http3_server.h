/* http3_server.h — SLeeLa HTTP 3.0 portable server (Linux · macOS · Windows 10+)
 *
 * Internet-facing server for the HTTP 3.0 application protocol.  Transport is
 * HTTP/1.x over the portable sleela_net socket layer; the H3 envelope/pipeline
 * core (with the integrity gate: DIGEST, INTACTX, NONCE) rides inside the
 * HTTP request body at /__sleela/h3 (POST).
 */
#ifndef SLEELA_HTTP3_SERVER_H
#define SLEELA_HTTP3_SERVER_H

#include <stddef.h>
#include <stdint.h>

#include "http3_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── limits ─────────────────────────────────────────────────────────── */

#define H3S_MAX_HEADER   (64 * 1024)
#define H3S_MAX_BODY     (8 * 1024 * 1024)
#define H3S_MAX_HEADERS  100
#define H3S_MAX_METHOD   16
#define H3S_MAX_URI      8192
#define H3S_MAX_VERSION  16
#define H3S_MAX_HNAME    256
#define H3S_MAX_HVALUE   8192

#define H3S_DOWNLOAD_THRESHOLD (50ULL * 1024 * 1024)

/* ── HTTP-layer types ───────────────────────────────────────────────── */

typedef struct {
    char name[H3S_MAX_HNAME];
    char value[H3S_MAX_HVALUE];
} h3s_header;

typedef struct {
    char         method[H3S_MAX_METHOD];
    char         uri[H3S_MAX_URI];
    char         version[H3S_MAX_VERSION];
    h3s_header   headers[H3S_MAX_HEADERS];
    int          header_count;
    char        *body;
    size_t       body_length;

    /* SLeeLa routing (from X-SLeeLa-* headers). */
    char sleela_port[64];
    char service_id[128];
    char op_id[128];
    char sleela_version[32];
    char sleela_accept[256];
} h3s_request;

typedef struct {
    int          status_code;
    const char  *reason;
    h3s_header   headers[H3S_MAX_HEADERS];
    int          header_count;
    const char  *body;
    size_t       body_length;
    const char  *content_type;
} h3s_response;

/* ── download state ─────────────────────────────────────────────────── */

typedef struct {
    char     session_id[64];
    char     datetime[32];
    char     file_id[128];
    char     file_name[256];
    uint64_t index;
    uint64_t offset;
    uint64_t total_size;
} h3s_download_state;

/* ── HTTP-layer route handler ───────────────────────────────────────── */

typedef int (*h3s_http_handler)(const h3s_request *req,
                                h3s_response      *resp,
                                void              *user_data);

/* ── server configuration ───────────────────────────────────────────── */

typedef struct {
    uint16_t           port;
    const char        *bind_address;
    int                backlog;
    h3s_http_handler   handler;
    void              *user_data;
    const char        *server_name;
    int                allow_negotiation_fallback;
    http3_pipeline_t  *pipeline;  /* H3 pipeline (with integrity gate) */
} h3s_server_config;

/* ── public API ─────────────────────────────────────────────────────── */

void h3s_server_config_init(h3s_server_config *cfg);
int  h3s_server_run(const h3s_server_config *cfg);
void h3s_server_stop(void);

int  h3s_response_add_header(h3s_response *resp,
                             const char *name, const char *value);

int  h3s_download_parse(const char *line, h3s_download_state *out);
int  h3s_download_serialise(const h3s_download_state *st,
                            char *buf, size_t buf_size);
int  h3s_download_should_resume(uint64_t content_length);

int  h3s_self_test(void);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_HTTP3_SERVER_H */

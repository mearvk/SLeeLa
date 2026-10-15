/* http4_server.h — SLeeLa HTTP 4.0 portable server (Linux · macOS · Windows 10+)
 *
 * Internet-facing server for the HTTP 4.0 frame-based application protocol.
 * Transport is HTTP/1.x over sleela_net portable sockets; H4 frames ride
 * inside the HTTP request body at /__sleela/h4 (POST).
 */
#ifndef SLEELA_HTTP4_SERVER_H
#define SLEELA_HTTP4_SERVER_H

#include <stddef.h>
#include <stdint.h>

#include "http4_protocol.h"
#include "http4_packet_context.h"
#include "http4_packet_payload.h"
#include "http4_segmentation.h"
#include "http4_reassembly.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── limits ─────────────────────────────────────────────────────────── */

#define H4S_MAX_HEADER   (64 * 1024)
#define H4S_MAX_BODY     (8 * 1024 * 1024)
#define H4S_MAX_HEADERS  100
#define H4S_MAX_METHOD   16
#define H4S_MAX_URI      8192
#define H4S_MAX_VERSION  16
#define H4S_MAX_HNAME    256
#define H4S_MAX_HVALUE   8192

#define H4S_DOWNLOAD_THRESHOLD (50ULL * 1024 * 1024)

/* ── HTTP-layer types ───────────────────────────────────────────────── */

typedef struct {
    char name[H4S_MAX_HNAME];
    char value[H4S_MAX_HVALUE];
} h4s_header;

typedef struct {
    char         method[H4S_MAX_METHOD];
    char         uri[H4S_MAX_URI];
    char         version[H4S_MAX_VERSION];
    h4s_header   headers[H4S_MAX_HEADERS];
    int          header_count;
    char        *body;
    size_t       body_length;
    char sleela_port[64];
    char service_id[128];
    char op_id[128];
    char sleela_version[32];
    char sleela_accept[256];
} h4s_request;

typedef struct {
    int          status_code;
    const char  *reason;
    h4s_header   headers[H4S_MAX_HEADERS];
    int          header_count;
    const char  *body;
    size_t       body_length;
    const char  *content_type;
} h4s_response;

/* ── download state ─────────────────────────────────────────────────── */

typedef struct {
    char     session_id[64];
    char     datetime[32];
    char     file_id[128];
    char     file_name[256];
    uint64_t index;
    uint64_t offset;
    uint64_t total_size;
} h4s_download_state;

/* ── frame handler callback ─────────────────────────────────────────── */

typedef int (*h4s_frame_handler)(const http4_frame_view_t *frame,
                                 uint8_t *resp_payload,
                                 size_t   resp_cap,
                                 size_t  *resp_len,
                                 void    *user_data);

/* ── HTTP-layer route handler ───────────────────────────────────────── */

typedef int (*h4s_http_handler)(const h4s_request *req,
                                h4s_response      *resp,
                                void              *user_data);

/* ── server configuration ───────────────────────────────────────────── */

typedef struct {
    uint16_t           port;
    const char        *bind_address;
    int                backlog;
    h4s_http_handler   handler;
    h4s_frame_handler  frame_handler;
    void              *user_data;
    const char        *server_name;
    int                allow_negotiation_fallback;
} h4s_server_config;

/* ── public API ─────────────────────────────────────────────────────── */

void h4s_server_config_init(h4s_server_config *cfg);
int  h4s_server_run(const h4s_server_config *cfg);
void h4s_server_stop(void);

int  h4s_response_add_header(h4s_response *resp,
                             const char *name, const char *value);

int  h4s_download_parse(const char *line, h4s_download_state *out);
int  h4s_download_serialise(const h4s_download_state *st,
                            char *buf, size_t buf_size);
int  h4s_download_should_resume(uint64_t content_length);

int  h4s_self_test(void);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_HTTP4_SERVER_H */

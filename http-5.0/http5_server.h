/* http5_server.h — SLeeLa HTTP 5.0 portable server (Linux · macOS · Windows 10+)
 *
 * Internet-facing server for the HTTP 5.0 frame-based protocol with Friends'
 * Packs.  Transport: HTTP/1.x over sleela_net.  H5 frames ride inside the
 * HTTP request body at /__sleela/h5 (POST).
 */
#ifndef SLEELA_HTTP5_SERVER_H
#define SLEELA_HTTP5_SERVER_H

#include <stddef.h>
#include <stdint.h>
#include "http5_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

#define H5S_MAX_HEADER   (64 * 1024)
#define H5S_MAX_BODY     (8 * 1024 * 1024)
#define H5S_MAX_HEADERS  100
#define H5S_MAX_METHOD   16
#define H5S_MAX_URI      8192
#define H5S_MAX_VERSION  16
#define H5S_MAX_HNAME    256
#define H5S_MAX_HVALUE   8192
#define H5S_DOWNLOAD_THRESHOLD (50ULL * 1024 * 1024)

typedef struct { char name[H5S_MAX_HNAME]; char value[H5S_MAX_HVALUE]; } h5s_header;

typedef struct {
    char method[H5S_MAX_METHOD]; char uri[H5S_MAX_URI]; char version[H5S_MAX_VERSION];
    h5s_header headers[H5S_MAX_HEADERS]; int header_count;
    char *body; size_t body_length;
    char sleela_port[64]; char service_id[128]; char op_id[128];
    char sleela_version[32]; char sleela_accept[256];
} h5s_request;

typedef struct {
    int status_code; const char *reason;
    h5s_header headers[H5S_MAX_HEADERS]; int header_count;
    const char *body; size_t body_length; const char *content_type;
} h5s_response;

typedef struct {
    char session_id[64]; char datetime[32]; char file_id[128]; char file_name[256];
    uint64_t index; uint64_t offset; uint64_t total_size;
} h5s_download_state;

typedef int (*h5s_frame_handler)(const http5_frame_view_t *frame,
                                 uint8_t *resp_payload, size_t resp_cap,
                                 size_t *resp_len, void *user_data);
typedef int (*h5s_http_handler)(const h5s_request *req, h5s_response *resp,
                                void *user_data);

typedef struct {
    uint16_t port; const char *bind_address; int backlog;
    h5s_http_handler handler; h5s_frame_handler frame_handler;
    void *user_data; const char *server_name;
    int allow_negotiation_fallback;
} h5s_server_config;

void h5s_server_config_init(h5s_server_config *cfg);
int  h5s_server_run(const h5s_server_config *cfg);
void h5s_server_stop(void);
int  h5s_response_add_header(h5s_response *r, const char *n, const char *v);
int  h5s_download_parse(const char *line, h5s_download_state *out);
int  h5s_download_serialise(const h5s_download_state *st, char *buf, size_t cap);
int  h5s_download_should_resume(uint64_t cl);
int  h5s_self_test(void);

#ifdef __cplusplus
}
#endif
#endif /* SLEELA_HTTP5_SERVER_H */

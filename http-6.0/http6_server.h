/* http6_server.h — SLeeLa HTTP 6.0 portable server (Linux · macOS · Windows 10+) */
#ifndef SLEELA_HTTP6_SERVER_H
#define SLEELA_HTTP6_SERVER_H
#include <stddef.h>
#include <stdint.h>
#include "http6_protocol.h"
#ifdef __cplusplus
extern "C" {
#endif

#define H6S_MAX_HEADER  (64*1024)
#define H6S_MAX_BODY    (8*1024*1024)
#define H6S_MAX_HEADERS 100
#define H6S_MAX_METHOD  16
#define H6S_MAX_URI     8192
#define H6S_MAX_VERSION 16
#define H6S_MAX_HNAME   256
#define H6S_MAX_HVALUE  8192
#define H6S_DOWNLOAD_THRESHOLD (50ULL*1024*1024)

typedef struct { char name[H6S_MAX_HNAME]; char value[H6S_MAX_HVALUE]; } h6s_header;
typedef struct {
    char method[H6S_MAX_METHOD]; char uri[H6S_MAX_URI]; char version[H6S_MAX_VERSION];
    h6s_header headers[H6S_MAX_HEADERS]; int header_count;
    char *body; size_t body_length;
    char sleela_port[64]; char service_id[128]; char op_id[128];
    char sleela_version[32]; char sleela_accept[256];
} h6s_request;
typedef struct {
    int status_code; const char *reason;
    h6s_header headers[H6S_MAX_HEADERS]; int header_count;
    const char *body; size_t body_length; const char *content_type;
} h6s_response;
typedef struct {
    char session_id[64]; char datetime[32]; char file_id[128]; char file_name[256];
    uint64_t index; uint64_t offset; uint64_t total_size;
} h6s_download_state;

typedef int (*h6s_frame_handler)(const http6_frame_view_t*, uint8_t*, size_t, size_t*, void*);
typedef int (*h6s_http_handler)(const h6s_request*, h6s_response*, void*);

typedef struct {
    uint16_t port; const char *bind_address; int backlog;
    h6s_http_handler handler; h6s_frame_handler frame_handler;
    void *user_data; const char *server_name;
    int allow_negotiation_fallback;
} h6s_server_config;

void h6s_server_config_init(h6s_server_config *cfg);
int  h6s_server_run(const h6s_server_config *cfg);
void h6s_server_stop(void);
int  h6s_response_add_header(h6s_response*, const char*, const char*);
int  h6s_download_parse(const char*, h6s_download_state*);
int  h6s_download_serialise(const h6s_download_state*, char*, size_t);
int  h6s_download_should_resume(uint64_t);
int  h6s_self_test(void);

#ifdef __cplusplus
}
#endif
#endif

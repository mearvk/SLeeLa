/* http7_server.h — SLeeLa HTTP 7.0 portable server (Linux · macOS · Windows 10+) */
#ifndef SLEELA_HTTP7_SERVER_H
#define SLEELA_HTTP7_SERVER_H
#include <stddef.h>
#include <stdint.h>
#include "http7_protocol.h"
#ifdef __cplusplus
extern "C" {
#endif

#define H7S_MAX_HEADER  (64*1024)
#define H7S_MAX_BODY    (8*1024*1024)
#define H7S_MAX_HEADERS 100
#define H7S_DOWNLOAD_THRESHOLD (50ULL*1024*1024)

typedef struct { char name[256]; char value[8192]; } h7s_header;
typedef struct {
    char method[16]; char uri[8192]; char version[16];
    h7s_header headers[H7S_MAX_HEADERS]; int header_count;
    char *body; size_t body_length;
    char sleela_port[64]; char service_id[128]; char op_id[128];
    char sleela_version[32]; char sleela_accept[256];
} h7s_request;
typedef struct {
    int status_code; const char *reason;
    h7s_header headers[H7S_MAX_HEADERS]; int header_count;
    const char *body; size_t body_length; const char *content_type;
} h7s_response;
typedef struct {
    char session_id[64]; char datetime[32]; char file_id[128]; char file_name[256];
    uint64_t index; uint64_t offset; uint64_t total_size;
} h7s_download_state;

typedef int (*h7s_frame_handler)(const http7_frame_view_t*, uint8_t*, size_t, size_t*, void*);
typedef int (*h7s_http_handler)(const h7s_request*, h7s_response*, void*);

typedef struct {
    uint16_t port; const char *bind_address; int backlog;
    h7s_http_handler handler; h7s_frame_handler frame_handler;
    void *user_data; const char *server_name;
    int allow_negotiation_fallback;
} h7s_server_config;

void h7s_server_config_init(h7s_server_config*);
int  h7s_server_run(const h7s_server_config*);
void h7s_server_stop(void);
int  h7s_download_parse(const char*, h7s_download_state*);
int  h7s_download_serialise(const h7s_download_state*, char*, size_t);
int  h7s_download_should_resume(uint64_t);
int  h7s_self_test(void);

#ifdef __cplusplus
}
#endif
#endif

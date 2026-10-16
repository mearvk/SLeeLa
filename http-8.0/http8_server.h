/* http8_server.h — SLeeLa HTTP 8.0 portable server (Linux · macOS · Windows 10+) */
#ifndef SLEELA_HTTP8_SERVER_H
#define SLEELA_HTTP8_SERVER_H
#include <stddef.h>
#include <stdint.h>
#include "http8_protocol.h"
#ifdef __cplusplus
extern "C" {
#endif
#define H8S_MAX_HEADER  (64*1024)
#define H8S_MAX_BODY    (8*1024*1024)
#define H8S_MAX_HEADERS 100
#define H8S_DOWNLOAD_THRESHOLD (50ULL*1024*1024)
typedef struct{char name[256];char value[8192];}h8s_header;
typedef struct{char method[16];char uri[8192];char version[16];
    h8s_header headers[H8S_MAX_HEADERS];int header_count;char*body;size_t body_length;
    char sleela_port[64];char service_id[128];char op_id[128];
    char sleela_version[32];char sleela_accept[256];}h8s_request;
typedef struct{int status_code;const char*reason;h8s_header headers[H8S_MAX_HEADERS];int header_count;
    const char*body;size_t body_length;const char*content_type;}h8s_response;
typedef struct{char session_id[64];char datetime[32];char file_id[128];char file_name[256];
    uint64_t index;uint64_t offset;uint64_t total_size;}h8s_download_state;
typedef int(*h8s_frame_handler)(const http8_frame_view_t*,uint8_t*,size_t,size_t*,void*);
typedef int(*h8s_http_handler)(const h8s_request*,h8s_response*,void*);
typedef struct{uint16_t port;const char*bind_address;int backlog;
    h8s_http_handler handler;h8s_frame_handler frame_handler;
    void*user_data;const char*server_name;int allow_negotiation_fallback;}h8s_server_config;
void h8s_server_config_init(h8s_server_config*);
int  h8s_server_run(const h8s_server_config*);
void h8s_server_stop(void);
int  h8s_download_parse(const char*,h8s_download_state*);
int  h8s_download_serialise(const h8s_download_state*,char*,size_t);
int  h8s_download_should_resume(uint64_t);
int  h8s_self_test(void);
#ifdef __cplusplus
}
#endif
#endif

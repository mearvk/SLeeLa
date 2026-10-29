#ifndef SLEELA_HTTP_API_H
#define SLEELA_HTTP_API_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SLEELA_HTTP_LOGICAL_PORT_BYTES 20u
#define SLEELA_HTTP_LOGICAL_PORT_DECIMAL_DIGITS 48u
#define SLEELA_HTTP_DOWNLOAD_THRESHOLD_BYTES (50ULL * 1000ULL * 1000ULL)

typedef struct { uint8_t bytes[20]; } sleela_http_port_t;
typedef enum { SLEELA_HTTP_1_0=10, SLEELA_HTTP_2_0=20, SLEELA_HTTP_2_1=21, SLEELA_HTTP_3_0=30 } sleela_http_version_t;
typedef enum { SLEELA_HTTP_MULTIPLEX_REQUEST=1, SLEELA_HTTP_MULTIPLEX_HTTP2_STREAM=2, SLEELA_HTTP_MULTIPLEX_HTTP3_STREAM=3 } sleela_http_multiplex_mode_t;
typedef enum { SLEELA_HTTP_DOWNLOAD_STANDARD=0, SLEELA_HTTP_DOWNLOAD_RESUMABLE=1 } sleela_http_download_mode_t;

typedef struct {
    char session_id[128];
    char datetime[64];
    char file_id[128];
    char file_name[512];
    uint64_t index;
    uint64_t offset;
    uint64_t total_size;
} sleela_http_download_resume_t;

typedef struct {
    sleela_http_version_t version;
    sleela_http_multiplex_mode_t multiplex_mode;
    sleela_http_port_t port;
    uint32_t service_id;
    uint32_t operation_id;
} sleela_http_route_t;

int sleela_http_port_from_decimal(sleela_http_port_t*, const char*);
int sleela_http_port_to_decimal(const sleela_http_port_t*, char*, size_t);
int sleela_http_port_is_valid(const sleela_http_port_t*);
int sleela_http_port_encode(const sleela_http_port_t*, uint8_t[20]);
int sleela_http_port_decode(sleela_http_port_t*, const uint8_t[20]);
sleela_http_multiplex_mode_t sleela_http_multiplex_mode_for(sleela_http_version_t);
sleela_http_download_mode_t sleela_http_download_mode_for_size(uint64_t);
void sleela_http_download_resume_init(sleela_http_download_resume_t*, const char*, const char*, const char*, const char*, uint64_t);
int sleela_http_download_resume_set_position(sleela_http_download_resume_t*, uint64_t, uint64_t);
int sleela_http_download_resume_matches(const sleela_http_download_resume_t*, const char*, uint64_t, uint64_t);

#ifdef __cplusplus
}
#endif
#endif
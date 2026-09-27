#ifndef HTTP90_H
#define HTTP90_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define HTTP90_VERSION "HTTP/9.0"
#define HTTP90_MAX_ID 128
#define HTTP90_MAX_FREQUENCY 64
typedef enum http90_status { HTTP90_OK=0, HTTP90_INVALID_ARGUMENT=400, HTTP90_UNAUTHORIZED=401, HTTP90_FORBIDDEN=403, HTTP90_INVALID_METADATA=422, HTTP90_NOT_CONFIGURED=428 } http90_status;
typedef struct http90_frequency { char value[HTTP90_MAX_FREQUENCY]; int enabled; } http90_frequency;
typedef struct http90_identity { char police_id[HTTP90_MAX_ID]; char international_id[HTTP90_MAX_ID]; } http90_identity;
typedef struct http90_monitoring { http90_frequency police_scanner_frequency; http90_frequency international_police_monitoring_frequency; } http90_monitoring;
typedef struct http90_packet_metadata {
    char protocol_grade[32];
    char prior_packet_metadata[4096];
    http90_identity identity;
    http90_monitoring monitoring;
    uint64_t sequence;
} http90_packet_metadata;
void http90_init(http90_packet_metadata *metadata);
int http90_set_identity(http90_packet_metadata *metadata,const char *police_id,const char *international_id);
int http90_set_frequency(http90_frequency *frequency,const char *value,int enabled);
int http90_validate(const http90_packet_metadata *metadata);
int http90_can_transmit(const http90_packet_metadata *metadata);
int http90_can_receive(const http90_packet_metadata *metadata);
#ifdef __cplusplus
}
#endif
#endif

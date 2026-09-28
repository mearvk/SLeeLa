#ifndef SLEELA_HTTP4_PROTOCOL_H
#define SLEELA_HTTP4_PROTOCOL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP4_VERSION 4u
#define HTTP4_MAX_PAYLOAD (16u * 1024u * 1024u)

typedef enum {
    HTTP4_FRAME_OPEN = 1,
    HTTP4_FRAME_DATA = 2,
    HTTP4_FRAME_END = 3,
    HTTP4_FRAME_RESET = 4,
    HTTP4_FRAME_WINDOW = 5,
    HTTP4_FRAME_PING = 6,
    HTTP4_FRAME_PONG = 7,
    HTTP4_FRAME_RESUME = 8,
    HTTP4_FRAME_CAPSULE = 9
} http4_frame_type_t;

enum {
    HTTP4_FLAG_FIN = 0x01,
    HTTP4_FLAG_ACK = 0x02,
    HTTP4_FLAG_URGENT = 0x04,
    HTTP4_FLAG_INCREMENTAL = 0x08,
    HTTP4_FLAG_RESUMABLE = 0x10
};

typedef struct {
    uint8_t version;
    uint8_t type;
    uint16_t flags;
    uint64_t stream_id;
    uint64_t request_id;
    uint64_t sequence;
    uint32_t payload_length;
} http4_frame_header_t;

typedef struct {
    http4_frame_header_t header;
    const uint8_t *payload;
} http4_frame_view_t;

#define HTTP4_FRAME_HEADER_SIZE 32u

int http4_frame_encode(const http4_frame_header_t *header,
                       const uint8_t *payload,
                       uint8_t *out,
                       size_t out_size,
                       size_t *written);

int http4_frame_decode(const uint8_t *wire,
                       size_t wire_size,
                       http4_frame_view_t *out,
                       size_t *consumed);

int http4_frame_validate(const http4_frame_header_t *header, size_t available_payload);

const char *http4_frame_type_name(http4_frame_type_t type);

#ifdef __cplusplus
}
#endif

#endif

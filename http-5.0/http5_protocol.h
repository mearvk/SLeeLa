#ifndef SLEELA_HTTP5_PROTOCOL_H
#define SLEELA_HTTP5_PROTOCOL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP5_VERSION 5u
#define HTTP5_MAX_PAYLOAD (16u * 1024u * 1024u)

typedef enum {
    HTTP5_FRAME_OPEN = 1, HTTP5_FRAME_DATA = 2, HTTP5_FRAME_END = 3,
    HTTP5_FRAME_RESET = 4, HTTP5_FRAME_WINDOW = 5, HTTP5_FRAME_PING = 6,
    HTTP5_FRAME_PONG = 7, HTTP5_FRAME_RESUME = 8, HTTP5_FRAME_CAPSULE = 9,
    HTTP5_FRAME_FRIENDS_PACK = 10, HTTP5_FRAME_BONUS_OFFER = 11,
    HTTP5_FRAME_FP_UPDATE = 12, HTTP5_FRAME_AUDIT = 13
} http5_frame_type_t;

enum {
    HTTP5_FLAG_FIN = 0x01,
    HTTP5_FLAG_ACK = 0x02,
    HTTP5_FLAG_URGENT = 0x04,
    HTTP5_FLAG_INCREMENTAL = 0x08,
    HTTP5_FLAG_RESUMABLE = 0x10,
    HTTP5_FLAG_OPTIONAL = 0x20
};

#define HTTP5_DEFINED_FLAGS ((uint16_t)(HTTP5_FLAG_FIN | HTTP5_FLAG_ACK | \
    HTTP5_FLAG_URGENT | HTTP5_FLAG_INCREMENTAL | HTTP5_FLAG_RESUMABLE | \
    HTTP5_FLAG_OPTIONAL))

typedef struct {
    uint8_t version;
    uint8_t type;
    uint16_t flags;
    uint64_t stream_id;
    uint64_t request_id;
    uint64_t sequence;
    uint32_t payload_length;
} http5_frame_header_t;

typedef struct {
    http5_frame_header_t header;
    const uint8_t *payload;
} http5_frame_view_t;

#define HTTP5_FRAME_HEADER_SIZE 32u

/* Validate the fixed header and the number of payload bytes available. */
int http5_frame_validate(const http5_frame_header_t *header,
                         size_t available_payload);

/* Encode one frame; on failure, *written is not changed. */
int http5_frame_encode(const http5_frame_header_t *header,
                       const uint8_t *payload,
                       uint8_t *out,
                       size_t out_size,
                       size_t *written);

/* Decode one frame at the beginning of wire; trailing bytes remain unconsumed. */
int http5_frame_decode(const uint8_t *wire,
                       size_t wire_size,
                       http5_frame_view_t *out,
                       size_t *consumed);

const char *http5_frame_type_name(http5_frame_type_t type);

typedef struct {
    uint64_t friend_id;
    uint32_t fp_remaining;
    uint32_t bonus_count;
} http5_friends_pack_t;

int http5_fp_can_offer(const http5_friends_pack_t *pack);
int http5_fp_consume(http5_friends_pack_t *pack);

#ifdef __cplusplus
}
#endif
#endif

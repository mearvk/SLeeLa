#ifndef SLEELA_HTTP6_PROTOCOL_H
#define SLEELA_HTTP6_PROTOCOL_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define HTTP6_VERSION 6u
#define HTTP6_MAX_PAYLOAD (16u * 1024u * 1024u)
typedef enum {
 HTTP6_FRAME_OPEN=1, HTTP6_FRAME_DATA=2, HTTP6_FRAME_END=3, HTTP6_FRAME_RESET=4,
 HTTP6_FRAME_WINDOW=5, HTTP6_FRAME_PING=6, HTTP6_FRAME_PONG=7, HTTP6_FRAME_RESUME=8,
 HTTP6_FRAME_CAPSULE=9, HTTP6_FRAME_FRIENDS_PACK=10, HTTP6_FRAME_BONUS_OFFER=11,
 HTTP6_FRAME_FP_UPDATE=12, HTTP6_FRAME_AUDIT=13,
 HTTP6_FRAME_CONSOLIDATED_FRIENDS_BET=14, HTTP6_FRAME_TEAMSTER_DEBATE=15,
 HTTP6_FRAME_CONSOLIDATE_IQ=16, HTTP6_FRAME_TEAM_AREA=17,
 HTTP6_FRAME_DEBATE_TOPIC=18, HTTP6_FRAME_DEBATE_POSITION=19,
 HTTP6_FRAME_RECIPIENT_LABEL=20
} http6_frame_type_t;
enum { HTTP6_FLAG_FIN=0x01, HTTP6_FLAG_ACK=0x02, HTTP6_FLAG_URGENT=0x04,
 HTTP6_FLAG_INCREMENTAL=0x08, HTTP6_FLAG_RESUMABLE=0x10, HTTP6_FLAG_OPTIONAL=0x20,
 HTTP6_FLAG_USER_AUTHORED=0x40 };
typedef struct { uint8_t version; uint8_t type; uint16_t flags; uint64_t stream_id;
 uint64_t request_id; uint64_t sequence; uint32_t payload_length; } http6_frame_header_t;
typedef struct { http6_frame_header_t header; const uint8_t *payload; } http6_frame_view_t;
#define HTTP6_FRAME_HEADER_SIZE 32u
int http6_frame_validate(const http6_frame_header_t *, size_t);
int http6_frame_encode(const http6_frame_header_t *, const uint8_t *, uint8_t *, size_t, size_t *);
int http6_frame_decode(const uint8_t *, size_t, http6_frame_view_t *, size_t *);
const char *http6_frame_type_name(http6_frame_type_t);
#ifdef __cplusplus
}
#endif
#endif

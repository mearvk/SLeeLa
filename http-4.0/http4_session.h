#ifndef SLEELA_HTTP4_SESSION_H
#define SLEELA_HTTP4_SESSION_H
#include "http4_protocol.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP4_SESSION_MAX_STREAMS 256u

typedef struct {
    uint64_t stream_id;
    uint64_t request_id;
    uint64_t next_sequence;
    uint64_t received_bytes;
    int active;
    int receive_closed;
    int reset;
} http4_stream_state_t;

typedef struct {
    http4_stream_state_t streams[HTTP4_SESSION_MAX_STREAMS];
    size_t active_streams;
    uint64_t max_stream_bytes;
    uint64_t max_total_received_bytes;
    uint64_t total_received_bytes;
} http4_session_state_t;

void http4_session_init(http4_session_state_t *s, uint64_t max_stream_bytes,
                        uint64_t max_total_received_bytes);
/* Accepts one already-decoded inbound frame. OPEN must be sequence 0. */
int http4_session_accept(http4_session_state_t *s,
                         const http4_frame_view_t *frame);
const http4_stream_state_t *http4_session_find(const http4_session_state_t *s,
                                                uint64_t stream_id);
#ifdef __cplusplus
}
#endif
#endif

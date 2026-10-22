#include "http4_session.h"
#include <string.h>

void http4_session_init(http4_session_state_t *s, uint64_t max_stream_bytes,
                        uint64_t max_total_received_bytes) {
    if (!s) return;
    memset(s, 0, sizeof(*s));
    s->max_stream_bytes = max_stream_bytes;
    s->max_total_received_bytes = max_total_received_bytes;
}

const http4_stream_state_t *http4_session_find(const http4_session_state_t *s,
                                                uint64_t stream_id) {
    if (!s) return NULL;
    for (size_t i = 0; i < HTTP4_SESSION_MAX_STREAMS; ++i)
        if (s->streams[i].active && s->streams[i].stream_id == stream_id)
            return &s->streams[i];
    return NULL;
}

int http4_session_accept(http4_session_state_t *s,
                         const http4_frame_view_t *frame) {
    if (!s || !frame) return -1;
    const http4_frame_header_t *h = &frame->header;
    if (http4_frame_validate(h, h->payload_length) != 0) return -2;

    http4_stream_state_t *st = NULL;
    size_t free_slot = HTTP4_SESSION_MAX_STREAMS;
    for (size_t i = 0; i < HTTP4_SESSION_MAX_STREAMS; ++i) {
        if (s->streams[i].active && s->streams[i].stream_id == h->stream_id) {
            st = &s->streams[i];
            break;
        }
        if (!s->streams[i].active && free_slot == HTTP4_SESSION_MAX_STREAMS)
            free_slot = i;
    }

    if (h->type == HTTP4_FRAME_OPEN) {
        if (st || free_slot == HTTP4_SESSION_MAX_STREAMS || h->sequence != 0)
            return -3;
        if (s->active_streams >= HTTP4_SESSION_MAX_STREAMS) return -4;
        st = &s->streams[free_slot];
        memset(st, 0, sizeof(*st));
        st->active = 1;
        st->stream_id = h->stream_id;
        st->request_id = h->request_id;
        st->next_sequence = 1;
        ++s->active_streams;
        return 0;
    }

    if (!st || st->reset || st->receive_closed) return -5;
    if (h->request_id != st->request_id || h->sequence != st->next_sequence)
        return -6;
    if (st->next_sequence == UINT64_MAX) return -7;

    if (h->type == HTTP4_FRAME_DATA) {
        uint64_t n = h->payload_length;
        if (n > UINT64_MAX - st->received_bytes ||
            n > UINT64_MAX - s->total_received_bytes) return -8;
        if (s->max_stream_bytes && n > s->max_stream_bytes - st->received_bytes)
            return -9;
        if (s->max_total_received_bytes &&
            n > s->max_total_received_bytes - s->total_received_bytes) return -10;
        st->received_bytes += n;
        s->total_received_bytes += n;
    } else if (h->type == HTTP4_FRAME_END) {
        st->receive_closed = 1;
    } else if (h->type == HTTP4_FRAME_RESET) {
        st->reset = 1;
        st->receive_closed = 1;
    }
    ++st->next_sequence;
    return 0;
}

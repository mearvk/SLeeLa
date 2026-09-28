#include "http4_protocol.h"

#include <string.h>

static void put16(uint8_t *p, uint16_t v) {
    p[0] = (uint8_t)(v >> 8); p[1] = (uint8_t)v;
}
static void put32(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)(v >> 24); p[1] = (uint8_t)(v >> 16);
    p[2] = (uint8_t)(v >> 8); p[3] = (uint8_t)v;
}
static void put64(uint8_t *p, uint64_t v) {
    for (int i = 7; i >= 0; --i) { p[i] = (uint8_t)v; v >>= 8; }
}
static uint16_t get16(const uint8_t *p) {
    return (uint16_t)(((uint16_t)p[0] << 8) | p[1]);
}
static uint32_t get32(const uint8_t *p) {
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
           ((uint32_t)p[2] << 8) | p[3];
}
static uint64_t get64(const uint8_t *p) {
    uint64_t v = 0;
    for (int i = 0; i < 8; ++i) v = (v << 8) | p[i];
    return v;
}

int http4_frame_validate(const http4_frame_header_t *h, size_t available_payload) {
    if (!h) return -1;
    if (h->version != HTTP4_VERSION) return -2;
    if (h->type < HTTP4_FRAME_OPEN || h->type > HTTP4_FRAME_CAPSULE) return -3;
    if (h->payload_length > HTTP4_MAX_PAYLOAD) return -4;
    if ((size_t)h->payload_length > available_payload) return -5;
    return 0;
}

int http4_frame_encode(const http4_frame_header_t *h, const uint8_t *payload,
                       uint8_t *out, size_t out_size, size_t *written) {
    if (!h || !out || !written) return -1;
    if (http4_frame_validate(h, h->payload_length) != 0) return -2;
    if (h->payload_length && !payload) return -3;
    if (out_size < HTTP4_FRAME_HEADER_SIZE + h->payload_length) return -4;

    out[0] = h->version;
    out[1] = h->type;
    put16(out + 2, h->flags);
    put64(out + 4, h->stream_id);
    put64(out + 12, h->request_id);
    put64(out + 20, h->sequence);
    put32(out + 28, h->payload_length);
    if (h->payload_length) memcpy(out + HTTP4_FRAME_HEADER_SIZE, payload, h->payload_length);
    *written = HTTP4_FRAME_HEADER_SIZE + h->payload_length;
    return 0;
}

int http4_frame_decode(const uint8_t *wire, size_t wire_size,
                       http4_frame_view_t *out, size_t *consumed) {
    if (!wire || !out || !consumed) return -1;
    if (wire_size < HTTP4_FRAME_HEADER_SIZE) return -2;

    http4_frame_header_t h;
    h.version = wire[0];
    h.type = wire[1];
    h.flags = get16(wire + 2);
    h.stream_id = get64(wire + 4);
    h.request_id = get64(wire + 12);
    h.sequence = get64(wire + 20);
    h.payload_length = get32(wire + 28);

    size_t payload_available = wire_size - HTTP4_FRAME_HEADER_SIZE;
    if (http4_frame_validate(&h, payload_available) != 0) return -3;

    out->header = h;
    out->payload = wire + HTTP4_FRAME_HEADER_SIZE;
    *consumed = HTTP4_FRAME_HEADER_SIZE + h.payload_length;
    return 0;
}

const char *http4_frame_type_name(http4_frame_type_t type) {
    switch (type) {
        case HTTP4_FRAME_OPEN: return "OPEN";
        case HTTP4_FRAME_DATA: return "DATA";
        case HTTP4_FRAME_END: return "END";
        case HTTP4_FRAME_RESET: return "RESET";
        case HTTP4_FRAME_WINDOW: return "WINDOW";
        case HTTP4_FRAME_PING: return "PING";
        case HTTP4_FRAME_PONG: return "PONG";
        case HTTP4_FRAME_RESUME: return "RESUME";
        case HTTP4_FRAME_CAPSULE: return "CAPSULE";
        default: return "UNKNOWN";
    }
}

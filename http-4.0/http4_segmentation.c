#include "http4_segmentation.h"

#include <limits.h>
#include <string.h>

static void put16(uint8_t *p, uint16_t v) {
    p[0] = (uint8_t)(v >> 8);
    p[1] = (uint8_t)v;
}

static void put32(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)(v >> 24);
    p[1] = (uint8_t)(v >> 16);
    p[2] = (uint8_t)(v >> 8);
    p[3] = (uint8_t)v;
}

static void put64(uint8_t *p, uint64_t v) {
    for (int i = 7; i >= 0; --i) {
        p[i] = (uint8_t)v;
        v >>= 8;
    }
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

static int checked_u32(size_t value, uint32_t *out) {
    if (value > UINT32_MAX || !out) return -1;
    *out = (uint32_t)value;
    return 0;
}

size_t http4_segment_count(size_t total_length, size_t mtu) {
    if (mtu <= HTTP4_FRAME_HEADER_SIZE + HTTP4_SEGMENT_HEADER_SIZE) return 0;
    size_t capacity = mtu - HTTP4_FRAME_HEADER_SIZE - HTTP4_SEGMENT_HEADER_SIZE;
    if (total_length == 0) return 1;
    return (total_length + capacity - 1u) / capacity;
}

int http4_segment_frame_encode(uint64_t stream_id,
                               uint64_t request_id,
                               uint64_t sequence,
                               uint64_t segment_id,
                               uint32_t segment_index,
                               const uint8_t *data,
                               size_t total_length,
                               size_t mtu,
                               uint8_t *out,
                               size_t out_size,
                               size_t *written) {
    if (!out || !written) return -1;
    if (total_length > UINT32_MAX) return -2;
    if (segment_index >= http4_segment_count(total_length, mtu)) return -3;

    size_t capacity = mtu - HTTP4_FRAME_HEADER_SIZE - HTTP4_SEGMENT_HEADER_SIZE;
    size_t offset = (size_t)segment_index * capacity;
    if (segment_index != 0 && offset / capacity != segment_index) return -4;
    if (offset > total_length) return -5;

    size_t data_length = total_length - offset;
    if (data_length > capacity) data_length = capacity;
    if (data_length && !data) return -6;

    uint32_t total32, offset32, data32;
    if (checked_u32(total_length, &total32) ||
        checked_u32(offset, &offset32) ||
        checked_u32(data_length, &data32)) return -7;

    size_t count = http4_segment_count(total_length, mtu);
    if (count > UINT32_MAX) return -8;
    if (sequence > UINT64_MAX - segment_index) return -9;

    uint16_t segment_flags = 0;
    if (segment_index == 0) segment_flags |= HTTP4_SEGMENT_FLAG_FIRST;
    if ((size_t)segment_index + 1u == count) segment_flags |= HTTP4_SEGMENT_FLAG_LAST;

    http4_frame_header_t frame = {
        HTTP4_VERSION,
        HTTP4_FRAME_DATA,
        (uint16_t)(HTTP4_FLAG_INCREMENTAL | HTTP4_FLAG_RESUMABLE |
                   (((segment_flags & HTTP4_SEGMENT_FLAG_LAST) != 0) ? HTTP4_FLAG_FIN : 0)),
        stream_id,
        request_id,
        sequence + segment_index,
        (uint32_t)(HTTP4_SEGMENT_HEADER_SIZE + data_length)
    };

    if (frame.payload_length > HTTP4_MAX_PAYLOAD ||
        out_size < HTTP4_FRAME_HEADER_SIZE + frame.payload_length) return -10;

    if (http4_frame_encode(&frame, NULL, out, out_size, written) != -3) {
        /* The protocol encoder requires a payload pointer. Build the header here,
           then append the segmentation header and segment data. */
    }

    out[0] = frame.version;
    out[1] = frame.type;
    put16(out + 2, frame.flags);
    put64(out + 4, frame.stream_id);
    put64(out + 12, frame.request_id);
    put64(out + 20, frame.sequence);
    put32(out + 28, frame.payload_length);

    uint8_t *p = out + HTTP4_FRAME_HEADER_SIZE;
    put32(p + 0, HTTP4_SEGMENT_MAGIC);
    p[4] = HTTP4_SEGMENT_VERSION;
    p[5] = (uint8_t)segment_flags;
    put16(p + 6, 0);
    put64(p + 8, segment_id);
    put32(p + 16, segment_index);
    put32(p + 20, (uint32_t)count);
    put32(p + 24, total32);
    put32(p + 28, offset32);
    /* data_length follows the fixed 32-byte metadata block. */
    /* The header layout reserves bytes 28..31 for offset; data_length is
       represented by the enclosing frame payload length minus 32. */
    if (data_length) memcpy(p + HTTP4_SEGMENT_HEADER_SIZE, data + offset, data_length);

    *written = HTTP4_FRAME_HEADER_SIZE + frame.payload_length;
    return 0;
}

int http4_segment_frame_decode(const uint8_t *wire,
                               size_t wire_size,
                               http4_frame_view_t *frame,
                               http4_segment_view_t *segment,
                               size_t *consumed) {
    if (!wire || !frame || !segment || !consumed) return -1;

    size_t used = 0;
    if (http4_frame_decode(wire, wire_size, frame, &used) != 0) return -2;
    if (frame->header.type != HTTP4_FRAME_DATA) return -3;
    if (frame->header.payload_length < HTTP4_SEGMENT_HEADER_SIZE) return -4;

    const uint8_t *p = frame->payload;
    http4_segment_header_t h;
    h.magic = get32(p + 0);
    h.version = p[4];
    h.flags = p[5];
    h.reserved = get16(p + 6);
    h.segment_id = get64(p + 8);
    h.segment_index = get32(p + 16);
    h.segment_count = get32(p + 20);
    h.total_length = get32(p + 24);
    h.offset = get32(p + 28);
    h.data_length = frame->header.payload_length - HTTP4_SEGMENT_HEADER_SIZE;

    if (h.magic != HTTP4_SEGMENT_MAGIC || h.version != HTTP4_SEGMENT_VERSION) return -5;
    if (h.reserved != 0 || h.segment_count == 0) return -6;
    if (h.segment_index >= h.segment_count) return -7;
    if (h.data_length > h.total_length || h.offset > h.total_length ||
        h.data_length > h.total_length - h.offset) return -8;

    if ((h.flags & HTTP4_SEGMENT_FLAG_FIRST) != 0 && h.segment_index != 0) return -9;
    if ((h.flags & HTTP4_SEGMENT_FLAG_LAST) != 0 &&
        h.segment_index + 1u != h.segment_count) return -10;
    if ((h.segment_index == 0) != ((h.flags & HTTP4_SEGMENT_FLAG_FIRST) != 0)) return -11;
    if ((h.segment_index + 1u == h.segment_count) !=
        ((h.flags & HTTP4_SEGMENT_FLAG_LAST) != 0)) return -12;
    if ((h.flags & ~(HTTP4_SEGMENT_FLAG_FIRST | HTTP4_SEGMENT_FLAG_LAST)) != 0) return -13;
    if ((h.flags & HTTP4_SEGMENT_FLAG_LAST) != 0 &&
        (frame->header.flags & HTTP4_FLAG_FIN) == 0) return -14;
    if ((h.flags & HTTP4_SEGMENT_FLAG_LAST) == 0 &&
        (frame->header.flags & HTTP4_FLAG_FIN) != 0) return -15;

    segment->header = h;
    segment->data = p + HTTP4_SEGMENT_HEADER_SIZE;
    *consumed = used;
    return 0;
}

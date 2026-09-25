#ifndef SLEELA_HTTP4_SEGMENTATION_H
#define SLEELA_HTTP4_SEGMENTATION_H

#include <stddef.h>
#include <stdint.h>
#include "http4_protocol.h"

#define HTTP4_SEGMENT_VERSION 1u
#define HTTP4_SEGMENT_MAGIC 0x48345347u
#define HTTP4_SEGMENT_HEADER_SIZE 32u
#define HTTP4_SEGMENT_FLAG_FIRST 0x01u
#define HTTP4_SEGMENT_FLAG_LAST 0x02u

typedef struct {
    uint32_t magic;
    uint8_t version;
    uint8_t flags;
    uint16_t reserved;
    uint64_t segment_id;
    uint32_t segment_index;
    uint32_t segment_count;
    uint32_t total_length;
    uint32_t offset;
    uint32_t data_length; /* derived from the enclosing frame payload */
} http4_segment_header_t;

typedef struct {
    http4_segment_header_t header;
    uint64_t stream_id;
    uint64_t request_id;
    uint64_t sequence;
    const uint8_t *data;
} http4_segment_view_t;

size_t http4_segment_count(size_t total_length, size_t mtu);

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
                               size_t *written);

int http4_segment_frame_decode(const uint8_t *wire,
                               size_t wire_size,
                               http4_frame_view_t *frame,
                               http4_segment_view_t *segment,
                               size_t *consumed);

#endif

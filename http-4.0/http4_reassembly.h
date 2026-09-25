#ifndef SLEELA_HTTP4_REASSEMBLY_H
#define SLEELA_HTTP4_REASSEMBLY_H

#include <stddef.h>
#include <stdint.h>
#include "http4_segmentation.h"

typedef struct {
    uint64_t stream_id;
    uint64_t request_id;
    uint64_t segment_id;
    uint32_t total_length;
    uint32_t segment_count;
    uint32_t received_segments;
    size_t received_bytes;
    uint8_t *buffer;
    uint8_t *bitmap;
} http4_reassembly_t;

int http4_reassembly_init(http4_reassembly_t *state,
                          size_t total_length,
                          uint32_t segment_count,
                          uint64_t stream_id,
                          uint64_t request_id,
                          uint64_t segment_id);

void http4_reassembly_reset(http4_reassembly_t *state);
int http4_reassembly_add(http4_reassembly_t *state,
                         const http4_segment_view_t *segment);
int http4_reassembly_complete(const http4_reassembly_t *state);
const uint8_t *http4_reassembly_data(const http4_reassembly_t *state);
size_t http4_reassembly_size(const http4_reassembly_t *state);

#endif

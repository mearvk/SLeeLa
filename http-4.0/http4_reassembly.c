#include "http4_reassembly.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>

static int bitmap_test(const uint8_t *bitmap, uint32_t index) {
    return (bitmap[index / 8u] & (uint8_t)(1u << (index % 8u))) != 0;
}
static void bitmap_set(uint8_t *bitmap, uint32_t index) {
    bitmap[index / 8u] |= (uint8_t)(1u << (index % 8u));
}
static int coverage_test(const uint8_t *coverage, uint32_t index) {
    return (coverage[index / 8u] & (uint8_t)(1u << (index % 8u))) != 0;
}
static void coverage_set(uint8_t *coverage, uint32_t index) {
    coverage[index / 8u] |= (uint8_t)(1u << (index % 8u));
}

int http4_reassembly_init(http4_reassembly_t *state, size_t total_length,
                          uint32_t segment_count, uint64_t stream_id,
                          uint64_t request_id, uint64_t segment_id) {
    if (!state || total_length > HTTP4_MAX_PAYLOAD || total_length > UINT32_MAX ||
        segment_count == 0 || segment_count > HTTP4_MAX_REASSEMBLY_SEGMENTS) return -1;

    const size_t bitmap_bytes = ((size_t)segment_count + 7u) / 8u;
    const size_t coverage_bytes = (total_length + 7u) / 8u;
    if (bitmap_bytes > SIZE_MAX - total_length ||
        coverage_bytes > SIZE_MAX - total_length - bitmap_bytes) return -2;

    memset(state, 0, sizeof(*state));
    state->buffer = (uint8_t *)malloc(total_length ? total_length : 1u);
    state->bitmap = (uint8_t *)calloc(bitmap_bytes ? bitmap_bytes : 1u, 1u);
    state->coverage = (uint8_t *)calloc(coverage_bytes ? coverage_bytes : 1u, 1u);
    if (!state->buffer || !state->bitmap || !state->coverage) {
        http4_reassembly_reset(state);
        return -3;
    }
    state->stream_id = stream_id;
    state->request_id = request_id;
    state->segment_id = segment_id;
    state->total_length = (uint32_t)total_length;
    state->segment_count = segment_count;
    return 0;
}

void http4_reassembly_reset(http4_reassembly_t *state) {
    if (!state) return;
    free(state->buffer);
    free(state->bitmap);
    free(state->coverage);
    memset(state, 0, sizeof(*state));
}

int http4_reassembly_add(http4_reassembly_t *state,
                         const http4_segment_view_t *segment) {
    if (!state || !segment || !state->buffer || !state->bitmap || !state->coverage) return -1;
    const http4_segment_header_t *h = &segment->header;
    if (h->magic != HTTP4_SEGMENT_MAGIC || h->version != HTTP4_SEGMENT_VERSION) return -2;
    if (segment->stream_id != state->stream_id ||
        segment->request_id != state->request_id ||
        h->segment_id != state->segment_id ||
        h->segment_count != state->segment_count ||
        h->total_length != state->total_length) return -3;
    if (h->segment_index >= state->segment_count) return -4;
    if ((size_t)h->offset > state->total_length ||
        (size_t)h->data_length > state->total_length - h->offset) return -5;
    if (h->data_length && !segment->data) return -6;
    if (bitmap_test(state->bitmap, h->segment_index)) return -7;

    for (uint32_t i = 0; i < h->data_length; ++i)
        if (coverage_test(state->coverage, h->offset + i)) return -8;

    if (h->data_length) {
        memcpy(state->buffer + h->offset, segment->data, h->data_length);
        for (uint32_t i = 0; i < h->data_length; ++i)
            coverage_set(state->coverage, h->offset + i);
    }
    bitmap_set(state->bitmap, h->segment_index);
    state->received_segments++;
    state->received_bytes += h->data_length;
    return 0;
}

int http4_reassembly_complete(const http4_reassembly_t *state) {
    return state && state->buffer && state->bitmap && state->coverage &&
           state->received_segments == state->segment_count &&
           state->received_bytes == state->total_length;
}
const uint8_t *http4_reassembly_data(const http4_reassembly_t *state) {
    return state && http4_reassembly_complete(state) ? state->buffer : NULL;
}
size_t http4_reassembly_size(const http4_reassembly_t *state) {
    return http4_reassembly_complete(state) ? state->total_length : 0;
}

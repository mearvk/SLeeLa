#include "http4_segmentation.h"
#include "http4_reassembly.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    const uint8_t source[] =
        "SLeeLa HTTP 4.0 segmentation must tolerate a constrained carrier MTU "
        "without relying on IP fragmentation.";
    const size_t source_len = sizeof(source) - 1;
    const size_t mtu = 96;
    const size_t count = http4_segment_count(source_len, mtu);
    assert(count > 1);

    http4_reassembly_t reassembly;
    assert(http4_reassembly_init(&reassembly, source_len, (uint32_t)count,
                                 7, 42, 9001) == 0);

    uint8_t wire[128];
    for (uint32_t i = 0; i < (uint32_t)count; ++i) {
        size_t written = 0, consumed = 0;
        http4_frame_view_t frame;
        http4_segment_view_t segment;
        assert(http4_segment_frame_encode(7, 42, 100, 9001, i,
                                          source, source_len, mtu,
                                          wire, sizeof(wire), &written) == 0);
        assert(written <= mtu);
        assert(http4_segment_frame_decode(wire, written, &frame, &segment,
                                          &consumed) == 0);
        assert(consumed == written);
        assert(segment.stream_id == 7);
        assert(segment.request_id == 42);
        assert(segment.header.segment_index == i);
        assert(segment.header.segment_count == count);
        assert(http4_reassembly_add(&reassembly, &segment) == 0);
    }

    assert(http4_reassembly_complete(&reassembly));
    assert(http4_reassembly_size(&reassembly) == source_len);
    assert(memcmp(http4_reassembly_data(&reassembly), source, source_len) == 0);

    {
        size_t written = 0, consumed = 0;
        http4_frame_view_t frame;
        http4_segment_view_t segment;
        assert(http4_segment_frame_encode(7, 42, 100, 9001, 0,
                                          source, source_len, mtu,
                                          wire, sizeof(wire), &written) == 0);
        assert(http4_segment_frame_decode(wire, written, &frame, &segment,
                                          &consumed) == 0);
        assert(http4_reassembly_add(&reassembly, &segment) != 0);
    }

    http4_reassembly_reset(&reassembly);
    puts("SLeeLa HTTP 4.0 segmentation/reassembly self-test: PASS");
    return 0;
}

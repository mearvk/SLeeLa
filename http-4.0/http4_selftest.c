#include "http4_protocol.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    const uint8_t payload[] = "incremental payload";
    http4_frame_header_t h = {
        HTTP4_VERSION, HTTP4_FRAME_DATA,
        HTTP4_FLAG_INCREMENTAL | HTTP4_FLAG_RESUMABLE,
        7, 42, 3, (uint32_t)(sizeof(payload) - 1)
    };
    uint8_t wire[128];
    size_t written = 0, consumed = 0;
    http4_frame_view_t view;

    assert(http4_frame_encode(&h, payload, wire, sizeof(wire), &written) == 0);
    {
        const uint8_t expected_header[HTTP4_FRAME_HEADER_SIZE] = {
            0x04,0x02,0x00,0x18,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2a,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,
            0x00,0x00,0x00,0x13
        };
        assert(memcmp(wire, expected_header, sizeof(expected_header)) == 0);
    }
    assert(written == HTTP4_FRAME_HEADER_SIZE + sizeof(payload) - 1);
    assert(http4_frame_decode(wire, written, &view, &consumed) == 0);
    assert(consumed == written);
    assert(view.header.stream_id == 7);
    assert(view.header.request_id == 42);
    assert(view.header.sequence == 3);
    assert(view.header.flags & HTTP4_FLAG_INCREMENTAL);
    assert(memcmp(view.payload, payload, sizeof(payload) - 1) == 0);
    assert(strcmp(http4_frame_type_name(HTTP4_FRAME_RESUME), "RESUME") == 0);

    /* Truncated header and payload must fail closed. */
    assert(http4_frame_decode(wire, HTTP4_FRAME_HEADER_SIZE - 1, &view, &consumed) != 0);
    assert(http4_frame_decode(wire, written - 1, &view, &consumed) != 0);

    /* Unknown version and reserved flags are rejected. */
    wire[0] = 3;
    assert(http4_frame_decode(wire, written, &view, &consumed) != 0);
    wire[0] = HTTP4_VERSION;
    wire[3] |= 0x80; /* high bit of FLAGS */
    assert(http4_frame_decode(wire, written, &view, &consumed) != 0);

    /* Decoder consumes one frame, not trailing bytes. */
    wire[3] &= 0x1f;
    wire[written] = 0xAA;
    assert(http4_frame_decode(wire, written + 1, &view, &consumed) == 0);
    assert(consumed == written);

    puts("SLeeLa HTTP 4.0 frame self-test: PASS");
    return 0;
}

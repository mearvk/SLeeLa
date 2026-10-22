#include "http4_protocol.h"
#include "http4_segmentation.h"
#include "http4_packet_context.h"
#include <stddef.h>
#include <stdint.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    http4_frame_view_t frame;
    size_t consumed = 0;
    if (http4_frame_decode(data, size, &frame, &consumed) == 0) {
        http4_segment_view_t segment;
        size_t segment_consumed = 0;
        (void)http4_segment_frame_decode(data, size, &frame, &segment,
                                         &segment_consumed);
    }
    http4_packet_context_t context;
    (void)http4_packet_context_parse(data, size, &context);
    return 0;
}

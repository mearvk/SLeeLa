#include "http4_packet_payload.h"
size_t http4_packet_payload_wrap(const http4_packet_context_t *ctx,
                                 const uint8_t *content, size_t content_len,
                                 uint8_t *out, size_t out_cap)
{
    if (!ctx || !out || content_len > SIZE_MAX - HTTP4_PACKET_CONTEXT_SIZE) return 0;
    if (out_cap < HTTP4_PACKET_CONTEXT_SIZE + content_len) return 0;
    if (!http4_packet_context_serialize(ctx, out, out_cap)) return 0;
    if (content_len && !content) return 0;
    for (size_t i=0;i<content_len;++i)
        out[HTTP4_PACKET_CONTEXT_SIZE+i] = content[i];
    return HTTP4_PACKET_CONTEXT_SIZE + content_len;
}
int http4_packet_payload_unwrap(const uint8_t *wire, size_t wire_len,
                                http4_packet_context_t *ctx,
                                const uint8_t **content, size_t *content_len)
{
    if (!wire || !ctx || !content || !content_len ||
        wire_len < HTTP4_PACKET_CONTEXT_SIZE) return -1;
    if (http4_packet_context_parse(wire, wire_len, ctx) != 0) return -2;
    *content = wire + HTTP4_PACKET_CONTEXT_SIZE;
    *content_len = wire_len - HTTP4_PACKET_CONTEXT_SIZE;
    return 0;
}

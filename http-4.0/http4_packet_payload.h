#ifndef SLEELA_HTTP4_PACKET_PAYLOAD_H
#define SLEELA_HTTP4_PACKET_PAYLOAD_H
#include <stddef.h>
#include <stdint.h>
#include "http4_packet_context.h"
#ifdef __cplusplus
extern "C" {
#endif
size_t http4_packet_payload_wrap(const http4_packet_context_t *ctx,
                                 const uint8_t *content, size_t content_len,
                                 uint8_t *out, size_t out_cap);
int http4_packet_payload_unwrap(const uint8_t *wire, size_t wire_len,
                                http4_packet_context_t *ctx,
                                const uint8_t **content, size_t *content_len);
#ifdef __cplusplus
}
#endif
#endif

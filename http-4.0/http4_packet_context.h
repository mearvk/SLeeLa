#ifndef SLEELA_HTTP4_PACKET_CONTEXT_H
#define SLEELA_HTTP4_PACKET_CONTEXT_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP4_EDU_TOKEN_VERSION 1u
#define HTTP4_EDU_LEVEL_12_OR_HIGHER 12u
#define HTTP4_JURISDICTION_UNKNOWN 0u
#define HTTP4_EDU_TOKEN_SIZE 4u

#define HTTP4_BASKET_ITEMS 14u
#define HTTP4_BASKET_BLOCK_SIZE (4u + HTTP4_BASKET_ITEMS * 12u)
#define HTTP4_PACKET_CONTEXT_SIZE (HTTP4_EDU_TOKEN_SIZE + HTTP4_BASKET_BLOCK_SIZE)

typedef struct {
    uint8_t version;
    uint8_t level;
    uint8_t jurisdiction;
    uint8_t flags;
} http4_education_token_t;

typedef struct {
    http4_education_token_t education;
    uint8_t basket[HTTP4_BASKET_BLOCK_SIZE];
} http4_packet_context_t;

void http4_education_token_default(http4_education_token_t *token);
int http4_education_token_set_jurisdiction(http4_education_token_t *token,
                                            uint8_t jurisdiction);

size_t http4_packet_context_serialize(const http4_packet_context_t *ctx,
                                      uint8_t *out, size_t out_cap);
int http4_packet_context_parse(const uint8_t *in, size_t in_len,
                               http4_packet_context_t *ctx);

#ifdef __cplusplus
}
#endif
#endif

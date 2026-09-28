#include "http4_packet_context.h"

#include <string.h>

static const uint32_t k_number[HTTP4_BASKET_ITEMS] = {
    1u,2u,3u,4u,5u,6u,7u,8u,9u,10u,11u,12u,13u,14u
};
static const uint64_t k_value[HTTP4_BASKET_ITEMS] = {
    56249759ull,34243099ull,69987683ull,43104945ull,
    29864648ull,17981345ull,6610716ull,12259381ull,
    65041790ull,31566795ull,24592204ull,65211966ull,
    35009362ull,35480545ull
};

static void put16(uint8_t *p, uint16_t v) { p[0]=(uint8_t)(v>>8); p[1]=(uint8_t)v; }
static void put32(uint8_t *p, uint32_t v) {
    p[0]=(uint8_t)(v>>24); p[1]=(uint8_t)(v>>16);
    p[2]=(uint8_t)(v>>8); p[3]=(uint8_t)v;
}
static void put64(uint8_t *p, uint64_t v) {
    for (int i=7;i>=0;--i) { p[i]=(uint8_t)v; v>>=8; }
}
static uint16_t get16(const uint8_t *p) { return (uint16_t)(((uint16_t)p[0]<<8)|p[1]); }

void http4_education_token_default(http4_education_token_t *token)
{
    if (!token) return;
    token->version = HTTP4_EDU_TOKEN_VERSION;
    token->level = HTTP4_EDU_LEVEL_12_OR_HIGHER;
    token->jurisdiction = HTTP4_JURISDICTION_UNKNOWN;
    token->flags = 0;
}

int http4_education_token_set_jurisdiction(http4_education_token_t *token,
                                            uint8_t jurisdiction)
{
    if (!token) return -1;
    token->jurisdiction = jurisdiction;
    return 0;
}

static void basket_default(uint8_t *out)
{
    put16(out, 840u);
    put16(out + 2, HTTP4_BASKET_ITEMS);
    size_t pos = 4;
    for (size_t i=0;i<HTTP4_BASKET_ITEMS;++i) {
        put32(out + pos, k_number[i]); pos += 4;
        put64(out + pos, k_value[i]); pos += 8;
    }
}

size_t http4_packet_context_serialize(const http4_packet_context_t *ctx,
                                      uint8_t *out, size_t out_cap)
{
    if (!ctx || !out || out_cap < HTTP4_PACKET_CONTEXT_SIZE) return 0;
    out[0]=ctx->education.version;
    out[1]=ctx->education.level;
    out[2]=ctx->education.jurisdiction;
    out[3]=ctx->education.flags;
    memcpy(out + HTTP4_EDU_TOKEN_SIZE, ctx->basket, HTTP4_BASKET_BLOCK_SIZE);
    return HTTP4_PACKET_CONTEXT_SIZE;
}

int http4_packet_context_parse(const uint8_t *in, size_t in_len,
                               http4_packet_context_t *ctx)
{
    if (!in || !ctx || in_len < HTTP4_PACKET_CONTEXT_SIZE) return -1;
    if (in[0] != HTTP4_EDU_TOKEN_VERSION) return -2;
    if (in[1] < HTTP4_EDU_LEVEL_12_OR_HIGHER) return -3;
    if (get16(in + HTTP4_EDU_TOKEN_SIZE) != 840u) return -4;
    if (get16(in + HTTP4_EDU_TOKEN_SIZE + 2) != HTTP4_BASKET_ITEMS) return -5;
    ctx->education.version=in[0];
    ctx->education.level=in[1];
    ctx->education.jurisdiction=in[2];
    ctx->education.flags=in[3];
    memcpy(ctx->basket, in + HTTP4_EDU_TOKEN_SIZE, HTTP4_BASKET_BLOCK_SIZE);
    return 0;
}

/* Initializes a context for callers that want the frozen HTTP/3 basket. */
void http4_packet_context_init_defaults(http4_packet_context_t *ctx)
{
    if (!ctx) return;
    memset(ctx, 0, sizeof(*ctx));
    http4_education_token_default(&ctx->education);
    basket_default(ctx->basket);
}

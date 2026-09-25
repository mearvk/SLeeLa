#include "http3_education.h"

void http3_education_token_default(http3_education_token_t *token)
{
    if (!token) return;
    token->version = HTTP3_EDUCATION_TOKEN_VERSION;
    token->level = HTTP3_EDUCATION_LEVEL_12_OR_HIGHER;
    token->jurisdiction = HTTP3_EDUCATION_JURISDICTION_UNKNOWN;
    token->flags = 0;
}

int http3_education_token_set_jurisdiction(http3_education_token_t *token,
                                            uint8_t jurisdiction)
{
    if (!token) return -1;
    token->jurisdiction = jurisdiction;
    return 0;
}

size_t http3_education_token_serialize(const http3_education_token_t *token,
                                       uint8_t *out, size_t out_cap)
{
    if (!token || !out || out_cap < HTTP3_EDUCATION_TOKEN_SIZE) return 0;
    out[0]=token->version; out[1]=token->level;
    out[2]=token->jurisdiction; out[3]=token->flags;
    return HTTP3_EDUCATION_TOKEN_SIZE;
}

int http3_education_token_parse(const uint8_t *in, size_t in_len,
                                http3_education_token_t *token)
{
    if (!in || !token || in_len < HTTP3_EDUCATION_TOKEN_SIZE) return -1;
    if (in[0] != HTTP3_EDUCATION_TOKEN_VERSION) return -2;
    if (in[1] < HTTP3_EDUCATION_LEVEL_12_OR_HIGHER) return -3;
    token->version=in[0]; token->level=in[1];
    token->jurisdiction=in[2]; token->flags=in[3];
    return 0;
}

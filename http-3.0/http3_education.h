#ifndef SLEELA_HTTP3_EDUCATION_H
#define SLEELA_HTTP3_EDUCATION_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP3_EDUCATION_TOKEN_VERSION 1u
#define HTTP3_EDUCATION_LEVEL_12_OR_HIGHER 12u
#define HTTP3_EDUCATION_TOKEN_SIZE 4u
#define HTTP3_EDUCATION_JURISDICTION_UNKNOWN 0u

typedef struct {
    uint8_t version;
    uint8_t level;
    uint8_t jurisdiction;
    uint8_t flags;
} http3_education_token_t;

void http3_education_token_default(http3_education_token_t *token);
int http3_education_token_set_jurisdiction(http3_education_token_t *token,
                                            uint8_t jurisdiction);
size_t http3_education_token_serialize(const http3_education_token_t *token,
                                       uint8_t *out, size_t out_cap);
int http3_education_token_parse(const uint8_t *in, size_t in_len,
                                http3_education_token_t *token);

#ifdef __cplusplus
}
#endif
#endif

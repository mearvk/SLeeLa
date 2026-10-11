#ifndef SLEELA_REST_CONNECTOR_H
#define SLEELA_REST_CONNECTOR_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct sleela_http_response { long status; char *body; size_t size; char error[256]; } sleela_http_response;
int sleela_http_get(const char *url,long timeout_seconds,sleela_http_response *out);
int sleela_http_post_json(const char *url,const char *json_body,long timeout_seconds,sleela_http_response *out);
void sleela_http_response_free(sleela_http_response *r);
#ifdef __cplusplus
}
#endif
#endif

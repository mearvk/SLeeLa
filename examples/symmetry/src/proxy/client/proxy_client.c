#include <stddef.h>

typedef struct { const char *proxy; unsigned long request_count; } proxy_client_t;
void proxy_client_connect(proxy_client_t *c, const char *proxy) { c->proxy = proxy; c->request_count = 0; }
const char *proxy_client_request(proxy_client_t *c, const char *target, const char *payload) { (void)target; c->request_count++; return payload; }

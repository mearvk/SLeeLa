#include <stddef.h>

typedef struct { unsigned long active_connections; unsigned long forwarded_requests; } proxy_server_t;
void proxy_server_init(proxy_server_t *s) { s->active_connections = 0; s->forwarded_requests = 0; }
void proxy_server_connect(proxy_server_t *s) { s->active_connections++; }
const char *proxy_server_forward(proxy_server_t *s, const char *target, const char *payload) { (void)target; s->forwarded_requests++; return payload; }
void proxy_server_disconnect(proxy_server_t *s) { if (s->active_connections) s->active_connections--; }

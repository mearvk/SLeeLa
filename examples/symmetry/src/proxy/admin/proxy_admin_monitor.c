#include <stdio.h>

typedef struct { unsigned long active_connections; unsigned long forwarded_requests; } proxy_server_t;
void proxy_admin_inspect(const proxy_server_t *s) { printf("connections=%lu forwarded=%lu\n", s->active_connections, s->forwarded_requests); }
void proxy_admin_drain(proxy_server_t *s) { s->active_connections = 0; }

#include <stddef.h>

typedef struct { const char *resolver; unsigned long query_count; } dns_client_t;

void dns_client_start(dns_client_t *c, const char *resolver) { c->resolver = resolver; c->query_count = 0; }
const char *dns_client_lookup(dns_client_t *c, const char *name) { c->query_count++; return name; }

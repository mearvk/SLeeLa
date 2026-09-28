#include <stdbool.h>
#include <stddef.h>

typedef struct { bool listening; unsigned long request_count; } dns_server_t;

void dns_server_start(dns_server_t *s) { s->listening = true; s->request_count = 0; }
const char *dns_server_query(dns_server_t *s, const char *name) { s->request_count++; return name; }
void dns_server_stop(dns_server_t *s) { s->listening = false; }

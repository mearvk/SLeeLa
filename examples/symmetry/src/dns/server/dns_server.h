#ifndef SLEEELA_DNS_SERVER_H
#define SLEEELA_DNS_SERVER_H
#include <stdbool.h>
typedef struct { bool listening; unsigned long request_count; } dns_server_t;
void dns_server_start(dns_server_t *s);
const char *dns_server_query(dns_server_t *s, const char *name);
void dns_server_stop(dns_server_t *s);
#endif

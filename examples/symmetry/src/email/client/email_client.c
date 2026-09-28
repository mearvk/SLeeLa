#include <stddef.h>

typedef struct { const char *server; unsigned long sent_count; } email_client_t;
void email_client_connect(email_client_t *c, const char *server) { c->server = server; c->sent_count = 0; }
void email_client_send(email_client_t *c, const char *message) { (void)message; c->sent_count++; }

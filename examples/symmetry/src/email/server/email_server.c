#include <stddef.h>

typedef struct { unsigned long delivered_count; unsigned long queue_size; } email_server_t;
void email_server_init(email_server_t *s) { s->delivered_count = 0; s->queue_size = 0; }
void email_server_submit(email_server_t *s, const char *message) { (void)message; s->queue_size++; s->delivered_count++; }
unsigned long email_server_queue_size(const email_server_t *s) { return s->queue_size; }

#include <stdio.h>

typedef struct { unsigned long delivered_count; unsigned long queue_size; } email_server_t;
void email_admin_inspect(const email_server_t *s) { printf("queue=%lu delivered=%lu\n", s->queue_size, s->delivered_count); }

#include <stdio.h>
#include "../server/dns_server.h"

void dns_admin_inspect(const dns_server_t *server) {
    printf("listening=%d requests=%lu\n", server->listening, server->request_count);
}

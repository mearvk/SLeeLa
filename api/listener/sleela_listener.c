#include "sleela_listener.h"
#include <stdio.h>
#include <string.h>

static int fail(char *error, size_t n, const char *msg) {
    if (error && n) snprintf(error,n,"%s",msg);
    return -1;
}

int sleela_listener_validate(const sleela_listener_t *x, char *error, size_t n) {
    if (!x) return fail(error,n,"listener is null");
    if (!x->bind_host || !x->bind_host[0]) return fail(error,n,"listener bind_host is required");
    if (x->port == 0) return fail(error,n,"listener port must be non-zero");
    if (!x->protocol || strcmp(x->protocol,"http") != 0)
        return fail(error,n,"listener protocol must be http");
    if (!x->route || x->route[0] != '/') return fail(error,n,"listener route must be an absolute path");
    if (strlen(x->bind_host)>255 || strlen(x->route)>4096)
        return fail(error,n,"listener field exceeds SLeeLa limit");
    return 0;
}

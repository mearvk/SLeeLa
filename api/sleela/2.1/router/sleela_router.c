#include "sleela_router.h"
#include <stdio.h>
#include <string.h>

static int fail(char *error, size_t n, const char *msg) {
    if (error && n) snprintf(error,n,"%s",msg);
    return -1;
}

int sleela_router_validate(const sleela_route_t *r, char *error, size_t n) {
    if (!r) return fail(error,n,"route is null");
    if (!r->method || !r->method[0]) return fail(error,n,"route method is required");
    if (!r->path || r->path[0] != '/') return fail(error,n,"route path must be an absolute path");
    if (!r->target || r->target[0] != '/') return fail(error,n,"route target must be an absolute path");
    if (strlen(r->method)>16 || strlen(r->path)>4096 || strlen(r->target)>4096)
        return fail(error,n,"route field exceeds SLeeLa limit");
    return 0;
}

int sleela_router_match(const sleela_route_t *r, const char *method, const char *path) {
    if (!r || !method || !path) return 0;
    return strcmp(r->method,method)==0 && strcmp(r->path,path)==0;
}

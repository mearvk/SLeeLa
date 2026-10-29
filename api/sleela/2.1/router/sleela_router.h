#ifndef SLEELA_ROUTER_H
#define SLEELA_ROUTER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char *method;
    const char *path;
    const char *target;
} sleela_route_t;

int sleela_router_validate(const sleela_route_t *route, char *error, size_t error_size);
int sleela_router_match(const sleela_route_t *route, const char *method, const char *path);

#ifdef __cplusplus
}
#endif
#endif

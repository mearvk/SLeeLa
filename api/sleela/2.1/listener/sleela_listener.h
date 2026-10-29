#ifndef SLEELA_LISTENER_H
#define SLEELA_LISTENER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char *bind_host;
    unsigned short port;
    const char *protocol;
    const char *route;
} sleela_listener_t;

int sleela_listener_validate(const sleela_listener_t *listener, char *error, size_t error_size);

#ifdef __cplusplus
}
#endif
#endif

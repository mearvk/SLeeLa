#ifndef SLEELA_RESOLVER_H
#define SLEELA_RESOLVER_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif

#define SLEELA_RESOLVER_MAX_ADDRESS 128
#define SLEELA_RESOLVER_MAX_PATH 1024

typedef enum {
    SLEELA_RESOLVE_NONE = 0,
    SLEELA_RESOLVE_FORWARD = 1,
    SLEELA_RESOLVE_REVERSE = 2,
    SLEELA_RESOLVE_PATH = 4
} sleela_resolver_kind;

typedef struct {
    sleela_resolver_kind kind;
    char input[SLEELA_RESOLVER_MAX_PATH];
    char canonical[SLEELA_RESOLVER_MAX_PATH];
    char address[SLEELA_RESOLVER_MAX_ADDRESS];
    int family;
    int dynamic;
} sleela_resolution;

int sleela_resolver_resolve(const char *input, sleela_resolution *out);
int sleela_resolver_forward(const char *hostname, char *address, size_t address_size, int family);
int sleela_resolver_reverse(const char *address, char *hostname, size_t hostname_size);
int sleela_resolver_path(const char *path, char *canonical, size_t canonical_size);
int sleela_resolver_event_policy(const char *protocol, const char *target);
int sleela_resolver_self_test(void);

#ifdef __cplusplus
}
#endif
#endif

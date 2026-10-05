#if !defined(_WIN32)
#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE  /* expose realpath() from <stdlib.h> on glibc */
#endif
#include "resolver.h"
#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if !defined(_WIN32)
#include <sys/stat.h>
#include <unistd.h>
#endif

#if defined(_WIN32)
static void resolver_windows_init(void) { static int ready = 0; if (!ready) { WSADATA w; if (WSAStartup(MAKEWORD(2,2), &w) == 0) ready = 1; } }
#endif

static int is_ip(const char *s, int *family) {
    struct in_addr v4; struct in6_addr v6;
    if (inet_pton(AF_INET, s, &v4) == 1) { if (family) *family = AF_INET; return 1; }
    if (inet_pton(AF_INET6, s, &v6) == 1) { if (family) *family = AF_INET6; return 1; }
    return 0;
}

int sleela_resolver_forward(const char *hostname, char *address, size_t address_size, int family) {
    if (!hostname || !address || address_size == 0) return 0;
#if defined(_WIN32)
    resolver_windows_init();
#endif
    struct addrinfo hints, *list = NULL;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = family ? family : AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(hostname, NULL, &hints, &list) != 0) return 0;
    int ok = 0;
    for (struct addrinfo *p = list; p; p = p->ai_next) {
        if (getnameinfo(p->ai_addr, p->ai_addrlen, address, (socklen_t)address_size,
                        NULL, 0, NI_NUMERICHOST) == 0) { ok = 1; break; }
    }
    freeaddrinfo(list);
    return ok;
}

int sleela_resolver_reverse(const char *address, char *hostname, size_t hostname_size) {
    if (!address || !hostname || hostname_size == 0) return 0;
#if defined(_WIN32)
    resolver_windows_init();
#endif
    struct sockaddr_storage ss; memset(&ss, 0, sizeof(ss));
    socklen_t len = 0;
    struct sockaddr_in *v4 = (struct sockaddr_in *)&ss;
    struct sockaddr_in6 *v6 = (struct sockaddr_in6 *)&ss;
    if (inet_pton(AF_INET, address, &v4->sin_addr) == 1) {
        v4->sin_family = AF_INET; len = sizeof(*v4);
    } else if (inet_pton(AF_INET6, address, &v6->sin6_addr) == 1) {
        v6->sin6_family = AF_INET6; len = sizeof(*v6);
    } else return 0;
    return getnameinfo((struct sockaddr *)&ss, len, hostname, (socklen_t)hostname_size,
                       NULL, 0, NI_NAMEREQD) == 0;
}

int sleela_resolver_path(const char *path, char *canonical, size_t canonical_size) {
    if (!path || !canonical || canonical_size == 0) return 0;
#if defined(_WIN32)
    DWORD n = GetFullPathNameA(path, (DWORD)canonical_size, canonical, NULL);
    return n > 0 && n < canonical_size;
#else
    char *resolved = realpath(path, NULL);
    if (!resolved) return 0;
    size_t n = strlen(resolved);
    if (n + 1 > canonical_size) { free(resolved); return 0; }
    memcpy(canonical, resolved, n + 1);
    free(resolved);
    return 1;
#endif
}

int sleela_resolver_resolve(const char *input, sleela_resolution *out) {
    if (!input || !*input || !out) return 0;
    memset(out, 0, sizeof(*out));
    snprintf(out->input, sizeof(out->input), "%s", input);
    int family = 0;
    if (is_ip(input, &family)) {
        out->kind = SLEELA_RESOLVE_REVERSE;
        out->family = family;
        if (sleela_resolver_reverse(input, out->canonical, sizeof(out->canonical))) {
            out->dynamic = 1; snprintf(out->address, sizeof(out->address), "%s", input); return 1;
        }
        snprintf(out->canonical, sizeof(out->canonical), "%s", input);
        snprintf(out->address, sizeof(out->address), "%s", input);
        return 1;
    }
    if (sleela_resolver_forward(input, out->address, sizeof(out->address), AF_UNSPEC)) {
        out->kind = SLEELA_RESOLVE_FORWARD;
        out->family = strchr(out->address, ':') ? AF_INET6 : AF_INET;
        snprintf(out->canonical, sizeof(out->canonical), "%s", input);
        out->dynamic = 1;
        return 1;
    }
    if (sleela_resolver_path(input, out->canonical, sizeof(out->canonical))) {
        out->kind = SLEELA_RESOLVE_PATH;
        out->dynamic = 0;
        return 1;
    }
    return 0;
}

int sleela_resolver_event_policy(const char *protocol, const char *target) {
    const char *configured = target;
    if (!configured || !*configured) configured = getenv("SLEELA_RESOLVER_TARGET");
    if (!configured || !*configured) return 1; /* Resolver is opt-in for packet policy. */
    sleela_resolution r;
    if (!sleela_resolver_resolve(configured, &r)) return 0;
    const char *required = getenv("SLEELA_RESOLVER_REQUIRE_DYNAMIC");
    if (required && strcmp(required, "true") == 0 && !r.dynamic) return 0;
    (void)protocol;
    return 1;
}

int sleela_resolver_self_test(void) {
    char address[SLEELA_RESOLVER_MAX_ADDRESS];
    char host[SLEELA_RESOLVER_MAX_PATH];
    char path[SLEELA_RESOLVER_MAX_PATH];
    int ok = sleela_resolver_forward("localhost", address, sizeof(address), AF_UNSPEC);
    ok = ok && sleela_resolver_reverse(address, host, sizeof(host));
    ok = ok && sleela_resolver_path(".", path, sizeof(path));
    return ok ? 0 : 1;
}


#include "sleela_net.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

static SLNetPlatform requested_platform(void) {
    const char* value = getenv("SLEELA_NET_PLATFORM");
    if (!value || !*value || strcmp(value, "auto") == 0) return SL_NET_AUTO;
    if (strcmp(value, "linux") == 0 || strcmp(value, "posix") == 0) return SL_NET_LINUX;
    if (strcmp(value, "windows") == 0 || strcmp(value, "win32") == 0) return SL_NET_WINDOWS;
    return SL_NET_AUTO;
}

SLNetPlatform slnet_platform(void) {
#ifdef _WIN32
    return SL_NET_WINDOWS;
#else
    return SL_NET_LINUX;
#endif
}

const char* slnet_platform_name(void) {
    return slnet_platform() == SL_NET_WINDOWS ? "windows" : "linux";
}

int slnet_platform_is_available(SLNetPlatform platform) {
    if (platform == SL_NET_AUTO) return 1;
    return platform == slnet_platform();
}

int slnet_startup(void) {
    SLNetPlatform requested = requested_platform();
    if (requested != SL_NET_AUTO && requested != slnet_platform()) return -1;
#ifdef _WIN32
    WSADATA data;
    return WSAStartup(MAKEWORD(2, 2), &data) == 0 ? 0 : -1;
#else
    return 0;
#endif
}

void slnet_shutdown(void) {
#ifdef _WIN32
    WSACleanup();
#endif
}

static int valid_port(uint16_t port) {
    (void)port;
    return 1;
}

SLNetHandle slnet_listen(uint16_t port, int backlog) {
    if (!valid_port(port) || backlog < 1) return SL_NET_INVALID;
#ifdef _WIN32
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) return SL_NET_INVALID;
#else
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) return SL_NET_INVALID;
#endif
    int yes = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes)) < 0) {
        slnet_close((SLNetHandle)s);
        return SL_NET_INVALID;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0 || listen(s, backlog) < 0) {
        slnet_close((SLNetHandle)s);
        return SL_NET_INVALID;
    }
    return (SLNetHandle)s;
}

SLNetHandle slnet_accept(SLNetHandle listener) {
#ifdef _WIN32
    SOCKET s = accept((SOCKET)listener, NULL, NULL);
    return s == INVALID_SOCKET ? SL_NET_INVALID : (SLNetHandle)s;
#else
    int s = accept((int)listener, NULL, NULL);
    return s < 0 ? SL_NET_INVALID : (SLNetHandle)s;
#endif
}

SLNetHandle slnet_connect(const char* host, uint16_t port) {
    if (!host || !*host || !valid_port(port)) return SL_NET_INVALID;
    char service[16];
    snprintf(service, sizeof(service), "%u", (unsigned)port);
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo* list = NULL;
    if (getaddrinfo(host, service, &hints, &list) != 0) return SL_NET_INVALID;
    SLNetHandle result = SL_NET_INVALID;
    for (struct addrinfo* p = list; p; p = p->ai_next) {
#ifdef _WIN32
        SOCKET s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (s == INVALID_SOCKET) continue;
        if (connect(s, p->ai_addr, (int)p->ai_addrlen) == 0) {
            result = (SLNetHandle)s;
            break;
        }
        closesocket(s);
#else
        int s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (s < 0) continue;
        if (connect(s, p->ai_addr, p->ai_addrlen) == 0) {
            result = (SLNetHandle)s;
            break;
        }
        close(s);
#endif
    }
    freeaddrinfo(list);
    return result;
}

SLNetCount slnet_read(SLNetHandle handle, char* buffer, size_t capacity) {
    if (!buffer || capacity == 0 || !slnet_valid(handle)) return -1;
#ifdef _WIN32
    int n = recv((SOCKET)handle, buffer, (int)(capacity > INT_MAX ? INT_MAX : capacity), 0);
#else
    ssize_t n = recv((int)handle, buffer, capacity, 0);
#endif
    return n < 0 ? -1 : (SLNetCount)n;
}

SLNetCount slnet_write(SLNetHandle handle, const char* data, size_t length) {
    if (!data || length == 0 || !slnet_valid(handle)) return length == 0 ? 0 : -1;
#ifdef _WIN32
    int n = send((SOCKET)handle, data, (int)(length > INT_MAX ? INT_MAX : length), 0);
#else
    ssize_t n = send((int)handle, data, length, 0);
#endif
    return n < 0 ? -1 : (SLNetCount)n;
}

int slnet_close(SLNetHandle handle) {
    if (!slnet_valid(handle)) return -1;
#ifdef _WIN32
    return closesocket((SOCKET)handle) == 0 ? 0 : -1;
#else
    return close((int)handle) == 0 ? 0 : -1;
#endif
}

int slnet_valid(SLNetHandle handle) {
#ifdef _WIN32
    return handle != SL_NET_INVALID && (SOCKET)handle != INVALID_SOCKET;
#else
    return handle >= 0;
#endif
}

int slnet_last_error(void) {
#ifdef _WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}

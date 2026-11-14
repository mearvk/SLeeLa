/* ==========================================================================
 * sleela_java28_port.c -- Portable TCP client for the Java 28 SecureJDK
 * memory link. Newline-framed request/response over loopback.
 * ========================================================================== */
#include "sleela_java28.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
typedef SOCKET sl_socket_t;
#define SL_INVALID_SOCKET INVALID_SOCKET
static int sl_socket_startup(void) {
    WSADATA data;
    return WSAStartup(MAKEWORD(2, 2), &data) == 0 ? 0 : -1;
}
static void sl_socket_cleanup(void) { WSACleanup(); }
static void sl_socket_close(sl_socket_t fd) { closesocket(fd); }
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
typedef int sl_socket_t;
#define SL_INVALID_SOCKET (-1)
static int sl_socket_startup(void) { return 0; }
static void sl_socket_cleanup(void) { }
static void sl_socket_close(sl_socket_t fd) { (void)close(fd); }
#endif

#define SL_J28_BUF 8192

struct SLJava28Session {
    sl_socket_t fd;
    int socket_started;
    char resp[SL_J28_BUF];
    char err[256];
    char rbuf[SL_J28_BUF];
    size_t rlen;
    size_t rpos;
};

static void set_err(SLJava28Session *s, const char *m) {
    if (s) snprintf(s->err, sizeof s->err, "%s", m ? m : "unknown");
}

static int write_all(sl_socket_t fd, const void *buf, size_t len) {
    const char *p = (const char *)buf;
    while (len) {
        size_t chunk = len > (size_t)INT_MAX ? (size_t)INT_MAX : len;
#ifdef _WIN32
        int n = send(fd, p, (int)chunk, 0);
#else
#ifdef MSG_NOSIGNAL
        int n = (int)send(fd, p, chunk, MSG_NOSIGNAL);
#else
        int n = (int)send(fd, p, chunk, 0);
#endif
#endif
        if (n <= 0) return -1;
        p += n;
        len -= (size_t)n;
    }
    return 0;
}

SLJava28Session *sl_java28_port_open(int port) {
    if (port < 1 || port > 65535) return NULL;
    if (sl_socket_startup() != 0) return NULL;

    SLJava28Session *s = (SLJava28Session *)calloc(1, sizeof *s);
    if (!s) {
        sl_socket_cleanup();
        return NULL;
    }
    s->fd = SL_INVALID_SOCKET;
    s->socket_started = 1;
    s->fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s->fd == SL_INVALID_SOCKET) {
        set_err(s, "socket creation failed");
        sl_socket_cleanup();
        free(s);
        return NULL;
    }
#if !defined(_WIN32) && defined(SO_NOSIGPIPE)
    {
        int no_sigpipe = 1;
        (void)setsockopt(s->fd, SOL_SOCKET, SO_NOSIGPIPE,
                         &no_sigpipe, (socklen_t)sizeof no_sigpipe);
    }
#endif

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons((unsigned short)port);
    if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) != 1) {
        set_err(s, "invalid loopback address");
        sl_socket_close(s->fd);
        sl_socket_cleanup();
        free(s);
        return NULL;
    }
    if (connect(s->fd, (struct sockaddr *)&addr, (int)sizeof addr) != 0) {
        set_err(s, "connection to 127.0.0.1 failed");
        sl_socket_close(s->fd);
        sl_socket_cleanup();
        free(s);
        return NULL;
    }
    return s;
}

/* Read exactly one line, preserving the response framing and rejecting
 * oversized lines instead of silently truncating protocol data. */
static int read_line(SLJava28Session *s) {
    size_t out = 0;
    for (;;) {
        if (s->rpos >= s->rlen) {
#ifdef _WIN32
            int n = recv(s->fd, s->rbuf, (int)sizeof s->rbuf, 0);
#else
            int n = (int)recv(s->fd, s->rbuf, sizeof s->rbuf, 0);
#endif
            if (n <= 0) {
                set_err(s, "connection closed while reading response");
                return -1;
            }
            s->rlen = (size_t)n;
            s->rpos = 0;
        }
        while (s->rpos < s->rlen) {
            char c = s->rbuf[s->rpos++];
            if (c == '\n') {
                if (out && s->resp[out - 1] == '\r') --out;
                s->resp[out] = '\0';
                return 0;
            }
            if (out + 1 >= sizeof s->resp) {
                set_err(s, "response line exceeds buffer limit");
                return -1;
            }
            s->resp[out++] = c;
        }
    }
}

const char *sl_java28_call(SLJava28Session *s, const char *request_line) {
    if (!s || s->fd == SL_INVALID_SOCKET || !request_line) return NULL;
    if (strchr(request_line, '\n') || strchr(request_line, '\r')) {
        set_err(s, "request must contain exactly one protocol line");
        return NULL;
    }
    size_t len = strlen(request_line);
    if (write_all(s->fd, request_line, len) != 0 ||
        write_all(s->fd, "\n", 1) != 0) {
        set_err(s, "socket write failed");
        return NULL;
    }
    if (read_line(s) != 0) return NULL;
    return s->resp;
}

const char *sl_java28_hello(SLJava28Session *s) {
    return sl_java28_call(s, "hello i:1");
}

void sl_java28_close(SLJava28Session *s) {
    if (!s) return;
    if (s->fd != SL_INVALID_SOCKET) {
        (void)write_all(s->fd, "bye\n", 4);
        sl_socket_close(s->fd);
    }
    if (s->socket_started) sl_socket_cleanup();
    free(s);
}

const char *sl_java28_error(SLJava28Session *s) {
    return (s && s->err[0]) ? s->err : NULL;
}

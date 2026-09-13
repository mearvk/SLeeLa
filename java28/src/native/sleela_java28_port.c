/* ==========================================================================
 * sleela_java28_port.c -- Port (socket) channel client for the Java 28
 * SecureJDK memory link. Implements the Sleela-side of J28-MEM-0001 §4:
 * newline-framed request/response over a TCP loopback connection.
 * ========================================================================== */
#include "sleela_java28.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define SL_J28_BUF 8192

struct SLJava28Session {
    int fd;
    char resp[SL_J28_BUF];   /* last response line (NUL-terminated, no newline) */
    char err[256];
    /* small read buffer for line framing */
    char rbuf[SL_J28_BUF];
    size_t rlen;
    size_t rpos;
};

static void set_err(SLJava28Session *s, const char *m) {
    if (!s) return;
    snprintf(s->err, sizeof s->err, "%s", m ? m : "unknown");
}

SLJava28Session *sl_java28_port_open(int port) {
    SLJava28Session *s = (SLJava28Session *)calloc(1, sizeof *s);
    if (!s) return NULL;
    s->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (s->fd < 0) { free(s); return NULL; }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons((unsigned short)port);
    if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) != 1) {
        close(s->fd); free(s); return NULL;
    }
    if (connect(s->fd, (struct sockaddr *)&addr, sizeof addr) != 0) {
        close(s->fd); free(s); return NULL;
    }
    return s;
}

/* Read exactly one line (up to '\n') into s->resp, stripping the newline. */
static int read_line(SLJava28Session *s) {
    size_t out = 0;
    for (;;) {
        if (s->rpos >= s->rlen) {
            ssize_t n = read(s->fd, s->rbuf, sizeof s->rbuf);
            if (n <= 0) { set_err(s, "connection closed"); return -1; }
            s->rlen = (size_t)n;
            s->rpos = 0;
        }
        while (s->rpos < s->rlen) {
            char c = s->rbuf[s->rpos++];
            if (c == '\n') { s->resp[out] = '\0'; return 0; }
            if (out + 1 < sizeof s->resp) s->resp[out++] = c;
        }
    }
}

const char *sl_java28_call(SLJava28Session *s, const char *request_line) {
    if (!s || s->fd < 0 || !request_line) return NULL;
    size_t len = strlen(request_line);
    /* write request + newline */
    if (write(s->fd, request_line, len) != (ssize_t)len) { set_err(s, "write failed"); return NULL; }
    if (write(s->fd, "\n", 1) != 1) { set_err(s, "write failed"); return NULL; }
    if (read_line(s) != 0) return NULL;
    return s->resp;
}

const char *sl_java28_hello(SLJava28Session *s) {
    return sl_java28_call(s, "hello i:1");
}

void sl_java28_close(SLJava28Session *s) {
    if (!s) return;
    if (s->fd >= 0) {
        /* best-effort graceful close */
        (void)write(s->fd, "bye\n", 4);
        close(s->fd);
    }
    free(s);
}

const char *sl_java28_error(SLJava28Session *s) {
    return (s && s->err[0]) ? s->err : NULL;
}

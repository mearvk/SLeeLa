/* http_server.c -- a minimal but real HTTP/1.1 server, as a toolchain input.
 *
 * Uses POSIX sockets only. Serves a few fixed routes with proper status codes,
 * mirroring the routing logic of HttpServer.sleela / HttpServer.java here.
 * Single-threaded, handles one connection at a time; sufficient for a demo.
 *
 *   cc -std=c11 -O2 -Wall -Wextra http_server.c -o http_server
 *   ./http_server 8080     # then: curl -i http://localhost:8080/health
 */
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static bool is_known_method(const char *m) {
    return strcmp(m, "GET") == 0 || strcmp(m, "HEAD") == 0 || strcmp(m, "POST") == 0;
}

static int status_for(const char *method, const char *path) {
    if (!is_known_method(method)) {
        return 405;
    }
    if (strcmp(path, "/") == 0 || strcmp(path, "/index.html") == 0 ||
        strcmp(path, "/health") == 0) {
        return 200;
    }
    return 404;
}

static const char *reason(int code) {
    switch (code) {
        case 200: return "OK";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        default:  return "Bad Request";
    }
}

static const char *body_for(const char *path, int code, char *buf, size_t n) {
    if (code != 200) {
        snprintf(buf, n, "error %d", code);
        return buf;
    }
    if (strcmp(path, "/health") == 0) {
        return "OK";
    }
    return "<html><body>Sleela HTTP model</body></html>";
}

static void handle_connection(int conn) {
    char req[4096];
    ssize_t got = read(conn, req, sizeof(req) - 1);
    if (got <= 0) return;
    req[got] = '\0';

    /* Parse the request line: METHOD SP PATH SP VERSION */
    char method[16] = {0};
    char path[1024] = {0};
    if (sscanf(req, "%15s %1023s", method, path) != 2) {
        strcpy(method, "GET");
        strcpy(path, "/");
    }

    int code = status_for(method, path);
    char bodybuf[64];
    const char *body = body_for(path, code, bodybuf, sizeof(bodybuf));

    char resp[4096];
    int len = snprintf(resp, sizeof(resp),
                       "HTTP/1.1 %d %s\r\n"
                       "Content-Type: text/html; charset=utf-8\r\n"
                       "Content-Length: %zu\r\n"
                       "Connection: close\r\n"
                       "\r\n"
                       "%s",
                       code, reason(code), strlen(body), body);
    write(conn, resp, (size_t)len);
    printf("%s %s -> %d\n", method, path, code);
}

int main(int argc, char **argv) {
    int port = (argc > 1) ? atoi(argv[1]) : 8080;

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) { perror("socket"); return 1; }

    int yes = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons((uint16_t)port);

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }
    if (listen(fd, 16) < 0) {
        perror("listen");
        return 1;
    }
    printf("HTTP server listening on http://localhost:%d\n", port);

    for (;;) {
        int conn = accept(fd, NULL, NULL);
        if (conn < 0) { perror("accept"); continue; }
        handle_connection(conn);
        close(conn);
    }
    /* not reached */
}

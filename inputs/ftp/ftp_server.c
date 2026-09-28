/* ftp_server.c -- a model FTP control-channel server (RFC 959), as a toolchain
 * input. It speaks the FTP control protocol over a real TCP socket: it sends
 * 3-digit reply codes and handles USER/PASS/PWD/LIST/QUIT. Data connections are
 * intentionally not implemented -- this focuses on the control-channel state
 * machine, mirroring FtpServer.sleela / FtpServer.java here.
 *
 *   cc -std=c11 -O2 -Wall -Wextra ftp_server.c -o ftp_server
 *   ./ftp_server 2121   # then: nc localhost 2121  (USER x / PASS y / PWD / QUIT)
 */
#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static void reply(int conn, int code, const char *text) {
    char line[256];
    int n = snprintf(line, sizeof(line), "%d %s\r\n", code, text);
    write(conn, line, (size_t)n);
    printf("S: %d %s\n", code, text);
}

/* Read one CRLF/LF-terminated line into buf; returns length or -1 on EOF. */
static ssize_t read_line(int conn, char *buf, size_t cap) {
    size_t i = 0;
    while (i + 1 < cap) {
        char c;
        ssize_t r = read(conn, &c, 1);
        if (r <= 0) return (i == 0) ? -1 : (ssize_t)i;
        if (c == '\n') break;
        if (c != '\r') buf[i++] = c;
    }
    buf[i] = '\0';
    return (ssize_t)i;
}

static void handle_session(int conn) {
    bool logged_in = false;
    reply(conn, 220, "Service ready");

    char line[512];
    while (read_line(conn, line, sizeof(line)) >= 0) {
        /* split into VERB and (unused here) ARG */
        char verb[16] = {0};
        sscanf(line, "%15s", verb);
        for (char *p = verb; *p; p++) *p = (char)toupper((unsigned char)*p);

        if (strcmp(verb, "USER") == 0) {
            reply(conn, 331, "User name okay, need password");
        } else if (strcmp(verb, "PASS") == 0) {
            logged_in = true; /* model: any PASS after USER authenticates */
            reply(conn, 230, "User logged in, proceed");
        } else if (strcmp(verb, "PWD") == 0) {
            if (logged_in) reply(conn, 257, "\"/\" is current directory");
            else reply(conn, 530, "Not logged in");
        } else if (strcmp(verb, "LIST") == 0) {
            if (logged_in) reply(conn, 226, "Directory send OK");
            else reply(conn, 530, "Not logged in");
        } else if (strcmp(verb, "QUIT") == 0) {
            reply(conn, 221, "Goodbye");
            return;
        } else if (verb[0] != '\0') {
            reply(conn, 502, "Command not implemented");
        }
    }
}

int main(int argc, char **argv) {
    int port = (argc > 1) ? atoi(argv[1]) : 2121;

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
    printf("FTP control server listening on port %d\n", port);

    for (;;) {
        int conn = accept(fd, NULL, NULL);
        if (conn < 0) { perror("accept"); continue; }
        handle_session(conn);
        close(conn);
    }
    /* not reached */
}

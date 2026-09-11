#define _GNU_SOURCE
#include "gardulus_ii.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

static volatile sig_atomic_t running = 1;
static FILE *log_file = NULL;
static int out_fd = -1;
static void stop_now(int sig) { (void)sig; running = 0; }

static int emit(const gardulus_sample *s, void *unused) {
    (void)unused;
    char json[1024];
    int n = gardulus_emit_json(s, json, sizeof(json));
    if (n < 0) return 1;
    if (log_file) { fputs(json, log_file); fflush(log_file); }
    else fputs(json, stdout);
    if (out_fd >= 0) (void)write(out_fd, json, (size_t)n);
    return running ? 0 : 1;
}

static int connect_listener(const char *path) {
    struct sockaddr_un addr;
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) return -1;
    memset(&addr, 0, sizeof(addr)); addr.sun_family = AF_UNIX;
    if (strlen(path) >= sizeof(addr.sun_path)) { close(fd); return -1; }
    strcpy(addr.sun_path, path);
    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) != 0) { close(fd); return -1; }
    return fd;
}

int main(int argc, char **argv) {
    int interval_ms = 1000, iterations = 0;
    const char *log_path = NULL, *socket_path = NULL;
    if (argc > 1) interval_ms = atoi(argv[1]);
    if (argc > 2) iterations = atoi(argv[2]);
    if (argc > 3) log_path = argv[3];
    if (argc > 4) socket_path = argv[4];
    if (interval_ms < 1) { fprintf(stderr, "interval must be a positive integer (milliseconds)\n"); return 2; }
    if (log_path) { log_file = fopen(log_path, "a"); if (!log_file) return 3; }
    if (socket_path) out_fd = connect_listener(socket_path);
    signal(SIGINT, stop_now); signal(SIGTERM, stop_now);
    int rc = gardulus_run(interval_ms, iterations, emit, NULL);
    if (out_fd >= 0) close(out_fd);
    if (log_file) fclose(log_file);
    return rc;
}

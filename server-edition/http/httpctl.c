#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../../api/http-multiplexing/sleela_http_api.h"

static int arg_value(int argc, char **argv, const char *name, const char **out) {
    for (int i = 1; i + 1 < argc; ++i) {
        if (strcmp(argv[i], name) == 0) { *out = argv[i + 1]; return 0; }
    }
    return -1;
}

int main(int argc, char **argv) {
    const char *port_text = NULL, *size_text = NULL, *version_text = NULL;
    if (arg_value(argc, argv, "--logical-port", &port_text) != 0 ||
        arg_value(argc, argv, "--size", &size_text) != 0 ||
        arg_value(argc, argv, "--version", &version_text) != 0) {
        fprintf(stderr, "httpctl: --logical-port PORT --size BYTES --version 1.0|2.0|2.1|3.0 required\n");
        return 2;
    }

    sleela_http_port_t port;
    if (sleela_http_port_from_decimal(&port, port_text) != 0) {
        fprintf(stderr, "httpctl: logical PORT must be an integer in [0, 10^48)\n");
        return 2;
    }

    char *end = NULL;
    unsigned long long size = strtoull(size_text, &end, 10);
    if (!end || *end != '\0') {
        fprintf(stderr, "httpctl: invalid file size\n");
        return 2;
    }

    sleela_http_version_t version;
    if (strcmp(version_text, "1.0") == 0) version = SLEELA_HTTP_1_0;
    else if (strcmp(version_text, "2.0") == 0) version = SLEELA_HTTP_2_0;
    else if (strcmp(version_text, "2.1") == 0) version = SLEELA_HTTP_2_1;
    else if (strcmp(version_text, "3.0") == 0) version = SLEELA_HTTP_3_0;
    else {
        fprintf(stderr, "httpctl: unsupported HTTP version\n");
        return 2;
    }

    const sleela_http_multiplex_mode_t multiplex = sleela_http_multiplex_mode_for(version);
    const sleela_http_download_mode_t download = sleela_http_download_mode_for_size((uint64_t)size);

    const char *multiplex_name =
        multiplex == SLEELA_HTTP_MULTIPLEX_REQUEST ? "request" :
        multiplex == SLEELA_HTTP_MULTIPLEX_HTTP2_STREAM ? "http2-stream" : "http3-stream";
    const char *download_name =
        download == SLEELA_HTTP_DOWNLOAD_RESUMABLE ? "resumable" : "standard";

    char decimal[64];
    if (sleela_http_port_to_decimal(&port, decimal, sizeof(decimal)) != 0) return 1;
    printf("logical-port=%s\n", decimal);
    printf("http-version=%s\n", version_text);
    printf("multiplex-mode=%s\n", multiplex_name);
    printf("download-mode=%s\n", download_name);
    printf("download-threshold-bytes=%llu\n",
           (unsigned long long)SLEELA_HTTP_DOWNLOAD_THRESHOLD_BYTES);
    printf("resume-fields=SESSION-ID,DATETIME,FILE-ID,FILE-NAME,INDEX,OFFSET,TOTAL-SIZE\n");
    return 0;
}

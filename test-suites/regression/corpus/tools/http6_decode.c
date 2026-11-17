/*
 * http6_decode.c -- corpus driver: decode one HTTP 6.0 frame given as hex text.
 *
 *     http6_decode FILE
 *
 * FILE holds hexadecimal byte pairs separated by whitespace; '#' starts a comment
 * that runs to the end of the line.  The bytes go to http6_frame_decode in an
 * exact-size heap block (so a read past the end is visible to a sanitizer), and
 * one line is printed:
 *
 *     rc=<decode result> consumed=<bytes> type=<frame type> length=<payload length>
 *
 * Exit status: 0 after printing, 2 for an unreadable or malformed hex file.
 */
#include "../../../../http-6.0/http6_protocol.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    FILE *f;
    unsigned char *buf;
    size_t cap = 64, len = 0;
    int c, half = -1, in_comment = 0;
    http6_frame_view_t view;
    size_t used = 0;
    int rc;

    if (argc != 2) { fputs("usage: http6_decode FILE\n", stderr); return 2; }
    f = fopen(argv[1], "r");
    if (!f) { perror(argv[1]); return 2; }
    buf = (unsigned char *)malloc(cap);
    if (!buf) return 2;
    while ((c = fgetc(f)) != EOF) {
        int v;
        if (in_comment) { if (c == '\n') in_comment = 0; continue; }
        if (c == '#') { in_comment = 1; continue; }
        if (isspace((unsigned char)c)) { if (half >= 0) { fputs("odd number of hex digits\n", stderr); return 2; } continue; }
        if (!isxdigit((unsigned char)c)) { fprintf(stderr, "not a hex digit: '%c'\n", c); return 2; }
        v = isdigit((unsigned char)c) ? c - '0' : (tolower(c) - 'a' + 10);
        if (half < 0) { half = v; continue; }
        if (len == cap) { cap *= 2; buf = (unsigned char *)realloc(buf, cap); if (!buf) return 2; }
        buf[len++] = (unsigned char)(half * 16 + v);
        half = -1;
    }
    fclose(f);
    if (half >= 0) { fputs("odd number of hex digits\n", stderr); return 2; }

    {   /* exact-size copy: a decoder that over-reads touches memory the sanitizer watches */
        unsigned char *exact = (unsigned char *)malloc(len ? len : 1);
        if (!exact) return 2;
        if (len) memcpy(exact, buf, len);
        free(buf);
        memset(&view, 0, sizeof view);
        rc = http6_frame_decode(exact, len, &view, &used);
        printf("rc=%d consumed=%zu type=%u length=%u\n", rc, rc == 0 ? used : (size_t)0,
               rc == 0 ? (unsigned)view.header.type : 0u, rc == 0 ? (unsigned)view.header.payload_length : 0u);
        free(exact);
    }
    return 0;
}

/*
 * operating-system/src/sleela_os_aggregate.c
 * Implementation of the OS build report aggregator (see sleela_os_aggregate.h).
 * Mirrors SLOSBuildReport.
 */
#include "sleela_os_aggregate.h"
#include <stdio.h>
#include <string.h>

static void copy_bounded(char *dst, size_t cap, const char *src) {
    size_t n;
    if (cap == 0) return;
    if (src == NULL) { dst[0] = '\0'; return; }
    n = strlen(src);
    if (n >= cap) n = cap - 1;
    memcpy(dst, src, n);
    dst[n] = '\0';
}

void sleela_os_report_init(sleela_os_report *r, const char *model_identity) {
    if (r == NULL) return;
    copy_bounded(r->identity, sizeof r->identity, model_identity);
    r->count = 0;
    r->ok = 1;
}

int sleela_os_report_record(sleela_os_report *r,
                            sleela_os_artifact_kind kind,
                            int64_t bytes,
                            const char *path) {
    sleela_os_artifact *a;
    if (r == NULL || path == NULL) return -1;
    if (r->count >= SLEELA_OS_MAX_ARTIFACTS) return -1;
    a = &r->items[r->count];
    a->kind = kind;
    a->bytes = bytes;
    copy_bounded(a->path, sizeof a->path, path);
    r->count += 1;
    return r->count;
}

void sleela_os_report_fail(sleela_os_report *r) {
    if (r != NULL) r->ok = 0;
}

const char *sleela_os_artifact_kind_name(sleela_os_artifact_kind kind) {
    switch (kind) {
        case SLEELA_OS_ART_SOURCE: return "source";
        case SLEELA_OS_ART_CONFIG: return "config";
        case SLEELA_OS_ART_SCRIPT: return "script";
        case SLEELA_OS_ART_ISO:    return "iso";
        default:                   return "other";
    }
}

int sleela_os_report_count(const sleela_os_report *r) {
    return r != NULL ? r->count : 0;
}

int sleela_os_report_succeeded(const sleela_os_report *r) {
    return (r != NULL && r->ok == 1) ? 1 : 0;
}

int64_t sleela_os_report_total_bytes(const sleela_os_report *r) {
    int64_t total = 0;
    int i;
    if (r == NULL) return 0;
    for (i = 0; i < r->count; ++i) total += r->items[i].bytes;
    return total;
}

int sleela_os_report_index(const sleela_os_report *r, char *buf, size_t cap) {
    int written = 0;
    int i;
    if (buf == NULL || cap == 0) return -1;
    buf[0] = '\0';
    if (r == NULL) return 0;
    for (i = 0; i < r->count; ++i) {
        const sleela_os_artifact *a = &r->items[i];
        int n = snprintf(buf + (written < (int)cap ? written : (int)cap - 1),
                         (written < (int)cap) ? cap - (size_t)written : 1,
                         "%s%s %lld %s",
                         (i == 0) ? "" : "\n",
                         sleela_os_artifact_kind_name(a->kind),
                         (long long)a->bytes, a->path);
        if (n < 0) return -1;
        written += n;
    }
    return written;
}

int sleela_os_report_summary(const sleela_os_report *r, char *buf, size_t cap) {
    const char *state;
    if (buf == NULL || cap == 0) return -1;
    if (r == NULL) { buf[0] = '\0'; return 0; }
    state = (r->ok == 1) ? "OK" : "FAILED";
    return snprintf(buf, cap, "%s: %s -> %d artifacts", state, r->identity, r->count);
}

/*
 * Sleelavirin(TM) scan bridge - implementation.
 * Max Rupplin - MEARVK LLC - 2026.
 *
 * Original Sleelavirin code; portable C11; no third-party AV engine.
 */
#include "sleelavirin_compat.h"
#include "sleelavirin_bridge.h"

#include <stdio.h>
#include <string.h>

#include "sleelavirin_native.h"

static int field_ok(const char *s, size_t cap)
{
    size_t i;
    if (!s) return 0;
    for (i = 0; i < cap; ++i)
        if (s[i] == '\0') return (i > 0); /* non-empty, NUL-terminated */
    return 0;                             /* not terminated within cap  */
}

static void set_result(sleelavirin_bridge_result_t *r, const char *verdict,
                       const char *detail, uint32_t detections, uint8_t native)
{
    memset(r, 0, sizeof(*r));
    snprintf(r->verdict, sizeof(r->verdict), "%s", verdict);
    snprintf(r->detail, sizeof(r->detail), "%s", detail ? detail : "");
    r->detections = detections;
    r->engine_native = native;
}

int sleelavirin_bridge_validate_request(const sleelavirin_bridge_request_t *req)
{
    if (!req) return -1;
    if (!field_ok(req->target_path, sizeof(req->target_path))) return -2;
    if (!field_ok(req->database_path, sizeof(req->database_path))) return -3;
    if (!field_ok(req->engine_name, sizeof(req->engine_name))) return -4;
    /* Only the native engine is implemented in-process today. */
    if (strcmp(req->engine_name, "native") != 0) return -5;
    if (req->scope != SLEELAVIRIN_SCOPE_LOCAL &&
        req->scope != SLEELAVIRIN_SCOPE_MOUNT &&
        req->scope != SLEELAVIRIN_SCOPE_NETWORK)
        return -6;
    return 0;
}

static int file_too_large(const char *path, uint64_t limit, uint64_t *size_out)
{
    FILE *f;
    uint64_t end = 0;
    if (limit == 0) { if (size_out) *size_out = 0; return 0; }
    f = fopen(path, "rb");
    if (!f) return 0; /* let the scanner report the unreadable-file error */
    if (slv_file_size(f, &end) != 0) { fclose(f); return 0; }
    fclose(f);
    if (size_out) *size_out = end;
    return (end > limit) ? 1 : 0;
}

int sleelavirin_bridge_scan_native(const sleelavirin_bridge_request_t *req,
                                   sleelavirin_bridge_result_t *result)
{
    char name[256];
    int rc;
    uint64_t size = 0;

    if (!result) return -1;
    if (sleelavirin_bridge_validate_request(req) != 0) {
        if (result) set_result(result, "ERROR", "invalid scan request", 0, 0);
        return -1;
    }

    if (file_too_large(req->target_path, req->max_file_size_bytes, &size)) {
        set_result(result, "ERROR", "target exceeds max_file_size_bytes", 0, 1);
        return -2;
    }

    name[0] = '\0';
    rc = sleelavirin_native_scan_file(req->target_path, req->database_path,
                                      name, sizeof(name));
    if (rc < 0) {
        set_result(result, "ERROR",
                   "file or signature database unavailable/unreadable", 0, 1);
        return -3;
    }
    if (rc == 1) {
        char detail[SLEELAVIRIN_BRIDGE_DETAIL_MAX];
        snprintf(detail, sizeof(detail), "signature match%s%s",
                 name[0] ? ": " : "", name);
        set_result(result, "DETECTED", detail, 1, 1);
        return 0;
    }
    set_result(result, "CLEAN",
               "no signature match (not proof the file is safe)", 0, 1);
    return 0;
}

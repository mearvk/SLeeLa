/*
 * Sleelavirin(TM) scan bridge - a stable C ABI that SLeeLa (and other callers)
 * use to request a scan and receive a normalized verdict.
 * Max Rupplin - MEARVK LLC - 2026.
 *
 * The bridge is the single, documented entry point described in SCANNERS.md:
 * a caller fills a request, the bridge validates it, then dispatches to the
 * native engine (SHA-256 hash index and/or content signatures) or - in a future
 * revision - to an external provider adapter. Verdicts are plain strings so the
 * SLeeLa side never needs to know engine internals.
 *
 * Original Sleelavirin code; portable C11; no third-party AV engine.
 */
#ifndef SLEELAVIRIN_BRIDGE_H
#define SLEELAVIRIN_BRIDGE_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SLEELAVIRIN_BRIDGE_PATH_MAX 4096u
#define SLEELAVIRIN_BRIDGE_NAME_MAX 64u
#define SLEELAVIRIN_BRIDGE_VERDICT_MAX 32u
#define SLEELAVIRIN_BRIDGE_DETAIL_MAX 512u

/* Scope of a scan request (how far the caller is authorized to reach). */
typedef enum {
    SLEELAVIRIN_SCOPE_LOCAL = 0, /* a path on the local machine */
    SLEELAVIRIN_SCOPE_MOUNT,     /* a mounted volume            */
    SLEELAVIRIN_SCOPE_NETWORK    /* a network location          */
} sleelavirin_bridge_scope_t;

typedef struct {
    char target_path[SLEELAVIRIN_BRIDGE_PATH_MAX];   /* file to scan          */
    char database_path[SLEELAVIRIN_BRIDGE_PATH_MAX]; /* signature index file  */
    char engine_name[SLEELAVIRIN_BRIDGE_NAME_MAX];   /* "native" (others TBD) */
    char mode[SLEELAVIRIN_BRIDGE_NAME_MAX];          /* e.g. "single-file"    */
    sleelavirin_bridge_scope_t scope;
    uint8_t recursive;                /* reserved for directory scans */
    uint8_t follow_links;             /* reserved */
    uint8_t require_admin;            /* caller attests admin context */
    uint64_t max_file_size_bytes;     /* refuse larger targets (0 = no limit) */
    uint32_t timeout_seconds;         /* advisory */
} sleelavirin_bridge_request_t;

typedef struct {
    char verdict[SLEELAVIRIN_BRIDGE_VERDICT_MAX]; /* "CLEAN"/"DETECTED"/"ERROR" */
    char detail[SLEELAVIRIN_BRIDGE_DETAIL_MAX];   /* human-readable detail      */
    uint32_t detections;                          /* number of detections       */
    uint8_t engine_native;                        /* 1 if the native engine ran */
} sleelavirin_bridge_result_t;

/*
 * Validate a request. Returns 0 if the request is well-formed and actionable,
 * or a negative value otherwise. Checks: non-empty NUL-terminated target and
 * database paths, a recognized engine name, and a recognized scope. Does NOT
 * touch the filesystem.
 */
int sleelavirin_bridge_validate_request(const sleelavirin_bridge_request_t *req);

/*
 * Scan a single file with the native engine. Fills *result with a normalized
 * verdict. Returns 0 if the scan completed (whether CLEAN or DETECTED), or a
 * negative value if the scan could not run (bad args, unreadable file/db); in
 * that case result->verdict is "ERROR". A CLEAN verdict is NOT proof of safety.
 *
 * If request->max_file_size_bytes is non-zero and the target is larger, the
 * verdict is "ERROR" with an explanatory detail and a negative return.
 */
int sleelavirin_bridge_scan_native(const sleelavirin_bridge_request_t *req,
                                   sleelavirin_bridge_result_t *result);

#ifdef __cplusplus
}
#endif
#endif /* SLEELAVIRIN_BRIDGE_H */
